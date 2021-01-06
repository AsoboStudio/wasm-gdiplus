/*
 * pathgradientbrush.cpp
 *
 * Copyright (C) 2003-2004,2007 Novell, Inc. http://www.novell.com
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
 *	Vladimir Vukicevic (vladimir@pobox.com)
 *	Ravindra (rkumar@novell.com)
 *	Sol Roo (sroo@asobostudio.com)
 */

#include "gdiplusflat.h"

namespace DLLExports {

	GpStatus WINGDIPAPI
		GdipCreatePathGradient(GDIPCONST GpPointF* points, INT count, GpWrapMode wrapMode, GpPathGradient** polyGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreatePathGradientI(GDIPCONST GpPoint* points, INT count, GpWrapMode wrapMode, GpPathGradient** polyGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreatePathGradientFromPath(GDIPCONST GpPath* path, GpPathGradient** polyGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientCenterColor(GpPathGradient* brush, ARGB* colors) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientCenterColor(GpPathGradient* brush, ARGB colors) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientSurroundColorsWithCount(GpPathGradient* brush, ARGB* colors, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientSurroundColorsWithCount(GpPathGradient* brush, GDIPCONST ARGB* colors, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientPath(GpPathGradient* brush, GpPath* path) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientPath(GpPathGradient* brush, GDIPCONST GpPath* path) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientCenterPoint(GpPathGradient* brush, GpPointF* point) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientCenterPointI(GpPathGradient* brush, GpPoint* point) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientCenterPoint(GpPathGradient* brush, GDIPCONST GpPointF* point) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientCenterPointI(GpPathGradient* brush, GDIPCONST GpPoint* point) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientRect(GpPathGradient* brush, GpRectF* rect) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientRectI(GpPathGradient* brush, GpRect* rect) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientPointCount(GpPathGradient* brush, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientGammaCorrection(GpPathGradient* brush, BOOL useGammaCorrection) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientGammaCorrection(GpPathGradient* brush, BOOL* useGammaCorrection) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientSurroundColorCount(GpPathGradient* brush, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientBlendCount(GpPathGradient* brush, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientBlend(GpPathGradient* brush, REAL* blend, REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientBlend(GpPathGradient* brush, GDIPCONST REAL* blend, GDIPCONST REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientPresetBlendCount(GpPathGradient* brush, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientPresetBlend(GpPathGradient* brush, ARGB* blend, REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientPresetBlend(GpPathGradient* brush, GDIPCONST ARGB* blend, GDIPCONST REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientSigmaBlend(GpPathGradient* brush, REAL focus, REAL scale) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientLinearBlend(GpPathGradient* brush, REAL focus, REAL scale) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientWrapMode(GpPathGradient* brush, GpWrapMode* wrapMode) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientWrapMode(GpPathGradient* brush, GpWrapMode wrapMode) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientTransform(GpPathGradient* brush, GpMatrix* matrix) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientTransform(GpPathGradient* brush, GpMatrix* matrix) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipResetPathGradientTransform(GpPathGradient* brush) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipMultiplyPathGradientTransform(GpPathGradient* brush, GDIPCONST GpMatrix* matrix, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipTranslatePathGradientTransform(GpPathGradient* brush, REAL dx, REAL dy, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipScalePathGradientTransform(GpPathGradient* brush, REAL sx, REAL sy, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipRotatePathGradientTransform(GpPathGradient* brush, REAL angle, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPathGradientFocusScales(GpPathGradient* brush, REAL* xScale, REAL* yScale) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPathGradientFocusScales(GpPathGradient* brush, REAL xScale, REAL yScale) {
		return GpStatus::NotImplemented;
	}

}