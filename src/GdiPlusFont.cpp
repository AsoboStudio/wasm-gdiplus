// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers.

#include "GdiPlus.h"
#include <locale>
#include <codecvt>


#include "FlatNVG/fontcollection-private.h"
#include "FlatNVG/fontfamily-private.h"
#include "FlatNVG/font-private.h"


namespace Gdiplus {
	// Font

	Font::Font(const FontFamily* family,
		REAL emSize, INT style, Unit unit) :
		nativeFont(NULL), lastStatus(Ok)
	{
		lastStatus = DLLExports::GdipCreateFont(
			family ? family->nativeFontFamily : NULL,
			emSize, style, unit, &nativeFont);
	}


	Font::Font(HDC hdc, HFONT hfont) :
		nativeFont(NULL), lastStatus(Ok)
	{
		/*WASM: NOT SUPPORTED
		LOGFONTA WASM: NOT SUPPORTED;

		if (hfont && GetObjectA(hfont, sizeof logfont, &logfont)) {
			lastStatus = DLLExports::GdipCreateFontFromLogfontA(
					hdc, &logfont, &nativeFont);
		} else*/ {
			lastStatus = DLLExports::GdipCreateFontFromDC(
				hdc, &nativeFont);
		}
	}

	Font::Font(HDC hdc, const LOGFONTA* logfont) :
		nativeFont(NULL), lastStatus(Ok)
	{
		lastStatus = DLLExports::GdipCreateFontFromLogfontA(
			hdc, logfont, &nativeFont);
	}

	Font::Font(HDC hdc, const LOGFONTW* logfont) :
		nativeFont(NULL), lastStatus(Ok)
	{
		lastStatus = DLLExports::GdipCreateFontFromLogfontW(
			hdc, logfont, &nativeFont);
	}

	Font::Font(HDC hdc) :
		nativeFont(NULL), lastStatus(Ok)
	{
		lastStatus = DLLExports::GdipCreateFontFromDC(hdc, &nativeFont);
	}

#ifdef _MSFS_WASM_PROXY_GDIPLUS
	
	Font::Font(const char* familyName, const char* fontPath, REAL emSize, INT style, Unit unit, const FontCollection* fontCollection)
	{
		nativeFont = (GpFont*)DLLExports::GdipAlloc(sizeof(GpFont));
		nativeFont->fontid = -1;
		nativeFont->size = emSize;
		nativeFont->style = style;
		nativeFont->unit = unit;

		nativeFont->name = new std::string(familyName);
		nativeFont->path = new std::string(fontPath);
	}

	Font::Font(const WCHAR* familyName, const WCHAR* fontPath, REAL emSize, INT style, Unit unit, const FontCollection* fontCollection)
	{
		nativeFont = (GpFont*)DLLExports::GdipAlloc(sizeof(GpFont));
		nativeFont->fontid = -1;
		nativeFont->size = emSize;
		nativeFont->style = style;
		nativeFont->unit = unit;

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conversion;
		nativeFont->name = new std::string(conversion.to_bytes(familyName));
		nativeFont->path = new std::string(conversion.to_bytes(fontPath));
	}

	Font::Font(const WCHAR* familyName, REAL emSize, INT style, Unit unit, const FontCollection* fontCollection)
	{
		nativeFont = (GpFont*)DLLExports::GdipAlloc(sizeof(GpFont));
		nativeFont->fontid = -1;
		nativeFont->size = emSize;
		nativeFont->name = NULL;
		nativeFont->path = NULL;
	}
	BOOL Font::Load(Graphics* gfx)
	{
		lastStatus = DLLExports::GdipLoadFont(gfx?gfx->nativeGraphics:NULL, nativeFont);
		return lastStatus == GpStatus::Ok;
	}
#else
	Font::Font(const WCHAR* familyName, REAL emSize, INT style,
		Unit unit, const FontCollection* fontCollection) :
		nativeFont(NULL), lastStatus(Ok)
	{
		GpFontFamily* nativeFamily = NULL;
		lastStatus = DLLExports::GdipCreateFontFamilyFromName(
			familyName,
			fontCollection ? fontCollection->nativeFontCollection : NULL,
			&nativeFamily);

		if (nativeFamily) {
			lastStatus = DLLExports::GdipCreateFont(
				nativeFamily, emSize, style, unit, &nativeFont);
			DLLExports::GdipDeleteFontFamily(nativeFamily);
		}
	}
#endif

	Font::~Font()
	{
		DLLExports::GdipDeleteFont(nativeFont);
	}

