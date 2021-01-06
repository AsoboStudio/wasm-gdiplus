/*
 * pen.cpp
 *
 * Copyright (c) 2003 Alexandre Pigolkine
 * Copyright (C) 2006 Novell, Inc (http://www.novell.com)
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
 *   Alexandre Pigolkine(pigolkine@gmx.de)
 *   Duncan Mak (duncan@ximian.com)
 *   Ravindra (rkumar@novell.com)
 *	Sol Roo (sroo@asobostudio.com)
 */

#include "FlatNVG\pen-private.h"
#include "FlatNVG\solidbrush-private.h"
#include "FlatNVG\matrix-private.h"
#include "FlatNVG\general-private.h"
#include "FlatNVG\graphics-private.h"
#include "FlatNVG\customlinecap-private.h"


static void
gdip_pen_init(GpPen* pen)
{
	pen->color = 0;
	pen->brush = NULL;
	pen->own_brush = FALSE;
	pen->width = 1;
	pen->miter_limit = 10;
	pen->line_join = GpLineJoin::LineJoinMiter;
	pen->dash_style = GpDashStyle::DashStyleSolid;
	pen->line_cap = GpLineCap::LineCapFlat;
	pen->end_cap = GpLineCap::LineCapFlat;		/* ignored, Cairo only support a single start/end line cap */
	pen->dash_cap = GpDashCap::DashCapFlat;		/* ignored */
	pen->mode = GpPenAlignment::PenAlignmentCenter;
	pen->dash_offset = 0;
	pen->dash_count = 0;
	pen->own_dash_array = FALSE;
	pen->dash_array = NULL;
	pen->compound_count = 0;
	pen->compound_array = NULL;
	pen->unit = GpUnit::UnitWorld;
	pen->changed = TRUE;
	pen->custom_start_cap = NULL;
	pen->custom_end_cap = NULL;
	//cairo_matrix_init_identity(&pen->matrix);
}

static GpPen*
gdip_pen_new(void)
{
	GpPen* result = (GpPen*)DLLExports::GdipAlloc(sizeof(GpPen));

	if (result)
		gdip_pen_init(result);

	return result;
}
#if 0
static cairo_line_join_t
convert_line_join(GpLineJoin join)
{
	switch (join) {

	case LineJoinMiter:
		return CAIRO_LINE_JOIN_MITER;

	case LineJoinBevel:
		return CAIRO_LINE_JOIN_BEVEL;

	case LineJoinRound:
		return CAIRO_LINE_JOIN_ROUND;

	case LineJoinMiterClipped:
	default:
		return CAIRO_LINE_JOIN_MITER;
	}
}

static cairo_line_cap_t
convert_line_cap(GpPen* pen)
{
	switch (pen->line_cap) {

		/* HACK - this keeps SWF (mostly) happy with results very similar to GDI+
		 * (under those specific cases) and also keeps the pen's functionalities
		 * on par with GDI+
		 */
	case LineCapFlat:
		if (pen->dash_array || (pen->width > 1.0))
			return CAIRO_LINE_CAP_BUTT;
		else
			return CAIRO_LINE_CAP_SQUARE;

	case LineCapSquare:
		return CAIRO_LINE_CAP_SQUARE;

	case LineCapRound:
		return CAIRO_LINE_CAP_ROUND;

	case LineCapTriangle:
	case LineCapNoAnchor:
	case LineCapSquareAnchor:
	case LineCapRoundAnchor:
	case LineCapDiamondAnchor:
	case LineCapArrowAnchor:
	case LineCapCustom:
	default:
		return CAIRO_LINE_CAP_BUTT;
	}
}

static double*
convert_dash_array(float* f, double width, int count)
{
	double* retval = GdipAlloc(sizeof(double) * count);
	if (!retval)
		return NULL;

	int i;
	for (i = 0; i < count; i++) {
		retval[i] = (double)f[i] * width;
	}

	return retval;
}

