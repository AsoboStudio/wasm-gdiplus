/*
 * lineargradientbrush.cpp
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
 * AuthorS:
 *      Ravindra (rkumar@novell.com)
 *		Sol Roo (sroo@asobostudio.com)
 */
#include "gdiplusflat.h"

namespace DLLExports {


	GpStatus WINGDIPAPI
		GdipCreateLineBrushI(GDIPCONST GpPoint* point1, GDIPCONST GpPoint* point2, ARGB color1, ARGB color2, GpWrapMode wrapMode, GpLineGradient** lineGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateLineBrush(GDIPCONST GpPointF* point1, GDIPCONST GpPointF* point2, ARGB color1, ARGB color2, GpWrapMode wrapMode, GpLineGradient** lineGradient) {
		return GpStatus::NotImplemented;
	}

	static float
		get_angle_from_linear_gradient_mode(GpLinearGradientMode mode) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateLineBrushFromRectI(GDIPCONST GpRect* rect, ARGB color1, ARGB color2, GpLinearGradientMode mode, GpWrapMode wrapMode, GpLineGradient** lineGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateLineBrushFromRect(GDIPCONST GpRectF* rect, ARGB color1, ARGB color2, GpLinearGradientMode mode, GpWrapMode wrapMode, GpLineGradient** lineGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateLineBrushFromRectWithAngleI(GDIPCONST GpRect* rect, ARGB color1, ARGB color2, REAL angle, BOOL isAngleScalable, GpWrapMode wrapMode, GpLineGradient** lineGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipCreateLineBrushFromRectWithAngle(GDIPCONST GpRectF* rect, ARGB color1, ARGB color2, REAL angle, BOOL isAngleScalable, GpWrapMode wrapMode, GpLineGradient** lineGradient) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineBlendCount(GpLineGradient* brush, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLineBlend(GpLineGradient* brush, GDIPCONST REAL* blend, GDIPCONST REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineBlend(GpLineGradient* brush, REAL* blend, REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLineGammaCorrection(GpLineGradient* brush, BOOL useGammaCorrection) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineGammaCorrection(GpLineGradient* brush, BOOL* useGammaCorrection) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLinePresetBlendCount(GpLineGradient* brush, INT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLinePresetBlend(GpLineGradient* brush, GDIPCONST ARGB* blend, GDIPCONST REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLinePresetBlend(GpLineGradient* brush, ARGB* blend, REAL* positions, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLineColors(GpLineGradient* brush, ARGB color1, ARGB color2) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineColors(GpLineGradient* brush, ARGB* colors) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineRectI(GpLineGradient* brush, GpRect* rect) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineRect(GpLineGradient* brush, GpRectF* rect) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineTransform(GpLineGradient* brush, GpMatrix* matrix) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLineTransform(GpLineGradient* brush, GDIPCONST GpMatrix* matrix) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLineWrapMode(GpLineGradient* brush, GpWrapMode* wrapMode) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLineWrapMode(GpLineGradient* brush, GpWrapMode wrapMode) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLineLinearBlend(GpLineGradient* brush, REAL focus, REAL scale) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetLineSigmaBlend(GpLineGradient* brush, REAL focus, REAL scale) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipMultiplyLineTransform(GpLineGradient* brush, GpMatrix* matrix, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipResetLineTransform(GpLineGradient* brush) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipRotateLineTransform(GpLineGradient* brush, REAL angle, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipScaleLineTransform(GpLineGradient* brush, REAL sx, REAL sy, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipTranslateLineTransform(GpLineGradient* brush, REAL dx, REAL dy, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

}