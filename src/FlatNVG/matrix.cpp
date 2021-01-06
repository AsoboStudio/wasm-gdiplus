/**
 * matrix.cpp
 *
 * Copyright (C) Novell, Inc. 2003-2004, 2007.
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
 *	Duncan Mak (duncan@ximian.com)
 *	Sol Roo (sroo@asobostudio.com)
 *
 * Copyright (C) 2003-2007 Novell, Inc (http://www.novell.com)
 */

#include "gdiplusflat.h"

#include "FlatNVG/general-private.h"
#include "FlatNVG/matrix-private.h"
#include <string>
#include <float.h>

 /*
	 GDI+ matrix takes 6 elements arranged in 3 rows by 2 columns. The identity matrix is

	 [1, 0]					[1, 0, 0]
	 [0, 1] that is a simplification of 	[0, 1, 0]
	 [0, 0] 					[0, 0, 1]

	 Point v1, v2	Matrix: [m11, m12]
				 [m21, m22]
				 [m31, m32]

	 Calcutation of X, Y using the previous matrix

	 X = v1 * m11 + v2 * m12 + m31
	 Y = v1 * m21 + v2 * m22 + m32

	 M31 and M32 are used to do matrix translations

 */


 /* private functions */


 /*
  * In System.Drawing it is often impossible to specify a 'null' matrix.
  * Instead we supply an empty matrix (i.e. new Matrix ()). However this
  * "empty" matrix can cause a lot of extra calculation in libgdiplus
  * (e.g. invalidating the bitmap) unless we consider it as a special case.
  */
  BOOL
  gdip_is_matrix_empty(const GpMatrix* matrix)
  {
	  if (!matrix)
		  return TRUE;

	  // compare the matrix elements with the empty (no-op) version
	  return (gdip_near_one(matrix->m[0]) && gdip_near_zero(matrix->m[1]) &&
		  gdip_near_zero(matrix->m[2]) && gdip_near_one(matrix->m[3]) &&
		  gdip_near_zero(matrix->m[4]) && gdip_near_zero(matrix->m[5]));
  }
  

 /* GDI+ maps values between [ FLT_MAX, Infinity [ to FLT_MAX, instead of Infinity */
REAL
gdip_double_to_float(double value)
{
	if (value != INFINITY && value > FLT_MAX)
		return (REAL)FLT_MAX;

	if (value != -INFINITY && value < -FLT_MAX)
		return (REAL)-FLT_MAX;

	return (REAL)value;
}

BOOL
gdip_is_matrix_a_translation(const GpMatrix* matrix)
{
	if (!matrix)
		return TRUE;

	return ((matrix->m[0] == 1.0f) && (matrix->m[1] == 0.0f) &&
		(matrix->m[2] == 0.0f) && (matrix->m[3] == 1.0f));
}

/* this helper function avoid GpMatrix allocation and reduce code duplication */

GpStatus
gdip_matrix_init_from_rect_3points(GpMatrix* matrix, const GpRectF* rect, const GpPointF* dstplg)
{
	GpPointF* p, * p0, * p1, * p2;
	double m11, m12, m21, m22;
#ifdef GDIFLAT_SAFE
	if (!matrix || !rect || !dstplg)
		return GpStatus::InvalidParameter;
#endif
	if ((rect->Width == 0) || (rect->Height == 0))
		return GpStatus::OutOfMemory;

	p = (GpPointF*)dstplg;
	p0 = p++;
	p1 = p++;
	p2 = p;

	m11 = (p1->X - p0->X) / rect->Width;
	m12 = (p1->Y - p0->Y) / rect->Width;
	m21 = (p2->X - p0->X) / rect->Height;
	m22 = (p2->Y - p0->Y) / rect->Height;

	DLLExports::GdipSetMatrixElements(matrix, m11, m12, m21, m22, p0->X - rect->X, p0->Y - rect->Y);
	return GpStatus::Ok;
}


namespace DLLExports {

	/* public (exported) functions */

	// coverity[+alloc : arg-*0]
	GpStatus WINGDIPAPI
		GdipCreateMatrix(GpMatrix** matrix) {
#ifdef GDIFLAT_SAFE
		if (!matrix) {
			return GpStatus::InvalidParameter;
		}
#endif
		GpMatrix* result = (GpMatrix*)GdipAlloc(sizeof(GpMatrix));

		nvgTransformIdentity(result->m);

		*matrix = result;
		return GpStatus::Ok;
	}

	// coverity[+alloc : arg-*6]
	GpStatus WINGDIPAPI
		GdipCreateMatrix2(REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy, GpMatrix** matrix) {
#ifdef GDIFLAT_SAFE
		if (!matrix) {
			return GpStatus::InvalidParameter;
		}
#endif
		GpMatrix* result = (GpMatrix*)GdipAlloc(sizeof(GpMatrix));

		result->m[0] = m11;
		result->m[1] = m12;
		result->m[2] = m21;
		result->m[3] = m22;
		result->m[4] = dx;
		result->m[5] = dy;

		*matrix = result;
		return GpStatus::Ok;
	}

