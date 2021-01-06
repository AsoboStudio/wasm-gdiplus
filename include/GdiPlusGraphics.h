// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_GRAPHICS_H
#define __GDIPLUS_GRAPHICS_H

#include "GdiPlus.h"
#include "gdiplusflat.h"

namespace Gdiplus
{
	class Graphics : public GdiplusBase
	{
		friend class Bitmap;
		friend class CachedBitmap;
		friend class Font;
		friend class GraphicsPath;
		friend class Metafile;
		friend class Region;

	
#ifdef _MSFS_WASM_PROXY_GDIPLUS
	private:
		bool bIsCtxInternal;
	public:
		NVGcontext* ctx;

		Graphics(NVGcontext* _ctx) : nativeGraphics(NULL), lastStatus(Ok)
		{
			ctx = _ctx;
			bIsCtxInternal = false;
			lastStatus = DLLExports::GdipCreateFromHDC(
				(HDC)ctx, &nativeGraphics);
		}

		Graphics(FsContext fsctx) : nativeGraphics(NULL), lastStatus(Ok)
		{
			NVGparams params;
			params.userPtr = fsctx;
			params.edgeAntiAlias = true;
			NVGcontext* nvgctx = nvgCreateInternal(&params);
			ctx = nvgctx;
			bIsCtxInternal = true;

			lastStatus = DLLExports::GdipCreateFromHDC(
				(HDC)ctx, &nativeGraphics);
		}
#endif
	public:
		static Graphics* FromHDC(HDC hdc)
		{
			return new Graphics(hdc);
		}
		static Graphics* FromHDC(HDC hdc, HANDLE hdevice)
		{
			return new Graphics(hdc, hdevice);
		}
		static Graphics* FromHWND(HWND hwnd, BOOL icm = FALSE)
		{
			return new Graphics(hwnd, icm);
		}
		static Graphics* FromImage(Image* image)
		{
			return new Graphics(image);
		}
		static HPALETTE GetHalftonePalette()
		{
			return DLLExports::GdipCreateHalftonePalette();
		}

		Graphics(Image* image) : nativeGraphics(NULL), lastStatus(Ok)
		{
			bIsCtxInternal = false;
			lastStatus = DLLExports::GdipGetImageGraphicsContext(
				image ? image->nativeImage : NULL,
				&nativeGraphics);
		}
		Graphics(HDC hdc) : nativeGraphics(NULL), lastStatus(Ok)
		{
			bIsCtxInternal = false;
			lastStatus = DLLExports::GdipCreateFromHDC(
				hdc, &nativeGraphics);
		}
		Graphics(HDC hdc, HANDLE hdevice) : nativeGraphics(NULL), lastStatus(Ok)
		{
			bIsCtxInternal = false;
			lastStatus = DLLExports::GdipCreateFromHDC2(
				hdc, hdevice, &nativeGraphics);
		}
		Graphics(HWND hwnd, BOOL icm = FALSE) :
			nativeGraphics(NULL), lastStatus(Ok)
		{
			bIsCtxInternal = false;
			if (icm) {
				lastStatus = DLLExports::GdipCreateFromHWNDICM(
					hwnd, &nativeGraphics);
			}
			else {
				lastStatus = DLLExports::GdipCreateFromHWND(
					hwnd, &nativeGraphics);
			}
		}
		~Graphics()
		{
			if (nativeGraphics) {
				DLLExports::GdipDeleteGraphics(nativeGraphics);
			}
			if (bIsCtxInternal) {
				nvgDeleteInternal(ctx);
			}
		}

