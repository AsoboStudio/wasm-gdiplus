// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "gdiplus.h"
#include <stdlib.h>
#include <wchar.h>
#include <locale>
#include <codecvt>

#include "GdiPlusHatchStylePatterns.inl"
namespace Gdiplus {
	// Graphics

	Graphics::Graphics(NVGcontext* _ctx)
	{
		ctx = _ctx;

		if (ctx) {
			for (int p = 0; p < HatchStyleTotal; ++p) {
				m_brushPatterns[p] = nvgGenerateBrushPattern(ctx, (HatchStyle)p);
			}
		}
		else {
			for (int p = 0; p < HatchStyleTotal; ++p) {
				m_brushPatterns[p] = 0;
			}
		}
	}
	Graphics::Graphics(Image* image)
		: Graphics((NVGcontext*) nullptr)
	{
	}
	//TODO INTEGRATE: this code needs to be called from the gauge, as the destructor is called after NVGcontext has been released
	Status Graphics::Release()
	{
		if (ctx)
		{
			for (int p = 0; p < HatchStyleTotal; ++p) {
				if (m_brushPatterns[p] != 0) {
					nvgDeleteImage(ctx, m_brushPatterns[p]);
				}
			}
		}
		return Status::Ok;
	}
	Graphics::~Graphics()
	{
	}
	Status Graphics::DrawString(const WCHAR* string, INT length, const Font* font, const PointF& origin, const Brush* brush)
	{
		if (!font->Load(ctx))
			return GenericError;
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string u8str = converter.to_bytes(string);
		const char* cStr = u8str.c_str();
		ConvertBrush(brush);
		nvgFontFaceId(ctx, font->fontid);
		nvgFontSize(ctx, font->size);
		nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
		nvgBeginPath(ctx);
		nvgText(ctx, origin.X, origin.Y, cStr, nullptr);
		nvgFill(ctx);
		return Ok;
	}

