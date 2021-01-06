/*
 * graphics.cpp
 *
 * Copyright (c) 2003 Alexandre Pigolkine, Novell Inc.
 * Copyright (C) 2006-2007 Novell, Inc (http://www.novell.com)
 * Copyright (C) 2020 Asobo Studio.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *   Alexandre Pigolkine (pigolkine@gmx.de)
 *   Duncan Mak (duncan@ximian.com)
 *   Sebastien Pouliot  <sebastien@ximian.com>
 *   Frederik Carlier <frederik.carlier@quamotion.mobi>
 *	 Sol Roo (sroo@asobostudio.com)
 *
 */
#include "FlatNVG\graphics-private.h"
#include "FlatNVG\general-private.h"
#include "FlatNVG\graphics-cairo-private.h"
#include "FlatNVG\graphics-metafile-private.h"
#include "FlatNVG\region-private.h"
#include "FlatNVG\graphics-path-private.h"
#include "FlatNVG\brush-private.h"
#include "FlatNVG\matrix-private.h"
#include "FlatNVG\bitmap-private.h"
#include "FlatNVG\metafile-private.h"
#include "FlatNVG\matrix-private.h"

 // custom files of PROXY implementation
#include "FlatNVG\solidbrush-private.h"
#include "FlatNVG\hatchbrush-private.h"
#include "GdiPlusHatchStylePatterns.inl"

#define MAX_GRAPHICS_STATE_STACK 512

NVGcolor ConvertARGB(ARGB value) {
	return nvgRGBA((BYTE)(value >> 16), (BYTE)(value >> 8), (BYTE)value, (BYTE)(value >> 24));
}
NVGcolor ConvertColor(Color* color) {
	return ConvertARGB(color->Value);
}

void SetClipMode(GpGraphics* graphics, GpCombineMode combineMode) {
	if (!graphics->clipped) {

		// For substractive clipping when no clipping has taken place
		// set the clipping to the full image before appling the stencil operation
		if (combineMode == GpCombineMode::CombineModeComplement
			|| combineMode == GpCombineMode::CombineModeExclude) {

			int prevPath = nvgCurrentPath(graphics->ctx);
			nvgSelectPath(graphics->ctx, 1);
			nvgSetBuffer(graphics->ctx, 1);
			nvgBeginPath(graphics->ctx);
			nvgRect(graphics->ctx, graphics->bounds.X, graphics->bounds.Y, graphics->bounds.Width, graphics->bounds.Height);
			nvgFill(graphics->ctx);
			nvgSetBuffer(graphics->ctx, 0);
			nvgSelectPath(graphics->ctx, prevPath);
			graphics->clipped = true;
			nvgSetClipped(graphics->ctx, true);
		}
		else {
			combineMode = GpCombineMode::CombineModeUnion;
		}
	}
	else if (combineMode == GpCombineMode::CombineModeReplace) {
		DLLExports::GdipResetClip(graphics);
	}
	/* Other combine modes do not require any action (or are unsupported)
	else if (combineMode == CombineMode::CombineModeUnion) {
		// new | existing (default render behaviour)
		// handled by backend
	}
	else if (combineMode == CombineMode::CombineModeIntersect) {
		// new & existing
		// handled by backend
	}
	else if (combineMode == CombineMode::CombineModeExclude) {
		// new - existing
		// handled by backend
	}
	else if (combineMode == CombineMode::CombineModeComplement) {
		// existing - new
		// handled by backend
	}
	else if (combineMode == CombineMode::CombineModeXor) {
		// existing != new
		// not supported
	}*/

	nvgSetClipMode(graphics->ctx, (NVGclipMode)combineMode);
}

//WASM TODO: todo use libgdiplus approach with the associated GpBrush
void convertPen(GpGraphics* graphics, GpPen* pen) {
	nvgStrokeColor(graphics->ctx, ConvertARGB(pen->color));
	nvgStrokeWidth(graphics->ctx, pen->width);
}

static void
apply_world_to_bounds(GpGraphics* graphics)
{
	GpPointF pts[2];

	pts[0].X = graphics->orig_bounds.X;
	pts[0].Y = graphics->orig_bounds.Y;
	pts[1].X = graphics->orig_bounds.X + graphics->orig_bounds.Width;
	pts[1].Y = graphics->orig_bounds.Y + graphics->orig_bounds.Height;
	DLLExports::GdipTransformMatrixPoints(graphics->clip_matrix, pts, 2);

	if (pts[0].X > pts[1].X) {
		graphics->bounds.X = pts[1].X;
		graphics->bounds.Width = iround(pts[0].X - pts[1].X);
	}
	else {
		graphics->bounds.X = pts[0].X;
		graphics->bounds.Width = iround(pts[1].X - pts[0].X);
	}
	if (pts[0].Y > pts[1].Y) {
		graphics->bounds.Y = pts[1].Y;
		graphics->bounds.Height = iround(pts[0].Y - pts[1].Y);
	}
	else {
		graphics->bounds.Y = pts[0].Y;
		graphics->bounds.Height = iround(pts[1].Y - pts[0].Y);
	}
}


float
gdip_unit_conversion(GpUnit from, GpUnit to, float dpi, GraphicsType type, float nSrc)
{
	float inchs = 0;

	switch (from) {
	case GpUnit::UnitDocument:
		inchs = nSrc / 300.0f;
		break;
	case GpUnit::UnitInch:
		inchs = nSrc;
		break;
	case GpUnit::UnitMillimeter:
		inchs = nSrc / 25.4f;
		break;
	case GpUnit::UnitDisplay:
		if (type == gtPostScript) { /* Uses 1/100th on printers */
			inchs = nSrc / 100;
		}
		else { /* Pixel for video display */
			inchs = nSrc / dpi;
		}
		break;
	case GpUnit::UnitPixel:
	case GpUnit::UnitWorld:
		inchs = nSrc / dpi;
		break;
	case GpUnit::UnitPoint:
		inchs = nSrc / 72.0f;
		break;
	default:
		return nSrc;
	}

	switch (to) {
	case GpUnit::UnitDocument:
		return inchs * 300.0f;
	case GpUnit::UnitInch:
		return inchs;
	case GpUnit::UnitMillimeter:
		return inchs * 25.4f;
	case GpUnit::UnitDisplay:
		if (type == gtPostScript) { // Uses 1/100th on printers
			return inchs * 100;
		}
		else { // Pixel for video display
			return inchs * dpi;
		}
	case GpUnit::UnitPixel:
	case GpUnit::UnitWorld:
		return inchs * dpi;
	case GpUnit::UnitPoint:
		return inchs * 72.0f;
	default:
		return nSrc;
	}
}

static void
gdip_graphics_reset(GpGraphics* graphics)
{
	// if required, previous_matrix will be assigned later (e.g. containers)
	nvgTransformIdentity(graphics->previous_matrix.m);
	DLLExports::GdipResetClip(graphics);
	nvgTransformIdentity(graphics->clip_matrix->m);
	graphics->page_unit = GpUnit::UnitDisplay;
	graphics->scale = 1.0f;
	graphics->interpolation = GpInterpolationMode::InterpolationModeBilinear;
	graphics->composite_quality = GpCompositingQuality::CompositingQualityDefault;
	graphics->composite_mode = GpCompositingMode::CompositingModeSourceOver;
	graphics->text_mode = GpTextRenderingHint::TextRenderingHintSystemDefault;
	graphics->pixel_mode = GpPixelOffsetMode::PixelOffsetModeDefault;
	graphics->text_contrast = DEFAULT_TEXT_CONTRAST;

	DLLExports::GdipSetSmoothingMode(graphics, GpSmoothingMode::SmoothingModeNone);
}

static void
gdip_graphics_common_init(GpGraphics* graphics)
{
	//graphics->image = NULL;

	DLLExports::GdipCreateMatrix(&graphics->copy_of_ctm);
	nvgTransformIdentity(graphics->copy_of_ctm->m);

	DLLExports::GdipCreateRegion(&graphics->clip);
	DLLExports::GdipCreateMatrix(&graphics->clip_matrix);
	graphics->overall_clip = graphics->clip;
	graphics->previous_clip = NULL;
	graphics->bounds.X = graphics->bounds.Y = graphics->bounds.Width = graphics->bounds.Height = 0;
	graphics->orig_bounds.X = graphics->orig_bounds.Y = graphics->orig_bounds.Width = graphics->orig_bounds.Height = 0;
	graphics->last_pen = NULL;
	graphics->last_brush = NULL;
	graphics->saved_status = NULL;
	graphics->saved_status_pos = 0;
	graphics->render_origin_x = 0;
	graphics->render_origin_y = 0;
	graphics->dpi_x = graphics->dpi_y = 0;
	graphics->state = GraphicsStateValid;

#if defined(HAVE_X11) && CAIRO_HAS_XLIB_SURFACE
	graphics->display = (Display*)NULL;
	graphics->drawable = (Drawable)NULL;
#endif

	gdip_graphics_reset(graphics);
}


