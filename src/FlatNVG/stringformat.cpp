/*
 * Copyright (C) 2004 Ximian inc. http://www.ximian.com
 * Copyright (C) 2004,2006-2007 Novell Inc. http://www.novell.com
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
 *	  Jordi Mas i Hernandez <jordi@ximian.com>, 2004
 *	  Ravindra <rkumar@novell.com>
 *	  Sebastien Pouliot  <sebastien@ximian.com>
 *	  Sol Roo (sroo@asobostudio.com)
 */
#include "gdiplusflat.h"
#include "FlatNVG\graphics-private.h"
#include "FlatNVG\stringformat-private.h"

void ConvertStringFormat(GpGraphics* graphics, CONST GpStringFormat* format) {

	int nvg_align = 0;
	switch (format->alignment)
	{
	case GpStringAlignment::StringAlignmentNear:
		nvg_align |= NVG_ALIGN_LEFT;
		break;
	case GpStringAlignment::StringAlignmentCenter:
		nvg_align |= NVG_ALIGN_CENTER;
		break;
	case GpStringAlignment::StringAlignmentFar:
		nvg_align |= NVG_ALIGN_RIGHT;
		break;
	}
	switch (format->lineAlignment)
	{
	case GpStringAlignment::StringAlignmentNear:
		nvg_align |= NVG_ALIGN_TOP;
		break;
	case GpStringAlignment::StringAlignmentCenter:
		nvg_align |= NVG_ALIGN_MIDDLE;
		break;
	case GpStringAlignment::StringAlignmentFar:
		nvg_align |= NVG_ALIGN_BOTTOM;
		break;
	}
	nvgTextAlign(graphics->ctx, nvg_align);
}


static GpStringFormat stringFormatDefault;
static GpStringFormat stringFormatTypographic;

static void
gdip_string_format_clear(GpStringFormat* format)
{
	/*
	if (format->tabStops) {
		GdipFree(format->tabStops);
		format->tabStops = NULL;
	}

	if (format->charRanges) {
		GdipFree(format->charRanges);
		format->charRanges = NULL;
	}*/
}

static void
gdip_string_format_init(GpStringFormat* result)
{
	result->alignment = GpStringAlignment::StringAlignmentNear;
	result->lineAlignment = GpStringAlignment::StringAlignmentNear;
	//result->hotkeyPrefix = HotkeyPrefixNone;
	result->formatFlags = 0;
	result->trimming = GpStringTrimming::StringTrimmingCharacter;
	//result->substitute = StringDigitSubstituteUser;
	//result->language = 0;
	//result->firstTabOffset = 0;
	//result->tabStops = NULL;
	//result->numtabStops = 0;
	//result->charRanges = NULL;
	//result->charRangeCount = 0;
}

void
gdip_create_generic_stringformats()
{
	gdip_string_format_init(&stringFormatDefault);

	gdip_string_format_init(&stringFormatTypographic);
	stringFormatTypographic.formatFlags = GpStringFormatFlags::StringFormatFlagsNoFitBlackBox | GpStringFormatFlags::StringFormatFlagsLineLimit | GpStringFormatFlags::StringFormatFlagsNoClip;
	stringFormatTypographic.trimming = GpStringTrimming::StringTrimmingNone;
}

void
gdip_delete_generic_stringformats()
{
	gdip_string_format_clear(&stringFormatDefault);
	gdip_string_format_clear(&stringFormatTypographic);
}

static GpStringFormat*
gdip_string_format_new()
{
	GpStringFormat* result = (GpStringFormat*)DLLExports::GdipAlloc(sizeof(GpStringFormat));
	if (result)
		gdip_string_format_init(result);

	return result;
}
namespace DLLExports {

	extern bool gdiplusInitialized;

	/* coverity[+alloc : arg-*2] */
	GpStatus WINGDIPAPI
		GdipCreateStringFormat(INT formatAttributes, LANGID language, GpStringFormat** format)
	{
		GpStringFormat* result;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		if (!format)
			return GpStatus::InvalidParameter;

		result = gdip_string_format_new();
		if (!result) {
			*format = NULL;
			return GpStatus::OutOfMemory;
		}

		result->formatFlags = (GpStringFormatFlags) formatAttributes;

		*format = result;
		return GpStatus::Ok;
	}
	/* coverity[+alloc : arg-*0] */
	GpStatus WINGDIPAPI
		GdipStringFormatGetGenericDefault(GpStringFormat** format)
	{
		if (!format)
			return GpStatus::InvalidParameter;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		*format = &stringFormatDefault;
		return GpStatus::Ok;
	}