	Font* Font::Clone() const
	{
		GpFont* cloneFont = NULL;
		Status status = updateStatus(DLLExports::GdipCloneFont(
			nativeFont, &cloneFont));
		if (status == GpStatus::Ok) {
			Font* result = new Font(cloneFont, lastStatus);
			if (!result) {
				DLLExports::GdipDeleteFont(cloneFont);
				lastStatus = OutOfMemory;
			}
			return result;
		}
		else {
			return NULL;
		}
	}

	Status Font::GetFamily(FontFamily* family) const
	{
		if (!family) return lastStatus = GpStatus::InvalidParameter;
		// FIXME: do we need to call GdipDeleteFontFamily first?
		return family->lastStatus = updateStatus(DLLExports::GdipGetFamily(
			nativeFont, &family->nativeFontFamily));
	}

	REAL Font::GetHeight(const Graphics* graphics) const
	{
		REAL result = 0.0f;
		updateStatus(DLLExports::GdipGetFontHeight(
			nativeFont, graphics ? graphics->nativeGraphics : NULL,
			&result));
		return result;
	}

	REAL Font::GetHeight(REAL dpi) const
	{
		REAL result = 0.0f;
		updateStatus(DLLExports::GdipGetFontHeightGivenDPI(
			nativeFont, dpi, &result));
		return result;
	}

	Status Font::GetLogFontA(const Graphics* graphics, LOGFONTA* logfontA)
		const
	{
		return updateStatus(DLLExports::GdipGetLogFontA(
			nativeFont, graphics ? graphics->nativeGraphics : NULL,
			logfontA));
	}

	Status Font::GetLogFontW(const Graphics* graphics, LOGFONTW* logfontW)
		const
	{
		return updateStatus(DLLExports::GdipGetLogFontW(
			nativeFont, graphics ? graphics->nativeGraphics : NULL,
			logfontW));
	}

	REAL Font::GetSize() const
	{
		REAL result = 0.0;
		updateStatus(DLLExports::GdipGetFontSize(nativeFont, &result));
		return result;
	}

	INT Font::GetStyle() const
	{
		INT result = FontStyleRegular;
		updateStatus(DLLExports::GdipGetFontStyle(nativeFont, &result));
		return result;
	}

	Unit Font::GetUnit() const
	{
		Unit result = UnitPoint;
		updateStatus(DLLExports::GdipGetFontUnit(nativeFont, &result));
		return result;
	}




	// FontCollection

	FontCollection::FontCollection() :
		nativeFontCollection(NULL), lastStatus(Ok)
	{
	}

	Status FontCollection::GetFamilies(INT numSought,
		FontFamily* families, INT* numFound) const
	{
		if (numSought <= 0 || !families || !numFound)
			return lastStatus = GpStatus::InvalidParameter;

		for (int i = 0; i < numSought; ++i) {
			families[i].nativeFontFamily = NULL;
			families[i].lastStatus = FontFamilyNotFound;
		}
		*numFound = 0;

		GpFontFamily** nativeFamilyArray =new GpFontFamily *[numSought];
		if (!nativeFamilyArray)
			return lastStatus = OutOfMemory;

		Status status = updateStatus(DLLExports::GdipGetFontCollectionFamilyList(
			nativeFontCollection, numSought, nativeFamilyArray,
			numFound));

		// FIXME: must the native GpFontFamily objects be cloned? Seems so.
		// (if this is not done, the "Creating a Private Font Collection"
		// example crashes on "delete[] pFontFamily")

		if (status == GpStatus::Ok) {
			for (int i = 0; i < *numFound; ++i) {
				families[i].lastStatus =
					updateStatus(DLLExports::GdipCloneFontFamily(
						nativeFamilyArray[i],
						&families[i].nativeFontFamily));
			}
		}

		delete[](nativeFamilyArray);
		return status;
	}

	INT FontCollection::GetFamilyCount() const
	{
		INT result = 0;
		updateStatus(DLLExports::GdipGetFontCollectionFamilyCount(
			nativeFontCollection, &result));
		return result;
	}


	// FontFamily

	// FIXME: do FontFamily::GenericMonospace() et al. need to be thread safe?
	// FIXME: maybe put parts of this in gdiplus.c

	extern "C" void* _GdipFontFamilyCachedGenericMonospace;
	extern "C" void* _GdipFontFamilyCachedGenericSansSerif;
	extern "C" void* _GdipFontFamilyCachedGenericSerif;

	const FontFamily* FontFamily::GenericMonospace()
	{
		if (!_GdipFontFamilyCachedGenericMonospace) {
			GpFontFamily* nativeFontFamily = 0;
			Status status = DLLExports::GdipGetGenericFontFamilyMonospace(
				&nativeFontFamily);
			if (status == Ok && nativeFontFamily) {
				_GdipFontFamilyCachedGenericMonospace = (void*)
					new FontFamily(nativeFontFamily, GpStatus::Ok);
			}
		}
		return (FontFamily*)_GdipFontFamilyCachedGenericMonospace;
	}

