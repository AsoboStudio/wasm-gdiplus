/*
 * customlinecap.cpp
 *
 * Copyright (C) Novell, Inc. 2003-2004.
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
 * Author:
 *      Ravindra (rkumar@novell.com)
 *		Sol Roo (sroo@asobostudio.com)
 *
 */
#include "FlatNVG\customlinecap.h"

namespace DLLExports {

	GpStatus WINGDIPAPI
		GdipCreateCustomLineCap(GpPath* fillPath, GpPath* strokePath, GpLineCap baseCap, REAL baseInset, GpCustomLineCap** customCap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDeleteCustomLineCap(GpCustomLineCap* customCap)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI
		GdipCloneCustomLineCap(GpCustomLineCap* customCap, GpCustomLineCap** clonedCap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetCustomLineCapType(GpCustomLineCap* customCap, GpCustomLineCapType* capType)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetCustomLineCapStrokeCaps(GpCustomLineCap* customCap, GpLineCap startCap, GpLineCap endCap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetCustomLineCapStrokeCaps(GpCustomLineCap* customCap, GpLineCap* startCap, GpLineCap* endCap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetCustomLineCapStrokeJoin(GpCustomLineCap* customCap, GpLineJoin lineJoin)
	{
		if (!customCap)
			return GpStatus::InvalidParameter;

		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetCustomLineCapStrokeJoin(GpCustomLineCap* customCap, GpLineJoin* lineJoin)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetCustomLineCapBaseCap(GpCustomLineCap* customCap, GpLineCap baseCap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetCustomLineCapBaseCap(GpCustomLineCap* customCap, GpLineCap* baseCap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetCustomLineCapBaseInset(GpCustomLineCap* customCap, REAL inset)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI
		GdipGetCustomLineCapBaseInset(GpCustomLineCap* customCap, REAL* inset)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetCustomLineCapWidthScale(GpCustomLineCap* customCap, REAL widthScale)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetCustomLineCapWidthScale(GpCustomLineCap* customCap, REAL* widthScale)
	{
		return GpStatus::NotImplemented;
	}
}