	// coverity[+alloc : arg-*2]
	GpStatus WINGDIPAPI
		GdipCreateMatrix3(GDIPCONST GpRectF* rect, GDIPCONST GpPointF* dstplg, GpMatrix** matrix) {
#ifdef GDIFLAT_SAFE
		if (!matrix) {
			return GpStatus::InvalidParameter;
		}
#endif
		GpMatrix* result = (GpMatrix*)GdipAlloc(sizeof(GpMatrix));

		auto status = gdip_matrix_init_from_rect_3points(result, rect, dstplg);

		*matrix = result;
		return GpStatus::Ok;

	}

	// coverity[+alloc : arg-*2]
	GpStatus WINGDIPAPI
		GdipCreateMatrix3I(GDIPCONST GpRect* rect, GDIPCONST GpPoint* dstplg, GpMatrix** matrix) {
		GpRectF rectf;
		GpPointF pts[3];

		//if (!gdiplusInitialized)
		//	return GdiplusNotInitialized;

		if (!rect || !dstplg || !matrix)
			return GpStatus::InvalidParameter;

		gdip_RectF_from_Rect(rect, &rectf);

		pts[0].X = dstplg[0].X;
		pts[0].Y = dstplg[0].Y;
		pts[1].X = dstplg[1].X;
		pts[1].Y = dstplg[1].Y;
		pts[2].X = dstplg[2].X;
		pts[2].Y = dstplg[2].Y;

		return GdipCreateMatrix3(&rectf, (GpPointF*)&pts, matrix);

	}

	// coverity[+alloc : arg-*1]
	GpStatus WINGDIPAPI
		GdipCloneMatrix(GpMatrix* matrix, GpMatrix** cloneMatrix) {
#ifdef GDIFLAT_SAFE
		if (!matrix) {
			return GpStatus::InvalidParameter;
		}
#endif
		return GdipCreateMatrix2(matrix->m[0], matrix->m[1], matrix->m[2], matrix->m[3], matrix->m[4], matrix->m[5], cloneMatrix);
	}

