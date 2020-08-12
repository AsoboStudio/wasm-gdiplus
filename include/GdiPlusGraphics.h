// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_GRAPHICS_H
#define __GDIPLUS_GRAPHICS_H

#include "gdiplus.h"

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusgraphics.h."
#endif
namespace Gdiplus {

    class Graphics//: public GdiplusBase
    {
    public:
        friend class Region;
        friend class GraphicsPath;
        friend class Image;
        friend class Bitmap;
        friend class Metafile;
        friend class Font;
        friend class FontFamily;
        friend class FontCollection;
        friend class CachedBitmap;

        NVGcontext* ctx;

        Graphics(NVGcontext* _ctx);
        Graphics(Image* image);
        Status DrawString(const WCHAR* string, INT length, const Font* font, const PointF& origin, const Brush* brush);
        Status DrawString(const WCHAR* string, INT length, const Font* font, const PointF& origin, const StringFormat* stringFormat, const Brush* brush);
        Status DrawString(const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, const StringFormat* stringFormat, const Brush* brush);
        Status DrawLine(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2);
        Status DrawLine(const Pen* pen, INT x1, INT y1, INT x2, INT y2);
        Status DrawLine(const Pen* pen, const PointF& pt1, const PointF& pt2);
        Status ResetClip();
        Status ResetTransform();
        Status SetTransform(const Matrix* matrix);
        Status TranslateTransform(REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend);
        Status RotateTransform(REAL angle, MatrixOrder order = MatrixOrderPrepend);
        Status ScaleTransform(REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend);
        Status DrawPolygon(const Pen* pen, const PointF* points, INT count);
        Status DrawPolygon(const Pen* pen, const Point* points, INT count);
        Status DrawRectangle(const Pen* pen, REAL x, REAL y, REAL width, REAL height);
        Status DrawRectangle(const Pen* pen, INT x, INT y, INT width, INT height);
        Status DrawRectangle(const Pen* pen, const RectF& rect);
        Status DrawRectangle(const Pen* pen, const Rect& rect);
        Status DrawCurve(const Pen* pen, const PointF* points, INT count);
        Status DrawCurve(const Pen* pen, const Point* points, INT count);
        Status DrawCurve(const Pen* pen, const PointF* points, INT count, REAL tension);
        Status DrawCurve(const Pen* pen, const Point* points, INT count, REAL tension);
        Status DrawCurve(const Pen* pen, const PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension);
        Status DrawCurve(const Pen* pen, const Point* points, INT count, INT offset, INT numberOfSegments, REAL tension);
        Status DrawEllipse(const Pen* pen, REAL x, REAL y, REAL width, REAL height);
        Status DrawEllipse(const Pen* pen, INT x, INT y, INT width, INT height);
        Status DrawEllipse(const Pen* pen, const RectF& rect);
        Status DrawEllipse(const Pen* pen, const Rect& rect);
        Status DrawImage(Image* image, REAL x, REAL y);
        Status DrawImage(Image* image, INT x, INT y);
        Status DrawImage(Image* image, const PointF& point);
        Status DrawImage(Image* image, const Point& point);
        Status DrawImage(Image* image, REAL x, REAL y, REAL width, REAL height);
        Status DrawImage(Image* image, INT x, INT y, INT width, INT height);
        Status DrawImage(Image* image, const RectF& rect);
        Status DrawImage(Image* image, const Rect& rect);
        Status DrawImage(Image* image, const PointF* destPoints, INT count);
        Status DrawImage(Image* image, const Point* destPoints, INT count);
        Status DrawImage(Image* image, REAL x, REAL y, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit);
        Status DrawImage(Image* image, INT x, INT y, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit);
        /*
        Status DrawImage(Image* image, const RectF& destRect, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL);
        Status DrawImage(Image* image, const Rect& destRect, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL);
        Status DrawImage(Image* image, const RectF& destRect, const RectF& sourceRect, Unit srcUnit, const ImageAttributes* imageAttributes = NULL);
        Status DrawImage(Image* image, const PointF* destPoints, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL);
        Status DrawImage(Image* image, const Point* destPoints, INT count, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, const ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL);
        */
        Status FillRectangle(const Brush* brush, REAL x, REAL y, REAL width, REAL height);
        Status FillRectangle(const Brush* brush, INT x, INT y, INT width, INT height);
        Status FillRectangle(const Brush* brush, const RectF& rect);
        Status FillRectangle(const Brush* brush, const Rect& rect);
        Status SetPageUnit(Unit unit);
        Status DrawPath(const Pen* pen, const GraphicsPath* path);
        Status FillEllipse(const Brush* brush, REAL x, REAL y, REAL width, REAL height);
        Status FillEllipse(const Brush* brush, INT x, INT y, INT width, INT height);
        Status FillEllipse(const Brush* brush, const RectF& rect);
        Status FillEllipse(const Brush* brush, const Rect& rect);
        Status FillPath(const Brush* brush, const GraphicsPath* path);
        Status FillPie(const Brush* brush, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
        Status FillPie(const Brush* brush, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
        Status FillPie(const Brush* brush, const RectF& rect, REAL startAngle, REAL sweepAngle);
        Status FillPie(const Brush* brush, const Rect& rect, REAL startAngle, REAL sweepAngle);
        Status FillPolygon(const Brush* brush, const PointF* points, INT count);
        Status FillPolygon(const Brush* brush, const Point* points, INT count);
        Status FillPolygon(const Brush* brush, const PointF* points, INT count, FillMode fillMode);
        Status FillPolygon(const Brush* brush, const Point* points, INT count, FillMode fillMode);
        Status MeasureString(const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, RectF* boundingBox) const;
        Status MeasureString(const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, const StringFormat* stringFormat, RectF* boundingBox, INT* codepointsFitted = NULL, INT* linesFitted = NULL) const;
        Status MeasureString(const WCHAR* string, INT length, const Font* font, const SizeF& layoutRectSize, const StringFormat* stringFormat, SizeF* size, INT* codepointsFitted = NULL, INT* linesFitted = NULL) const;
        Status MeasureString(const WCHAR* string, INT length, const Font* font, const PointF& origin, RectF* boundingBox) const;
        Status MeasureString(const WCHAR* string, INT length, const Font* font, const PointF& origin, const StringFormat* stringFormat, RectF* boundingBox) const;
        Status SetSmoothingMode(SmoothingMode smoothingMode);
        Status SetTextRenderingHint(TextRenderingHint textRenderingHint);
        Status Clear(const Color& color);
        Status DrawArc(const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
        Status DrawArc(const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
        Status DrawArc(const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle);
        Status DrawArc(const Pen* pen, const Rect& rect, REAL startAngle, REAL sweepAngle);
        Status DrawBezier(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);
        Status DrawBezier(const Pen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4);
        Status DrawBezier(const Pen* pen, const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4);
        Status DrawBezier(const Pen* pen, const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4);
        Status DrawLines(const Pen* pen, const PointF* points, INT count);
        Status GetTransform(Matrix* matrix) const;
        Status GetVisibleClipBounds(RectF* rect) const;
        Status GetVisibleClipBounds(Rect* rect) const;
        Status SetClip(const GraphicsPath* path, CombineMode combineMode = CombineModeReplace);

        Status Release();
        ~Graphics();

    private:
        int m_brushPatterns[HatchStyleTotal];
        void ConvertBrush(const Brush* brush);
        void nvg_SetAlignment(const StringFormat* stringFormat);
        bool m_clipped;

        /**** NOT SUPPORTED YET***\
            static Graphics* FromImage(IN Image *image);
            VOID Flush(IN FlushIntention intention = FlushIntentionFlush);

            //------------------------------------------------------------------------
            // Rendering modes
            //------------------------------------------------------------------------
            Status SetRenderingOrigin(IN INT x, IN INT y);
            Status GetRenderingOrigin(OUT INT *x, OUT INT *y) const;
            Status SetCompositingMode(IN CompositingMode compositingMode);
            CompositingMode GetCompositingMode() const;
            Status SetCompositingQuality(IN CompositingQuality compositingQuality);
            CompositingQuality GetCompositingQuality() const;
            Status SetTextRenderingHint(IN TextRenderingHint newMode);
            TextRenderingHint GetTextRenderingHint() const;
            Status SetTextContrast(IN UINT contrast);
            UINT GetTextContrast() const;
            InterpolationMode GetInterpolationMode() const;
            Status SetInterpolationMode(IN InterpolationMode interpolationMode);
            Status SetAbort(GdiplusAbort *pIAbort);
            SmoothingMode GetSmoothingMode() const;
            Status SetSmoothingMode(IN SmoothingMode smoothingMode);
            PixelOffsetMode GetPixelOffsetMode() const;
            Status SetPixelOffsetMode(IN PixelOffsetMode pixelOffsetMode);

            //------------------------------------------------------------------------
            // Manipulate current world transform
            //------------------------------------------------------------------------

            Status MultiplyTransform(IN const Matrix* matrix,
                                     IN MatrixOrder order = MatrixOrderPrepend);

            Status TranslateTransform(IN REAL dx,
                                      IN REAL dy,
                                      IN MatrixOrder order = MatrixOrderPrepend);
            Status ScaleTransform(IN REAL sx,
                                  IN REAL sy,
                                  IN MatrixOrder order = MatrixOrderPrepend);

            Status RotateTransform(IN REAL angle,
                                   IN MatrixOrder order = MatrixOrderPrepend);

            Status GetTransform(OUT Matrix* matrix) const;
            Status SetPageUnit(IN Unit unit);

            Status SetPageScale(IN REAL scale); GetPageUnit() const;
            REAL GetPageScale() const;

            REAL GetDpiX() const;
            REAL GetDpiY() const;

            Status TransformPoints(IN CoordinateSpace destSpace,
                                   IN CoordinateSpace srcSpace,
                                   IN OUT PointF* pts,
                                   IN INT count) const;

            Status TransformPoints(IN CoordinateSpace destSpace,
                                   IN CoordinateSpace srcSpace,
                                   IN OUT Point* pts,
                                   IN INT count) const;

            //------------------------------------------------------------------------
            // GetNearestColor (for <= 8bpp surfaces).  Note: Alpha is ignored.
            //------------------------------------------------------------------------
              Status GetNearestColor(IN OUT Color* color) const;

             //------------------------------------------------------------------------
            // Drawing functions
            //------------------------------------------------------------------------
            Status DrawLines(IN const Pen* pen,
                             IN const PointF* points,
                             IN INT count);


            Status DrawLine(IN const Pen* pen,
                            IN INT x1,
                            IN INT y1,
                            IN INT x2,
                            IN INT y2);

            Status DrawLine(IN const Pen* pen,
                            IN const Point& pt1,
                            IN const Point& pt2);

            Status DrawLines(IN const Pen* pen,
                             IN const Point* points,
                             IN INT count);

            Status DrawBezier(IN const Pen* pen,
                              IN REAL x1,
                              IN REAL y1,
                              IN REAL x2,
                              IN REAL y2,
                              IN REAL x3,
                              IN REAL y3,
                              IN REAL x4,
                              IN REAL y4);

            Status DrawBezier(IN const Pen* pen,
                              IN const PointF& pt1,
                              IN const PointF& pt2,
                              IN const PointF& pt3,
                              IN const PointF& pt4);
            Status DrawBeziers(IN const Pen* pen,
                               IN const PointF* points,
                               IN INT count);

            Status DrawBezier(IN const Pen* pen,
                              IN INT x1,
                              IN INT y1,
                              IN INT x2,
                              IN INT y2,
                              IN INT x3,
                              IN INT y3,
                              IN INT x4,
                              IN INT y4);

            Status DrawBezier(IN const Pen* pen,
                              IN const Point& pt1,
                              IN const Point& pt2,
                              IN const Point& pt3,
                              IN const Point& pt4);

            Status DrawBeziers(IN const Pen* pen,
                               IN const Point* points,
                               IN INT count);

            Status DrawPath(IN const Pen* pen,
                            IN const GraphicsPath* path);

            Status DrawCurve(IN const Pen* pen,
                             IN const PointF* points,
                             IN INT count);

            Status DrawCurve(IN const Pen* pen,
                             IN const PointF* points,
                             IN INT count,
                             IN REAL tension);

            Status DrawCurve(IN const Pen* pen,
                             IN const PointF* points,
                             IN INT count,
                             IN INT offset,
                             IN INT numberOfSegments,
                             IN REAL tension = 0.5f);us DrawCurve(IN const Pen* pen,
                             IN const Point* points,
                             IN INT count);

            Status DrawCurve(IN const Pen* pen,
                             IN const Point* points,
                             IN INT count,
                             IN REAL tension);

            Status DrawCurve(IN const Pen* pen,
                             IN const Point* points,
                             IN INT count,
                             IN INT offset,
                             IN INT numberOfSegments,
                             IN REAL tension = 0.5f);

            Status DrawClosedCurve(IN const Pen* pen,
                                   IN const PointF* points,
                                   IN INT count);
            Status DrawClosedCurve(IN const Pen *pen,
                                   IN const PointF* points,
                                   IN INT count,
                                   IN REAL tension);

            Status DrawClosedCurve(IN const Pen* pen,
                                   IN const Point* points,
                                   IN INT count);

            Status DrawClosedCurve(IN const Pen *pen,
                                   IN const Point* points,
                                   IN INT count,
                                   IN REAL tension);

            Status Clear(IN const Color &color);

            Status FillRectangles(IN const Brush* brush,
                                  IN const RectF* rects,
                                  IN INT count);

            Status FillRectangles(IN const Brush* brush,
                                  IN const Rect* rects,
                                  IN INT count);

            Status FillPath(IN const Brush* brush,
                            IN const GraphicsPath* path);

            Status FillClosedCurve(IN const Brush* brush,
                                   IN const PointF* points,
                                   IN INT count);

            Status FillClosedCurve(IN const Brush* brush,
                                   IN const PointF* points,
                                   IN INT count,
                                   IN FillMode fillMode,
                                   IN REAL tension = 0.5f);
            Status FillClosedCurve(IN const Brush* brush,
                                   IN const Point* points,
                                   IN INT count);

            Status FillClosedCurve(IN const Brush* brush,
                                   IN const Point* points,
                                   IN INT count,
                                   IN FillMode fillMode,
                                   IN REAL tension = 0.5f);

            Status FillRegion(IN const Brush* brush,
                              IN const Region* region);

            Status
            DrawString(
                IN const WCHAR        *string,
                IN INT                 length,
                IN const Font         *font,
                IN const RectF        &layoutRect,
                IN const StringFormat *stringFormat,
                IN const Brush        *brush
            );

            Status
            DrawString(
                const WCHAR        *string,
                INT                 length,
                const Font         *font,
                const PointF       &origin,
                const Brush        *brush
            );

            Status
            DrawString(
                const WCHAR        *string,
                INT                 length,
                const Font         *font,
                const PointF       &origin,
                const StringFormat *stringFormat,
                const Brush        *brush
            );

            Status
            MeasureString(
                IN const WCHAR        *string,
                IN INT                 length,
                IN const Font         *font,
                IN const RectF        &layoutRect,
                IN const StringFormat *stringFormat,
                OUT RectF             *boundingBox,
                OUT INT               *codepointsFitted = 0,
                OUT INT               *linesFilled      = 0
            ) const;

            Status
            MeasureString(
                IN const WCHAR        *string,
                IN INT                 length,
                IN const Font         *font,
                IN const SizeF        &layoutRectSize,
                IN const StringFormat *stringFormat,
                OUT SizeF             *size,
                OUT INT               *codepointsFitted = 0,
                OUT INT               *linesFilled      = 0
            ) const;

            Status
            MeasureString(
                IN const WCHAR        *string,
                IN INT                 length,
                IN const Font         *font,
                IN const PointF       &origin,
                IN const StringFormat *stringFormat,
                OUT RectF             *boundingBox
            ) const;

            Status
            MeasureString(
                IN const WCHAR  *string,
                IN INT           length,
                IN const Font   *font,
                IN const RectF  &layoutRect,
                OUT RectF       *boundingBox
            ) const;

            Status
            MeasureString(
                IN const WCHAR  *string,
                IN INT           length,
                IN const Font   *font,
                IN const PointF &origin,
                OUT RectF       *boundingBox
            ) const;

            Status
            MeasureCharacterRanges(
                IN const WCHAR        *string,
                IN INT                 length,
                IN const Font         *font,
                IN const RectF        &layoutRect,
                IN const StringFormat *stringFormat,
                IN INT                 regionCount,
                OUT Region            *regions
            ) const;


            Status DrawDriverString(
                IN const UINT16  *text,
                IN INT            length,
                IN const Font    *font,
                IN const Brush   *brush,
                IN const PointF  *positions,
                IN INT            flags,
                IN const Matrix        *matrix
            );

            Status MeasureDriverString(
                IN const UINT16  *text,
                IN INT            length,
                IN const Font    *font,
                IN const PointF  *positions,
                IN INT            flags,
                IN const Matrix        *matrix,
                OUT RectF        *boundingBox
            ) const;

            // Draw a cached bitmap on this graphics destination offset by
            // x, y. Note this will fail with WrongState if the CachedBitmap
            // native format differs from this Graphics.

            Status DrawCachedBitmap(IN CachedBitmap *cb,
                                    IN INT x,
                                    IN INT y);


            // The following methods are for playing an EMF+ to a graphics
            // via the enumeration interface.  Each record of the EMF+ is
            // sent to the callback (along with the callbackData).  Then
            // the callback can invoke the Metafile::PlayRecord method
            // to play the particular record.

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const PointF &          destPoint,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const Point &           destPoint,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const RectF &           destRect,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const Rect &            destRect,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const PointF *          destPoints,
                IN INT                     count,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const Point *           destPoints,
                IN INT                     count,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const PointF &          destPoint,
                IN const RectF &           srcRect,
                IN Unit                    srcUnit,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const Point &           destPoint,
                IN const Rect &            srcRect,
                IN Unit                    srcUnit,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const RectF &           destRect,
                IN const RectF &           srcRect,
                IN Unit                    srcUnit,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const Rect &            destRect,
                IN const Rect &            srcRect,
                IN Unit                    srcUnit,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const PointF *          destPoints,
                IN INT                     count,
                IN const RectF &           srcRect,
                IN Unit                    srcUnit,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );

            Status
            EnumerateMetafile(
                IN const Metafile *        metafile,
                IN const Point *           destPoints,
                IN INT                     count,
                IN const Rect &            srcRect,
                IN Unit                    srcUnit,
                IN EnumerateMetafileProc   callback,
                IN VOID *                  callbackData    = NULL,
                IN const ImageAttributes *       imageAttributes = NULL
                );
            Status SetClip(IN const Graphics* g,
                           IN CombineMode combineMode = CombineModeReplace);

            Status SetClip(IN const RectF& rect,
                           IN CombineMode combineMode = CombineModeReplace);

            Status SetClip(IN const Rect& rect,
                           IN CombineMode combineMode = CombineModeReplace);

            Status SetClip(IN const Region* region,
                           IN CombineMode combineMode = CombineModeReplace);

            // This is different than the other SetClip methods because it assumes
            // that the HRGN is already in device units, so it doesn't transform
            // the coordinates in the HRGN.

            Status SetClip(IN HRGN hRgn,
                           IN CombineMode combineMode = CombineModeReplace);

            Status IntersectClip(IN const RectF& rect);
            Status IntersectClip(IN const Rect& rect);

            Status IntersectClip(IN const Region* region);

            Status ExcludeClip(IN const RectF& rect);

            Status ExcludeClip(IN const Rect& rect);

            Status ExcludeClip(IN const Region* region);

            Status ResetClip();

            Status TranslateClip(IN REAL dx,
                                 IN REAL dy);
            Status TranslateClip(IN INT dx,
                                 IN INT dy);

            Status GetClip(OUT Region* region) const;
            Status GetClipBounds(OUT RectF* rect) const;
            Status GetClipBounds(OUT Rect* rect) const;

            BOOL IsClipEmpty() const;

            Status GetVisibleClipBounds(OUT RectF *rect) const;

            Status GetVisibleClipBounds(OUT Rect *rect) const;

            BOOL IsVisibleClipEmpty() const;

            BOOL IsVisible(IN INT x,
                           IN INT y) const;
            BOOL IsVisible(IN const Point& point) const;

            BOOL IsVisible(IN INT x,
                           IN INT y,
                           IN INT width,
                           IN INT height) const;
            BOOL IsVisible(IN const Rect& rect) const;

            BOOL IsVisible(IN REAL x,
                           IN REAL y) const;
            BOOL IsVisible(IN const PointF& point) const;
            BOOL IsVisible(IN REAL x,
                           IN REAL y,
                           IN REAL width,
                           IN REAL height) const;

            BOOL IsVisible(IN const RectF& rect) const;
            GraphicsState Save() const;

            Status Restore(IN GraphicsState gstate);
            GraphicsContainer BeginContainer(IN const RectF &dstrect,
                                             IN const RectF &srcrect,
                                             IN Unit         unit);
            GraphicsContainer BeginContainer(IN const Rect    &dstrect,
                                             IN const Rect    &srcrect,
                                             IN Unit           unit);
            GraphicsContainer BeginContainer();
            Status EndContainer(IN GraphicsContainer state);
            // Only valid when recording metafiles.
            Status AddMetafileComment(IN const BYTE * data,
                                      IN UINT sizeData);
            static HPALETTE GetHalftonePalette();
            Status GetLastStatus() const;
        private:
            Graphics(const Graphics &);
            Graphics& operator=(const Graphics &);

        protected:
            Graphics(GpGraphics* graphics);

            VOID SetNativeGraphics(GpGraphics *graphics);
            Status SetStatus(Status status) const;
            GpGraphics* GetNativeGraphics() const;

            GpPen* GetNativePen(const Pen* pen);

            \**** NOT SUPPORTED YET***/
    };
}

#endif /* __GDIPLUS_GRAPHICS_H */
