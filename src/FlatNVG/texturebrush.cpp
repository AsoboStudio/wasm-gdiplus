/*
 * texturebrush.cpp
 *
 * Copyright (C) 2003,2006-2007 Novell, Inc. http://www.novell.com
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
 *   Ravindra (rkumar@novell.com)
 *   Sebastien Pouliot  <sebastien@ximian.com>
 *	 Sol Roo (sroo@asobostudio.com)
 */

#include "gdiplusflat.h"

namespace DLLExports {

	GpStatus WINGDIPAPI
		GdipCreateTexture(GpImage* image, GpWrapMode wrapmode, GpTexture** texture) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateTexture2(GpImage* image, GpWrapMode wrapmode, REAL x, REAL y, REAL width, REAL height, GpTexture** texture) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateTexture2I(GpImage* image, GpWrapMode wrapmode, INT x, INT y, INT width, INT height, GpTexture** texture) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateTextureIA(GpImage* image, GpImageAttributes* imageAttributes, REAL x, REAL y, REAL width, REAL height, GpTexture** texture) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateTextureIAI(GpImage* image, GpImageAttributes* imageAttributes, INT x, INT y, INT width, INT height, GpTexture** texture) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetTextureTransform(GpTexture* texture, GpMatrix* matrix) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetTextureTransform(GpTexture* texture, GDIPCONST GpMatrix* matrix) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipResetTextureTransform(GpTexture* texture) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipMultiplyTextureTransform(GpTexture* texture, GpMatrix* matrix, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipTranslateTextureTransform(GpTexture* texture, REAL dx, REAL dy, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipScaleTextureTransform(GpTexture* texture, REAL sx, REAL sy, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipRotateTextureTransform(GpTexture* texture, REAL angle, GpMatrixOrder order) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetTextureWrapMode(GpTexture* texture, GpWrapMode wrapMode) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetTextureWrapMode(GpTexture* texture, GpWrapMode* wrapMode) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetTextureImage(GpTexture* texture, GpImage** image) {
		return GpStatus::NotImplemented;
	}

}