GpStatus gdip_get_visible_clip(GpGraphics* graphics, GpRegion** visible_clip)
{
	GpStatus status = GpStatus::Ok;
	GpRegion* clip = NULL;
	BOOL empty;

	DLLExports::GdipIsEmptyRegion(graphics->overall_clip, graphics, &empty);
	if (empty) {
		status = DLLExports::GdipCloneRegion(graphics->overall_clip, visible_clip);
	}
	else if (!gdip_is_InfiniteRegion(graphics->overall_clip)) {
		status = DLLExports::GdipCloneRegion(graphics->overall_clip, &clip);
		if (status != GpStatus::Ok)
			return status;

		if (!gdip_is_matrix_empty(graphics->clip_matrix)) {
			DLLExports::GdipTransformRegion(clip, graphics->clip_matrix);
		}

		status = DLLExports::GdipCombineRegionRectI(clip, &graphics->bounds, GpCombineMode::CombineModeIntersect);
		*visible_clip = clip;
	}
	else {
		status = DLLExports::GdipCreateRegionRectI(&graphics->bounds, visible_clip);
	}

	if (status != GpStatus::Ok && clip)
		DLLExports::GdipDeleteRegion(clip);
	return status;
}

GpStatus
gdip_set_clip(GpGraphics* graphics)
{
	GpRegion* work;
	GpRectF* rect;
	int i;

	DLLExports::GdipResetClip(graphics);

	if (gdip_is_InfiniteRegion(graphics->overall_clip))
		return GpStatus::Ok;


	if (gdip_is_matrix_empty(graphics->clip_matrix)) {
		work = graphics->overall_clip;
	}
	else {
		DLLExports::GdipCloneRegion(graphics->overall_clip, &work);
		DLLExports::GdipTransformRegion(work, graphics->clip_matrix);
	}

	SetClipMode(graphics, GpCombineMode::CombineModeReplace);
	int prevPath = nvgCurrentPath(graphics->ctx);
	nvgSelectPath(graphics->ctx, 1);
	nvgSetBuffer(graphics->ctx, 1);
	nvgBeginPath(graphics->ctx);

	switch (work->type) {
	case RegionTypeRect:
		for (i = 0, rect = work->rects; i < work->cnt; i++, rect++) {
			nvgRect(graphics->ctx, rect->X, rect->Y, rect->Width, rect->Height);
		}
		break;
	case RegionTypePath:
		if (work->tree && work->tree->path) {
			gdip_apply_path(graphics, work->tree->path);
		}
		else {
			UINT count;
			GpMatrix matrix;
			nvgTransformIdentity(matrix.m);
			// I admit that's a (not so cute) hack - anyone with a better idea ? 
			if ((DLLExports::GdipGetRegionScansCount(work, &count, &matrix) == GpStatus::Ok) && (count > 0)) {
				GpRectF* rects = (GpRectF*)DLLExports::GdipAlloc(count * sizeof(GpRectF));
				if (rects) {
					INT countTemp;
					DLLExports::GdipGetRegionScans(work, rects, &countTemp, &matrix);
					for (i = 0, rect = rects; i < countTemp; i++, rect++) {
						nvgRect(graphics->ctx, rect->X, rect->Y, rect->Width, rect->Height);
					}
					DLLExports::GdipFree(rects);
				}
			}
		}
		break;
	default:
		//g_warning("Unknown region type %d", work->type);
		break;
	}

	nvgFill(graphics->ctx);
	nvgSetBuffer(graphics->ctx, 0);
	nvgSelectPath(graphics->ctx, prevPath);
	graphics->clipped = true;
	nvgSetClipped(graphics->ctx, true);

	// destroy the clone, if one was needed
	if (work != graphics->overall_clip)
		DLLExports::GdipDeleteRegion(work);

	return GpStatus::Ok;
}

GpStatus gdip_calculate_overall_clipping(GpGraphics* graphics)
{
	GpStatus status = GpStatus::Ok;

	if (!graphics->previous_clip) {
		graphics->overall_clip = graphics->clip;
	}
	else {
		if (graphics->overall_clip && graphics->overall_clip != graphics->clip)
			DLLExports::GdipDeleteRegion(graphics->overall_clip);
		status = DLLExports::GdipCloneRegion(graphics->previous_clip, &graphics->overall_clip);
		if (status != GpStatus::Ok)
			return status;
		status = DLLExports::GdipCombineRegionRegion(graphics->overall_clip, graphics->clip, GpCombineMode::CombineModeIntersect);
	}
	return status;
}

static void
make_pie(GpGraphics* graphics, float x, float y, float width, float height,
	float startAngle, float sweepAngle, BOOL antialiasing)
{
	float rx, ry, cx, cy, alpha;
	double sin_alpha, cos_alpha;

	/* if required deal, once and for all, with unit conversions */
	/*if (!OPTIMIZE_CONVERSION(graphics)) {
		x = gdip_unitx_convgr(graphics, x);
		y = gdip_unity_convgr(graphics, y);
		width = gdip_unitx_convgr(graphics, width);
		height = gdip_unity_convgr(graphics, height);
	}*/

	rx = width / 2;
	ry = height / 2;

	// center
	cx = x + rx;
	cy = y + ry;

	// angles in radians
	alpha = startAngle * PI / 180;

	// adjust angle for ellipses
	alpha = atan2(rx * sin(alpha), ry * cos(alpha));

	sin_alpha = sin(alpha);
	cos_alpha = cos(alpha);

	// if required deal, once and for all, with antialiasing
	/*if (antialiasing && !gdip_is_scaled(graphics)) {
		cx += graphics->aa_offset_x;
		cy += graphics->aa_offset_y;
	}*/

	// draw pie edge 
	if (fabs(sweepAngle) >= 360)
		nvgMoveTo(graphics->ctx, cx + rx * cos_alpha, cy + ry * sin_alpha);
	else {
		nvgMoveTo(graphics->ctx, cx, cy);
		nvgLineTo(graphics->ctx, cx + rx * cos_alpha, cy + ry * sin_alpha);
	}

	// draw the arcs */
	nvgEllipticalArc(graphics->ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, nvgDegToRad(startAngle), nvgDegToRad(startAngle + sweepAngle), (sweepAngle < 0 ? NVG_CCW : NVG_CW));
	// draws line back to center
	if (fabs(sweepAngle) >= 360)
		nvgMoveTo(graphics->ctx, cx, cy);
	else
		nvgLineTo(graphics->ctx, cx, cy);
}

namespace DLLExports {

	static BOOL gdiplusInitialized = true;


	void InitializeNVG(GpGraphics* graphics) {
		if (graphics->ctx) {
			for (int p = 0; p < GpHatchStyle::HatchStyleTotal; ++p) {
				graphics->m_brushPatterns[p] = nvgGenerateBrushPattern(graphics->ctx, (GpHatchStyle)p);
			}
		}
		else {
			for (int p = 0; p < GpHatchStyle::HatchStyleTotal; ++p) {
				graphics->m_brushPatterns[p] = -1;
			}
		}
	}

	void ReleaseNVG(GpGraphics* graphics) {
		// @sroo: todo review : brushes need to be released before the destruction of the NVGcontext
		// but there is no guarantee that the destructor will be callled on PRE_KILL
		/*if (graphics->ctx) {
			for (int p = 0; p < GpHatchStyle::HatchStyleTotal; ++p) {
				if (graphics->m_brushPatterns[p] != -1) {
					nvgDeleteImage(graphics->ctx, graphics->m_brushPatterns[p]);
					graphics->m_brushPatterns[p] = -1;
				}
			}
		}*/
	}

