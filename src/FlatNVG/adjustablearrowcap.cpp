/*
 * adjustablearrowcap.cpp
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
 * Copyright (C) 2003-2007 Novell, Inc (http://www.novell.com)
 * Copyright (C) 2020 Asobo Studio.
 */
#include "FlatNVG\adjustablearrowcap.h"

namespace DLLExports {

	/* AdjustableArrowCap functions */
	GpStatus WINGDIPAPI
		GdipCreateAdjustableArrowCap(REAL height, REAL width, BOOL isFilled, GpAdjustableArrowCap** cap)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetAdjustableArrowCapHeight(GpAdjustableArrowCap* cap, REAL height)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI
		GdipGetAdjustableArrowCapHeight(GpAdjustableArrowCap* cap, REAL* height)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetAdjustableArrowCapWidth(GpAdjustableArrowCap* cap, REAL width)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetAdjustableArrowCapWidth(GpAdjustableArrowCap* cap, REAL* width)
	{
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetAdjustableArrowCapMiddleInset(GpAdjustableArrowCap* cap, REAL middleInset)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI
		GdipGetAdjustableArrowCapMiddleInset(GpAdjustableArrowCap* cap, REAL* middleInset)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI
		GdipSetAdjustableArrowCapFillState(GpAdjustableArrowCap* cap, BOOL isFilled)
	{
		return GpStatus::NotImplemented;
	}
	GpStatus WINGDIPAPI
		GdipGetAdjustableArrowCapFillState(GpAdjustableArrowCap* cap, BOOL* isFilled)
	{
		return GpStatus::NotImplemented;
	}
}