GpStatus
gdip_pen_setup(GpGraphics* graphics, GpPen* pen)
{
	GpStatus status;
	cairo_matrix_t product;
	double widthx, widthy;

	if (!graphics || !pen)
		return GpStatus::InvalidParameter;

	status = gdip_brush_setup(graphics, pen->brush);
	if (status != GpStatus::Ok)
		return status;

	cairo_matrix_init_identity(&product);

	/* Here we use product of pen->matrix and graphics->copy_of_ctm.
	 * This gives us absolute results with respect to graphics. We
	 * do following irrespective of the pen->changed state since graphics
	 * has its own matrix and we need to multiply that with pen->matrix
	 * every time we perform stroke operations. Graphics matrix gets
	 * reset to its own state after stroking.
	 */
	cairo_matrix_multiply(&product, &pen->matrix, graphics->copy_of_ctm);
	/* Pen scaling by 0 are supported by MS GDI+ but would error in Cairo, see bug #338233 */
	if (gdip_near_zero(product.xx) || gdip_near_zero(product.yy)) {
		/* *both* X and Y are affected if either is 0 */
		product.xx = product.yy = 0.0001f;
	}
	gdip_cairo_set_matrix(graphics, &product);

	/* Don't need to setup, if pen is the same as the cached pen and
	 * it is not changed. Just comparing pointers may not be sufficient
	 * to say that the pens are same. It is possible to have different
	 * pen on the same memory, but probability is very low. We would
	 * need a function to check the equality of the pens in that case.
	 */
	if (pen == graphics->last_pen && !pen->changed)
		return GpStatus::Ok;

	widthx = 1.0;
	widthy = 1.0;
	cairo_device_to_user_distance(graphics->ct, &widthx, &widthy);
	widthx = fmax(fabs(widthx), fabs(widthy));

	if (pen->width > widthx) { /* we draw a pixel wide line if the output width is < 1.0 */
		widthx = (double)pen->width;
	}
	cairo_set_line_width(graphics->ct, widthx);

	cairo_set_miter_limit(graphics->ct, (double)pen->miter_limit);
	cairo_set_line_join(graphics->ct, convert_line_join(pen->line_join));
	cairo_set_line_cap(graphics->ct, convert_line_cap(pen));

	if (pen->dash_count > 0) {
		double* dash_array;

		/* note: pen->width may be different from what was used to
		call cairo_set_line_width, e.g. 0.0 (#78742) */
		dash_array = convert_dash_array(pen->dash_array, widthx, pen->dash_count);
		if (!dash_array)
			return GpStatus::OutOfMemory;

		cairo_set_dash(graphics->ct, dash_array, pen->dash_count, pen->dash_offset);
		DLLExports::GdipFree(dash_array);
	}
	else /* Clear the dashes, if set in previous calls */
		cairo_set_dash(graphics->ct, NULL, 0, 0);

	/* We are done with using all the changes in the pen. */
	pen->changed = FALSE;
	graphics->last_pen = pen;

	return gdip_get_status(cairo_status(graphics->ct));
}

GpStatus
gdip_pen_draw_custom_start_cap(GpGraphics* graphics, GpPen* pen, float x1, float y1, float x2, float y2)
{
	if (!graphics || !pen)
		return GpStatus::InvalidParameter;

	if (pen->custom_start_cap) {
		/* Draw the end cap */
		gdip_linecap_draw(graphics, pen, pen->custom_start_cap, x1, y1, x2, y2);
	}

	return gdip_get_status(cairo_status(graphics->ct));
}

GpStatus
gdip_pen_draw_custom_end_cap(GpGraphics* graphics, GpPen* pen, float x1, float y1, float x2, float y2)
{
	if (!graphics || !pen)
		return GpStatus::InvalidParameter;

	if (pen->custom_end_cap) {
		/* Draw the end cap */
		gdip_linecap_draw(graphics, pen, pen->custom_end_cap, x1, y1, x2, y2);
	}

	return gdip_get_status(cairo_status(graphics->ct));
}
#endif
namespace DLLExports {

	extern BOOL gdiplusInitialized;