	GpStatus WINGDIPAPI
		GdipCreateFromHDC(HDC hdc, GpGraphics** graphics)
	{

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		GpGraphics* gfx = (GpGraphics *) GdipAlloc(sizeof(GpGraphics));
		*graphics = gfx;
		NVGcontext* nvgctx = (NVGcontext*)hdc;
		gfx->ctx = nvgctx;
		InitializeNVG(*graphics);
		gdip_graphics_common_init(*graphics);

		//@sroo: todo, set the resolution as part of the NVGcontext
		gfx->bounds.X = REGION_INFINITE_POSITION;
		gfx->bounds.Y = REGION_INFINITE_POSITION;
		gfx->bounds.Width = REGION_INFINITE_LENGTH;
		gfx->bounds.Height = REGION_INFINITE_LENGTH;
		gfx->orig_bounds = gfx->bounds;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipCreateFromHDC2(HDC hdc, HANDLE hDevice, GpGraphics** graphics)
	{
		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		if (!graphics)
			return GpStatus::InvalidParameter;

		if (hDevice)
			return GpStatus::NotImplemented;

		return GdipCreateFromHDC(hdc, graphics);
	}

	GpStatus WINGDIPAPI
		GdipCreateFromHWND(HWND hwnd, GpGraphics** graphics)
	{
		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		if (!graphics)
			return GpStatus::InvalidParameter;

		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateFromHWNDICM(HWND hwnd, GpGraphics** graphics)
	{
		return GdipCreateFromHWND(hwnd, graphics);
	}

	GpStatus WINGDIPAPI
		GdipDeleteGraphics(GpGraphics* graphics)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state != GraphicsStateValid)
			return GpStatus::ObjectBusy;
		*/
		ReleaseNVG(graphics);

		GdipFree(graphics);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetDC(GpGraphics* graphics, HDC* hdc)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !hdc)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/

		* hdc = (HDC)(graphics->ctx);
		graphics->state = GraphicsStateBusy;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipReleaseDC(GpGraphics* graphics, HDC hdc)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !hdc || graphics->state != GraphicsStateBusy)
			return GpStatus::InvalidParameter;

		if (hdc != (void*)graphics)
			return GpStatus::InvalidParameter;
#endif
		graphics->state = GraphicsStateValid;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI GdipCancelFrame(GpGraphics* graphics)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		nvgCancelFrame(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI GdipStartFrame(GpGraphics* graphics, REAL windowWidth, REAL windowHeight, REAL devicePixelRatio)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		nvgBeginFrame(graphics->ctx, windowWidth, windowHeight, devicePixelRatio);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipRestoreGraphics(GpGraphics* graphics, GpGraphicsState state)
	{
		GpStatus status;
		GpState* pos_state;

#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		// Do nothing if the state is invalid.
		if (state <= 0 || (state - 1) >= MAX_GRAPHICS_STATE_STACK || (state - 1) > graphics->saved_status_pos)
			return GpStatus::Ok;

		pos_state = graphics->saved_status;
		pos_state += (state - 1);

		// Save from GpState to Graphics 
		gdip_matrix_copy(graphics->copy_of_ctm, &pos_state->matrix);
		gdip_matrix_copy(&graphics->previous_matrix, &pos_state->previous_matrix);

		GdipSetRenderingOrigin(graphics, pos_state->org_x, pos_state->org_y);

		if (graphics->overall_clip != graphics->clip) {
			GdipDeleteRegion(graphics->overall_clip);
		}
		graphics->overall_clip = NULL;

		if (graphics->clip) {
			GdipDeleteRegion(graphics->clip);
		}
		status = GdipCloneRegion(pos_state->clip, &graphics->clip);
		if (status != GpStatus::Ok)
			return status;

		if (graphics->previous_clip) {
			GdipDeleteRegion(graphics->previous_clip);
			graphics->previous_clip = NULL;
		}
		if (pos_state->previous_clip) {
			status = GdipCloneRegion(pos_state->previous_clip, &graphics->previous_clip);
			if (status != GpStatus::Ok)
				return status;
		}

		gdip_matrix_copy(graphics->clip_matrix, &pos_state->clip_matrix);

		gdip_calculate_overall_clipping(graphics);

		graphics->composite_mode = pos_state->composite_mode;
		graphics->composite_quality = pos_state->composite_quality;
		graphics->interpolation = pos_state->interpolation;
		graphics->page_unit = pos_state->page_unit;
		graphics->scale = pos_state->scale;
		GdipSetSmoothingMode(graphics, pos_state->draw_mode);
		graphics->text_mode = pos_state->text_mode;
		graphics->pixel_mode = pos_state->pixel_mode;
		graphics->text_contrast = pos_state->text_contrast;

		graphics->saved_status_pos = state - 1;

		// re-adjust clipping (region and matrix) 
		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, graphics->copy_of_ctm->m[0], graphics->copy_of_ctm->m[1], graphics->copy_of_ctm->m[2], graphics->copy_of_ctm->m[3], graphics->copy_of_ctm->m[4], graphics->copy_of_ctm->m[5]);


		// GdipCloneRegion was called, but for some reason, not registred as an allocation 
		/* coverity[freed_arg] */
		return gdip_set_clip(graphics);
	}

	GpStatus WINGDIPAPI
		GdipSaveGraphics(GpGraphics* graphics, GpGraphicsState* state)
	{
		GpStatus status;
		GpState* pos_state;
#ifdef GDIFLAT_SAFE
		if (!graphics || !state)
			return GpStatus::InvalidParameter;
#endif
		if (!graphics->saved_status) {
			graphics->saved_status = (GpState*)gdip_calloc(MAX_GRAPHICS_STATE_STACK, sizeof(GpState));
			if (!graphics->saved_status)
				return GpStatus::OutOfMemory;

			graphics->saved_status_pos = 0;
		}

		if (graphics->saved_status_pos >= MAX_GRAPHICS_STATE_STACK)
			return GpStatus::OutOfMemory;

		pos_state = graphics->saved_status;
		pos_state += graphics->saved_status_pos;

		/* Save from Graphics to GpState */
		gdip_matrix_copy(&pos_state->matrix, graphics->copy_of_ctm);
		GdipGetRenderingOrigin(graphics, &pos_state->org_x, &pos_state->org_y);

		gdip_matrix_copy(&pos_state->previous_matrix, &graphics->previous_matrix);

		if (pos_state->clip)
			GdipDeleteRegion(pos_state->clip);
		status = GdipCloneRegion(graphics->clip, &pos_state->clip);
		if (status != GpStatus::Ok)
			return status;

		if (pos_state->previous_clip) {
			GdipDeleteRegion(pos_state->previous_clip);
			pos_state->previous_clip = NULL;
		}
		if (graphics->previous_clip) {
			status = GdipCloneRegion(graphics->previous_clip, &pos_state->previous_clip);
			if (status != GpStatus::Ok)
				return status;
		}

		gdip_matrix_copy(&pos_state->clip_matrix, graphics->clip_matrix);

		pos_state->composite_mode = graphics->composite_mode;
		pos_state->composite_quality = graphics->composite_quality;
		pos_state->interpolation = graphics->interpolation;
		pos_state->page_unit = graphics->page_unit;
		pos_state->scale = graphics->scale;
		pos_state->draw_mode = graphics->draw_mode;
		pos_state->text_mode = graphics->text_mode;
		pos_state->pixel_mode = graphics->pixel_mode;
		pos_state->text_contrast = graphics->text_contrast;

		*state = graphics->saved_status_pos + 1; // make sure GraphicsState is non-zero for compat with GDI+
		graphics->saved_status_pos++;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipResetWorldTransform(GpGraphics* graphics)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_is_matrix_empty(&graphics->previous_matrix)) {
			// inside a container only reset to the previous transform 
			gdip_matrix_copy(graphics->copy_of_ctm, &graphics->previous_matrix);
			gdip_matrix_copy(graphics->clip_matrix, &graphics->previous_matrix);
			GdipInvertMatrix(graphics->clip_matrix);
		}
		else {
			nvgTransformIdentity(graphics->copy_of_ctm->m);
			nvgTransformIdentity(graphics->clip_matrix->m);
		}

		apply_world_to_bounds(graphics);

		nvgResetTransform(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetWorldTransform(GpGraphics* graphics, GpMatrix* matrix)
	{
		BOOL invertible;

#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
		//if (graphics->state == GraphicsStateBusy)
		//	return ObjectBusy;
		if (!matrix)
			return GpStatus::InvalidParameter;
#endif
		// Inverting an identity matrix result in the identity matrix.
		if (gdip_is_matrix_empty(matrix))
			return GdipResetWorldTransform(graphics);

		// The matrix must be invertible to be used.
		GdipIsMatrixInvertible(matrix, &invertible);
		if (!invertible)
			return GpStatus::InvalidParameter;

		GpMatrix matrixCopy;
		gdip_matrix_copy(&matrixCopy, matrix);

		if (!gdip_is_matrix_empty(&graphics->previous_matrix)) {
			// inside a container the transform is appended to the previous transform
			GdipMultiplyMatrix(&matrixCopy, &graphics->previous_matrix, GpMatrixOrder::MatrixOrderAppend);
		}

		gdip_matrix_copy(graphics->copy_of_ctm, &matrixCopy);
		gdip_matrix_copy(graphics->clip_matrix, &matrixCopy);

		/* we already know it's invertible */
		GdipInvertMatrix(graphics->clip_matrix);

		apply_world_to_bounds(graphics);

		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, graphics->copy_of_ctm->m[0], graphics->copy_of_ctm->m[1], graphics->copy_of_ctm->m[2], graphics->copy_of_ctm->m[3], graphics->copy_of_ctm->m[4], graphics->copy_of_ctm->m[5]);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetWorldTransform(GpGraphics* graphics, GpMatrix* matrix)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !matrix)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return ObjectBusy;
			*/
		// get the effective matrix from cairo 
		gdip_matrix_copy(matrix, graphics->copy_of_ctm);

		/* if we're inside a container then the previous matrix are hidden */
		if (!gdip_is_matrix_empty(&graphics->previous_matrix)) {
			GpMatrix inverted;
			/* substract the previous matrix from the effective matrix */
			gdip_matrix_copy(&inverted, &graphics->previous_matrix);
			GdipInvertMatrix(&inverted);
			return GdipMultiplyMatrix(matrix, &inverted, GpMatrixOrder::MatrixOrderAppend);
		}
		return GpStatus::Ok;
	}
	GpStatus WINGDIPAPI
		GdipMultiplyWorldTransform(GpGraphics* graphics, GpMatrix* matrix, GpMatrixOrder order)
	{
		GpStatus s;
		BOOL invertible;
		GpMatrix inverted;

#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/* the matrix MUST be invertible to be used */
		s = GdipIsMatrixInvertible(matrix, &invertible);
		if (!invertible || (s != GpStatus::Ok))
			return GpStatus::InvalidParameter;

		s = GdipMultiplyMatrix(graphics->copy_of_ctm, matrix, order);
		if (s != GpStatus::Ok)
			return s;

		/* Multiply the inverted matrix with the clipping matrix */
		gdip_matrix_copy(&inverted, matrix);
		s = GdipInvertMatrix(&inverted);
		if (s != GpStatus::Ok)
			return s;

		s = GdipMultiplyMatrix(graphics->clip_matrix, &inverted, order);
		if (s != GpStatus::Ok)
			return s;

		apply_world_to_bounds(graphics);
		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, graphics->copy_of_ctm->m[0], graphics->copy_of_ctm->m[1], graphics->copy_of_ctm->m[2], graphics->copy_of_ctm->m[3], graphics->copy_of_ctm->m[4], graphics->copy_of_ctm->m[5]);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipRotateWorldTransform(GpGraphics* graphics, REAL angle, GpMatrixOrder order)
	{
		GpStatus s;

#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		//if (graphics->state == GraphicsStateBusy)
		//	return GpStatus::ObjectBusy;

		s = GdipRotateMatrix(graphics->copy_of_ctm, angle, order);
		if (s != GpStatus::Ok)
			return s;

		s = GdipRotateMatrix(graphics->clip_matrix, -angle, gdip_matrix_reverse_order(order));
		if (s != GpStatus::Ok)
			return s;

		apply_world_to_bounds(graphics);

		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, graphics->copy_of_ctm->m[0], graphics->copy_of_ctm->m[1], graphics->copy_of_ctm->m[2], graphics->copy_of_ctm->m[3], graphics->copy_of_ctm->m[4], graphics->copy_of_ctm->m[5]);
		return GpStatus::Ok;

	}

	GpStatus WINGDIPAPI
		GdipScaleWorldTransform(GpGraphics* graphics, REAL sx, REAL sy, GpMatrixOrder order)
	{
		GpStatus s;

#ifdef GDIFLAT_SAFE
		if (!graphics || (sx == 0.0f) || (sy == 0.0f))
			return GpStatus::InvalidParameter;
#endif
		s = GdipScaleMatrix(graphics->copy_of_ctm, sx, sy, order);
		if (s != GpStatus::Ok)
			return s;
		s = GdipScaleMatrix(graphics->clip_matrix, (1.0f / sx), (1.0f / sy), gdip_matrix_reverse_order(order));
		if (s != GpStatus::Ok)
			return s;

		apply_world_to_bounds(graphics);

		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, graphics->copy_of_ctm->m[0], graphics->copy_of_ctm->m[1], graphics->copy_of_ctm->m[2], graphics->copy_of_ctm->m[3], graphics->copy_of_ctm->m[4], graphics->copy_of_ctm->m[5]);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipTranslateWorldTransform(GpGraphics* graphics, REAL dx, REAL dy, GpMatrixOrder order)
	{
		GpStatus s;
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		s = GdipTranslateMatrix(graphics->copy_of_ctm, dx, dy, order);
		if (s != GpStatus::Ok)
			return s;

		s = GdipTranslateMatrix(graphics->clip_matrix, -dx, -dy, gdip_matrix_reverse_order(order));
		if (s != GpStatus::Ok)
			return s;

		apply_world_to_bounds(graphics);

		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, graphics->copy_of_ctm->m[0], graphics->copy_of_ctm->m[1], graphics->copy_of_ctm->m[2], graphics->copy_of_ctm->m[3], graphics->copy_of_ctm->m[4], graphics->copy_of_ctm->m[5]);

		return GpStatus::Ok;
	}

	/*
	 * Draw operations - validate parameters and delegate to cairo/metafile backends
	 */

	GpStatus WINGDIPAPI
		GdipDrawArc(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgEllipticalArc(graphics->ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, nvgDegToRad(startAngle), nvgDegToRad(startAngle + sweepAngle), (sweepAngle < 0 ? NVG_CCW : NVG_CW));
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawArcI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return GdipDrawArc(graphics, pen, (REAL)x, (REAL)y, (REAL)width, (REAL)height, startAngle, sweepAngle);
	}

	GpStatus WINGDIPAPI
		GdipDrawBezier(GpGraphics* graphics, GpPen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3,
			REAL x4, REAL y4)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, x1, y1);
		nvgBezierTo(graphics->ctx, x2, y2, x3, y3, x4, y4);
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawBezierI(GpGraphics* graphics, GpPen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4)
	{
		return GdipDrawBezier(graphics, pen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2, (REAL)x3, (REAL)y3, (REAL)x4, (REAL)y4);
	}

	GpStatus WINGDIPAPI
		GdipDrawBeziers(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (!points || count <= 0 || (count > 3 && (count % 3) != 1))
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!pen)
			return GpStatus::InvalidParameter;
