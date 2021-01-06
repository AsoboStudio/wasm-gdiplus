/*
 * bitmap.cpp
 *
 * Copyright (c) 2003 Alexandre Pigolkine
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
 *   Alexandre Pigolkine (pigolkine@gmx.de)
 *   Vladimir Vukicevic (vladimir@pobox.com)
 *   Jordi Mas (jordi@ximian.com)
 *   Jonathan Gilbert (logic@deltaq.org)
 *   Sebastien Pouliot  <sebastien@ximian.com>
 *	 Sol Roo (sroo@asobostudio.com)
 *
 * Copyright (C) 2003-2007 Novell, Inc (http://www.novell.com)
 */

#include "gdiplusflat.h"
#include "FlatNVG\bitmap-private.h"
#include "FlatNVG\graphics-private.h"

#include "FlatNVG\bitmap-private.h"
#include <locale>
#include <codecvt>

namespace DLLExports {
	extern BOOL gdiplusInitialized;

	GpStatus
		GdipCreateBitmapFromStream(void* stream, GpBitmap** bitmap)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus
		GdipCreateBitmapFromFile(GDIPCONST WCHAR* filename, GpBitmap** bitmap)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string pathUTF8(converter.to_bytes(filename));

		return GdipCreateBitmapFromFileUTF8(pathUTF8.c_str(), bitmap);
	}
	GpStatus
		GdipCreateBitmapFromFileLinked(GDIPCONST WCHAR* filename, GpBitmap** bitmap, GpGraphics* graphics)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string pathUTF8(converter.to_bytes(filename));

		return GdipCreateBitmapFromFileUTF8Linked(pathUTF8.c_str(), bitmap, graphics);
	}
	GpStatus
		GdipCreateBitmapFromFileUTF8Linked(GDIPCONST char* filename, GpBitmap** bitmap, GpGraphics* graphics)
	{
		GpBitmap* nativeBitmap = (GpBitmap*)DLLExports::GdipAlloc(sizeof(GpBitmap));
		if (!graphics) {
			nativeBitmap->bStatic = FALSE;
			nativeBitmap->ctx = NULL;
			nativeBitmap->path = new std::string(filename);
			nativeBitmap->hImage = -1;
			nativeBitmap->iWidth = 0;
			nativeBitmap->iHeight = 0;
			nativeBitmap->bContainsPixelData = FALSE;
			nativeBitmap->scan0 = NULL;
		}
		else
		{
			nativeBitmap->bStatic = TRUE;
			nativeBitmap->ctx = graphics->ctx;
			nativeBitmap->path = new std::string(filename);
			nativeBitmap->hImage = nvgCreateImage(graphics->ctx, filename, 0);
			nvgImageSize(nativeBitmap->ctx, nativeBitmap->hImage, &nativeBitmap->iWidth, &nativeBitmap->iHeight);
			nativeBitmap->bContainsPixelData = FALSE;
			nativeBitmap->scan0 = NULL;
		}
		*bitmap = nativeBitmap;

		return GpStatus::Ok;
	}
	GpStatus
		GdipCreateBitmapFromFileUTF8(GDIPCONST char* filename, GpBitmap** bitmap) {
		return GdipCreateBitmapFromFileUTF8Linked(filename, bitmap, NULL);
	}

	GpStatus
		GdipCreateBitmapFromStreamICM(void* stream, GpBitmap** bitmap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipCreateBitmapFromFileICM(GDIPCONST WCHAR* filename, GpBitmap** bitmap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipCreateBitmapFromScan0(INT width, INT height, INT stride, PixelFormat format, BYTE* scan0, GpBitmap** bitmap)
	{
		GpStatus status;
		GpBitmap* result;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		GpBitmap* nativeBitmap = (GpBitmap*)DLLExports::GdipAlloc(sizeof(GpBitmap));
		nativeBitmap->bStatic = FALSE;
		nativeBitmap->ctx = (*bitmap) != NULL ? (*bitmap)->ctx : NULL;
		nativeBitmap->path = NULL;
		nativeBitmap->hImage = -1;
		nativeBitmap->iWidth = width;
		nativeBitmap->iHeight = height;
		nativeBitmap->bContainsPixelData = TRUE;
		nativeBitmap->scan0 = (BYTE*)DLLExports::GdipAlloc(sizeof(BYTE) * width * height * 4);
		memset(nativeBitmap->scan0, 0, sizeof(BYTE) * width * height * 4);

		*bitmap = nativeBitmap;
		return GpStatus::Ok;
	}

	GpStatus
		GdipCreateBitmapFromGraphics(int width, int height, GpGraphics* graphics, GpBitmap** bitmap)
	{
		GpStatus status;
		GpBitmap* result;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		if (!graphics || !bitmap)
			return GpStatus::InvalidParameter;

		GpBitmap* nativeBitmap = (GpBitmap*)DLLExports::GdipAlloc(sizeof(GpBitmap));
		nativeBitmap->bStatic = FALSE;
		nativeBitmap->ctx = graphics->ctx;
		nativeBitmap->path = NULL;
		nativeBitmap->iWidth = width;
		nativeBitmap->iHeight = height;
		nativeBitmap->bContainsPixelData = TRUE;
		nativeBitmap->scan0 = (BYTE*)DLLExports::GdipAlloc(sizeof(BYTE) * width * height * 4);
		memset(nativeBitmap->scan0, 0, sizeof(BYTE) * width * height * 4);
		nativeBitmap->hImage = -1;

		*bitmap = nativeBitmap;
		return GpStatus::Ok;
	}

	GpStatus
		GdipCreateBitmapFromHBITMAP(HBITMAP hbm, HPALETTE hpal, GpBitmap** bitmap)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus
		GdipCreateHBITMAPFromBitmap(GpBitmap* bitmap, HBITMAP* hbmReturn, ARGB background)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipCreateBitmapFromHICON(HICON hicon, GpBitmap** bitmap) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipCreateHICONFromBitmap(GpBitmap* bitmap, HICON* hbmReturn) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCreateBitmapFromResource(HINSTANCE hInstance, GDIPCONST WCHAR* lpBitmapName, GpBitmap** bitmap) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCloneBitmapAreaI(INT x, INT y, INT width, INT height, PixelFormat format,
			GpBitmap* original, GpBitmap** bitmap) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCloneBitmapArea(REAL x, REAL y, REAL w, REAL h, PixelFormat format,
			GpBitmap* original, GpBitmap** bitmap) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipBitmapLockBits(GpBitmap* bitmap, GDIPCONST GpRect* rect, UINT flags, PixelFormat format, GpBitmapData* lockedBitmapData) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipBitmapUnlockBits(GpBitmap* bitmap, GpBitmapData* lockedBitmapData) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipBitmapSetPixel(GpBitmap* bitmap, INT x, INT y, ARGB color) {

#ifdef  GDIFLAT_SAFE
		if (!bitmap || !bitmap->bContainsPixelData) {
			return GpStatus::InvalidParameter;
		}
		if (x < 0 || x >= bitmap->iWidth || y < 0 || y >= bitmap->iHeight)
			return GpStatus::InvalidParameter;

		if (bitmap->scan0 == NULL) {
			return GpStatus::InvalidParameter;
		}
#endif //  GDIFLAT_SAFE

		BYTE* pixel = &bitmap->scan0[(y * bitmap->iWidth + x) * 4];
		pixel[0] = (BYTE)(color >> 16);
		pixel[1] = (BYTE)(color >> 8);
		pixel[2] = (BYTE)color;
		pixel[3] = (BYTE)(color >> 24);
		bitmap->bDirty = true;

		return GpStatus::Ok;
	}


	GpStatus WINGDIPAPI
		GdipBitmapGetPixel(GpBitmap* bitmap, INT x, INT y, ARGB* color) {
#ifdef  GDIFLAT_SAFE
		if (!bitmap || !bitmap->bContainsPixelData) {
			return GpStatus::InvalidParameter;
		}
		if (x < 0 || x >= bitmap->iWidth || y < 0 || y >= bitmap->iHeight)
			return GpStatus::InvalidParameter;

		if (bitmap->scan0 == NULL) {
			return GpStatus::InvalidParameter;
}
#endif //  GDIFLAT_SAFE

		BYTE* pixel = &bitmap->scan0[(y * bitmap->iWidth + x) * 4];
		*color = (BYTE)(pixel[0] << 16) + (BYTE)(pixel[1] << 8) + (BYTE)pixel[2] + (BYTE)(pixel[3] << 24);

		return GpStatus::Ok;
	}


	GpStatus WINGDIPAPI
		GdipBitmapSetResolution(GpBitmap* bitmap, REAL xdpi, REAL ydpi) {
		return GpStatus::NotImplemented;
	}

}