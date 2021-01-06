/*
 * Copyright (c) 2004 Ximian
 * Copyright (c) 2004-2007 Novell, Inc.
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
 *	Jordi Mas i Hernandez <jordi@ximian.com>, 2004-2006
 *	Peter Dennis Bartok <pbartok@novell.com>
 *	Sebastien Pouliot  <sebastien@ximian.com>
 *	Jeffrey Stedfast <fejj@novell.com>
 *	Sol Roo (sroo@asobostudio.com)
 *
 */
#include "FlatNVG\GdiPlusFlatImpl.h"
#include "FlatNVG\font-private.h"
#include "FlatNVG\graphics-private.h"

inline bool gdip_load_font(GpGraphics* graphics, GpFont* font) {

#ifdef GDIFLAT_SAFE
	if (!graphics || !font) {
		return false;
	}
#endif
	if ((!font->loaded || font->ctx != graphics->ctx) && font->name != NULL) {
		// before loading, attempt to reuse a pre-loaded font
		font->fontid = nvgFindFont(graphics->ctx, font->name->c_str());
		// if not previously done, load the font file
		if (font->fontid == -1 && font->path != NULL) {
			font->fontid = nvgCreateFont(graphics->ctx, font->name->c_str(), font->path->c_str());
		}

		// save context and load attempt to avoid reloading
		font->loaded = true;
		font->ctx = graphics->ctx;
	}

	return (font->fontid != -1);
}

namespace DLLExports {

	GpStatus WINGDIPAPI GdipLoadFont(GpGraphics* graphics, GpFont* font) {
#ifdef GDIFLAT_SAFE
		if (!graphics || !font) {
			return GpStatus::InvalidParameter;
		}
#endif
		if (gdip_load_font(graphics, font)) {
			return GpStatus::Ok;
		}
		else {
			return GpStatus::GenericError;
		}

	}
	GpStatus WINGDIPAPI
		GdipNewInstalledFontCollection(GpFontCollection** fontCollection) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipNewPrivateFontCollection(GpFontCollection** fontCollection) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipDeletePrivateFontCollection(GpFontCollection** fontCollection) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipPrivateAddFontFile(GpFontCollection* fontCollection, GDIPCONST WCHAR* filename) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCloneFontFamily(GpFontFamily* fontFamily, GpFontFamily** clonedFontFamily) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDeleteFontFamily(GpFontFamily* fontFamily) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetFontCollectionFamilyCount(GpFontCollection* fontCollection, INT* numFound) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetFontCollectionFamilyList(GpFontCollection* fontCollection, INT numSought, GpFontFamily* gpfamilies[], INT* numFound) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipCreateFontFamilyFromName(GDIPCONST WCHAR* name, GpFontCollection* font_collection, GpFontFamily** fontFamily) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetFamilyName(GDIPCONST GpFontFamily* family, WCHAR name[LF_FACESIZE], LANGID language) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetGenericFontFamilySansSerif(GpFontFamily** nativeFamily) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetGenericFontFamilySerif(GpFontFamily** nativeFamily) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetGenericFontFamilyMonospace(GpFontFamily** nativeFamily) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetEmHeight(GDIPCONST GpFontFamily* family, INT style, UINT16* EmHeight) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetCellAscent(GDIPCONST GpFontFamily* family, INT style, UINT16* CellAscent) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetCellDescent(GDIPCONST GpFontFamily* family, INT style, UINT16* CellDescent) {
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI
		GdipGetLineSpacing(GDIPCONST GpFontFamily* family, INT style, UINT16* LineSpacing) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipIsStyleAvailable(GDIPCONST GpFontFamily* family, INT style, BOOL* IsStyleAvailable) {
		return GpStatus::NotImplemented;
	}

	/* Font functions */

	GpStatus
		GdipCreateFont(GDIPCONST GpFontFamily* family, REAL emSize, INT style, GpUnit unit, GpFont** font) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCloneFont(GpFont* font, GpFont** cloneFont) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDeleteFont(GpFont* font) {
		if (font) {
			if (font->path) delete(font->path);
			if (font->name) delete(font->name);
			GdipFree(font);
		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipCreateFontFromDC(HDC hdc, GpFont** font) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateFontFromHfontA(HFONT hfont, GpFont** font, void* lf) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLogFontW(GpFont* font, GpGraphics* graphics, LOGFONTW* logfontW) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetLogFontA(GpFont* font, GpGraphics* graphics, LOGFONTA* logfontA) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipCreateFontFromLogfontA(HDC hdc, GDIPCONST LOGFONTA* logfont, GpFont** font) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipCreateFontFromLogfontW(HDC hdc, GDIPCONST LOGFONTW* logfont, GpFont** font) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipPrivateAddMemoryFont(GpFontCollection* fontCollection, GDIPCONST void* memory, INT length) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetFontHeight(GDIPCONST GpFont* font, GDIPCONST GpGraphics* graphics, REAL* height) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetFontHeightGivenDPI(GDIPCONST GpFont* font, REAL dpi, REAL* height) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetFontSize(GpFont* font, REAL* size)
	{
		if (!font || !size)
			return GpStatus::InvalidParameter;

		//*size = font->emSize;
		//return GpStatus::Ok;
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetFontStyle(GpFont* font, INT* style)
	{
		if (!font || !style)
			return GpStatus::InvalidParameter;

		*style = font->style;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetFontUnit(GpFont* font, GpUnit* unit)
	{
		if (!font || !unit)
			return GpStatus::InvalidParameter;

		*unit = font->unit;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetFamily(GpFont* font, GpFontFamily** family)
	{
		if (!font || !family)
			return GpStatus::InvalidParameter;

		return GdipCloneFontFamily(font->family, family);
	}
}