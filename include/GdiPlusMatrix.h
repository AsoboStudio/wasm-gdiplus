// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_MATRIX_H
#define __GDIPLUS_MATRIX_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusmatrix.h."
#endif
namespace Gdiplus {

#define GDIP_MATRIX_PI \
	3.1415926535897932384626433832795028841971693993751058209749445923078164

    class Matrix : public GdiplusBase
    {
        friend class Graphics;
        friend class GraphicsPath;
        friend class TextureBrush;
        friend class LinearGradientBrush;
        friend class PathGradientBrush;
        friend class Pen;
        friend class Region;

        float m[6];


        /**** NOT SUPPORTED YET***\
        // Default constructor is set to identity matrix.
        Matrix();

        Matrix(IN REAL m11,
            IN REAL m12,
            IN REAL m21,
            IN REAL m22,
            IN REAL dx,
            IN REAL dy);

        Matrix(IN const RectF& rect,
            IN const PointF* dstplg);

        Matrix(IN const Rect& rect,
            IN const Point* dstplg);

        ~Matrix();

        Matrix* Clone() const;

        Status GetElements(OUT REAL* m) const;

        Status SetElements(IN REAL m11,
            IN REAL m12,
            IN REAL m21,
            IN REAL m22,
            IN REAL dx,
            IN REAL dy);

        REAL OffsetX() const;

        REAL OffsetY() const;

        Status Reset();

        Status Multiply(IN const Matrix* matrix,
            IN MatrixOrder order = MatrixOrderPrepend);

        Status Translate(IN REAL offsetX,
            IN REAL offsetY,
            IN MatrixOrder order = MatrixOrderPrepend);
        Status Scale(IN REAL scaleX,
            IN REAL scaleY,
            IN MatrixOrder order = MatrixOrderPrepend);

        Status Rotate(IN REAL angle,
            IN MatrixOrder order = MatrixOrderPrepend);

        Status RotateAt(IN REAL angle,
            IN const PointF& center,
            IN MatrixOrder order = MatrixOrderPrepend);

        Status Shear(IN REAL shearX,
            IN REAL shearY,
            IN MatrixOrder order = MatrixOrderPrepend);

        Status Invert();

        // float version
        Status TransformPoints(IN OUT PointF* pts,
            IN INT count = 1) const;

        Status TransformPoints(IN OUT Point* pts,
            IN INT count = 1) const;

        Status TransformVectors(IN OUT PointF* pts,
            IN INT count = 1) const;

        Status TransformVectors(IN OUT Point* pts,
            IN INT count = 1) const;

        BOOL IsInvertible() const;

        BOOL IsIdentity() const;

        BOOL Equals(IN const Matrix* matrix) const;

        Status GetLastStatus() const;

    private:
        Matrix(const Matrix&);
        Matrix& operator=(const Matrix&);

    protected:
        Matrix(GpMatrix* nativeMatrix);

        VOID SetNativeMatrix(GpMatrix* nativeMatrix);

        Status SetStatus(Status status) const;

    protected:
        mutable Status lastResult;
    \**** NOT SUPPORTED YET***/
    };
}
#undef GDIP_MATRIX_PI

#endif /* __GDIPLUS_MATRIX_H */