		Status AddMetafileComment(const BYTE* data, UINT sizeData)
		{
			return GpStatus::Ok;
			/*WASM: NOT SUPPORTED
			return updateStatus(DLLExports::GdipComment(
				nativeGraphics, sizeData, data));*/
		}
		GraphicsContainer BeginContainer()
		{
			GraphicsContainer result = 0;
			updateStatus(DLLExports::GdipBeginContainer2(
				nativeGraphics, &result));
			return result;
		}
		GraphicsContainer BeginContainer(const RectF& dstrect,
			const RectF& srcrect, Unit unit)
		{
			GraphicsContainer result = 0;
			updateStatus(DLLExports::GdipBeginContainer(
				nativeGraphics, &dstrect, &srcrect, unit,
				&result));
			return result;
		}
		GraphicsContainer BeginContainer(const Rect& dstrect,
			const Rect& srcrect, Unit unit)
		{
			GraphicsContainer result = 0;
			updateStatus(DLLExports::GdipBeginContainerI(
				nativeGraphics, &dstrect, &srcrect, unit,
				&result));
			return result;
		}
		Status Clear(const Color& color)
		{
			return updateStatus(DLLExports::GdipGraphicsClear(
				nativeGraphics, color.GetValue()));
		}
		Status DrawArc(const Pen* pen, REAL x, REAL y, REAL width, REAL height,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawArc(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height, startAngle, sweepAngle));
		}
		Status DrawArc(const Pen* pen, INT x, INT y, INT width, INT height,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawArcI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height, startAngle, sweepAngle));
		}
		Status DrawArc(const Pen* pen, const RectF& rect,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawArc(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height,
				startAngle, sweepAngle));
		}
		Status DrawArc(const Pen* pen, const Rect& rect,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawArcI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height,
				startAngle, sweepAngle));
		}
		Status DrawBezier(const Pen* pen,
			REAL x1, REAL y1, REAL x2, REAL y2,
			REAL x3, REAL y3, REAL x4, REAL y4)
		{
			return updateStatus(DLLExports::GdipDrawBezier(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x1, y1, x2, y2, x3, y3, x4, y4));
		}
		Status DrawBezier(const Pen* pen,
			INT x1, INT y1, INT x2, INT y2,
			INT x3, INT y3, INT x4, INT y4)
		{
			return updateStatus(DLLExports::GdipDrawBezierI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x1, y1, x2, y2, x3, y3, x4, y4));
		}
		Status DrawBezier(const Pen* pen,
			const PointF& pt1, const PointF& pt2,
			const PointF& pt3, const PointF& pt4)
		{
			return updateStatus(DLLExports::GdipDrawBezier(
				nativeGraphics, pen ? pen->nativePen : NULL,
				pt1.X, pt1.Y, pt2.X, pt2.Y,
				pt3.X, pt3.Y, pt4.X, pt4.Y));
		}
		Status DrawBezier(const Pen* pen,
			const Point& pt1, const Point& pt2,
			const Point& pt3, const Point& pt4)
		{
			return updateStatus(DLLExports::GdipDrawBezierI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				pt1.X, pt1.Y, pt2.X, pt2.Y,
				pt3.X, pt3.Y, pt4.X, pt4.Y));
		}
		Status DrawBeziers(const Pen* pen, const PointF* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawBeziers(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawBeziers(const Pen* pen, const Point* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawBeziersI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawCachedBitmap(CachedBitmap* cb, INT x, INT y)
		{
			return GpStatus::Ok;
			/*WASM: NOT SUPPORTED
			return updateStatus(DLLExports::GdipDrawCachedBitmap(
				nativeGraphics,
				cb ? cb->nativeCachedBitmap : NULL,
				x, y));*/
		}
		Status DrawClosedCurve(const Pen* pen, const PointF* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawClosedCurve(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawClosedCurve(const Pen* pen, const Point* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawClosedCurveI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawClosedCurve(const Pen* pen, const PointF* points, INT count,
			REAL tension)
		{
			return updateStatus(DLLExports::GdipDrawClosedCurve2(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count, tension));
		}
		Status DrawClosedCurve(const Pen* pen, const Point* points, INT count,
			REAL tension)
		{
			return updateStatus(DLLExports::GdipDrawClosedCurve2I(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count, tension));
		}
		Status DrawCurve(const Pen* pen, const PointF* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawCurve(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawCurve(const Pen* pen, const Point* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawCurveI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawCurve(const Pen* pen, const PointF* points, INT count,
			REAL tension)
		{
			return updateStatus(DLLExports::GdipDrawCurve2(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count, tension));
		}
		Status DrawCurve(const Pen* pen, const Point* points, INT count,
			REAL tension)
		{
			return updateStatus(DLLExports::GdipDrawCurve2I(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count, tension));
		}
		Status DrawCurve(const Pen* pen, const PointF* points, INT count,
			INT offset, INT numberOfSegments, REAL tension)
		{
			return updateStatus(DLLExports::GdipDrawCurve3(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count, offset,
				numberOfSegments, tension));
		}
		Status DrawCurve(const Pen* pen, const Point* points, INT count,
			INT offset, INT numberOfSegments, REAL tension)
		{
			return updateStatus(DLLExports::GdipDrawCurve3I(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count, offset,
				numberOfSegments, tension));
		}
		Status DrawDriverString(const UINT16* text, INT length,
			const Font* font, const Brush* brush,
			const PointF* positions, INT flags,
			const Matrix* matrix)
		{
			return updateStatus(DLLExports::GdipDrawDriverString(
				nativeGraphics, text, length,
				font ? font->nativeFont : NULL,
				brush ? brush->nativeBrush : NULL,
				positions, flags,
				matrix ? matrix->nativeMatrix : NULL));
		}
		Status DrawEllipse(const Pen* pen,
			REAL x, REAL y, REAL width, REAL height)
		{
			return updateStatus(DLLExports::GdipDrawEllipse(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height));
		}
		Status DrawEllipse(const Pen* pen, INT x, INT y, INT width, INT height)
		{
			return updateStatus(DLLExports::GdipDrawEllipseI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height));
		}
		Status DrawEllipse(const Pen* pen, const RectF& rect)
		{
			return updateStatus(DLLExports::GdipDrawEllipse(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status DrawEllipse(const Pen* pen, const Rect& rect)
		{
			return updateStatus(DLLExports::GdipDrawEllipseI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status DrawImage(Image* image, REAL x, REAL y)
		{
			return updateStatus(DLLExports::GdipDrawImage(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				x, y));
		}
		Status DrawImage(Image* image, INT x, INT y)
		{
			return updateStatus(DLLExports::GdipDrawImageI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				x, y));
		}
		Status DrawImage(Image* image, const PointF& point)
		{
			return updateStatus(DLLExports::GdipDrawImage(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				point.X, point.Y));
		}
		Status DrawImage(Image* image, const Point& point)
		{
			return updateStatus(DLLExports::GdipDrawImageI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				point.X, point.Y));
		}
		Status DrawImage(Image* image, REAL x, REAL y, REAL width, REAL height)
		{
			return updateStatus(DLLExports::GdipDrawImageRect(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				x, y, width, height));
		}
		Status DrawImage(Image* image, INT x, INT y, INT width, INT height)
		{
			return updateStatus(DLLExports::GdipDrawImageRectI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				x, y, width, height));
		}
		Status DrawImage(Image* image, const RectF& rect)
		{
			return updateStatus(DLLExports::GdipDrawImageRect(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status DrawImage(Image* image, const Rect& rect)
		{
			return updateStatus(DLLExports::GdipDrawImageRectI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status DrawImage(Image* image, const PointF* destPoints, INT count)
		{
			return updateStatus(DLLExports::GdipDrawImagePoints(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				destPoints, count));
		}
		Status DrawImage(Image* image, const Point* destPoints, INT count)
		{
			return updateStatus(DLLExports::GdipDrawImagePointsI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				destPoints, count));
		}
		Status DrawImage(Image* image, REAL x, REAL y, REAL srcx, REAL srcy,
			REAL srcwidth, REAL srcheight, Unit srcUnit)
		{
			return updateStatus(DLLExports::GdipDrawImagePointRect(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				x, y, srcx, srcy, srcwidth, srcheight,
				srcUnit));
		}
		Status DrawImage(Image* image, INT x, INT y, INT srcx, INT srcy,
			INT srcwidth, INT srcheight, Unit srcUnit)
		{
			return updateStatus(DLLExports::GdipDrawImagePointRectI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				x, y, srcx, srcy, srcwidth, srcheight,
				srcUnit));
		}
		Status DrawImage(Image* image, const RectF& destRect,
			REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight,
			Unit srcUnit,
			const ImageAttributes* imageAttributes = NULL,
			DrawImageAbort callback = NULL,
			VOID* callbackData = NULL)
		{
			return updateStatus(DLLExports::GdipDrawImageRectRect(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				destRect.X, destRect.Y,
				destRect.Width, destRect.Height,
				srcx, srcy, srcwidth, srcheight, srcUnit,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
				callback, callbackData));
		}
		Status DrawImage(Image* image, const Rect& destRect,
			INT srcx, INT srcy, INT srcwidth, INT srcheight,
			Unit srcUnit,
			const ImageAttributes* imageAttributes = NULL,
			DrawImageAbort callback = NULL,
			VOID* callbackData = NULL)
		{
			return updateStatus(DLLExports::GdipDrawImageRectRectI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				destRect.X, destRect.Y,
				destRect.Width, destRect.Height,
				srcx, srcy, srcwidth, srcheight, srcUnit,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
				callback, callbackData));
		}
		Status DrawImage(Image* image, const RectF& destRect,
			const RectF& sourceRect, Unit srcUnit,
			const ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipDrawImageRectRectI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				destRect.X, destRect.Y,
				destRect.Width, destRect.Height,
				sourceRect.X, sourceRect.Y,
				sourceRect.Width, sourceRect.Height, srcUnit,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
				NULL, NULL));
		}
		Status DrawImage(Image* image, const PointF* destPoints, INT count,
			REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight,
			Unit srcUnit,
			const ImageAttributes* imageAttributes = NULL,
			DrawImageAbort callback = NULL,
			VOID* callbackData = NULL)
		{
			return updateStatus(DLLExports::GdipDrawImagePointsRect(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				destPoints, count,
				srcx, srcy, srcwidth, srcheight, srcUnit,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
				callback, callbackData));
		}
		Status DrawImage(Image* image, const Point* destPoints, INT count,
			INT srcx, INT srcy, INT srcwidth, INT srcheight,
			Unit srcUnit,
			const ImageAttributes* imageAttributes = NULL,
			DrawImageAbort callback = NULL,
			VOID* callbackData = NULL)
		{
			return updateStatus(DLLExports::GdipDrawImagePointsRectI(
				nativeGraphics,
				image ? image->nativeImage : NULL,
				destPoints, count,
				srcx, srcy, srcwidth, srcheight, srcUnit,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
				callback, callbackData));
		}
		// TODO: [GDI+ 1.1] Graphics::DrawImage(..Effect..)
		//Status DrawImage(Image *image, RectF *sourceRect, Matrix *matrix,
		//		Effect *effect, ImageAttributes *imageAttributes,
		//		Unit srcUnit)
		//{
		//	return updateStatus(DLLExports::GdipDrawImageFX(
		//			nativeGraphics,
		//			image ? image->nativeImage : NULL,
		//			sourceRect,
		//			matrix ? matrix->nativeMatrix : NULL,
		//			effect ? effect->nativeEffect : NULL,
		//			imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
		//			srcUnit));
		//}
		Status DrawLine(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2)
		{
			return updateStatus(DLLExports::GdipDrawLine(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x1, y1, x2, y2));
		}
		Status DrawLine(const Pen* pen, INT x1, INT y1, INT x2, INT y2)
		{
			return updateStatus(DLLExports::GdipDrawLineI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x1, y1, x2, y2));
		}
		Status DrawLine(const Pen* pen, const PointF& pt1, const PointF& pt2)
		{
			return updateStatus(DLLExports::GdipDrawLine(
				nativeGraphics, pen ? pen->nativePen : NULL,
				pt1.X, pt1.Y, pt2.X, pt2.Y));
		}
		Status DrawLine(const Pen* pen, const Point& pt1, const Point& pt2)
		{
			return updateStatus(DLLExports::GdipDrawLineI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				pt1.X, pt1.Y, pt2.X, pt2.Y));
		}
		Status DrawLines(const Pen* pen, const PointF* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawLines(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawLines(const Pen* pen, const Point* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawLinesI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawPath(const Pen* pen, const GraphicsPath* path)
		{
			return updateStatus(DLLExports::GdipDrawPath(
				nativeGraphics, pen ? pen->nativePen : NULL,
				path ? path->nativePath : NULL));
		}
		Status DrawPie(const Pen* pen, REAL x, REAL y, REAL width, REAL height,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawPie(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height, startAngle, sweepAngle));
		}
		Status DrawPie(const Pen* pen, INT x, INT y, INT width, INT height,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawPieI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height, startAngle, sweepAngle));
		}
		Status DrawPie(const Pen* pen, const RectF& rect,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawPie(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height,
				startAngle, sweepAngle));
		}
		Status DrawPie(const Pen* pen, const Rect& rect,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipDrawPieI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height,
				startAngle, sweepAngle));
		}
		Status DrawPolygon(const Pen* pen, const PointF* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawPolygon(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawPolygon(const Pen* pen, const Point* points, INT count)
		{
			return updateStatus(DLLExports::GdipDrawPolygonI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				points, count));
		}
		Status DrawRectangle(const Pen* pen,
			REAL x, REAL y, REAL width, REAL height)
		{
			return updateStatus(DLLExports::GdipDrawRectangle(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height));
		}
		Status DrawRectangle(const Pen* pen,
			INT x, INT y, INT width, INT height)
		{
			return updateStatus(DLLExports::GdipDrawRectangleI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				x, y, width, height));
		}
		Status DrawRectangle(const Pen* pen, const RectF& rect)
		{
			return updateStatus(DLLExports::GdipDrawRectangle(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status DrawRectangle(const Pen* pen, const Rect& rect)
		{
			return updateStatus(DLLExports::GdipDrawRectangleI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status DrawRectangles(const Pen* pen, const RectF* rects, INT count)
		{
			return updateStatus(DLLExports::GdipDrawRectangles(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rects, count));
		}
		Status DrawRectangles(const Pen* pen, const Rect* rects, INT count)
		{
			return updateStatus(DLLExports::GdipDrawRectanglesI(
				nativeGraphics, pen ? pen->nativePen : NULL,
				rects, count));
		}
		Status DrawString(const WCHAR* string, INT length, const Font* font,
			const PointF& origin, const Brush* brush)
		{
			RectF layoutRect(origin.X, origin.Y, 0.0f, 0.0f);
			return updateStatus(DLLExports::GdipDrawString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect, NULL,
				brush ? brush->nativeBrush : NULL));
		}
		Status DrawString(const WCHAR* string, INT length,
			const Font* font, const PointF& origin,
			const StringFormat* stringFormat, const Brush* brush)
		{
			RectF layoutRect(origin.X, origin.Y, 0.0f, 0.0f);
			return updateStatus(DLLExports::GdipDrawString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect,
				stringFormat ? stringFormat->nativeStringFormat : NULL,
				brush ? brush->nativeBrush : NULL));
		}
		Status DrawString(const WCHAR* string, INT length,
			const Font* font, const RectF& layoutRect,
			const StringFormat* stringFormat, const Brush* brush)
		{
			return updateStatus(DLLExports::GdipDrawString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect,
				stringFormat ? stringFormat->nativeStringFormat : NULL,
				brush ? brush->nativeBrush : NULL));
		}
		Status EndContainer(GraphicsContainer state)
		{
			return updateStatus(DLLExports::GdipEndContainer(
				nativeGraphics, state));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const PointF& destPoint,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileDestPoint(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destPoint, callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const Point& destPoint,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileDestPointI(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destPoint, callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const RectF& destRect,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileDestRect(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destRect, callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const Rect& destRect,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileDestRectI(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destRect, callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const PointF* destPoints, INT count,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileDestPoints(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				destPoints, count, callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const Point* destPoints, INT count,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileDestPointsI(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				destPoints, count, callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const PointF& destPoint,
			const RectF& srcRect, Unit srcUnit,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileSrcRectDestPoint(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destPoint, &srcRect, srcUnit,
				callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const Point& destPoint,
			const Rect& srcRect, Unit srcUnit,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileSrcRectDestPointI(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destPoint, &srcRect, srcUnit,
				callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const RectF& destRect,
			const RectF& srcRect, Unit srcUnit,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileSrcRectDestRect(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destRect, &srcRect, srcUnit,
				callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const Rect& destRect,
			const Rect& srcRect, Unit srcUnit,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileSrcRectDestRectI(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				&destRect, &srcRect, srcUnit,
				callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const PointF* destPoints, INT count,
			const RectF& srcRect, Unit srcUnit,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileSrcRectDestPoints(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				destPoints, count, &srcRect, srcUnit,
				callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status EnumerateMetafile(const Metafile* metafile,
			const Point* destPoints, INT count,
			const Rect& srcRect, Unit srcUnit,
			EnumerateMetafileProc callback,
			VOID* callbackData = NULL,
			ImageAttributes* imageAttributes = NULL)
		{
			return updateStatus(DLLExports::GdipEnumerateMetafileSrcRectDestPointsI(
				nativeGraphics,
				metafile ? ((GpMetafile*)metafile->nativeImage) : NULL,
				destPoints, count, &srcRect, srcUnit,
				callback, callbackData,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL));
		}
		Status ExcludeClip(const RectF& rect)
		{
			return updateStatus(DLLExports::GdipSetClipRect(
				nativeGraphics,
				rect.X, rect.Y, rect.Width, rect.Height,
				CombineModeExclude));
		}
		Status ExcludeClip(const Rect& rect)
		{
			return updateStatus(DLLExports::GdipSetClipRectI(
				nativeGraphics,
				rect.X, rect.Y, rect.Width, rect.Height,
				CombineModeExclude));
		}
		Status ExcludeClip(const Region* region)
		{
			return updateStatus(DLLExports::GdipSetClipRegion(
				nativeGraphics,
				region ? region->nativeRegion : NULL,
				CombineModeExclude));
		}
		Status FillClosedCurve(const Brush* brush,
			const PointF* points, INT count)
		{
			return updateStatus(DLLExports::GdipFillClosedCurve(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count));
		}
		Status FillClosedCurve(const Brush* brush,
			const Point* points, INT count)
		{
			return updateStatus(DLLExports::GdipFillClosedCurveI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count));
		}
		Status FillClosedCurve(const Brush* brush,
			const PointF* points, INT count,
			FillMode fillMode, REAL tension = 0.5f)
		{
			return updateStatus(DLLExports::GdipFillClosedCurve2(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count, tension, fillMode));
		}
		Status FillClosedCurve(const Brush* brush,
			const Point* points, INT count,
			FillMode fillMode, REAL tension = 0.5f)
		{
			return updateStatus(DLLExports::GdipFillClosedCurve2I(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count, tension, fillMode));
		}
		Status FillEllipse(const Brush* brush,
			REAL x, REAL y, REAL width, REAL height)
		{
			return updateStatus(DLLExports::GdipFillEllipse(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				x, y, width, height));
		}
		Status FillEllipse(const Brush* brush,
			INT x, INT y, INT width, INT height)
		{
			return updateStatus(DLLExports::GdipFillEllipseI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				x, y, width, height));
		}
		Status FillEllipse(const Brush* brush, const RectF& rect)
		{
			return updateStatus(DLLExports::GdipFillEllipse(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status FillEllipse(const Brush* brush, const Rect& rect)
		{
			return updateStatus(DLLExports::GdipFillEllipseI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status FillPath(const Brush* brush, const GraphicsPath* path)
		{
			return updateStatus(DLLExports::GdipFillPath(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				path ? path->nativePath : NULL));
		}
		Status FillPie(const Brush* brush,
			REAL x, REAL y, REAL width, REAL height,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipFillPie(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				x, y, width, height, startAngle, sweepAngle));
		}
		Status FillPie(const Brush* brush, INT x, INT y, INT width, INT height,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipFillPieI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				x, y, width, height, startAngle, sweepAngle));
		}
		Status FillPie(const Brush* brush, const RectF& rect,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipFillPie(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rect.X, rect.Y, rect.Width, rect.Height,
				startAngle, sweepAngle));
		}
		Status FillPie(const Brush* brush, const Rect& rect,
			REAL startAngle, REAL sweepAngle)
		{
			return updateStatus(DLLExports::GdipFillPieI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rect.X, rect.Y, rect.Width, rect.Height,
				startAngle, sweepAngle));
		}
		Status FillPolygon(const Brush* brush, const PointF* points, INT count)
		{
			return updateStatus(DLLExports::GdipFillPolygon(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count, FillModeAlternate));
		}
		Status FillPolygon(const Brush* brush, const Point* points, INT count)
		{
			return updateStatus(DLLExports::GdipFillPolygonI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count, FillModeAlternate));
		}
		Status FillPolygon(const Brush* brush, const PointF* points, INT count,
			FillMode fillMode)
		{
			return updateStatus(DLLExports::GdipFillPolygon(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count, fillMode));
		}
		Status FillPolygon(const Brush* brush, const Point* points, INT count,
			FillMode fillMode)
		{
			return updateStatus(DLLExports::GdipFillPolygonI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				points, count, fillMode));
		}
		Status FillRectangle(const Brush* brush,
			REAL x, REAL y, REAL width, REAL height)
		{
			return updateStatus(DLLExports::GdipFillRectangle(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				x, y, width, height));
		}
		Status FillRectangle(const Brush* brush,
			INT x, INT y, INT width, INT height)
		{
			return updateStatus(DLLExports::GdipFillRectangleI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				x, y, width, height));
		}
		Status FillRectangle(const Brush* brush, const RectF& rect)
		{
			return updateStatus(DLLExports::GdipFillRectangle(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status FillRectangle(const Brush* brush, const Rect& rect)
		{
			return updateStatus(DLLExports::GdipFillRectangleI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rect.X, rect.Y, rect.Width, rect.Height));
		}
		Status FillRectangles(const Brush* brush, const RectF* rects, INT count)
		{
			return updateStatus(DLLExports::GdipFillRectangles(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rects, count));
		}
		Status FillRectangles(const Brush* brush, const Rect* rects, INT count)
		{
			return updateStatus(DLLExports::GdipFillRectanglesI(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				rects, count));
		}
		Status FillRegion(const Brush* brush, const Region* region)
		{
			return updateStatus(DLLExports::GdipFillRegion(
				nativeGraphics,
				brush ? brush->nativeBrush : NULL,
				region ? region->nativeRegion : NULL));
		}
		VOID StartFrame(REAL width, REAL height, REAL pixelRatio)
		{
			updateStatus(DLLExports::GdipStartFrame(nativeGraphics, width, height, pixelRatio));
		}
		VOID CancelFrame()
		{
			updateStatus(DLLExports::GdipCancelFrame(nativeGraphics));
		}
		VOID Flush(FlushIntention intention = FlushIntentionFlush)
		{
			updateStatus(DLLExports::GdipFlush(nativeGraphics, intention));
		}
		Status GetClip(Region* region) const
		{
			return updateStatus(DLLExports::GdipGetClip(
				nativeGraphics,
				region ? region->nativeRegion : NULL));
		}
		Status GetClipBounds(RectF* rect) const
		{
			return updateStatus(DLLExports::GdipGetClipBounds(
				nativeGraphics, rect));
		}
		Status GetClipBounds(Rect* rect) const
		{
			return updateStatus(DLLExports::GdipGetClipBoundsI(
				nativeGraphics, rect));
		}
		CompositingMode GetCompositingMode() const
		{
			CompositingMode result = CompositingModeSourceOver;
			updateStatus(DLLExports::GdipGetCompositingMode(
				nativeGraphics, &result));
			return result;
		}
		CompositingQuality GetCompositingQuality() const
		{
			CompositingQuality result = CompositingQualityDefault;
			updateStatus(DLLExports::GdipGetCompositingQuality(
				nativeGraphics, &result));
			return result;
		}
		REAL GetDpiX() const
		{
			REAL result = 0.0f;
			updateStatus(DLLExports::GdipGetDpiX(nativeGraphics, &result));
			return result;
		}
		REAL GetDpiY() const
		{
			REAL result = 0.0f;
			updateStatus(DLLExports::GdipGetDpiY(nativeGraphics, &result));
			return result;
		}
		HDC GetHDC()
		{
			HDC result = NULL;
			updateStatus(DLLExports::GdipGetDC(nativeGraphics, &result));
			return result;
		}
		InterpolationMode GetInterpolationMode() const
		{
			InterpolationMode result = InterpolationModeDefault;
			updateStatus(DLLExports::GdipGetInterpolationMode(
				nativeGraphics, &result));
			return result;
		}
		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}
		Status GetNearestColor(Color* color) const
		{
			return updateStatus(DLLExports::GdipGetNearestColor(
				nativeGraphics, color ? &color->Value : NULL));
		}
		REAL GetPageScale() const
		{
			REAL result = 0.0f;
			updateStatus(DLLExports::GdipGetPageScale(
				nativeGraphics, &result));
			return result;
		}
		Unit GetPageUnit() const
		{
			Unit result = UnitWorld;
			updateStatus(DLLExports::GdipGetPageUnit(
				nativeGraphics, &result));
			return result;
		}
		PixelOffsetMode GetPixelOffsetMode() const
		{
			PixelOffsetMode result = PixelOffsetModeDefault;
			updateStatus(DLLExports::GdipGetPixelOffsetMode(
				nativeGraphics, &result));
			return result;
		}
		Status GetRenderingOrigin(INT* x, INT* y) const
		{
			return updateStatus(DLLExports::GdipGetRenderingOrigin(
				nativeGraphics, x, y));
		}
		SmoothingMode GetSmoothingMode() const
		{
			SmoothingMode result = SmoothingModeDefault;
			updateStatus(DLLExports::GdipGetSmoothingMode(
				nativeGraphics, &result));
			return result;
		}
		UINT GetTextContrast() const
		{
			UINT result = 0;
			updateStatus(DLLExports::GdipGetTextContrast(
				nativeGraphics, &result));
			return result;
		}
		TextRenderingHint GetTextRenderingHint() const
		{
			TextRenderingHint result = TextRenderingHintSystemDefault;
			updateStatus(DLLExports::GdipGetTextRenderingHint(
				nativeGraphics, &result));
			return result;
		}
		Status GetTransform(Matrix* matrix) const
		{
			return updateStatus(DLLExports::GdipGetWorldTransform(
				nativeGraphics,
				matrix ? matrix->nativeMatrix : NULL));
		}
		Status GetVisibleClipBounds(RectF* rect) const
		{
			return updateStatus(DLLExports::GdipGetVisibleClipBounds(
				nativeGraphics, rect));
		}
		Status GetVisibleClipBounds(Rect* rect) const
		{
			return updateStatus(DLLExports::GdipGetVisibleClipBoundsI(
				nativeGraphics, rect));
		}
		Status IntersectClip(const RectF& rect)
		{
			return updateStatus(DLLExports::GdipSetClipRect(
				nativeGraphics,
				rect.X, rect.Y, rect.Width, rect.Height,
				CombineModeIntersect));
		}
		Status IntersectClip(const Rect& rect)
		{
			return updateStatus(DLLExports::GdipSetClipRectI(
				nativeGraphics,
				rect.X, rect.Y, rect.Width, rect.Height,
				CombineModeIntersect));
		}
		Status IntersectClip(const Region* region)
		{
			return updateStatus(DLLExports::GdipSetClipRegion(
				nativeGraphics,
				region ? region->nativeRegion : NULL,
				CombineModeIntersect));
		}
		BOOL IsClipEmpty() const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsClipEmpty(
				nativeGraphics, &result));
			return result;
		}
		BOOL IsVisible(REAL x, REAL y) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisiblePoint(
				nativeGraphics, x, y, &result));
			return result;
		}
		BOOL IsVisible(INT x, INT y) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisiblePointI(
				nativeGraphics, x, y, &result));
			return result;
		}
		BOOL IsVisible(const PointF& point) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisiblePoint(
				nativeGraphics, point.X, point.Y, &result));
			return result;
		}
		BOOL IsVisible(const Point& point) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisiblePointI(
				nativeGraphics, point.X, point.Y, &result));
			return result;
		}
		BOOL IsVisible(REAL x, REAL y, REAL width, REAL height) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisibleRect(
				nativeGraphics, x, y, width, height, &result));
			return result;
		}
		BOOL IsVisible(INT x, INT y, INT width, INT height) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisibleRectI(
				nativeGraphics, x, y, width, height, &result));
			return result;
		}
		BOOL IsVisible(const RectF& rect) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisibleRect(
				nativeGraphics, rect.X, rect.Y,
				rect.Width, rect.Height, &result));
			return result;
		}
		BOOL IsVisible(const Rect& rect) const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisibleRectI(
				nativeGraphics, rect.X, rect.Y,
				rect.Width, rect.Height, &result));
			return result;
		}
		BOOL IsVisibleClipEmpty() const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipIsVisibleClipEmpty(
				nativeGraphics, &result));
			return result;
		}
		Status MeasureCharacterRanges(const WCHAR* string, INT length,
			const Font* font, const RectF& layoutRect,
			const StringFormat* stringFormat,
			INT regionCount, Region* regions) const
		{
			if (regionCount <= 0 || !regions)
				return lastStatus = GpStatus::InvalidParameter;

			GpRegion** nativeRegionArray = new GpRegion*[regionCount];
			if (!nativeRegionArray)
				return lastStatus = OutOfMemory;
			for (int i = 0; i < regionCount; ++i) {
				nativeRegionArray[i] = regions[i].nativeRegion;
			}
			Status status = updateStatus(DLLExports::GdipMeasureCharacterRanges(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect,
				stringFormat ? stringFormat->nativeStringFormat : NULL,
				regionCount, nativeRegionArray));
			delete[](nativeRegionArray);
			return status;
		}
		Status MeasureDriverString(const UINT16* text, INT length,
			const Font* font, const PointF* positions, INT flags,
			const Matrix* matrix, RectF* boundingBox) const
		{
			return updateStatus(DLLExports::GdipMeasureDriverString(
				nativeGraphics, text, length,
				font ? font->nativeFont : NULL,
				positions, flags,
				matrix ? matrix->nativeMatrix : NULL,
				boundingBox));
		}
		Status MeasureString(const WCHAR* string, INT length,
			const Font* font, const RectF& layoutRect,
			RectF* boundingBox) const
		{
			return updateStatus(DLLExports::GdipMeasureString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect, NULL, boundingBox, NULL, NULL));
		}
		Status MeasureString(const WCHAR* string, INT length,
			const Font* font, const RectF& layoutRect,
			const StringFormat* stringFormat, RectF* boundingBox,
			INT* codepointsFitted = NULL,
			INT* linesFitted = NULL) const
		{
			return updateStatus(DLLExports::GdipMeasureString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect,
				stringFormat ? stringFormat->nativeStringFormat : NULL,
				boundingBox, codepointsFitted, linesFitted));
		}
		Status MeasureString(const WCHAR* string, INT length,
			const Font* font, const SizeF& layoutRectSize,
			const StringFormat* stringFormat, SizeF* size,
			INT* codepointsFitted = NULL,
			INT* linesFitted = NULL) const
		{
			if (!size) return lastStatus = GpStatus::InvalidParameter;
			RectF layoutRect(PointF(0.0f, 0.0f), layoutRectSize);
			RectF boundingBox;
			Status status = updateStatus(DLLExports::GdipMeasureString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect,
				stringFormat ? stringFormat->nativeStringFormat : NULL,
				&boundingBox, codepointsFitted, linesFitted));
			boundingBox.GetSize(size);
			return status;
		}
		Status MeasureString(const WCHAR* string, INT length,
			const Font* font, const PointF& origin,
			RectF* boundingBox) const
		{
			RectF layoutRect(origin, SizeF(0.0f, 0.0f));
			return updateStatus(DLLExports::GdipMeasureString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect, NULL, boundingBox, NULL, NULL));
		}
		Status MeasureString(const WCHAR* string, INT length,
			const Font* font, const PointF& origin,
			const StringFormat* stringFormat,
			RectF* boundingBox) const
		{
			RectF layoutRect(origin, SizeF(0.0f, 0.0f));
			return updateStatus(DLLExports::GdipMeasureString(
				nativeGraphics, string, length,
				font ? font->nativeFont : NULL,
				&layoutRect,
				stringFormat ? stringFormat->nativeStringFormat : NULL,
				boundingBox, NULL, NULL));
		}
		Status MultiplyTransform(const Matrix* matrix,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipMultiplyWorldTransform(
				nativeGraphics,
				matrix ? matrix->nativeMatrix : NULL, order));
		}
		VOID ReleaseHDC(HDC hdc)
		{
			updateStatus(DLLExports::GdipReleaseDC(nativeGraphics, hdc));
		}
		Status ResetClip()
		{
			return updateStatus(DLLExports::GdipResetClip(nativeGraphics));
		}
		Status ResetTransform()
		{
			return updateStatus(DLLExports::GdipResetWorldTransform(
				nativeGraphics));
		}
		Status Restore(GraphicsState state)
		{
			return updateStatus(DLLExports::GdipRestoreGraphics(
				nativeGraphics, state));
		}
		Status RotateTransform(REAL angle,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipRotateWorldTransform(
				nativeGraphics, angle, order));
		}
		GraphicsState Save() const
		{
			GraphicsState result = 0;
			updateStatus(DLLExports::GdipSaveGraphics(
				nativeGraphics, &result));
			return result;
		}
		Status ScaleTransform(REAL sx, REAL sy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipScaleWorldTransform(
				nativeGraphics, sx, sy, order));
		}
		VOID SetAbort()
		{
			updateStatus(NotImplemented);
		}
		Status SetClip(const Graphics* g,
			CombineMode combineMode = CombineModeReplace)
		{
			return updateStatus(DLLExports::GdipSetClipGraphics(
				nativeGraphics, g ? g->nativeGraphics : NULL,
				combineMode));
		}
		Status SetClip(const RectF& rect,
			CombineMode combineMode = CombineModeReplace)
		{
			return updateStatus(DLLExports::GdipSetClipRect(
				nativeGraphics,
				rect.X, rect.Y, rect.Width, rect.Height,
				combineMode));
		}
		Status SetClip(const Rect& rect,
			CombineMode combineMode = CombineModeReplace)
		{
			return updateStatus(DLLExports::GdipSetClipRectI(
				nativeGraphics,
				rect.X, rect.Y, rect.Width, rect.Height,
				combineMode));
		}
		Status SetClip(const GraphicsPath* path,
			CombineMode combineMode = CombineModeReplace)
		{
			return updateStatus(DLLExports::GdipSetClipPath(
				nativeGraphics,
				path ? path->nativePath : NULL,
				combineMode));
		}
		Status SetClip(const Region* region,
			CombineMode combineMode = CombineModeReplace)
		{
			return updateStatus(DLLExports::GdipSetClipRegion(
				nativeGraphics,
				region ? region->nativeRegion : NULL,
				combineMode));
		}
		Status SetClip(HRGN hRgn, CombineMode combineMode = CombineModeReplace)
		{
			return updateStatus(DLLExports::GdipSetClipHrgn(
				nativeGraphics, hRgn, combineMode));
		}
		Status SetCompositingMode(CompositingMode compositingMode)
		{
			return updateStatus(DLLExports::GdipSetCompositingMode(
				nativeGraphics, compositingMode));
		}
		Status SetCompositingQuality(CompositingQuality compositingQuality)
		{
			return updateStatus(DLLExports::GdipSetCompositingQuality(
				nativeGraphics, compositingQuality));
		}
		Status SetInterpolationMode(InterpolationMode interpolationMode)
		{
			return updateStatus(DLLExports::GdipSetInterpolationMode(
				nativeGraphics, interpolationMode));
		}
		Status SetPageScale(REAL scale)
		{
			return updateStatus(DLLExports::GdipSetPageScale(
				nativeGraphics, scale));
		}
		Status SetPageUnit(Unit unit)
		{
			return updateStatus(DLLExports::GdipSetPageUnit(
				nativeGraphics, unit));
		}
		Status SetPixelOffsetMode(PixelOffsetMode pixelOffsetMode)
		{
			return updateStatus(DLLExports::GdipSetPixelOffsetMode(
				nativeGraphics, pixelOffsetMode));
		}
		Status SetRenderingOrigin(INT x, INT y)
		{
			return updateStatus(DLLExports::GdipSetRenderingOrigin(
				nativeGraphics, x, y));
		}
		Status SetSmoothingMode(SmoothingMode smoothingMode)
		{
			return updateStatus(DLLExports::GdipSetSmoothingMode(
				nativeGraphics, smoothingMode));
		}
		Status SetTextContrast(UINT contrast)
		{
			return updateStatus(DLLExports::GdipSetTextContrast(
				nativeGraphics, contrast));
		}
		Status SetTextRenderingHint(TextRenderingHint textRenderingHint)
		{
			return updateStatus(DLLExports::GdipSetTextRenderingHint(
				nativeGraphics, textRenderingHint));
		}
		Status SetTransform(const Matrix* matrix)
		{
			return updateStatus(DLLExports::GdipSetWorldTransform(
				nativeGraphics,
				matrix ? matrix->nativeMatrix : NULL));
		}
		Status TransformPoints(CoordinateSpace destSpace,
			CoordinateSpace srcSpace,
			PointF* pts, INT count) const
		{
			return updateStatus(DLLExports::GdipTransformPoints(
				nativeGraphics, destSpace, srcSpace,
				pts, count));
		}
		Status TransformPoints(CoordinateSpace destSpace,
			CoordinateSpace srcSpace,
			Point* pts, INT count) const
		{
			return updateStatus(DLLExports::GdipTransformPointsI(
				nativeGraphics, destSpace, srcSpace,
				pts, count));
		}
		Status TranslateClip(REAL dx, REAL dy)
		{
			return updateStatus(DLLExports::GdipTranslateClip(
				nativeGraphics, dx, dy));
		}
		Status TranslateClip(INT dx, INT dy)
		{
			return updateStatus(DLLExports::GdipTranslateClipI(
				nativeGraphics, dx, dy));
		}
		Status TranslateTransform(REAL dx, REAL dy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipTranslateWorldTransform(
				nativeGraphics, dx, dy, order));
		}

	private:
		Graphics(const Graphics&);
		Graphics& operator=(const Graphics&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != GpStatus::Ok) lastStatus = newStatus;
			return newStatus;
		}

		GpGraphics* nativeGraphics;
		mutable Status lastStatus;
	};
}
#endif /* __GDIPLUS_GRAPHICS_H */