#endif
		if (count < 3)
			return GpStatus::Ok;

		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
		for (int p = 1; p + 2 < count; p += 3)
		{
			nvgBezierTo(graphics->ctx, points[p].X, points[p].Y, points[p + 1].X, points[p + 1].Y, points[p + 2].X, points[p + 2].Y);
		}
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawBeziersI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (!graphics || !points || count <= 0 || (count > 3 && (count % 3) != 1))
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!pen)
			return GpStatus::InvalidParameter;
#endif
		if (count < 3)
			return GpStatus::Ok;

		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
		for (int p = 1; p + 2 < count; p += 3)
		{
			nvgBezierTo(graphics->ctx, points[p].X, points[p].Y, points[p + 1].X, points[p + 1].Y, points[p + 2].X, points[p + 2].Y);
		}
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawEllipse(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height)
	{
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!pen)
			return GpStatus::InvalidParameter;

		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgEllipse(graphics->ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f);
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawEllipseI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height)
	{
		return GdipDrawEllipse(graphics, pen, (REAL)x, (REAL)y, (REAL)width, (REAL)height);
	}

	GpStatus WINGDIPAPI
		GdipDrawLine(GpGraphics* graphics, GpPen* pen, REAL x1, REAL y1, REAL x2, REAL y2)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, x1, y1);
		nvgLineTo(graphics->ctx, x2, y2);
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawLineI(GpGraphics* graphics, GpPen* pen, INT x1, INT y1, INT x2, INT y2)
	{
		return GdipDrawLine(graphics, pen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2);
	}

	GpStatus WINGDIPAPI
		GdipDrawLines(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		if (count > 0) {
			convertPen(graphics, pen);
			nvgBeginPath(graphics->ctx);
			nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
			for (int p = 1; p < count; ++p) {
				nvgLineTo(graphics->ctx, points[p].X, points[p].Y);
			}
			nvgStroke(graphics->ctx);
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawLinesI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		if (count > 0) {
			convertPen(graphics, pen);
			nvgBeginPath(graphics->ctx);
			nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
			for (int p = 1; p < count; ++p) {
				nvgLineTo(graphics->ctx, points[p].X, points[p].Y);
			}
			nvgStroke(graphics->ctx);
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawPath(GpGraphics* graphics, GpPen* pen, GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		if (path && path->count > 0) {
			convertPen(graphics, pen);
			nvgBeginPath(graphics->ctx);
			gdip_apply_path(graphics, path);
			nvgStroke(graphics->ctx);
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawPie(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
		bool antialiasing = false;
		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		make_pie(graphics, x, y, width, height, startAngle, sweepAngle,antialiasing);
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawPieI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return GdipDrawPie(graphics, pen, x, y, width, height, startAngle, sweepAngle);
	}

	GpStatus WINGDIPAPI
		GdipDrawPolygon(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !pen || !points)
			return GpStatus::InvalidParameter;
#endif
		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
		{
			nvgLineTo(graphics->ctx, points[i].X, points[i].Y);
		}
		nvgClosePath(graphics->ctx);
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawPolygonI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !pen || !points)
			return GpStatus::InvalidParameter;
#endif
		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
		{
			nvgLineTo(graphics->ctx, points[i].X, points[i].Y);
		}
		nvgClosePath(graphics->ctx);
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawRectangle(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		convertPen(graphics, pen);
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, x, y, width, height);
		nvgStroke(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawRectangleI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height)
	{
		return GdipDrawRectangle(graphics, pen, (REAL)x, (REAL)y, (REAL)width, (REAL)height);
	}

	GpStatus WINGDIPAPI
		GdipDrawRectangles(GpGraphics* graphics, GpPen* pen, GDIPCONST GpRectF* rects, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !rects || count <= 0)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (!pen)
			return GpStatus::InvalidParameter;
#endif
		for (int r = 0; r < count; ++r) {
			GpStatus result = GdipDrawRectangle(graphics, pen, rects[r].X, rects[r].Y, rects[r].Width, rects[r].Height);
			if (result != GpStatus::Ok) {
				return result;
			}
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawRectanglesI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpRect* rects, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (!rects || count < 0)
			return GpStatus::InvalidParameter;
#endif
		GpStatus status;
		GpRectF* rectsF;
		for (int r = 0; r < count; ++r) {
			GpStatus result = GdipDrawRectangleI(graphics, pen, rects[r].X, rects[r].Y, rects[r].Width, rects[r].Height);
			if (result != GpStatus::Ok) {
				return result;
			}
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawClosedCurve(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count)
	{
		return GdipDrawClosedCurve2(graphics, pen, points, count, 0.5f);
	}

	GpStatus WINGDIPAPI
		GdipDrawClosedCurveI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count)
	{
		return GdipDrawClosedCurve2I(graphics, pen, points, count, 0.5f);
	}

	GpStatus WINGDIPAPI
		GdipDrawClosedCurve2(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count, REAL tension)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !points || count <= 0)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (!pen || count < 3)
			return GpStatus::InvalidParameter;
#endif

		if (tension == 0)
			return GdipDrawPolygon(graphics, pen, points, count);


		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDrawClosedCurve2I(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count, REAL tension)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (count < 0)
			return GpStatus::OutOfMemory;
		if (!points)
			return GpStatus::InvalidParameter;
#endif 
		GpStatus status;
		GpPointF* pointsF;

		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDrawCurve(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count)
	{
		return GdipDrawCurve2(graphics, pen, points, count, 0.5f);
	}

	GpStatus WINGDIPAPI
		GdipDrawCurveI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count)
	{
		return GdipDrawCurve2I(graphics, pen, points, count, 0.5f);
	}

	GpStatus WINGDIPAPI
		GdipDrawCurve2(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count, REAL tension)
	{
		if (count == 2)
			return GdipDrawLines(graphics, pen, points, count);

		return GdipDrawCurve3(graphics, pen, points, count, 0, count - 1, tension);
	}

	GpStatus WINGDIPAPI
		GdipDrawCurve2I(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count, REAL tension)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (count < 0)
			return GpStatus::OutOfMemory;
		if (!points)
			return GpStatus::InvalidParameter;
#endif

		GpStatus status;
		GpPointF* pointsF;

		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDrawCurve3(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count, INT offset, INT numOfSegments, REAL tension)
	{

#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (!graphics || !points || count <= 0 || !pen)
#endif
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (count < 2 || offset < 0 || offset >= count)
			return GpStatus::InvalidParameter;
		if (numOfSegments < 1 || numOfSegments >= count - offset)
			return GpStatus::InvalidParameter;
		if (tension == 0)
			return GdipDrawLines(graphics, pen, points, count);


		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDrawCurve3I(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count, INT offset, INT numOfSegments, REAL tension)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (count < 0)
			return GpStatus::OutOfMemory;
		if (!points)
			return GpStatus::InvalidParameter;
#endif
		GpStatus status;
		GpPointF* pointsF;

		return GpStatus::NotImplemented;
	}

	/*
	 * Fills
	 */
	GpStatus WINGDIPAPI
		GdipFillEllipse(GpGraphics* graphics, GpBrush* brush, REAL x, REAL y, REAL width, REAL height)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (!brush)
			return GpStatus::InvalidParameter;
#endif
		gdip_brush_setup(graphics, brush);
		nvgBeginPath(graphics->ctx);
		nvgEllipse(graphics->ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f);
		nvgFill(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillEllipseI(GpGraphics* graphics, GpBrush* brush, INT x, INT y, INT width, INT height)
	{
		return GdipFillEllipse(graphics, brush, (REAL)x, (REAL)y, (REAL)width, (REAL)height);
	}

	GpStatus WINGDIPAPI
		GdipFillRectangle(GpGraphics* graphics, GpBrush* brush, REAL x, REAL y, REAL width, REAL height)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !brush)
			return GpStatus::InvalidParameter;
#endif
		gdip_brush_setup(graphics, brush);
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, x, y, width, height);
		nvgFill(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillRectangleI(GpGraphics* graphics, GpBrush* brush, INT x, INT y, INT width, INT height)
	{
		return GdipFillRectangle(graphics, brush, (REAL)x, (REAL)y, (REAL)width, (REAL)height);
	}

	GpStatus WINGDIPAPI
		GdipFillRectangles(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpRectF* rects, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !rects || count <= 0)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (!brush)
			return GpStatus::InvalidParameter;
#endif
		for (int r = 0; r < count; ++r) {
			GpStatus result = GdipFillRectangle(graphics, brush, rects[r].X, rects[r].Y, rects[r].Width, rects[r].Height);
			if (result != GpStatus::Ok) {
				return result;
			}
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillRectanglesI(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpRect* rects, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		if (count < 0)
			return GpStatus::OutOfMemory;
		if (!rects)
			return GpStatus::InvalidParameter;
#endif
		GpStatus status;
		GpRectF* rectsF;

		for (int r = 0; r < count; ++r) {
			GpStatus result = GdipFillRectangleI(graphics, brush, rects[r].X, rects[r].Y, rects[r].Width, rects[r].Height);
			if (result != GpStatus::Ok) {
				return result;
			}
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillPie(GpGraphics* graphics, GpBrush* brush, REAL x, REAL y, REAL width, REAL height,
			REAL startAngle, REAL sweepAngle)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !brush)
			return GpStatus::InvalidParameter;

		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (width <= 0 || height <= 0)
			return GpStatus::InvalidParameter;
#endif

		// Don't do anything if sweep angle is zero.
		if (sweepAngle == 0)
			return GpStatus::Ok;

		bool antialiasing = false;
		gdip_brush_setup(graphics, brush);
		nvgBeginPath(graphics->ctx);
		make_pie(graphics, x, y, width, height, startAngle, sweepAngle, antialiasing);
		nvgFill(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillPieI(GpGraphics* graphics, GpBrush* brush, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return GdipFillPie(graphics, brush, (REAL)x, (REAL)y, (REAL)width, (REAL)height, startAngle, sweepAngle);
	}

	GpStatus WINGDIPAPI
		GdipFillPath(GpGraphics* graphics, GpBrush* brush, GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!brush || !path)
			return GpStatus::InvalidParameter;
#endif
		if (path && path->count > 0) {
			gdip_brush_setup(graphics, brush);

			gdip_apply_path(graphics, path);
			nvgFill(graphics->ctx);
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillPolygon(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count, GpFillMode fillMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !points || count <= 0)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!brush || fillMode > GpFillMode::FillModeWinding)
			return GpStatus::InvalidParameter;
#endif
		// Don't do anything if sweep angle is zero.
		if (count < 2)
			return GpStatus::Ok;

		gdip_brush_setup(graphics, brush);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(graphics->ctx, points[i].X, points[i].Y);
		nvgClosePath(graphics->ctx);
		nvgFill(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillPolygonI(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count, GpFillMode fillMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		if (count < 0)
			return GpStatus::OutOfMemory;
		if (!points)
			return GpStatus::InvalidParameter;
#endif
		GpStatus status;
		GpPointF* pointsF;
		gdip_brush_setup(graphics, brush);
		nvgBeginPath(graphics->ctx);
		nvgMoveTo(graphics->ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(graphics->ctx, points[i].X, points[i].Y);
		nvgClosePath(graphics->ctx);
		nvgFill(graphics->ctx);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipFillPolygon2(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count)
	{
		return GdipFillPolygon(graphics, brush, points, count, GpFillMode::FillModeAlternate);
	}

	GpStatus WINGDIPAPI
		GdipFillPolygon2I(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count)
	{
		return GdipFillPolygonI(graphics, brush, points, count, GpFillMode::FillModeAlternate);
	}

	GpStatus WINGDIPAPI
		GdipFillClosedCurve(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count)
	{
		return GdipFillClosedCurve2(graphics, brush, points, count, 0.5f, GpFillMode::FillModeAlternate);
	}

	GpStatus WINGDIPAPI
		GdipFillClosedCurveI(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count)
	{
		return GdipFillClosedCurve2I(graphics, brush, points, count, 0.5f, GpFillMode::FillModeAlternate);
	}

	GpStatus WINGDIPAPI
		GdipFillClosedCurve2(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count, REAL tension, GpFillMode fillMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !points || count <= 0)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!brush || fillMode > GpFillMode::FillModeWinding)
			return GpStatus::InvalidParameter;
#endif

		if (count < 3)
			return GpStatus::Ok;


		if (tension == 0)
			return GdipFillPolygon2(graphics, brush, points, count);
		else
		{
			return GpStatus::NotImplemented;
		}

	}

	GpStatus WINGDIPAPI
		GdipFillClosedCurve2I(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count, REAL tension, GpFillMode fillMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		if (count < 0)
			return GpStatus::OutOfMemory;
		if (!points)
			return GpStatus::InvalidParameter;
#endif
		GpStatus status;
		GpPointF* pointsF;
		pointsF = convert_points(points, count);
		if (!pointsF)
			return GpStatus::OutOfMemory;

		status = GdipFillClosedCurve2(graphics, brush, pointsF, count, tension, fillMode);

		GdipFree(pointsF);
		return status;
	}

	GpStatus WINGDIPAPI
		GdipFillRegion(GpGraphics* graphics, GpBrush* brush, GpRegion* region)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !brush || !region)
			return GpStatus::InvalidParameter;
#endif
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetRenderingOrigin(GpGraphics* graphics, INT x, INT y)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;

		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
#endif
		graphics->render_origin_x = x;
		graphics->render_origin_y = y;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetRenderingOrigin(GpGraphics* graphics, INT* x, INT* y)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !x || !y)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		* x = graphics->render_origin_x;
		*y = graphics->render_origin_y;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetDpiX(GpGraphics* graphics, REAL* dpi)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !dpi)
			return GpStatus::InvalidParameter;
#endif
		//if (graphics->state == GraphicsStateBusy)
		//	return GpStatus::ObjectBusy;

		* dpi = graphics->dpi_x;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetDpiY(GpGraphics* graphics, REAL* dpi)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !dpi)
			return GpStatus::InvalidParameter;
#endif
		//if (graphics->state == GraphicsStateBusy)
		//	return GpStatus::ObjectBusy;

		* dpi = graphics->dpi_y;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGraphicsClear(GpGraphics* graphics, ARGB color)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		nvgSave(graphics->ctx);
		nvgResetTransform(graphics->ctx);

		nvgFillColor(graphics->ctx, ConvertARGB(color));
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, 0, 0, 1920, 1080);
		nvgFill(graphics->ctx);

		nvgRestore(graphics->ctx);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetInterpolationMode(GpGraphics* graphics, GpInterpolationMode interpolationMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (interpolationMode <= GpInterpolationMode::InterpolationModeInvalid || interpolationMode > GpInterpolationMode::InterpolationModeHighQualityBicubic)
			return GpStatus::InvalidParameter;
#endif
		switch (interpolationMode) {
		case GpInterpolationMode::InterpolationModeDefault:
		case GpInterpolationMode::InterpolationModeLowQuality:
			graphics->interpolation = GpInterpolationMode::InterpolationModeBilinear;
			break;
		case GpInterpolationMode::InterpolationModeHighQuality:
			graphics->interpolation = GpInterpolationMode::InterpolationModeHighQualityBicubic;
			break;
		default:
			graphics->interpolation = interpolationMode;
			break;
		}

		//@sroo: todo verify: apply to nanoVG? the interpolation options are more restrictive
		// ...
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetInterpolationMode(GpGraphics* graphics, GpInterpolationMode* interpolationMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		//if (graphics->state == GraphicsStateBusy)
		//	return GpStatus::ObjectBusy;
		if (*interpolationMode <= GpInterpolationMode::InterpolationModeInvalid || *interpolationMode > GpInterpolationMode::InterpolationModeHighQualityBicubic)
			return GpStatus::InvalidParameter;

#endif
		switch (*interpolationMode) {
		case GpInterpolationMode::InterpolationModeDefault:
		case GpInterpolationMode::InterpolationModeLowQuality:
			graphics->interpolation = GpInterpolationMode::InterpolationModeBilinear;
			break;
		case GpInterpolationMode::InterpolationModeHighQuality:
			graphics->interpolation = GpInterpolationMode::InterpolationModeHighQualityBicubic;
			break;
		default:
			graphics->interpolation = *interpolationMode;
			break;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetTextRenderingHint(GpGraphics* graphics, GpTextRenderingHint mode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (mode > GpTextRenderingHint::TextRenderingHintClearTypeGridFit)
			return GpStatus::InvalidParameter;
#endif
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetTextRenderingHint(GpGraphics* graphics, GpTextRenderingHint* mode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !mode)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/

		return GpStatus::NotImplemented;
	}

	/* MonoTODO - pixel offset mode isn't supported */
	GpStatus WINGDIPAPI
		GdipSetPixelOffsetMode(GpGraphics* graphics, GpPixelOffsetMode pixelOffsetMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (pixelOffsetMode <= GpPixelOffsetMode::PixelOffsetModeInvalid || pixelOffsetMode > GpPixelOffsetMode::PixelOffsetModeHalf)
			return GpStatus::InvalidParameter;
#endif
		//graphics->pixel_mode = pixelOffsetMode;

		return GpStatus::NotImplemented;
	}

	/* MonoTODO - pixel offset mode isn't supported */
	GpStatus WINGDIPAPI
		GdipGetPixelOffsetMode(GpGraphics* graphics, GpPixelOffsetMode* pixelOffsetMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !pixelOffsetMode)
			return GpStatus::InvalidParameter;

		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
#endif
		//*pixelOffsetMode = graphics->pixel_mode;
		return GpStatus::NotImplemented;
	}

	/* MonoTODO - text contrast isn't supported */
	GpStatus WINGDIPAPI
		GdipSetTextContrast(GpGraphics* graphics, UINT contrast)
	{
		// The gamma correction value must be less than 12.
		// The default value is 4.
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (contrast > 12)
			return GpStatus::InvalidParameter;

		//graphics->text_contrast = contrast;

		return GpStatus::NotImplemented;
	}

	/* MonoTODO - text contrast isn't supported */
	GpStatus WINGDIPAPI
		GdipGetTextContrast(GpGraphics* graphics, UINT* contrast)
	{
#ifdef GDIFLAT_SAFE

		if (!graphics || !graphics->ctx || !contrast)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
				return GpStatus::ObjectBusy;*/

		//*contrast = graphics->text_contrast;
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetSmoothingMode(GpGraphics* graphics, GpSmoothingMode smoothingMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (smoothingMode <= GpSmoothingMode::SmoothingModeInvalid || smoothingMode > GpSmoothingMode::SmoothingModeAntiAlias + 1)
			return GpStatus::InvalidParameter;
#endif
		switch (smoothingMode) {
		case GpSmoothingMode::SmoothingModeDefault:
		case GpSmoothingMode::SmoothingModeHighSpeed:
			graphics->draw_mode = GpSmoothingMode::SmoothingModeNone;
			break;
		case GpSmoothingMode::SmoothingModeHighQuality:
			graphics->draw_mode = GpSmoothingMode::SmoothingModeAntiAlias;
			break;
		default:
			graphics->draw_mode = smoothingMode;
			break;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetSmoothingMode(GpGraphics* graphics, GpSmoothingMode* smoothingMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !smoothingMode)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		* smoothingMode = graphics->draw_mode;
		return GpStatus::Ok;
	}

	/* MonoTODO - dstrect, srcrect and unit support isn't implemented */
	GpStatus WINGDIPAPI
		GdipBeginContainer(GpGraphics* graphics, GDIPCONST GpRectF* dstrect, GDIPCONST GpRectF* srcrect, GpUnit unit, GpGraphicsContainer* state)
	{
		if (!graphics || !dstrect || !srcrect || (unit < GpUnit::UnitPixel) || (unit > GpUnit::UnitMillimeter))
			return GpStatus::InvalidParameter;

		return GdipBeginContainer2(graphics, state);
	}

	GpStatus WINGDIPAPI
		GdipBeginContainer2(GpGraphics* graphics, GpGraphicsContainer* state)
	{
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !state)
			return GpStatus::InvalidParameter;
#endif
		status = GdipSaveGraphics(graphics, state);
		if (status == GpStatus::Ok) {
			if (graphics->previous_clip) {
				GdipDeleteRegion(graphics->previous_clip);
				graphics->previous_clip = graphics->overall_clip;
				graphics->overall_clip = NULL;
			}
			else if (!gdip_is_InfiniteRegion(graphics->clip)) {
				GpRegion* clip;
				status = GdipCloneRegion(graphics->clip, &clip);
				if (status != GpStatus::Ok)
					return status;
				graphics->previous_clip = clip;
			}

			// reset most properties to defaults after saving them
			gdip_graphics_reset(graphics);
			// copy the current effective matrix as the preivous matrix
			gdip_matrix_copy(&graphics->previous_matrix, graphics->copy_of_ctm);
		}
		return status;
	}

	GpStatus WINGDIPAPI
		GdipBeginContainerI(GpGraphics* graphics, GDIPCONST GpRect* dstrect, GDIPCONST GpRect* srcrect, GpUnit unit, GpGraphicsContainer* state)
	{
		GpRectF dstrectF;
		GpRectF srcrectF;
#ifdef GDIFLAT_SAFE
		if (!dstrect || !srcrect)
			return GpStatus::InvalidParameter;
#endif
		gdip_RectF_from_Rect(dstrect, &dstrectF);
		gdip_RectF_from_Rect(srcrect, &srcrectF);
		return GdipBeginContainer(graphics, &dstrectF, &srcrectF, unit, state);
	}

	GpStatus WINGDIPAPI
		GdipEndContainer(GpGraphics* graphics, GpGraphicsContainer state)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		return GdipRestoreGraphics(graphics, state);
	}

	GpStatus WINGDIPAPI
		GdipFlush(GpGraphics* graphics, GpFlushIntention intention)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state != GraphicsStateValid)
			return GpStatus::ObjectBusy;
		*/
		nvgEndFrame(graphics->ctx);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetClipGraphics(GpGraphics* graphics, GpGraphics* srcgraphics, GpCombineMode combineMode)
	{
#ifdef GDIFLAT_SAFE

		if (!graphics)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (!srcgraphics)
			return GpStatus::InvalidParameter;
#endif
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetClipRect(GpGraphics* graphics, REAL x, REAL y, REAL width, REAL height, GpCombineMode combineMode)
	{
		GpStatus status;

#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (combineMode > GpCombineMode::CombineModeComplement)
			return GpStatus::InvalidParameter;
#endif
#ifdef DEBUG_CLIP_MASK
		{
			nvgSetClipped(graphics->ctx, false);
			nvgBeginPath(graphics->ctx);
			nvgRect(graphics->ctx, x, y, width, height);
			nvgStrokeColor(graphics->ctx, ConvertARGB(Color::Red));
			nvgStroke(graphics->ctx);
			nvgFillColor(graphics->ctx, nvgRGBA(255, 0, 0, 50));
			nvgFill(graphics->ctx);
			nvgSetClipped(graphics->ctx, graphics->clipped);
		}
#endif

		SetClipMode(graphics, combineMode);
		int prevPath = nvgCurrentPath(graphics->ctx);
		nvgSelectPath(graphics->ctx, 1);
		nvgSetBuffer(graphics->ctx, 1);
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, x, y, width, height);
		nvgFillColor(graphics->ctx, nvgRGBA(255,255,255,255));
		nvgFill(graphics->ctx);
		nvgSetBuffer(graphics->ctx, 0);
		nvgSelectPath(graphics->ctx, prevPath);
		graphics->clipped = true;
		nvgSetClipped(graphics->ctx, true);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetClipRectI(GpGraphics* graphics, INT x, INT y, INT width, INT height, GpCombineMode combineMode)
	{
		return GdipSetClipRect(graphics, (REAL)x, (REAL)y, (REAL)width, (REAL)height, combineMode);
	}

	GpStatus WINGDIPAPI
		GdipSetClipPath(GpGraphics* graphics, GpPath* path, GpCombineMode combineMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
			*/
		if (!path || combineMode > GpCombineMode::CombineModeComplement)
			return GpStatus::InvalidParameter;
#endif
		if (path->count > 0) {

#ifdef DEBUG_CLIP_MASK
			{
				nvgSetClipped(graphics->ctx, false);
				nvgBeginPath(graphics->ctx);
				gdip_apply_path(graphics, path);
				nvgStrokeColor(graphics->ctx, ConvertARGB(Color::Red));
				nvgStroke(graphics->ctx);
				nvgFillColor(graphics->ctx, nvgRGBA(255, 0, 0, 50));
				nvgFill(graphics->ctx);
				nvgSetClipped(graphics->ctx, graphics->clipped);
			}
#endif
			SetClipMode(graphics, combineMode);
			// prepare to draw on stencil
			nvgSetBuffer(graphics->ctx, 1);
			int prevPath = nvgCurrentPath(graphics->ctx);
			nvgSelectPath(graphics->ctx, 1);
			nvgBeginPath(graphics->ctx);
			// set path
			gdip_apply_path(graphics, path);
			// fill 
			nvgFillColor(graphics->ctx, nvgRGBA(255, 255, 255, 255));
			nvgFill(graphics->ctx);
			// restore
			nvgSetBuffer(graphics->ctx, 0);
			nvgSelectPath(graphics->ctx, prevPath);
			// mark as clipped
			graphics->clipped = true;
			nvgSetClipped(graphics->ctx, true);
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetClipRegion(GpGraphics* graphics, GpRegion* region, GpCombineMode combineMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!region || combineMode > GpCombineMode::CombineModeComplement)
			return GpStatus::InvalidParameter;
#endif
		return GpStatus::NotImplemented;
	}

	/* Note: not exposed in System.Drawing.dll */
	GpStatus WINGDIPAPI
		GdipSetClipHrgn(GpGraphics* graphics, void* hRgn, GpCombineMode combineMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (!hRgn)
			return GpStatus::InvalidParameter;
#endif
		return GdipSetClipRegion(graphics, (GpRegion*)hRgn, combineMode);
	}

	GpStatus WINGDIPAPI
		GdipResetClip(GpGraphics* graphics)
	{
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		GdipSetInfinite(graphics->clip);
		if (!gdip_is_matrix_empty(&graphics->previous_matrix)) {
			// inside a container only reset to the previous transform
			gdip_matrix_copy(graphics->clip_matrix, &graphics->previous_matrix);
			GdipInvertMatrix(graphics->clip_matrix);
		}
		else {
			nvgTransformIdentity(graphics->clip_matrix->m);
		}
		status = gdip_calculate_overall_clipping(graphics);
		if (status != GpStatus::Ok)
			return status;

		if (graphics->ctx && graphics->clipped) {
			nvgResetStencil(graphics->ctx);
			nvgResetScissor(graphics->ctx);
			graphics->clipped = FALSE;
			nvgSetClipped(graphics->ctx, FALSE);
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipTranslateClip(GpGraphics* graphics, REAL dx, REAL dy)
	{
		GpStatus status;

#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		status = GdipTranslateRegion(graphics->clip, dx, dy);
		if (status != GpStatus::Ok)
			return status;

		status = gdip_calculate_overall_clipping(graphics);
		if (status != GpStatus::Ok)
			return status;

		//@sroo: todo verify :apply to backend directly?

		return status;
	}

	GpStatus WINGDIPAPI
		GdipTranslateClipI(GpGraphics* graphics, INT dx, INT dy)
	{
		return GdipTranslateClip(graphics, (REAL)dx, (REAL)dy);
	}

	GpStatus WINGDIPAPI
		GdipGetClip(GpGraphics* graphics, GpRegion* region)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !region)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		gdip_clear_region(region);
		gdip_copy_region(graphics->clip, region);

		if (gdip_is_matrix_empty(graphics->clip_matrix))
			return GpStatus::Ok;
		return GdipTransformRegion(region, graphics->clip_matrix);
	}

	GpStatus WINGDIPAPI
		GdipGetClipBounds(GpGraphics* graphics, GpRectF* rect)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !rect)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
#endif
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetClipBoundsI(GpGraphics* graphics, GpRect* rect)
	{
		GpRectF rectF;
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!rect)
			return GpStatus::InvalidParameter;
#endif
		status = GdipGetClipBounds(graphics, &rectF);
		if (status != GpStatus::Ok)
			return status;

		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipIsClipEmpty(GpGraphics* graphics, BOOL* result)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !result)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		return GdipIsEmptyRegion(graphics->clip, graphics, result);
	}

	GpStatus WINGDIPAPI
		GdipGetVisibleClipBounds(GpGraphics* graphics, GpRectF* rect)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !rect)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		GpStatus status = GpStatus::Ok;
		GpRegion* clip = graphics->overall_clip;
		BOOL empty;

		// The clip bounds for empty bounds should be translated.
		GdipIsEmptyRegion(clip, graphics, &empty);
		if (empty) {
			status = GdipGetRegionBounds(clip, graphics, rect);
			if (status != GpStatus::Ok)
				goto cleanup;

			rect->X += graphics->clip_matrix->m[4];
			rect->Y += graphics->clip_matrix->m[5];
		}
		else if (!gdip_is_InfiniteRegion(clip)) {
			// if the matrix is empty, avoid region cloning and transform 
			if (!gdip_is_matrix_empty(graphics->clip_matrix)) {
				GdipCloneRegion(graphics->overall_clip, &clip);
				GdipTransformRegion(clip, graphics->clip_matrix);
			}

			GpRectF clipbound;
			status = GdipGetRegionBounds(clip, graphics, &clipbound);
			if (status != GpStatus::Ok)
				goto cleanup;

			// intersect clipping with bounds (for clips bigger than the graphics) 
			rect->X = (clipbound.X > graphics->bounds.X) ? clipbound.X : graphics->bounds.X;
			rect->Y = (clipbound.Y > graphics->bounds.Y) ? clipbound.Y : graphics->bounds.Y;
			rect->Width = (((clipbound.X + clipbound.Width) < (graphics->bounds.X + graphics->bounds.Width)) ?
				(clipbound.X + clipbound.Width) : (graphics->bounds.X + graphics->bounds.Width)) - rect->X;
			rect->Height = (((clipbound.Y + clipbound.Height) < (graphics->bounds.Y + graphics->bounds.Height)) ?
				(clipbound.Y + clipbound.Height) : (graphics->bounds.Y + graphics->bounds.Height)) - rect->Y;
		}
		else {
			rect->X = graphics->bounds.X;
			rect->Y = graphics->bounds.Y;
			rect->Width = graphics->bounds.Width;
			rect->Height = graphics->bounds.Height;
		}

	cleanup:
		if (clip != graphics->overall_clip)
			GdipDeleteRegion(clip);
		return status;
	}

	GpStatus WINGDIPAPI
		GdipGetVisibleClipBoundsI(GpGraphics* graphics, GpRect* rect)
	{
		GpStatus status;
		GpRectF rectF;
#ifdef GDIFLAT_SAFE
		if (!graphics || !rect)
			return GpStatus::InvalidParameter;
#endif
		status = GdipGetVisibleClipBounds(graphics, &rectF);
		if (status != GpStatus::Ok)
			return status;

		gdip_Rect_from_RectF(&rectF, rect);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipIsVisibleClipEmpty(GpGraphics* graphics, BOOL* result)
	{
		GpRectF visibleClipBounds;
#ifdef GDIFLAT_SAFE
		if (!graphics || !result)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		GdipGetVisibleClipBounds(graphics, &visibleClipBounds);

		*result = visibleClipBounds.Width == 0 || visibleClipBounds.Height == 0;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipIsVisiblePoint(GpGraphics* graphics, REAL x, REAL y, BOOL* result)
	{
		GpRegion* visible_clip;
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!graphics || !result)
			return GpStatus::InvalidParameter;
#endif
		status = gdip_get_visible_clip(graphics, &visible_clip);
		if (status != GpStatus::Ok)
			return status;

		status = GdipIsVisibleRegionPoint(visible_clip, x, y, graphics, result);

		GdipDeleteRegion(visible_clip);
		return status;
	}

	GpStatus WINGDIPAPI
		GdipIsVisiblePointI(GpGraphics* graphics, INT x, INT y, BOOL* result)
	{
		GpRegion* visible_clip;
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!graphics || !result)
			return GpStatus::InvalidParameter;
#endif
		status = gdip_get_visible_clip(graphics, &visible_clip);
		if (status != GpStatus::Ok)
			return status;

		status = GdipIsVisibleRegionPoint(visible_clip, x, y, graphics, result);

		GdipDeleteRegion(visible_clip);
		return status;
	}

	GpStatus WINGDIPAPI
		GdipIsVisibleRect(GpGraphics* graphics, REAL x, REAL y, REAL width, REAL height, BOOL* result)
	{
		GpRegion* visible_clip;
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!graphics || !result)
			return GpStatus::InvalidParameter;
