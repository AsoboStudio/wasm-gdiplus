// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers.

#include "GdiPlus.h"
#include "gdiplusflat.h"

// CustomLineCap
namespace Gdiplus {

	 CustomLineCap::CustomLineCap(
		const GraphicsPath* fillPath,
		const GraphicsPath* strokePath,
		LineCap baseCap, REAL baseInset) :
		nativeCustomLineCap(NULL), lastStatus(Ok)
	{
		lastStatus = DLLExports::GdipCreateCustomLineCap(
			fillPath ? fillPath->nativePath : NULL,
			strokePath ? strokePath->nativePath : NULL,
			baseCap, baseInset, &nativeCustomLineCap);
	}

	 LineCap CustomLineCap::GetBaseCap() const
	{
		LineCap result = LineCapFlat;
		updateStatus(DLLExports::GdipGetCustomLineCapBaseCap(
			nativeCustomLineCap, &result));
		return result;
	}

	 REAL CustomLineCap::GetBaseInset() const
	{
		REAL result = 0.0f;
		updateStatus(DLLExports::GdipGetCustomLineCapBaseInset(
			nativeCustomLineCap, &result));
		return result;
	}

	 Status CustomLineCap::GetStrokeCaps(LineCap* startCap,
		LineCap* endCap) const
	{
		return updateStatus(DLLExports::GdipGetCustomLineCapStrokeCaps(
			nativeCustomLineCap, startCap, endCap));
	}

	 LineJoin CustomLineCap::GetStrokeJoin() const
	{
		LineJoin result = LineJoinMiter;
		updateStatus(DLLExports::GdipGetCustomLineCapStrokeJoin(
			nativeCustomLineCap, &result));
		return result;
	}

	 REAL CustomLineCap::GetWidthScale() const
	{
		REAL result = 0.0f;
		updateStatus(DLLExports::GdipGetCustomLineCapWidthScale(
			nativeCustomLineCap, &result));
		return result;
	}

	 Status CustomLineCap::SetBaseCap(LineCap baseCap)
	{
		return updateStatus(DLLExports::GdipSetCustomLineCapBaseCap(
			nativeCustomLineCap, baseCap));
	}

	 Status CustomLineCap::SetBaseInset(REAL inset)
	{
		return updateStatus(DLLExports::GdipSetCustomLineCapBaseInset(
			nativeCustomLineCap, inset));
	}

	 Status CustomLineCap::SetStrokeCap(LineCap strokeCap)
	{
		return updateStatus(DLLExports::GdipSetCustomLineCapStrokeCaps(
			nativeCustomLineCap, strokeCap, strokeCap));
	}

	 Status CustomLineCap::SetStrokeCaps(LineCap startCap, LineCap endCap)
	{
		return updateStatus(DLLExports::GdipSetCustomLineCapStrokeCaps(
			nativeCustomLineCap, startCap, endCap));
	}

	 Status CustomLineCap::SetStrokeJoin(LineJoin lineJoin)
	{
		return updateStatus(DLLExports::GdipSetCustomLineCapStrokeJoin(
			nativeCustomLineCap, lineJoin));
	}

	 Status CustomLineCap::SetWidthScale(REAL widthScale)
	{
		return updateStatus(DLLExports::GdipSetCustomLineCapWidthScale(
			nativeCustomLineCap, widthScale));
	}
}