	// coverity[+alloc : arg-*3]
	GpStatus WINGDIPAPI
		GdipCreatePen1(ARGB argb, REAL width, GpUnit unit, GpPen** pen)
	{
		GpStatus status;
		GpPen* result;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		if (!pen /*|| unit > GpUnit::UnitCairoPoint*/ || unit == GpUnit::UnitDisplay)
			return GpStatus::InvalidParameter;

		result = gdip_pen_new();
		if (!result) {
			*pen = NULL;
			return GpStatus::OutOfMemory;
		}

		result->color = argb;
		result->width = width;
		result->unit = unit;
		result->own_brush = TRUE;

		status = GdipCreateSolidFill(argb, (GpSolidFill**)&result->brush);
		if (status != GpStatus::Ok) {
			GdipDeletePen(result);
			*pen = NULL;
			return status;
		}

		*pen = result;
		return GpStatus::Ok;
	}

	// coverity[+alloc : arg-*3]
	GpStatus WINGDIPAPI
		GdipCreatePen2(GpBrush* brush, REAL width, GpUnit unit, GpPen** pen)
	{
		GpPen* result;
		GpStatus status;
		GpBrushType type;
		ARGB color;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		if (!brush || !pen /*|| unit > GpUnit::UnitCairoPoint*/ || unit == GpUnit::UnitDisplay)
			return GpStatus::InvalidParameter;

		result = gdip_pen_new();
		if (!result) {
			*pen = NULL;
			return GpStatus::OutOfMemory;
		}

		result->width = width;
		result->unit = unit;
		result->own_brush = TRUE;

		/* The user supplied brush can be disposed, we must clone it to ensure
		 * it's valid when we need to set the pen. */
		status = GdipCloneBrush(brush, &result->brush);
		if (status != GpStatus::Ok) {
			GdipDeletePen(result);
			*pen = NULL;
			return status;
		}

		GdipGetBrushType(brush, &type);
		if (type == GpBrushType::BrushTypeSolidColor) {
			GdipGetSolidFillColor((GpSolidFill*)brush, &color);
			result->color = color;
		}

		*pen = result;
		return GpStatus::Ok;
	}

	// coverity[+alloc : arg-*1]
	GpStatus WINGDIPAPI
		GdipClonePen(GpPen* pen, GpPen** clonepen)
	{
		GpPen* result;

		if (!pen || !clonepen)
			return GpStatus::InvalidParameter;

		result = gdip_pen_new();
		if (!result) {
			*clonepen = NULL;
			return GpStatus::OutOfMemory;
		}

		return GpStatus::NotImplemented;
#if 0
		result->own_brush = pen->own_brush;
		result->color = pen->color;
		result->width = pen->width;
		result->miter_limit = pen->miter_limit;
		result->line_join = pen->line_join;
		result->dash_style = pen->dash_style;
		result->line_cap = pen->line_cap;
		result->end_cap = pen->end_cap;
		result->mode = pen->mode;
		result->dash_offset = pen->dash_offset;
		result->dash_count = pen->dash_count;
		result->own_dash_array = pen->own_dash_array;
		result->compound_count = pen->compound_count;
		result->unit = pen->unit;
		//gdip_cairo_matrix_copy(&result->matrix, &pen->matrix);
		result->changed = pen->changed;

		/* Make a copy of dash array only if it is owned by the pen - i.e. it is not
		 * a global array. */
		if (pen->dash_count > 0 && pen->own_dash_array) {
			result->dash_array = (float*)GdipAlloc(pen->dash_count * sizeof(float));
			if (!result->dash_array)
				goto error;

			memcpy(result->dash_array, pen->dash_array, pen->dash_count * sizeof(float));
		}
		else
			result->dash_array = pen->dash_array;

		if (pen->compound_count > 0) {
			result->compound_array = (float*)GdipAlloc(pen->compound_count * sizeof(float));
			if (!result->compound_array)
				goto error;

			memcpy(result->compound_array, pen->compound_array, pen->compound_count * sizeof(float));
		}

		if (pen->custom_start_cap) {
			GpStatus status = GdipCloneCustomLineCap(pen->custom_start_cap, &result->custom_start_cap);
			if (status != GpStatus::Ok)
				goto error;
		}

		if (pen->custom_end_cap) {
			GpStatus status = GdipCloneCustomLineCap(pen->custom_end_cap, &result->custom_end_cap);
			if (status != GpStatus::Ok)
				goto error;
		}

		if (pen->own_brush) {
			GpSolidFill* oldBrush = (GpSolidFill*)pen->brush;
			GpStatus status = GdipCreateSolidFill(oldBrush->color, (GpSolidFill**)&result->brush);
			if (status != GpStatus::Ok)
				goto error;
		}
		else {
			result->brush = pen->brush;
		}

		*clonepen = result;
		return GpStatus::Ok;

	error:
		GdipDeletePen(result);
		*clonepen = NULL;
		return GpStatus::OutOfMemory;
#endif
	}

