/*
 * text.cpp
 *
 * Copyright (c) 2003 Alexandre Pigolkine, Novell Inc.
 * Copyright (C) 2006-2007 Novell, Inc (http://www.novell.com)
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
 *   Duncan Mak (duncan@ximian.com)
 *   Sebastien Pouliot  <sebastien@ximian.com>
 *	 Sol Roo (sroo@asobostudio.com)
 *
 */
#include "gdiplusflat.h"
#include "FlatNVG\graphics-path-private.h"
#include "FlatNVG\font-private.h"
#include "FlatNVG\brush-private.h"

#include <locale>
#include <codecvt>

inline void ConvertFont(GpGraphics* graphics, GpFont* font) {

	nvgFontSize(graphics->ctx, font->size);
	nvgFontFaceId(graphics->ctx, font->fontid);
}
inline GpPointF GetPivot(const GpRectF* layoutRect, const GpStringFormat* stringFormat, float fontSize)
{
	GpPointF position;

	if (layoutRect->Width == 0 || stringFormat->alignment == GpStringAlignment::StringAlignmentNear)
		position.X = layoutRect->X;
	else if (stringFormat->alignment == GpStringAlignment::StringAlignmentCenter)
		position.X = layoutRect->X + layoutRect->Width / 2;
	else //(stringFormat->lineAlignment == GpStringAlignment::StringAlignmentFar) 
		position.X = layoutRect->X + layoutRect->Width;

	if (layoutRect->Height == 0 || stringFormat->lineAlignment == GpStringAlignment::StringAlignmentNear)
		position.Y = layoutRect->Y;
	else if (stringFormat->lineAlignment == GpStringAlignment::StringAlignmentCenter)
		position.Y = layoutRect->Y + layoutRect->Height / 2;
	else //(stringFormat->alignment == GpStringAlignment::StringAlignmentFar) 
		position.Y = layoutRect->Y + layoutRect->Height;

	return position;
}

namespace DLLExports {

	GpStatus WINGDIPAPI
		GdipDrawString(GpGraphics* graphics, GDIPCONST WCHAR* string, INT length, GpFont* font, GDIPCONST GpRectF* layoutRect,
			GDIPCONST GpStringFormat* stringFormat, GpBrush* brush)
	{
#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx) {
			return GpStatus::InvalidParameter;
		}
#endif
		if (!gdip_load_font(graphics, font)) {
			return GpStatus::InvalidParameter;
		}

		ConvertFont(graphics, font);

		gdip_brush_setup(graphics, brush);

		ConvertStringFormat(graphics, stringFormat);

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string u8str = converter.to_bytes(string);
		const char* cStr = u8str.c_str();

		GpPointF pivot = GetPivot(layoutRect, stringFormat, font->size);
		nvgBeginPath(graphics->ctx);
		nvgText(graphics->ctx, pivot.X, pivot.Y, cStr, nullptr);

#ifdef DEBUG_TEXT
		{
			// bounding box
			if (layoutRect->Width > 0 || layoutRect->Height > 0)
			{
				nvgBeginPath(graphics->ctx);
				nvgRect(graphics->ctx, layoutRect->X, layoutRect->Y, layoutRect->Width, layoutRect->Height);
				nvgFillColor(graphics->ctx, nvgRGBA(255, 0, 0, 50));
				nvgFill(graphics->ctx);
				nvgStrokeColor(graphics->ctx, ConvertARGB(Color::Red));
				nvgStroke(graphics->ctx);
			}
			// pivot
			nvgBeginPath(graphics->ctx);
			nvgCircle(graphics->ctx, pivot.X, pivot.Y, font->size / 8);
			nvgStrokeColor(graphics->ctx, ConvertARGB(Color::Red));
			nvgStroke(graphics->ctx);

		}
#endif
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipMeasureString(GpGraphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST GpFont* font, GDIPCONST GpRectF* layoutRect,
			GDIPCONST GpStringFormat* stringFormat, GpRectF* boundingBox, INT* codepointsFitted, INT* linesFilled) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipMeasureCharacterRanges(GpGraphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST GpFont* font,
			GDIPCONST GpRectF* layoutRect, GDIPCONST GpStringFormat* stringFormat, INT regionCount, GpRegion** regions) {
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI GdipDrawDriverString(GpGraphics* graphics, GDIPCONST UINT16* text, INT length, GpFont* font,
		GpBrush* brush, GDIPCONST GpPointF* positions, INT flags, GDIPCONST GpMatrix* matrix) {

#ifdef GDIFLAT_SAFE
		if (!graphics || !graphics->ctx || !text || !font || !brush || !positions) {
			return GpStatus::InvalidParameter;
		}
#endif
		if (!gdip_load_font(graphics, font)) {
			return GpStatus::InvalidParameter;
		}

		ConvertFont(graphics, font);
		gdip_brush_setup(graphics, brush);

		// [text] contains potentially [length] amount of substrings, in 16 bit encoding
		// to render, they need to be divided and rendered individually
		const wchar_t* substring = (wchar_t*)text;
		length = max(length, 1);
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conversion;

		for (int t = 0; t < length; ++t) {
			auto substringLength = wcslen((wchar_t*)text);
			// start with a char* buffer the length of the entry [text]
			std::string convertedText = conversion.to_bytes(substring);
			// draw the substring using the matching index of [positions]
			nvgText(graphics->ctx, positions[t].X, positions[t].Y, convertedText.c_str(), NULL);

			substring = &substring[length];

#ifdef DEBUG_TEXT
			{
				// pivot
				nvgBeginPath(graphics->ctx);
				nvgCircle(graphics->ctx, positions[t].X, positions[t].Y, font->size / 8);
				nvgStrokeColor(graphics->ctx, ConvertARGB(Color::Red));
				nvgStroke(graphics->ctx);
			}
#endif

		}
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipMeasureDriverString(GpGraphics* graphics, GDIPCONST UINT16* text, INT length, GDIPCONST GpFont* font,
			GDIPCONST GpPointF* positions, INT flags, GDIPCONST GpMatrix* matrix, GpRectF* boundingBox) {
		return GpStatus::NotImplemented;
	}

}