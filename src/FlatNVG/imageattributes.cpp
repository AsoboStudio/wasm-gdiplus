/*
 * Copyright (c) 2004-2005 Ximian
 * Copyright (C) 2007 Novell, Inc (http://www.novell.com)
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
 *	Jordi Mas i Hernandez <jordi@ximian.com>, 2004-2005
 *	Sebastien Pouliot  <sebastien@ximian.com>
 *	Frederik Carlier  <frederik.carlier@quamotion.mobi>
 *	Sol Roo (sroo@asobostudio.com)
 *
 */
#include "gdiplusflat.h"

namespace DLLExports {

	/* coverity[+alloc : arg-*0] */
	GpStatus WINGDIPAPI
		GdipCreateImageAttributes(GpImageAttributes** imageattr) {
		return GpStatus::NotImplemented;
	}

	/* coverity[+alloc : arg-*1] */
	GpStatus WINGDIPAPI
		GdipCloneImageAttributes(GDIPCONST GpImageAttributes* imageattr, GpImageAttributes** cloneImageattr) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDisposeImageAttributes(GpImageAttributes* imageattr) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesToIdentity(GpImageAttributes* imageattr, GpColorAdjustType type) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipResetImageAttributes(GpImageAttributes* imageattr, GpColorAdjustType type) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesThreshold(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag, REAL threshold) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipSetImageAttributesGamma(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag, REAL gamma) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesNoOp(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipSetImageAttributesColorKeys(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag, ARGB colorLow, ARGB colorHigh) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesOutputChannelColorProfile(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag, GDIPCONST WCHAR* colorProfileFilename) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipSetImageAttributesRemapTable(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag, UINT mapSize, GDIPCONST ColorMap* map) {
		return GpStatus::NotImplemented;
	}

	/*
		According to Microsoft documentation:
		clamp: This parameter has no effect in Microsoft® Windows® GDI+ version 1.0
	*/
	GpStatus WINGDIPAPI
		GdipSetImageAttributesWrapMode(GpImageAttributes* imageattr, GpWrapMode wrap, ARGB argb, BOOL clamp) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesICMMode(GpImageAttributes* imageAttr, BOOL on) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageAttributesAdjustedPalette(GpImageAttributes* imageattr, GpColorPalette* colorPalette, GpColorAdjustType type) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesColorMatrix(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag,
			GDIPCONST ColorMatrix* colorMatrix, GDIPCONST ColorMatrix* grayMatrix, GpColorMatrixFlags flags) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesOutputChannel(GpImageAttributes* imageattr, GpColorAdjustType type, BOOL enableFlag, GpColorChannelFlags channelFlags) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImageAttributesCachedBackground(GpImageAttributes* imageattr, BOOL enableFlag) {
		return GpStatus::NotImplemented;
	}

}