	const FontFamily* FontFamily::GenericSansSerif()
	{
		if (!_GdipFontFamilyCachedGenericSansSerif) {
			GpFontFamily* nativeFontFamily = 0;
			Status status = DLLExports::GdipGetGenericFontFamilySansSerif(
				&nativeFontFamily);
			if (status == Ok && nativeFontFamily) {
				_GdipFontFamilyCachedGenericSansSerif = (void*)
					new FontFamily(nativeFontFamily, GpStatus::Ok);
			}
		}
		return (FontFamily*)_GdipFontFamilyCachedGenericSansSerif;
	}

	const FontFamily* FontFamily::GenericSerif()
	{
		if (!_GdipFontFamilyCachedGenericSerif) {
			GpFontFamily* nativeFontFamily = 0;
			Status status = DLLExports::GdipGetGenericFontFamilySerif(
				&nativeFontFamily);
			if (status == Ok && nativeFontFamily) {
				_GdipFontFamilyCachedGenericSerif = (void*)
					new FontFamily(nativeFontFamily, GpStatus::Ok);
			}
		}
		return (FontFamily*)_GdipFontFamilyCachedGenericSerif;
	}

	FontFamily::FontFamily() :
		nativeFontFamily(NULL), lastStatus(Ok)
	{
	}

	FontFamily::FontFamily(const WCHAR* name,
		const FontCollection* fontCollection) :
		nativeFontFamily(NULL), lastStatus(Ok)
	{
		lastStatus = DLLExports::GdipCreateFontFamilyFromName(name,
			fontCollection ? fontCollection->nativeFontCollection : NULL,
			&nativeFontFamily);
	}

	FontFamily::~FontFamily()
	{
		DLLExports::GdipDeleteFontFamily(nativeFontFamily);
	}

	FontFamily* FontFamily::Clone() const
	{
		GpFontFamily* cloneFontFamily = NULL;
		Status status = updateStatus(DLLExports::GdipCloneFontFamily(
			nativeFontFamily, &cloneFontFamily));
		if (status == GpStatus::Ok) {
			FontFamily* result = new FontFamily(cloneFontFamily, lastStatus);
			if (!result) {
				DLLExports::GdipDeleteFontFamily(cloneFontFamily);
				lastStatus = OutOfMemory;
			}
			return result;
		}
		else {
			return NULL;
		}
	}

	UINT16 FontFamily::GetCellAscent(INT style) const
	{
		UINT16 result = 0;
		updateStatus(DLLExports::GdipGetCellAscent(
			nativeFontFamily, style, &result));
		return result;
	}

	UINT16 FontFamily::GetCellDescent(INT style) const
	{
		UINT16 result = 0;
		updateStatus(DLLExports::GdipGetCellDescent(
			nativeFontFamily, style, &result));
		return result;
	}

	UINT16 FontFamily::GetEmHeight(INT style) const
	{
		UINT16 result = 0;
		updateStatus(DLLExports::GdipGetEmHeight(
			nativeFontFamily, style, &result));
		return result;
	}

	Status FontFamily::GetFamilyName(WCHAR name[LF_FACESIZE],
		LANGID language) const
	{
		return updateStatus(DLLExports::GdipGetFamilyName(
			nativeFontFamily, name, language));
	}

	UINT16 FontFamily::GetLineSpacing(INT style) const
	{
		UINT16 result = 0;
		updateStatus(DLLExports::GdipGetLineSpacing(
			nativeFontFamily, style, &result));
		return result;
	}

	BOOL FontFamily::IsStyleAvailable(INT style) const
	{
		BOOL result = FALSE;
		updateStatus(DLLExports::GdipIsStyleAvailable(
			nativeFontFamily, style, &result));
		return result;
	}


	// InstalledFontCollection

	InstalledFontCollection::InstalledFontCollection()
	{
		lastStatus = DLLExports::GdipNewInstalledFontCollection(
			&nativeFontCollection);
	}


	// PrivateFontCollection

	PrivateFontCollection::PrivateFontCollection()
	{
		lastStatus = DLLExports::GdipNewPrivateFontCollection(
			&nativeFontCollection);
	}

	Status PrivateFontCollection::AddFontFile(const WCHAR* filename)
	{
		return updateStatus(DLLExports::GdipPrivateAddFontFile(
			nativeFontCollection, filename));
	}

	Status PrivateFontCollection::AddMemoryFont(
		const VOID* memory, INT length)
	{
		return updateStatus(DLLExports::GdipPrivateAddMemoryFont(
			nativeFontCollection, memory, length));
	}

};