	Status Graphics::DrawString(const WCHAR* string, INT length, const Font* font, const PointF& origin, const StringFormat* stringFormat, const Brush* brush)
	{
		if (!font->Load(ctx))
			return GenericError;
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string u8str = converter.to_bytes(string);
		const char* cStr = u8str.c_str();
		ConvertBrush(brush);
		nvgFontFaceId(ctx, font->fontid);
		nvgFontSize(ctx, font->size);
		nvg_SetAlignment(stringFormat);
		nvgBeginPath(ctx);
		nvgText(ctx, origin.X, origin.Y, cStr, nullptr);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::DrawString(const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, const StringFormat* stringFormat, const Brush* brush)
	{
		if (!font->Load(ctx))
			return GenericError;
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string u8str = converter.to_bytes(string);
		const char* cStr = u8str.c_str();
		ConvertBrush(brush);
		nvgFontFaceId(ctx, font->fontid);
		nvgFontSize(ctx, font->size);
		nvg_SetAlignment(stringFormat);
		nvgBeginPath(ctx);
		nvgText(ctx, layoutRect.X, layoutRect.Y, cStr, nullptr);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::DrawLine(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, x1, y1);
		nvgLineTo(ctx, x2, y2);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawLine(const Pen* pen, INT x1, INT y1, INT x2, INT y2)
	{
		return DrawLine(pen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2);
	}
	Status Graphics::DrawLine(const Pen* pen, const PointF& pt1, const PointF& pt2)
	{
		return DrawLine(pen, pt1.X, pt1.Y, pt2.X, pt2.Y);
	}
	Status Graphics::ResetClip()
	{
		if (ctx && m_clipped) {
			nvgResetStencil(ctx);
			nvgResetScissor(ctx);
			m_clipped = false;
			nvgSetClipped(ctx, false);
		}
		return Ok;
	}
	Status Graphics::ResetTransform()
	{
		nvgResetTransform(ctx);
		return Ok;
	}
	Status Graphics::SetTransform(const Matrix* matrix)
	{
		nvgTransform(ctx, matrix->m[0], matrix->m[1], matrix->m[2], matrix->m[3], matrix->m[4], matrix->m[5]);
		return Ok;
	}
	Status Graphics::TranslateTransform(REAL dx, REAL dy, MatrixOrder order)
	{
		nvgTranslate(ctx, dx, dy);
		return Ok;
	}
	Status Graphics::RotateTransform(REAL angle, MatrixOrder order)
	{
		nvgRotate(ctx, nvgDegToRad(angle));
		return Ok;
	}
	Status Graphics::ScaleTransform(REAL sx, REAL sy, MatrixOrder order)
	{
		nvgScale(ctx, sx, sy);
		return Ok;
	}
	Status Graphics::DrawPolygon(const Pen* pen, const PointF* points, INT count)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(ctx, points[i].X, points[i].Y);
		nvgClosePath(ctx);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawPolygon(const Pen* pen, const Point* points, INT count)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(ctx, points[i].X, points[i].Y);
		nvgClosePath(ctx);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawRectangle(const Pen* pen, REAL x, REAL y, REAL width, REAL height)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgRect(ctx, x, y, width, height);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawRectangle(const Pen* pen, INT x, INT y, INT width, INT height)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgRect(ctx, x, y, width, height);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawRectangle(const Pen* pen, const RectF& rect)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgRect(ctx, rect.X, rect.Y, rect.Width, rect.Height);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawRectangle(const Pen* pen, const Rect& rect)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgRect(ctx, rect.X, rect.Y, rect.Width, rect.Height);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawCurve(const Pen* pen, const PointF* points, INT count)
	{
		return Ok;
	}
	Status Graphics::DrawCurve(const Pen* pen, const Point* points, INT count)
	{
		return Ok;
	}
	Status Graphics::DrawCurve(const Pen* pen, const PointF* points, INT count, REAL tension)
	{
		return Ok;
	}
	Status Graphics::DrawCurve(const Pen* pen, const Point* points, INT count, REAL tension)
	{
		return Ok;
	}
	Status Graphics::DrawCurve(const Pen* pen, const PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension)
	{
		return Ok;
	}
	Status Graphics::DrawCurve(const Pen* pen, const Point* points, INT count, INT offset, INT numberOfSegments, REAL tension)
	{
		return Ok;
	}
	Status Graphics::DrawEllipse(const Pen* pen, REAL x, REAL y, REAL width, REAL height)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgEllipse(ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawEllipse(const Pen* pen, INT x, INT y, INT width, INT height)
	{
		return DrawEllipse(pen, (REAL)x, (REAL)y, (REAL)width, (REAL)height);
	}
	Status Graphics::DrawEllipse(const Pen* pen, const RectF& rect)
	{
		return DrawEllipse(pen, rect.X, rect.Y, rect.Width, rect.Height);
	}
	Status Graphics::DrawEllipse(const Pen* pen, const Rect& rect)
	{
		return DrawEllipse(pen, (REAL)rect.X, (REAL)rect.Y, (REAL)rect.Width, (REAL)rect.Height);
	}
	Status Graphics::DrawImage(Image* image, REAL x, REAL y)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, INT x, INT y)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const PointF& point)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const Point& point)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, REAL x, REAL y, REAL width, REAL height)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, INT x, INT y, INT width, INT height)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const RectF& rect)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const Rect& rect)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const PointF* destPoints, INT count)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const Point* destPoints, INT count)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, REAL x, REAL y, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, INT x, INT y, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit)
	{
		return Ok;
	}
	/*
	Status Graphics::DrawImage(Image* image, const RectF& destRect, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const Rect& destRect, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const RectF& destRect, const RectF& sourceRect, Unit srcUnit, const ImageAttributes* imageAttributes = NULL)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const PointF* destPoints, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL)
	{
		return Ok;
	}
	Status Graphics::DrawImage(Image* image, const Point* destPoints, INT count, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL)
	{
		return Ok;
	}
	*/

	Status Graphics::FillRectangle(const Brush* brush, REAL x, REAL y, REAL width, REAL height)
	{
		ConvertBrush(brush);
		nvgBeginPath(ctx);
		nvgRect(ctx, x, y, width, height);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::FillRectangle(const Brush* brush, INT x, INT y, INT width, INT height)
	{
		return FillRectangle(brush, (float)x, (float)y, (float)width, (float)height);
	}
	Status Graphics::FillRectangle(const Brush* brush, const RectF& rect)
	{
		return FillRectangle(brush, rect.X, rect.Y, rect.Width, rect.Height);
	}
	Status Graphics::FillRectangle(const Brush* brush, const Rect& rect)
	{
		return FillRectangle(brush, (float)rect.X, (float)rect.Y, (float)rect.Width, (float)rect.Height);
	}

	Status Graphics::DrawPath(const Pen* pen, const GraphicsPath* path)
	{
		return Ok;
	}
	Status Graphics::FillEllipse(const Brush* brush, REAL x, REAL y, REAL width, REAL height)
	{
		ConvertBrush(brush);
		nvgBeginPath(ctx);
		nvgEllipse(ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::FillEllipse(const Brush* brush, INT x, INT y, INT width, INT height)
	{
		return FillEllipse(brush, (REAL)x, (REAL)y, (REAL)width, (REAL)height);
	}
	Status Graphics::FillEllipse(const Brush* brush, const RectF& rect)
	{
		return FillEllipse(brush, rect.X, rect.Y, rect.Width, rect.Height);
	}
	Status Graphics::FillEllipse(const Brush* brush, const Rect& rect)
	{
		return FillEllipse(brush, (REAL)rect.X, (REAL)rect.Y, (REAL)rect.Width, (REAL)rect.Height);
	}
	Status Graphics::FillPath(const Brush* brush, const GraphicsPath* path)
	{
		return Ok;
	}
	Status Graphics::FillPie(const Brush* brush, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status Graphics::FillPie(const Brush* brush, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status Graphics::FillPie(const Brush* brush, const RectF& rect, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status Graphics::FillPie(const Brush* brush, const Rect& rect, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status Graphics::FillPolygon(const Brush* brush, const PointF* points, INT count)
	{
		ConvertBrush(brush);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(ctx, points[i].X, points[i].Y);
		nvgClosePath(ctx);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::FillPolygon(const Brush* brush, const Point* points, INT count)
	{
		ConvertBrush(brush);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(ctx, points[i].X, points[i].Y);
		nvgClosePath(ctx);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::FillPolygon(const Brush* brush, const PointF* points, INT count, FillMode fillMode)
	{
		ConvertBrush(brush);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(ctx, points[i].X, points[i].Y);
		nvgClosePath(ctx);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::FillPolygon(const Brush* brush, const Point* points, INT count, FillMode fillMode)
	{
		ConvertBrush(brush);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(ctx, points[i].X, points[i].Y);
		nvgClosePath(ctx);
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::MeasureString(const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, RectF* boundingBox) const
	{
		if (!font->Load(ctx))
			return GenericError;
		return Ok;
	}
	Status Graphics::MeasureString(const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, const StringFormat* stringFormat, RectF* boundingBox, INT* codepointsFitted, INT* linesFitted) const
	{
		if (!font->Load(ctx))
			return GenericError;
		return Ok;
	}
	Status Graphics::MeasureString(const WCHAR* string, INT length, const Font* font, const SizeF& layoutRectSize, const StringFormat* stringFormat, SizeF* size, INT* codepointsFitted, INT* linesFitted) const
	{
		if (!font->Load(ctx))
			return GenericError;
		return Ok;
	}
	Status Graphics::MeasureString(const WCHAR* string, INT length, const Font* font, const PointF& origin, RectF* boundingBox) const
	{
		if (!font->Load(ctx))
			return GenericError;
		return Ok;
	}
	Status Graphics::MeasureString(const WCHAR* string, INT length, const Font* font, const PointF& origin, const StringFormat* stringFormat, RectF* boundingBox) const
	{
		if (!font->Load(ctx))
			return GenericError;
		return Ok;
	}
	Status Graphics::SetSmoothingMode(SmoothingMode smoothingMode)
	{
		return Ok;
	}
	Status Graphics::SetTextRenderingHint(TextRenderingHint textRenderingHint)
	{
		return Ok;
	}
	Status Graphics::Clear(const Color& color)
	{
		nvgFillColor(ctx, color);
		nvgBeginPath(ctx);
		nvgRect(ctx, 0, 0, 1920, 1080); //TODO
		nvgFill(ctx);
		return Ok;
	}
	Status Graphics::DrawArc(const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgEllipticalArc(ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, nvgDegToRad(startAngle), nvgDegToRad(startAngle + sweepAngle), (sweepAngle < 0 ? NVG_CCW : NVG_CW));
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawArc(const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return DrawArc(pen, (REAL)x, (REAL)y, (REAL)width, (REAL)height, startAngle, sweepAngle);
	}
	Status Graphics::DrawArc(const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle)
	{
		return DrawArc(pen, rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
	}
	Status Graphics::DrawArc(const Pen* pen, const Rect& rect, REAL startAngle, REAL sweepAngle)
	{
		return DrawArc(pen, (REAL)rect.X, (REAL)rect.Y, (REAL)rect.Width, (REAL)rect.Height, startAngle, sweepAngle);
	}
	Status Graphics::DrawBezier(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, x1, y1);
		nvgBezierTo(ctx, x2, y2, x3, y3, x4, y4);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::DrawBezier(const Pen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4)
	{
		return DrawBezier(pen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2, (REAL)x3, (REAL)y3, (REAL)x4, (REAL)y4);
	}
	Status Graphics::DrawBezier(const Pen* pen, const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4)
	{
		return DrawBezier(pen, pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y, pt4.X, pt4.Y);
	}
	Status Graphics::DrawBezier(const Pen* pen, const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4)
	{
		return DrawBezier(pen, (REAL)pt1.X, (REAL)pt1.Y, (REAL)pt2.X, (REAL)pt2.Y, (REAL)pt3.X, (REAL)pt3.Y, (REAL)pt4.X, (REAL)pt4.Y);
	}
	Status Graphics::DrawLines(const Pen* pen, const PointF* points, INT count)
	{
		nvgStrokeColor(ctx, pen->color);
		nvgStrokeWidth(ctx, pen->width);
		nvgBeginPath(ctx);
		nvgMoveTo(ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(ctx, points[i].X, points[i].Y);
		nvgStroke(ctx);
		return Ok;
	}
	Status Graphics::GetTransform(Matrix* matrix) const
	{
		nvgCurrentTransform(ctx, matrix->m);
		return Ok;
	}
	Status Graphics::GetVisibleClipBounds(RectF* rect) const
	{
		rect->X = 0;
		rect->Y = 0;
		rect->Width = 1920;
		rect->Height = 1080;
		return Ok;
	}
	Status Graphics::GetVisibleClipBounds(Rect* rect) const
	{
		rect->X = 0;
		rect->Y = 0;
		rect->Width = 1920;
		rect->Height = 1080;
		return Ok;
	}
	Status Graphics::SetClip(const GraphicsPath* path, CombineMode combineMode)
	{
		if (!path->m_empty) {
			if (!m_clipped) {
				// only supported combinemode for an empty clip is union
				combineMode = CombineMode::CombineModeUnion;
			}
			else
			{
				if (combineMode == CombineMode::CombineModeReplace) {
					ResetClip();
				}
				/* Other combine modes handled by the backend
				else if (combineMode == CombineMode::CombineModeUnion) {
					// new | existing (default render behaviour)
				}
				else if (combineMode == CombineMode::CombineModeIntersect) {
					// new & existing
				}
				else if (combineMode == CombineMode::CombineModeExclude) {
					// new - existing
				}
				else if (combineMode == CombineMode::CombineModeComplement) {
					// existing - new
				}
				else if (combineMode == CombineMode::CombineModeXor) {
					// existing != new
				}*/
			}
			nvgSetClipMode(ctx, (NVGclipMode)combineMode);

			int prevPath = nvgCurrentPath(ctx);
			nvgSelectPath(ctx, path->m_pathIndex);
			nvgSetBuffer(ctx, 1);
			nvgFill(ctx);
			nvgSetBuffer(ctx, 0);
			nvgSelectPath(ctx, prevPath);
			m_clipped = true;
			nvgSetClipped(ctx, true);
		}
		return Ok;
	}

	void Graphics::nvg_SetAlignment(const StringFormat* stringFormat)
	{
		int nvg_align = 0;
		StringAlignment align = stringFormat->GetAlignment();
		switch (align)
		{
		case StringAlignmentNear:
			nvg_align |= NVG_ALIGN_LEFT;
			break;
		case StringAlignmentCenter:
			nvg_align |= NVG_ALIGN_CENTER;
			break;
		case StringAlignmentFar:
			nvg_align |= NVG_ALIGN_RIGHT;
			break;
		}
		StringAlignment line_align = stringFormat->GetLineAlignment();
		switch (line_align)
		{
		case StringAlignmentNear:
			nvg_align |= NVG_ALIGN_TOP;
			break;
		case StringAlignmentCenter:
			nvg_align |= NVG_ALIGN_MIDDLE;
			break;
		case StringAlignmentFar:
			nvg_align |= NVG_ALIGN_BOTTOM;
			break;
		}
		nvgTextAlign(ctx, nvg_align);
	}


	void Graphics::ConvertBrush(const Brush* brush) {
		int brushType = brush->GetType();
		switch (brushType) {
		case BrushTypeHatchFill:
		{

			HatchBrush* hatch = (HatchBrush*)brush;

			if (hatch->patternImage == 0) {
				hatch->patternImage = m_brushPatterns[hatch->style];
			}
			if (hatch->patternImage != 0) {
				int pattern_width, pattern_height;
				nvgImageSize(ctx, hatch->patternImage, &pattern_width, &pattern_height);
				NVGpaint paint = nvgImagePattern(ctx, (float)(-pattern_width / 2), (float)(-pattern_height / 2), (float)pattern_width, (float)pattern_height, 0.f, hatch->patternImage, 1.f);
				paint.innerColor = hatch->foreColor;
				paint.outerColor = hatch->backColor;
				nvgFillPaint(ctx, paint);
			}
			else {
				nvgFillColor(ctx, brush->GetColor());
			}
			break;
		}
		case BrushTypeSolidColor:
		default:
		{
			nvgFillColor(ctx, brush->GetColor());
			break;
		}
		}
	}

}