#endif
		if (width == 0 || height == 0) {
			*result = FALSE;
			return GpStatus::Ok;
		}

		status = gdip_get_visible_clip(graphics, &visible_clip);
		if (status != GpStatus::Ok)
			return status;

		status = GdipIsVisibleRegionRect(visible_clip, x, y, width, height, graphics, result);

		GdipDeleteRegion(visible_clip);
		return status;

	}

	GpStatus WINGDIPAPI
		GdipIsVisibleRectI(GpGraphics* graphics, INT x, INT y, INT width, INT height, BOOL* result)
	{
		return GdipIsVisibleRect(graphics, (REAL)x, (REAL)y, (REAL)width, (REAL)height, result);
	}

	GpStatus WINGDIPAPI
		GdipSetCompositingMode(GpGraphics* graphics, GpCompositingMode compositingMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		graphics->composite_mode = compositingMode;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetCompositingMode(GpGraphics* graphics, GpCompositingMode* compositingMode)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !compositingMode)
			return GpStatus::InvalidParameter;

#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		* compositingMode = graphics->composite_mode;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetCompositingQuality(GpGraphics* graphics, GpCompositingQuality compositingQuality)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		graphics->composite_quality = compositingQuality;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetCompositingQuality(GpGraphics* graphics, GpCompositingQuality* compositingQuality)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !compositingQuality)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		* compositingQuality = graphics->composite_quality;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetNearestColor(GpGraphics* graphics, ARGB* argb)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipResetPageTransform(GpGraphics* graphics)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPageScale(GpGraphics* graphics, REAL scale)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/
		if (scale <= 0.0 || scale > 1.0E+9)
			return GpStatus::InvalidParameter;