	GpStatus WINGDIPAPI
		GdipDeletePen(GpPen* pen)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		if (pen->dash_count != 0 && pen->own_dash_array) {
			delete (pen->dash_array);
			pen->dash_count = 0;
			pen->dash_array = NULL;
		}

		if (pen->own_brush && pen->brush) {
			GdipDeleteBrush(pen->brush);
			pen->brush = NULL;
		}

		if (pen->compound_count != 0) {
			delete (pen->compound_array);
			pen->compound_array = NULL;
			pen->compound_count = 0;
		}

		if (pen->custom_start_cap != NULL) {
			GdipDeleteCustomLineCap(pen->custom_start_cap);
			pen->custom_start_cap = NULL;
		}

		if (pen->custom_end_cap != NULL) {
			GdipDeleteCustomLineCap(pen->custom_end_cap);
			pen->custom_end_cap = NULL;
		}

		delete (pen);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenWidth(GpPen* pen, REAL width)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		if (pen->width != width) {
			pen->width = width;
			pen->changed = TRUE;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenWidth(GpPen* pen, REAL* width)
	{
		if (!pen || !width)
			return GpStatus::InvalidParameter;

		*width = pen->width;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenBrushFill(GpPen* pen, GpBrush* brush)
	{
		GpBrushType type;

		if (!pen || !brush)
			return GpStatus::InvalidParameter;

		GdipGetBrushType(brush, &type);
		if (type == GpBrushType::BrushTypeSolidColor)
			GdipGetSolidFillColor((GpSolidFill*)brush, &pen->color);
		else
			pen->color = 0;

		if (pen->own_brush && pen->brush)
			GdipDeleteBrush(pen->brush);

		pen->brush = brush;
		pen->changed = TRUE;
		pen->own_brush = FALSE;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenBrushFill(GpPen* pen, GpBrush** brush)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		return GdipCloneBrush(pen->brush, brush);
	}

	GpStatus WINGDIPAPI
		GdipGetPenFillType(GpPen* pen, GpPenType* type)
	{
		if (!pen || !type)
			return GpStatus::InvalidParameter;

		if (pen->brush)
			return GdipGetBrushType(pen->brush, (GpBrushType*)type);

		*type = GpPenType::PenTypeSolidColor;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenColor(GpPen* pen, ARGB argb)
	{
		GpStatus status;
		GpSolidFill* brush;
		GpBrushType type;

		if (!pen)
			return GpStatus::InvalidParameter;

		GdipGetBrushType(pen->brush, &type);
		if (type == GpBrushType::BrushTypeSolidColor) {
			ARGB brushColor;
			GdipGetSolidFillColor((GpSolidFill*)pen->brush, &brushColor);

			// Nothing to do.
			if (brushColor == argb)
				return GpStatus::Ok;
		}

		// Override the brush and set it to a solid fill.
		status = GdipCreateSolidFill(argb, &brush);
		if (status != GpStatus::Ok)
			return status;

		pen->color = argb;
		if (pen->own_brush)
			GdipDeleteBrush(pen->brush);
		pen->brush = (GpBrush*)brush;
		pen->own_brush = TRUE;
		pen->changed = TRUE;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenColor(GpPen* pen, ARGB* argb)
	{
		if (!pen || !argb)
			return GpStatus::InvalidParameter;

		if (pen->brush->vtable->type != GpBrushType::BrushTypeSolidColor)
			return GpStatus::InvalidParameter;

		*argb = pen->color;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenMiterLimit(GpPen* pen, REAL miterLimit)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		if (miterLimit < 1.0f)
			miterLimit = 1.0f;

		if (pen->miter_limit != miterLimit) {
			pen->miter_limit = miterLimit;
			pen->changed = TRUE;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenMiterLimit(GpPen* pen, REAL* miterLimit)
	{
		if (!pen || !miterLimit)
			return GpStatus::InvalidParameter;

		*miterLimit = pen->miter_limit;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenLineJoin(GpPen* pen, GpLineJoin lineJoin)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		if (pen->line_join != lineJoin) {
			pen->line_join = lineJoin;
			pen->changed = TRUE;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenLineJoin(GpPen* pen, GpLineJoin* lineJoin)
	{
		if (!pen || !lineJoin)
			return GpStatus::InvalidParameter;

		*lineJoin = pen->line_join;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenLineCap197819(GpPen* pen, GpLineCap startCap, GpLineCap endCap, GpDashCap dashCap)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		/* FIXME:
		 * Cairo supports only one cap for a line. We use startcap for that.
		 * Use end cap and dash cap when Cairo supports different caps for the
		 * line ends and dashcap.
		 */

		pen->line_cap = startCap;
		pen->end_cap = endCap;
		/* any invalid value is changed to GpDashCap::DashCapFlat */
		if ((dashCap == GpDashCap::DashCapRound) || (dashCap == GpDashCap::DashCapTriangle))
			pen->dash_cap = dashCap;
		else
			pen->dash_cap = GpDashCap::DashCapFlat; /* default */

		pen->changed = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenMode(GpPen* pen, GpPenAlignment penMode)
	{
		if (!pen)
			return GpStatus::InvalidParameter;
		if (penMode == GpPenAlignment::PenAlignmentInset && pen->compound_count > 0)
			return GpStatus::NotImplemented;

		if (pen->mode != penMode) {
			pen->mode = penMode;
			pen->changed = TRUE;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenMode(GpPen* pen, GpPenAlignment* penMode)
	{
		if (!pen || !penMode)
			return GpStatus::InvalidParameter;

		*penMode = pen->mode;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenUnit(GpPen* pen, GpUnit* unit)
	{
		if (!pen || !unit)
			return GpStatus::InvalidParameter;

		*unit = pen->unit;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenUnit(GpPen* pen, GpUnit unit)
	{
		if (!pen || unit < GpUnit::UnitWorld /*|| unit > GpUnit::UnitCairoPoint*/ || unit == GpUnit::UnitDisplay)
			return GpStatus::InvalidParameter;

		if (unit != pen->unit) {
			pen->unit = unit;
			pen->changed = TRUE;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenTransform(GpPen* pen, GpMatrix* matrix)
	{
		BOOL invertible;

		if (!pen || !matrix)
			return GpStatus::InvalidParameter;

		/* the matrix MUST be invertible to be used */
		GdipIsMatrixInvertible(matrix, &invertible);
		if (!invertible)
			return GpStatus::InvalidParameter;

		/*gdip_cairo_matrix_copy(&pen->matrix, matrix);
		pen->changed = TRUE;
		return GpStatus::Ok;*/
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPenTransform(GpPen* pen, GpMatrix* matrix)
	{
		if (!pen || !matrix)
			return GpStatus::InvalidParameter;

		/*gdip_cairo_matrix_copy(matrix, &pen->matrix);
		return GpStatus::Ok;
		*/
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipResetPenTransform(GpPen* pen)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		/*cairo_matrix_init_identity(&pen->matrix);
		pen->changed = TRUE;

		return GpStatus::Ok;*/
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipMultiplyPenTransform(GpPen* pen, GpMatrix* matrix, GpMatrixOrder order)
	{
		BOOL invertible;

		if (!pen)
			return GpStatus::InvalidParameter;

		if (!matrix)
			return GpStatus::Ok;

		/* the matrix MUST be invertible to be used */
		GdipIsMatrixInvertible(matrix, &invertible);
		if (!invertible)
			return GpStatus::InvalidParameter;
/*
		if (order == GpMatrixOrder::MatrixOrderPrepend)
			cairo_matrix_multiply(&pen->matrix, matrix, &pen->matrix);
		else
			cairo_matrix_multiply(&pen->matrix, &pen->matrix, matrix);

		pen->changed = TRUE;
		return GpStatus::Ok;
		*/
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipTranslatePenTransform(GpPen* pen, REAL dx, REAL dy, GpMatrixOrder order)
	{
		GpStatus status;

		if (!pen)
			return GpStatus::InvalidParameter;

		status = GdipTranslateMatrix(&pen->matrix, dx, dy, order);
		if (status != GpStatus::Ok)
			return status;

		pen->changed = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipScalePenTransform(GpPen* pen, REAL sx, REAL sy, GpMatrixOrder order)
	{
		GpStatus status;

		if (!pen)
			return GpStatus::InvalidParameter;

		status = GdipScaleMatrix(&pen->matrix, sx, sy, order);
		if (status != GpStatus::Ok)
			return status;

		pen->changed = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipRotatePenTransform(GpPen* pen, REAL angle, GpMatrixOrder order)
	{
		GpStatus status;

		if (!pen)
			return GpStatus::InvalidParameter;

		status = GdipRotateMatrix(&pen->matrix, angle, order);
		if (status != GpStatus::Ok)
			return status;

		pen->changed = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenDashStyle(GpPen* pen, GpDashStyle* dashstyle)
	{
		if (!pen || !dashstyle)
			return GpStatus::InvalidParameter;

		*dashstyle = pen->dash_style;
		return GpStatus::Ok;
	}

	static float Dot[] = { 1.0, 1.0 };
	static float Dash[] = { 3.0, 1.0 };
	static float DashDot[] = { 3.0, 1.0, 1.0, 1.0 };
	static float DashDotDot[] = { 3.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	GpStatus WINGDIPAPI
		GdipSetPenDashStyle(GpPen* pen, GpDashStyle dashstyle)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		/* Free old custom dash style if we are going to override it */
		if (pen->dash_count != 0 && pen->own_dash_array && dashstyle < GpDashStyle::DashStyleCustom) {
			delete (pen->dash_array);
			pen->dash_count = 0;
			pen->dash_array = NULL;
		}

		switch (dashstyle) {
		case GpDashStyle::DashStyleSolid:
			pen->dash_array = NULL;
			pen->dash_count = 0;
			pen->own_dash_array = FALSE;
			break;

		case GpDashStyle::DashStyleDashDot:
			pen->dash_array = DashDot;
			pen->dash_count = 4;
			pen->own_dash_array = FALSE;
			break;

		case GpDashStyle::DashStyleDashDotDot:
			pen->dash_array = DashDotDot;
			pen->dash_count = 6;
			pen->own_dash_array = FALSE;
			break;

		case GpDashStyle::DashStyleDot:
			pen->dash_array = Dot;
			pen->dash_count = 2;
			pen->own_dash_array = FALSE;
			break;

		case GpDashStyle::DashStyleDash:
			pen->dash_array = Dash;
			pen->dash_count = 2;
			pen->own_dash_array = FALSE;
			break;

		case GpDashStyle::DashStyleCustom:
			/* we keep the current assigned value when switching to Custom */
			/* other special stuff happens in System.Drawing (but not here) */
			break;

		default:
			/* GDI+ does nothing if the dash style is invalid */
			return GpStatus::Ok;
		}

		pen->dash_style = dashstyle;
		pen->changed = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenDashOffset(GpPen* pen, REAL* offset)
	{
		if (!pen || !offset)
			return GpStatus::InvalidParameter;

		*offset = pen->dash_offset;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenDashOffset(GpPen* pen, REAL offset)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		if (pen->dash_offset != offset) {
			pen->dash_offset = offset;
			pen->changed = TRUE;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenDashCount(GpPen* pen, INT* count)
	{
		if (!pen || !count)
			return GpStatus::InvalidParameter;

		*count = pen->dash_count;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenDashArray(GpPen* pen, REAL* dash, INT count)
	{
		if (!pen || !dash || count > pen->dash_count)
			return GpStatus::InvalidParameter;
		if (!pen->dash_array)
			return GpStatus::OutOfMemory;
		if (count < 0)
			return GpStatus::OutOfMemory;

		return GpStatus::NotImplemented;
		/*memcpy(dash, pen->dash_array, count * sizeof(float));
		return GpStatus::Ok;*/
	}

	GpStatus WINGDIPAPI
		GdipSetPenDashArray(GpPen* pen, GDIPCONST REAL* dash, INT count)
	{
		return GpStatus::NotImplemented;
		/*
		float* dash_array;
		REAL sum = 0;

		if (!pen || !dash || (count <= 0))
			return GpStatus::InvalidParameter;

		for (int i = 0; i < count; i++) {
			sum += dash[i];
			if (dash[i] < 0.0)
				return GpStatus::InvalidParameter;
		}

		if (sum == 0.0)
			return GpStatus::InvalidParameter;

		if (pen->dash_count != count || !pen->own_dash_array) {
			dash_array = (float*)GdipAlloc(count * sizeof(float));
			if (!dash_array)
				return GpStatus::OutOfMemory;

			// free the existing values, if we own them 
			if (pen->dash_count != 0 && pen->own_dash_array)
				GdipFree(pen->dash_array);

			pen->dash_array = dash_array;
			pen->dash_count = count;
			pen->own_dash_array = TRUE;
		}

		memcpy(pen->dash_array, dash, pen->dash_count * sizeof(float));
		pen->dash_style = GpDashStyle::DashStyleCustom;
		pen->changed = TRUE;

		return GpStatus::Ok;*/
	}

	/*
	 * MonoTODO: Find out what the difference is between CompoundArray and DashArray
	 */
	GpStatus WINGDIPAPI
		GdipGetPenCompoundCount(GpPen* pen, INT* count)
	{
		if (!pen || !count)
			return GpStatus::InvalidParameter;

		*count = pen->compound_count;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenCompoundArray(GpPen* pen, GDIPCONST REAL* compound, INT count)
	{
		return GpStatus::NotImplemented;
		/*
		float minValue = 0.0f;
		float* compound_array;

		if (!pen || !compound || count <= 0 || count % 2 == 1)
			return GpStatus::InvalidParameter;
		if (pen->mode == GpPenAlignment::PenAlignmentInset)
			return GpStatus::NotImplemented;

		// All values have to be in ascending order and be between 0 and 1. 
		for (int i = 0; i < count; i++) {
			float value = compound[i];
			if (value < minValue || value > 1.0f)
				return GpStatus::InvalidParameter;

			minValue = value;
		}

		if (pen->compound_count != count) {
			compound_array = (float*)GdipAlloc(count * sizeof(float));
			if (!compound_array)
				return GpStatus::OutOfMemory;

			// free the existing values 
			if (pen->compound_count)
				GdipFree(pen->compound_array);

			pen->compound_array = compound_array;
			pen->compound_count = count;
		}

		memcpy(pen->compound_array, compound, pen->compound_count * sizeof(float));
		return GpStatus::Ok;
		*/
	}

	GpStatus WINGDIPAPI
		GdipGetPenCompoundArray(GpPen* pen, REAL* compound, INT count)
	{
		
		if (!pen || !compound || count > pen->compound_count)
			return GpStatus::InvalidParameter;

		/* GDI+ does nothing if count <= 0. */
		if (count <= 0)
			return GpStatus::Ok;

		return GpStatus::NotImplemented;
		/*
		memcpy(compound, pen->compound_array, count * sizeof(float));

		return GpStatus::Ok;*/
	}

	GpStatus WINGDIPAPI
		GdipSetPenStartCap(GpPen* pen, GpLineCap startCap)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		GdipDeleteCustomLineCap(pen->custom_start_cap);
		pen->custom_start_cap = NULL;

		pen->line_cap = startCap;
		pen->changed = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPenStartCap(GpPen* pen, GpLineCap* startCap)
	{
		if (!pen || !startCap)
			return GpStatus::InvalidParameter;

		*startCap = pen->line_cap;
		return GpStatus::Ok;
	}

	/* MonoTODO - ignored (always same as start cap) */
	GpStatus WINGDIPAPI
		GdipSetPenEndCap(GpPen* pen, GpLineCap endCap)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		/*
		 * This is currently ignored, as Cairo does not support
		 * having a different start and end Cap
		 */

		GdipDeleteCustomLineCap(pen->custom_end_cap);
		pen->custom_end_cap = NULL;

		pen->end_cap = endCap;
		pen->changed = TRUE;

		return GpStatus::Ok;
	}

	/* MonoTODO - ignored (always same as start cap) */
	GpStatus WINGDIPAPI
		GdipGetPenEndCap(GpPen* pen, GpLineCap* endCap)
	{
		if (!pen || !endCap)
			return GpStatus::InvalidParameter;

		*endCap = pen->end_cap;
		return GpStatus::Ok;
	}

	/* MonoTODO - ignored */
	GpStatus WINGDIPAPI
		GdipSetPenDashCap197819(GpPen* pen, GpDashCap dashCap)
	{
		if (!pen)
			return GpStatus::InvalidParameter;

		/* Any invalid value is changed to GpDashCap::DashCapFlat. */
		if (dashCap == GpDashCap::DashCapRound || dashCap == GpDashCap::DashCapTriangle)
			pen->dash_cap = dashCap;
		else
			pen->dash_cap = GpDashCap::DashCapFlat;

		return GpStatus::Ok;
	}

	/* MonoTODO - ignored */
	GpStatus WINGDIPAPI
		GdipGetPenDashCap197819(GpPen* pen, GpDashCap* dashCap)
	{
		if (!pen || !dashCap)
			return GpStatus::InvalidParameter;

		*dashCap = pen->dash_cap;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPenCustomStartCap(GpPen* pen, GpCustomLineCap* customCap)
	{
		GpStatus status;
		GpCustomLineCap* clonedCap;

		if (!pen)
			return GpStatus::InvalidParameter;

		status = GdipCloneCustomLineCap(customCap, &clonedCap);
		if (status == GpStatus::Ok) {
			pen->custom_start_cap = clonedCap;
			pen->line_cap = GpLineCap::LineCapCustom;
		}

		return status;
	}

	GpStatus WINGDIPAPI
		GdipGetPenCustomStartCap(GpPen* pen, GpCustomLineCap** customCap)
	{
		if (!pen || !customCap)
			return GpStatus::InvalidParameter;

		if (!pen->custom_start_cap) {
			*customCap = NULL;
			return GpStatus::Ok;
		}

		return GdipCloneCustomLineCap(pen->custom_start_cap, customCap);
	}

	GpStatus WINGDIPAPI
		GdipSetPenCustomEndCap(GpPen* pen, GpCustomLineCap* customCap)
	{
		GpStatus status;
		GpCustomLineCap* clonedCap;

		if (!pen)
			return GpStatus::InvalidParameter;

		status = GdipCloneCustomLineCap(customCap, &clonedCap);
		if (status == GpStatus::Ok) {
			pen->custom_end_cap = clonedCap;
			pen->end_cap = GpLineCap::LineCapCustom;
		}

		return status;
	}

	GpStatus WINGDIPAPI
		GdipGetPenCustomEndCap(GpPen* pen, GpCustomLineCap** customCap)
	{
		if (!pen || !customCap)
			return GpStatus::InvalidParameter;

		if (!pen->custom_end_cap) {
			*customCap = NULL;
			return GpStatus::Ok;
		}

		return GdipCloneCustomLineCap(pen->custom_end_cap, customCap);
	}

}