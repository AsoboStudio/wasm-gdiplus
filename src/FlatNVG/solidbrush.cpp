/*
 * solidbrush.cpp
 * 
 * Copyright (c) 2003 Alexandre Pigolkine
 * Copyright (C) 2007 Novell, Inc. http://www.novell.com
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
 *   Alexandre Pigolkine(pigolkine@gmx.de)
 *   Ravindra (rkumar@novell.com)
 *	 Sol Roo (sroo@asobostudio.com)
 */

#include "FlatNVG\brush-private.h"
#include "FlatNVG\solidbrush-private.h"
#include "FlatNVG\graphics-private.h"

static GpStatus gdip_solidfill_setup(GpGraphics* graphics, GpBrush* brush);
static GpStatus gdip_solidfill_clone(GpBrush* brush, GpBrush** clonedBrush);
static GpStatus gdip_solidfill_destroy(GpBrush* brush);

/*
 * we have a single copy of vtable for
 * all instances of solidbrush.
 */

static BrushClass vtable = { GpBrushType::BrushTypeSolidColor,
				 gdip_solidfill_setup,
				 gdip_solidfill_clone,
				 gdip_solidfill_destroy };

static void
gdip_solidfill_init(GpSolidFill* brush)
{
	gdip_brush_init(&brush->base, &vtable);
	brush->color = 0x00000000;
	brush->base.brush = (void*)brush;
}

static GpSolidFill*
gdip_solidfill_new()
{
	GpSolidFill* result = (GpSolidFill*)DLLExports::GdipAlloc(sizeof(GpSolidFill));

	if (result)
		gdip_solidfill_init(result);

	return result;
}

GpStatus
gdip_solidfill_setup(GpGraphics* graphics, GpBrush* brush)
{
	if (!graphics || !brush)
		return GpStatus::InvalidParameter;

	GpSolidFill* solid = (GpSolidFill*)brush->brush;
	nvgFillColor(graphics->ctx, ConvertARGB(solid->color));

	return GpStatus::Ok;
}

GpStatus
gdip_solidfill_clone(GpBrush* brush, GpBrush** clonedBrush)
{
	GpSolidFill* result;
	GpSolidFill* solid;

	result = gdip_solidfill_new();
	if (!result)
		return GpStatus::OutOfMemory;

	solid = (GpSolidFill*)brush;

	result->base = solid->base;
	result->color = solid->color;
	result->base.changed = TRUE;

	*clonedBrush = (GpBrush*)result;
	return GpStatus::Ok;
}

GpStatus
gdip_solidfill_destroy(GpBrush* brush)
{
	return GpStatus::Ok;
}

namespace DLLExports {

	static BOOL gdiplusInitialized = true;

	// coverity[+alloc : arg-*1]
	GpStatus WINGDIPAPI
		GdipCreateSolidFill(ARGB color, GpSolidFill** brush)
	{
		GpSolidFill* result;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

		if (!brush)
			return GpStatus::InvalidParameter;

		result = gdip_solidfill_new();
		if (!result)
			return GpStatus::OutOfMemory;

		result->color = color;

		*brush = result;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetSolidFillColor(GpSolidFill* brush, ARGB color)
	{
		if (!brush)
			return GpStatus::InvalidParameter;

		brush->color = color;
		brush->base.changed = TRUE;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetSolidFillColor(GpSolidFill* brush, ARGB* color)
	{
		if (!brush || !color)
			return GpStatus::InvalidParameter;

		*color = brush->color;
		return GpStatus::Ok;
	}

}