#endif
		graphics->scale = scale;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPageScale(GpGraphics* graphics, REAL* scale)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !scale)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		* scale = graphics->scale;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPageUnit(GpGraphics* graphics, GpUnit unit)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		graphics->page_unit = unit;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPageUnit(GpGraphics* graphics, GpUnit* unit)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !unit)
			return GpStatus::InvalidParameter;
#endif
		/*if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;*/

		* unit = graphics->page_unit;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipTransformPoints(GpGraphics* graphics, GpCoordinateSpace destSpace, GpCoordinateSpace srcSpace, GpPointF* points, INT count)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipTransformPointsI(GpGraphics* graphics, GpCoordinateSpace destSpace, GpCoordinateSpace srcSpace, GpPoint* points, INT count)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileDestPoint(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPointF* destPoint,
			EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileDestPointI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPoint* destPoint,
			EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileDestRect(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpRectF* destRect,
			EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileDestRectI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpRect* destRect,
			EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileDestPoints(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPointF* destPoints, INT count,
			EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileDestPointsI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPoint* destPoints, INT count,
			EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileSrcRectDestPoint(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPointF* destPoint, GDIPCONST GpRectF* srcRect,
			GpUnit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileSrcRectDestPointI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPoint* destPoint, GDIPCONST GpRect* srcRect,
			GpUnit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileSrcRectDestRect(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpRectF* destRect, GDIPCONST GpRectF* srcRect,
			GpUnit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileSrcRectDestRectI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpRect* destRect, GDIPCONST GpRect* srcRect,
			GpUnit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileSrcRectDestPoints(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPointF* destPoints, INT count, GDIPCONST GpRectF* srcRect,
			GpUnit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipEnumerateMetafileSrcRectDestPointsI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST GpPoint* destPoints, INT count, GDIPCONST GpRect* srcRect,
			GpUnit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes)
	{
		return GpStatus::NotImplemented;
	}

	HPALETTE WINGDIPAPI
		GdipCreateHalftonePalette()
	{
		return NULL;
	}
}