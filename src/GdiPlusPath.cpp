// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers.

#include "GdiPlus.h"
#include "gdiplusflat.h"


namespace Gdiplus {
// GraphicsPath

 BOOL GraphicsPath::IsOutlineVisible(REAL x, REAL y, const Pen* pen,
	const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsOutlineVisiblePathPoint(
		nativePath, x, y, pen ? pen->nativePen : NULL,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}

 BOOL GraphicsPath::IsOutlineVisible(INT x, INT y, const Pen* pen,
	const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsOutlineVisiblePathPointI(
		nativePath, x, y, pen ? pen->nativePen : NULL,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}

 BOOL GraphicsPath::IsOutlineVisible(const PointF& point, const Pen* pen,
	const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsOutlineVisiblePathPoint(
		nativePath, point.X, point.Y,
		pen ? pen->nativePen : NULL,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}

 BOOL GraphicsPath::IsOutlineVisible(const Point& point, const Pen* pen,
	const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsOutlineVisiblePathPointI(
		nativePath, point.X, point.Y,
		pen ? pen->nativePen : NULL,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}

 BOOL GraphicsPath::IsVisible(REAL x, REAL y, const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisiblePathPoint(
		nativePath, x, y,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}
 BOOL GraphicsPath::IsVisible(INT x, INT y, const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisiblePathPointI(
		nativePath, x, y,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}

 BOOL GraphicsPath::IsVisible(const PointF& point,
	const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisiblePathPoint(
		nativePath, point.X, point.Y,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}

 BOOL GraphicsPath::IsVisible(const Point& point, const Graphics* g) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisiblePathPointI(
		nativePath, point.X, point.Y,
		g ? g->nativeGraphics : NULL, &result));
	return result;
}
}