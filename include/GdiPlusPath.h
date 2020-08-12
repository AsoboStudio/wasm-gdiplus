// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_PATH_H
#define __GDIPLUS_PATH_H

#include <MSFS\MSFS_WindowsTypes.h>
#include <MSFS\Render\GdiPlus\gdiplusheaders.h>
#include <MSFS\Render\GdiPlus\gdiplustypes.h>

#ifndef __cplusplus
#error "A C++ compiler is required to include gdipluspath.h."
#endif
namespace Gdiplus {

    // Note that some methods of GraphicsPath are implemented in gdiplusimpl.h.
    // This avoids a cyclic dependency on Graphics and Pen.

    class GraphicsPath//: public GdiplusBase
    {
    public:
        friend class Graphics;
        friend class Region;
        friend class PathGradientBrush;
        friend class GraphicsPathIterator;
        friend class CustomLineCap;

        GraphicsPath(Graphics* context, FillMode fillMode = FillModeAlternate);
        GraphicsPath(Graphics* context, const PointF* points, const BYTE* types, INT count, FillMode fillMode = FillModeAlternate);
        GraphicsPath(Graphics* context, const Point* points, const BYTE* types, INT count, FillMode fillMode = FillModeAlternate);
        Status AddEllipse(REAL x, REAL y, REAL width, REAL height);
        Status AddEllipse(INT x, INT y, INT width, INT height);
        Status AddEllipse(const RectF& rect);
        Status AddEllipse(const Rect& rect);
        Status AddArc(REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
        Status AddArc(INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
        Status AddArc(const RectF& rect, REAL startAngle, REAL sweepAngle);
        Status AddArc(const Rect& rect, REAL startAngle, REAL sweepAngle);
        Status AddPie(REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
        Status AddPie(INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
        Status AddPie(const RectF& rect, REAL startAngle, REAL sweepAngle);
        Status AddPie(const Rect& rect, REAL startAngle, REAL sweepAngle);
        Status AddRectangle(REAL x, REAL y, REAL width, REAL height);
        //Status AddRectangle(INT x, INT y, INT width, INT height);
        Status AddRectangle(const RectF& rect);
        Status AddRectangle(const Rect& rect);
        Status AddPolygon(const PointF* points, INT count);
        Status AddPolygon(const Point* points, INT count);
        Status Reset();
        Status CloseFigure();

        ~GraphicsPath() {};

        /**** NOT SUPPORTED YET***\

        GraphicsPath* Clone() const;
        FillMode GetFillMode() const;
        Status SetFillMode(IN FillMode fillmode);
        Status GetPathData(OUT PathData* pathData) const;
        Status StartFigure();
        Status CloseFigure();
        Status CloseAllFigures();
        Status SetMarker();
        Status ClearMarkers();
        Status Reverse();
        Status GetLastPoint(OUT PointF* lastPoint) const;

        Status AddLine(IN const PointF& pt1,
                       IN const PointF& pt2);

        Status AddLine(IN REAL x1,
                       IN REAL y1,
                       IN REAL x2,
                       IN REAL y2);

        Status AddLines(IN const PointF* points,
                        IN INT count);

        Status AddLine(IN const Point& pt1,
                       IN const Point& pt2);

        Status AddLine(IN INT x1,
                       IN INT y1,
                       IN INT x2,
                       IN INT y2);

        Status AddLines(IN const Point* points,
                        IN INT count);


                        Status AddBezier(IN const PointF& pt1,
                     IN const PointF& pt2,
                     IN const PointF& pt3,
                     IN const PointF& pt4);

        Status AddBezier(IN REAL x1,
                         IN REAL y1,
                         IN REAL x2,
                         IN REAL y2,
                         IN REAL x3,
                         IN REAL y3,
                         IN REAL x4,
                         IN REAL y4);

        Status AddBeziers(IN const PointF* points,
                          IN INT count);

        Status AddBezier(IN const Point& pt1,
                         IN const Point& pt2,
                         IN const Point& pt3,
                         IN const Point& pt4);

        Status AddBezier(IN INT x1,
                         IN INT y1,
                         IN INT x2,
                         IN INT y2,
                         IN INT x3,
                         IN INT y3,
                         IN INT x4,
                         IN INT y4);

        Status AddBeziers(IN const Point* points,
                          IN INT count);

        Status AddCurve(IN const PointF* points,
                        IN INT count);

        Status AddCurve(IN const PointF* points,
                        IN INT count,
                        IN REAL tension);

        Status AddCurve(IN const PointF* points,
                        IN INT count,
                        IN INT offset,
                        IN INT numberOfSegments,
                        IN REAL tension);

        Status AddCurve(IN const Point* points,
                        IN INT count);

        Status AddCurve(IN const Point* points,
                        IN INT count,
                        IN REAL tension);

        Status AddCurve(IN const Point* points,
                        IN INT count,
                        IN INT offset,
                        IN INT numberOfSegments,
                        IN REAL tension);

        Status AddClosedCurve(IN const PointF* points,
                              IN INT count);

        Status AddClosedCurve(IN const PointF* points,
                              IN INT count,
                              IN REAL tension);

        Status AddClosedCurve(IN const Point* points,
                              IN INT count);

        Status AddClosedCurve(IN const Point* points,
                              IN INT count,
                              IN REAL tension);

         Status AddRectangles(IN const RectF* rects,
                         IN INT count);

        Status AddRectangles(IN const Rect* rects, INT count);

        Status AddPath(IN const GraphicsPath* addingPath,
                   IN BOOL connect);

        Status AddString(
            IN const WCHAR         *string,
            IN INT                  length,
            IN const FontFamily    *family,
            IN INT                  style,
            IN REAL                 emSize,  // World units
            IN const PointF        &origin,
            IN const StringFormat  *format
        );

        Status AddString(
            IN const WCHAR         *string,
            IN INT                  length,
            IN const FontFamily    *family,
            IN INT                  style,
            IN REAL                 emSize,  // World units
            IN const RectF         &layoutRect,
            IN const StringFormat  *format
        );

        Status AddString(
            IN const WCHAR         *string,
            IN INT                  length,
            IN const FontFamily    *family,
            IN INT                  style,
            IN REAL                 emSize,  // World units
            IN const Point         &origin,
            IN const StringFormat  *format
        );

        Status AddString(
            IN const WCHAR         *string,
            IN INT                  length,
            IN const FontFamily    *family,
            IN INT                  style,
            IN REAL                 emSize,  // World units
            IN const Rect          &layoutRect,
            IN const StringFormat  *format
        );

        Status Transform(IN const Matrix* matrix);

        // This is not always the tightest bounds.

        Status GetBounds(OUT RectF* bounds,
                         IN const Matrix* matrix = NULL,
                         IN const Pen* pen = NULL) const;

        Status GetBounds(OUT Rect* bounds,
                         IN const Matrix* matrix = NULL,
                         IN const Pen* pen = NULL) const;

        // Once flattened, the resultant path is made of line segments and
        // the original path information is lost.  When matrix is NULL the
        // identity matrix is assumed.

        Status Flatten(IN const Matrix* matrix = NULL,
                       IN REAL flatness = FlatnessDefault);

        Status Widen(
            IN const Pen* pen,
            IN const Matrix* matrix = NULL,
            IN REAL flatness = FlatnessDefault
        );

        Status Outline(
            IN const Matrix *matrix = NULL,
            IN REAL flatness = FlatnessDefault
        );

        // Once this is called, the resultant path is made of line segments and
        // the original path information is lost.  When matrix is NULL, the
        // identity matrix is assumed.

        Status Warp(IN const PointF* destPoints,
                    IN INT count,
                    IN const RectF& srcRect,
                    IN const Matrix* matrix = NULL,
                    IN WarpMode warpMode = WarpModePerspective,
                    IN REAL flatness = FlatnessDefault);

        INT GetPointCount() const;

        Status GetPathTypes(OUT BYTE* types,
                            IN INT count) const;

        Status GetPathPoints(OUT PointF* points,
                             IN INT count) const;

        Status GetPathPoints(OUT Point* points,
                             IN INT count) const;

        Status GetLastStatus() const;

        BOOL IsVisible(IN const PointF& point,
                       IN const Graphics* g = NULL) const;

        BOOL IsVisible(IN REAL x,
                       IN REAL y,
                       IN const Graphics* g = NULL) const;

        BOOL IsVisible(IN const Point& point,
                       IN const Graphics* g = NULL) const;

        BOOL IsVisible(IN INT x,
                       IN INT y,
                       IN const Graphics* g = NULL) const;

        BOOL IsOutlineVisible(IN const PointF& point,
                              IN const Pen* pen,
                              IN const Graphics* g = NULL) const;

        BOOL IsOutlineVisible(IN REAL x,
                              IN REAL y,
                              IN const Pen* pen,
                              IN const Graphics* g = NULL) const;

        BOOL IsOutlineVisible(IN const Point& point,
                              IN const Pen* pen,
                              IN const Graphics* g = NULL) const;

        BOOL IsOutlineVisible(IN INT x,
                              IN INT y,
                              IN const Pen* pen,
                              IN const Graphics* g = NULL) const;

    protected:

        GraphicsPath(const GraphicsPath& path);
    private:
        GraphicsPath& operator=(const GraphicsPath &);

    protected:
        GraphicsPath(GpPath* nativePath);

        VOID SetNativePath(GpPath *nativePath);

        Status SetStatus(Status status) const;

        \**** NOT SUPPORTED YET***/

    protected:
        Graphics* m_context;
        FillMode m_fillmode;
        int m_pathIndex;
        bool m_empty;
    };
    /**** NOT SUPPORTED YET***\
    class GraphicsPathIterator: public GdiplusBase
    {
    public:
        GraphicsPathIterator(GraphicsPath *path):
            nativePathIterator(NULL), lastStatus(Ok);

        INT CopyData(PointF *points, BYTE *types, INT startIndex, INT endIndex);
        INT Enumerate(PointF *points, BYTE *types, INT count);
        INT GetCount() const;
        Status GetLastStatus() const;
        INT GetSubpathCount() const;
        BOOL HasCurve() const;
        INT NextMarker(INT *startIndex, INT *endIndex);
        INT NextMarker(GraphicsPath *path);
        INT NextPathType(BYTE *pathType, INT *startIndex, INT *endIndex);
        INT NextSubpath(INT *startIndex, INT *endIndex, BOOL *isClosed);
        INT NextSubpath(GraphicsPath *path, BOOL *isClosed);
        VOID Rewind();

    private:
        GraphicsPathIterator(GpPathIterator *pathIterator, Status status):
            nativePathIterator(pathIterator), lastStatus(status) {}
        GraphicsPathIterator(const GraphicsPathIterator&);
        GraphicsPathIterator& operator=(const GraphicsPathIterator&);

        Status updateStatus(Status newStatus) const;

        GpPathIterator *nativePathIterator;
        mutable Status lastStatus;
    };

    class PathGradientBrush: public Brush
    {
    public:
        PathGradientBrush(const PointF *points, INT count,
                WrapMode wrapMode = WrapModeClamp);
        PathGradientBrush(const Point *points, INT count,
                WrapMode wrapMode = WrapModeClamp);
        PathGradientBrush(const GraphicsPath *path);
        virtual PathGradientBrush *Clone() const;

        Status GetBlend(REAL *blendFactors, REAL *blendPositions,
                INT count) const;
        INT GetBlendCount() const;
        Status GetCenterColor(Color *color) const;
        Status GetCenterPoint(PointF *point) const;
        Status GetCenterPoint(Point *point) const;
        Status GetFocusScales(REAL *xScale, REAL *yScale) const;
        BOOL GetGammaCorrection() const;
        //Status GetGraphicsPath(GraphicsPath *path) const;
        INT GetInterpolationColorCount() const;
        Status GetInterpolationColors(Color *presetColors,
                REAL *blendPositions, INT count) const;
        INT GetPointCount() const;
        Status GetRectangle(RectF *rect) const;
        Status GetRectangle(Rect *rect) const;
        INT GetSurroundColorCount() const;
        Status GetSurroundColors(Color *colors, INT *count);
        Status GetTransform(Matrix *matrix) const;
        WrapMode GetWrapMode() const;
        Status MultiplyTransform(const Matrix *matrix,
                MatrixOrder order = MatrixOrderPrepend);
        Status ResetTransform();
        Status RotateTransform(REAL angle,
                MatrixOrder order = MatrixOrderPrepend);
        Status ScaleTransform(REAL sx, REAL sy,
                MatrixOrder order = MatrixOrderPrepend);
        Status SetBlend(REAL *blendFactors, REAL *blendPositions, INT count);
        Status SetBlendBellShape(REAL focus, REAL scale = 1.0f);
        Status SetBlendTriangularShape(REAL focus, REAL scale = 1.0f);
        Status SetCenterColor(const Color& color);
        Status SetCenterPoint(const PointF& point);
        Status SetCenterPoint(const Point& point);
        Status SetFocusScales(REAL xScale, REAL yScale);
        Status SetGammaCorrection(BOOL useGammaCorrection);
        //Status SetGraphicsPath(const GraphicsPath *path);
        Status SetInterpolationColors(const Color *presetColors,
                REAL *blendPositions, INT count);
        Status SetSurroundColors(const Color *colors, INT *count);
        Status SetTransform(const Matrix *matrix);
        Status SetWrapMode(WrapMode wrapMode);
        Status TranslateTransform(REAL dx, REAL dy,
                MatrixOrder order = MatrixOrderPrepend);

    private:
        PathGradientBrush(GpBrush *brush, Status status): Brush(brush, status);
        PathGradientBrush(const PathGradientBrush&);
        PathGradientBrush& operator=(const PathGradientBrush&);
    };
     \**** NOT SUPPORTED YET***/
}

#endif /* __GDIPLUS_PATH_H */