	/* coverity[+alloc : arg-*0] */
	GpStatus WINGDIPAPI
		GdipStringFormatGetGenericTypographic(GpStringFormat** format)
	{
		if (!format)
			return GpStatus::InvalidParameter;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		*format = &stringFormatTypographic;
		return GpStatus::Ok;
	}

	/* coverity[+alloc : arg-*1] */
	GpStatus WINGDIPAPI
		GdipCloneStringFormat(GDIPCONST GpStringFormat* format, GpStringFormat** newFormat)
	{
		GpStringFormat* result;

		if (!format || !newFormat)
			return GpStatus::InvalidParameter;

		result = gdip_string_format_new();
		if (!result)
			goto error;

		result->alignment = format->alignment;
		result->lineAlignment = format->lineAlignment;
		//result->hotkeyPrefix = format->hotkeyPrefix;
		result->formatFlags = format->formatFlags;
		result->trimming = format->trimming;
		//result->substitute = format->substitute;
		//result->language = format->language;
		//result->firstTabOffset = format->firstTabOffset;
		//result->numtabStops = format->numtabStops;
		//result->charRangeCount = format->charRangeCount;
		/*
		// Create a copy of tab stops for the clone 
		result->tabStops = (float*)GdipAlloc(sizeof(float) * format->numtabStops);
		if (!result->tabStops)
			goto error;

		memcpy(result->tabStops, format->tabStops, format->numtabStops * sizeof(float));

		// Create a copy of char ranges for the clone
		result->charRanges = (CharacterRange*)GdipAlloc(format->charRangeCount * sizeof(CharacterRange));
		if (result->charRanges == NULL)
			goto error;

		memcpy(result->charRanges, format->charRanges, format->charRangeCount * sizeof(CharacterRange));
		*/
		*newFormat = result;
		return GpStatus::Ok;

	error:
		if (result)
			GdipDeleteStringFormat(result);

		*newFormat = NULL;
		return GpStatus::OutOfMemory;
	}

	GpStatus WINGDIPAPI
		GdipDeleteStringFormat(GpStringFormat* format)
	{
		if (!format)
			return GpStatus::InvalidParameter;

		// These are singletons cleared on shutdown.
		if (format == &stringFormatDefault || format == &stringFormatTypographic)
			return GpStatus::Ok;

		gdip_string_format_clear(format);

		GdipFree(format);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetStringFormatAlign(GpStringFormat* format, GpStringAlignment align)
	{
		if (!format || align < GpStringAlignment::StringAlignmentNear || align > GpStringAlignment::StringAlignmentFar)
			return GpStatus::InvalidParameter;

		format->alignment = align;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatAlign(GDIPCONST GpStringFormat* format, GpStringAlignment* align)
	{
		if (!format || !align)
			return GpStatus::InvalidParameter;

		*align = format->alignment;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetStringFormatLineAlign(GpStringFormat* format, GpStringAlignment align)
	{
		if (!format || align < GpStringAlignment::StringAlignmentNear || align > GpStringAlignment::StringAlignmentFar)
			return GpStatus::InvalidParameter;

		format->lineAlignment = align;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatLineAlign(GDIPCONST GpStringFormat* format, GpStringAlignment* align)
	{
		if (!format || !align)
			return GpStatus::InvalidParameter;

		*align = format->lineAlignment;
		return GpStatus::Ok;
	}


	GpStatus WINGDIPAPI
		GdipSetStringFormatHotkeyPrefix(GpStringFormat* format, INT hotkeyPrefix)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatHotkeyPrefix(GDIPCONST GpStringFormat* format, INT* hotkeyPrefix)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetStringFormatFlags(GpStringFormat* format, INT flags)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatFlags(GDIPCONST GpStringFormat* format, INT* flags)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetStringFormatTrimming(GpStringFormat* format, GpStringTrimming trimming)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatTrimming(GDIPCONST GpStringFormat* format, GpStringTrimming* trimming)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetStringFormatTabStops(GpStringFormat* format, REAL firstTabOffset, INT count, GDIPCONST REAL* tabStops)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatDigitSubstitution(GDIPCONST GpStringFormat* format, LANGID* language, GpStringDigitSubstitute* substitute)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetStringFormatDigitSubstitution(GpStringFormat* format, LANGID language, GpStringDigitSubstitute substitute)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatTabStopCount(GDIPCONST GpStringFormat* format, INT* count)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatTabStops(GDIPCONST GpStringFormat* format, INT count, REAL* firstTabOffset, REAL* tabStops)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetStringFormatMeasurableCharacterRangeCount(GDIPCONST GpStringFormat* format, INT* count)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetStringFormatMeasurableCharacterRanges(GpStringFormat* format, INT rangeCount, GDIPCONST CharacterRange* ranges)
	{
		return GpStatus::NotImplemented;
	}

}