	GpStatus WINGDIPAPI
		GdipDeleteMatrix(GpMatrix* matrix) {
#ifdef GDIFLAT_SAFE
		if (!matrix) {
			return GpStatus::InvalidParameter;
		}
#endif
		GdipFree(matrix);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetMatrixElements(GpMatrix* matrix, REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy) {

		matrix->m[0] = m11;
		matrix->m[1] = m12;
		matrix->m[2] = m21;
		matrix->m[3] = m22;
		matrix->m[4] = dx;
		matrix->m[5] = dy;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetMatrixElements(GDIPCONST GpMatrix* matrix, REAL* matrixOut) {
		matrixOut[0] = matrix->m[0];
		matrixOut[1] = matrix->m[1];
		matrixOut[2] = matrix->m[2];
		matrixOut[3] = matrix->m[3];
		matrixOut[4] = matrix->m[4];
		matrixOut[5] = matrix->m[5];
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipMultiplyMatrix(GpMatrix* matrix, GpMatrix* matrix2, GpMatrixOrder order) {
#ifdef GDIFLAT_SAFE
		if (!matrix || !matrix2)
			return GpStatus::InvalidParameter;
#endif
		if (matrix == matrix2)
			return GpStatus::ObjectBusy;

		switch (order) {
		case GpMatrixOrder::MatrixOrderPrepend:
			nvgTransformPremultiply(matrix->m, matrix2->m);
			break;
		case GpMatrixOrder::MatrixOrderAppend:
			nvgTransformMultiply(matrix->m, matrix2->m);
			break;
		default:
			return GpStatus::InvalidParameter;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipTranslateMatrix(GpMatrix* matrix, REAL offsetX, REAL offsetY, GpMatrixOrder order) {

		float o_m[6];
		memcpy(o_m, matrix->m, sizeof(float) * 6);
		if (order == GpMatrixOrder::MatrixOrderPrepend)
		{
			//matrix->m[0] = o_m[0];
			//matrix->m[1] = o_m[1];
			//matrix->m[2] = o_m[2];
			//matrix->m[3] = o_m[3];
			matrix->m[4] = offsetX * o_m[0] + offsetY * o_m[2] + o_m[4];
			matrix->m[5] = offsetX * o_m[1] + offsetY * o_m[3] + o_m[5];
		}
		else if (order == GpMatrixOrder::MatrixOrderAppend)
		{
			//matrix->m[0] = o_m[0];
			//matrix->m[1] = o_m[1];
			//matrix->m[2] = o_m[2];
			//matrix->m[3] = o_m[3];
			matrix->m[4] = o_m[4] + offsetX;
			matrix->m[5] = o_m[5] + offsetY;
		}
		else {
			return GpStatus::InvalidParameter;
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipScaleMatrix(GpMatrix* matrix, REAL scaleX, REAL scaleY, GpMatrixOrder order) {

		float o_m[6];
		memcpy(o_m, matrix->m, sizeof(float) * 6);
		if (order == GpMatrixOrder::MatrixOrderPrepend)
		{
			matrix->m[0] = scaleX * o_m[0];
			matrix->m[1] = scaleX * o_m[1];
			matrix->m[2] = scaleY * o_m[2];
			matrix->m[3] = scaleY * o_m[3];
			//matrix->m[4] = o_m[4];
			//matrix->m[5] = o_m[5];
		}
		else if (order == GpMatrixOrder::MatrixOrderAppend)
		{
			matrix->m[0] = o_m[0] * scaleX;
			matrix->m[1] = o_m[1] * scaleY;
			matrix->m[2] = o_m[2] * scaleX;
			matrix->m[3] = o_m[3] * scaleY;
			matrix->m[4] = o_m[4] * scaleX;
			matrix->m[5] = o_m[5] * scaleY;
		}
		else {
			return GpStatus::InvalidParameter;
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipRotateMatrix(GpMatrix* matrix, REAL angle, GpMatrixOrder order) {

		REAL angleRadian = angle * M_PI / 180.0f;
		
		GpMatrix rotation;
		nvgTransformRotate(rotation.m, angleRadian);

		if (order == GpMatrixOrder::MatrixOrderPrepend)
			nvgTransformPremultiply(matrix->m, rotation.m);
		else if (order == GpMatrixOrder::MatrixOrderAppend)
			nvgTransformMultiply(matrix->m, rotation.m);
		else
			return GpStatus::InvalidParameter;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipShearMatrix(GpMatrix* matrix, REAL shearX, REAL shearY, GpMatrixOrder order) {
		REAL shear[6];
#ifdef GDIFLAT_SAFE
		if (!matrix)
			return GpStatus::InvalidParameter;
#endif
		/* prepare transformation matrix */
		shear[0] = 1.0;
		shear[1] = shearY;
		shear[2] = shearX;
		shear[3] = 1.0;
		shear[4] = 0.0;
		shear[5] = 0.0;
		
		if (order == GpMatrixOrder::MatrixOrderPrepend)
			nvgTransformPremultiply(matrix->m, shear);
		else if (order == GpMatrixOrder::MatrixOrderAppend)
			nvgTransformMultiply(matrix->m, shear);
		else 
			return GpStatus::InvalidParameter;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipInvertMatrix(GpMatrix* matrix) {
		float tmp[6];
		BOOL success = nvgTransformInverse(tmp, matrix->m);
		if (success) {
			memcpy(matrix->m,tmp,sizeof(float)*6);
			return GpStatus::Ok;
		}
		else {
			return GpStatus::InvalidParameter;
		}
	}

	GpStatus WINGDIPAPI
		GdipTransformMatrixPoints(GpMatrix* matrix, GpPointF* pts, INT count) {
		for (int i = 0; i < count; i++)
		{
			GpPointF pt = pts[i];
			pts[i].X = pt.X * matrix->m[0] + pt.Y * matrix->m[2] + matrix->m[4];
			pts[i].Y = pt.X * matrix->m[1] + pt.Y * matrix->m[3] + matrix->m[5];
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipTransformMatrixPointsI(GpMatrix* matrix, GpPoint* pts, INT count) {
		for (int i = 0; i < count; i++)
		{
			GpPoint pt = pts[i];
			pts[i].X = pt.X * matrix->m[0] + pt.Y * matrix->m[2] + matrix->m[4];
			pts[i].Y = pt.X * matrix->m[1] + pt.Y * matrix->m[3] + matrix->m[5];
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipVectorTransformMatrixPoints(GpMatrix* matrix, GpPointF* pts, INT count) {
		for (int i = 0; i < count; i++)
		{
			GpPointF pt = pts[i];
			pts[i].X = pt.X * matrix->m[0] + pt.Y * matrix->m[2];
			pts[i].Y = pt.X * matrix->m[1] + pt.Y * matrix->m[3];
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipVectorTransformMatrixPointsI(GpMatrix* matrix, GpPoint* pts, INT count) {
		for (int i = 0; i < count; i++)
		{
			GpPoint pt = pts[i];
			pts[i].X = pt.X * matrix->m[0] + pt.Y * matrix->m[2];
			pts[i].Y = pt.X * matrix->m[1] + pt.Y * matrix->m[3];
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipIsMatrixInvertible(GDIPCONST GpMatrix* matrix, BOOL* result) {
		// from nvg nvgTransformInverse
		double det = (double)matrix->m[0] * matrix->m[3] - (double)matrix->m[2] * matrix->m[1];

		*result = det < -1e-6 || det > 1e-6;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipIsMatrixIdentity(GDIPCONST GpMatrix* matrix, BOOL* result) {
		if ((matrix->m[0] == 1) && (matrix->m[0] == 0) && (matrix->m[0] == 0) && (matrix->m[0] == 1) && (matrix->m[0] == 0) && (matrix->m[0] == 0))
			*result = TRUE;
		*result = FALSE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipIsMatrixEqual(GDIPCONST GpMatrix* matrix, GDIPCONST GpMatrix* matrix2, BOOL* result) {
		if (memcmp(matrix->m, matrix2->m, sizeof(float) * 6) == 0)
			*result = TRUE;
		*result = FALSE;
		return GpStatus::Ok;
	}

}