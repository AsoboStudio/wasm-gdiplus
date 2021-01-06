// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_MATRIX_H
#define __GDIPLUS_MATRIX_H

#include <MSFS\MSFS_WindowsTypes.h>
#include "GdiPlus.h"
#include "gdiplusflat.h"

namespace Gdiplus
{
#define GDIP_MATRIX_PI \
	3.1415926535897932384626433832795028841971693993751058209749445923078164

	class Matrix : public GdiplusBase
	{
		friend class Graphics;
		friend class GraphicsPath;
		friend class LinearGradientBrush;
		friend class PathGradientBrush;
		friend class Pen;
		friend class Region;
		friend class TextureBrush;

	public:
		Matrix() : nativeMatrix(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreateMatrix(&nativeMatrix);
		}
		Matrix(REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy) :
			nativeMatrix(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreateMatrix2(
				m11, m12, m21, m22, dx, dy,
				&nativeMatrix);
		}
		Matrix(const RectF& rect, const PointF* dstplg) :
			nativeMatrix(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreateMatrix3(
				&rect, dstplg, &nativeMatrix);
		}
		Matrix(const Rect& rect, const Point* dstplg) :
			nativeMatrix(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreateMatrix3I(
				&rect, dstplg, &nativeMatrix);
		}
		~Matrix()
		{
			DLLExports::GdipDeleteMatrix(nativeMatrix);
		}
		Matrix* Clone() const
		{
			GpMatrix* cloneMatrix = NULL;
			Status status = updateStatus(DLLExports::GdipCloneMatrix(
				nativeMatrix, &cloneMatrix));
			if (status == GpStatus::Ok) {
				Matrix* result = new Matrix(cloneMatrix, lastStatus);
				if (!result) {
					DLLExports::GdipDeleteMatrix(cloneMatrix);
					lastStatus = OutOfMemory;
				}
				return result;
			}
			else {
				return NULL;
			}
		}

		BOOL Equals(const Matrix* matrix) const
		{
			BOOL result;
			updateStatus(DLLExports::GdipIsMatrixEqual(
				nativeMatrix,
				matrix ? matrix->nativeMatrix : NULL, &result));
			return result;
		}
		Status GetElements(REAL* m) const
		{
			return updateStatus(DLLExports::GdipGetMatrixElements(
				nativeMatrix, m));
		}
		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}
		Status Invert()
		{
			return updateStatus(DLLExports::GdipInvertMatrix(nativeMatrix));
		}
		BOOL IsIdentity() const
		{
			BOOL result;
			updateStatus(DLLExports::GdipIsMatrixIdentity(
				nativeMatrix, &result));
			return result;
		}
		BOOL IsInvertible() const
		{
			BOOL result;
			updateStatus(DLLExports::GdipIsMatrixInvertible(
				nativeMatrix, &result));
			return result;
		}
		Status Multiply(const Matrix* matrix,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipMultiplyMatrix(
				nativeMatrix,
				matrix ? matrix->nativeMatrix : NULL, order));
		}
		REAL OffsetX() const
		{
			REAL m[6];
			updateStatus(DLLExports::GdipGetMatrixElements(nativeMatrix, m));
			return m[4];
		}
		REAL OffsetY() const
		{
			REAL m[6];
			updateStatus(DLLExports::GdipGetMatrixElements(nativeMatrix, m));
			return m[5];
		}
		Status Reset()
		{
			return updateStatus(DLLExports::GdipSetMatrixElements(
				nativeMatrix,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
		}
		Status Rotate(REAL angle, MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipRotateMatrix(
				nativeMatrix, angle, order));
		}
		Status RotateAt(REAL angle, const PointF& center,
			MatrixOrder order = MatrixOrderPrepend)
		{
			REAL angleRadian = angle * GDIP_MATRIX_PI / 180.0f;
			REAL cosAngle = cos(angleRadian);
			REAL sinAngle = sin(angleRadian);
			REAL x = center.X;
			REAL y = center.Y;

			Matrix matrix2(cosAngle, sinAngle, -sinAngle, cosAngle,
				x * (1.0f - cosAngle) + y * sinAngle,
				-x * sinAngle + y * (1.0f - cosAngle));
			Status status = matrix2.GetLastStatus();
			if (status == GpStatus::Ok) {
				return Multiply(&matrix2, order);
			}
			else {
				return lastStatus = status;
			}
		}
		Status Scale(REAL scaleX, REAL scaleY,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipScaleMatrix(
				nativeMatrix, scaleX, scaleY, order));
		}
		Status SetElements(REAL m11, REAL m12, REAL m21, REAL m22,
			REAL dx, REAL dy)
		{
			return updateStatus(DLLExports::GdipSetMatrixElements(
				nativeMatrix, m11, m12, m21, m22, dx, dy));
		}
		Status Shear(REAL shearX, REAL shearY,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipShearMatrix(
				nativeMatrix, shearX, shearY, order));
		}
		Status TransformPoints(PointF* pts, INT count = 1) const
		{
			return updateStatus(DLLExports::GdipTransformMatrixPoints(
				nativeMatrix, pts, count));
		}
		Status TransformPoints(Point* pts, INT count = 1) const
		{
			return updateStatus(DLLExports::GdipTransformMatrixPointsI(
				nativeMatrix, pts, count));
		}
		Status TransformVectors(PointF* pts, INT count = 1) const
		{
			return updateStatus(DLLExports::GdipVectorTransformMatrixPoints(
				nativeMatrix, pts, count));
		}
		Status TransformVectors(Point* pts, INT count = 1) const
		{
			return updateStatus(DLLExports::GdipVectorTransformMatrixPointsI(
				nativeMatrix, pts, count));
		}
		Status Translate(REAL offsetX, REAL offsetY,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipTranslateMatrix(
				nativeMatrix, offsetX, offsetY, order));
		}

	private:
		Matrix(GpMatrix* matrix, Status status) :
			nativeMatrix(matrix), lastStatus(status) {}
		Matrix(const Matrix&);
		Matrix& operator=(const Matrix&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != GpStatus::Ok) lastStatus = newStatus;
			return newStatus;
		}

		GpMatrix* nativeMatrix;
		mutable Status lastStatus;
	};

#undef GDIP_MATRIX_PI
}
#endif /* __GDIPLUS_MATRIX_H */
