// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers.

#include "GdiPlus.h"
#include "gdiplusflat.h"


namespace Gdiplus {


// Region

 Region* Region::FromHRGN(HRGN hrgn)
{
	return new Region(hrgn);
}

 Region::Region() : nativeRegion(NULL), lastStatus(Ok)
{
	lastStatus = DLLExports::GdipCreateRegion(&nativeRegion);
}

 Region::Region(const RectF& rect) : nativeRegion(NULL), lastStatus(Ok)
{
	lastStatus = DLLExports::GdipCreateRegionRect(&rect, &nativeRegion);
}

 Region::Region(const Rect& rect) : nativeRegion(NULL), lastStatus(Ok)
{
	lastStatus = DLLExports::GdipCreateRegionRectI(&rect, &nativeRegion);
}

 Region::Region(const GraphicsPath* path) :
	nativeRegion(NULL), lastStatus(Ok)
{
	lastStatus = DLLExports::GdipCreateRegionPath(
		path ? path->nativePath : NULL, &nativeRegion);
}

 Region::Region(const BYTE* regionData, INT size) :
	nativeRegion(NULL), lastStatus(Ok)
{
	lastStatus = DLLExports::GdipCreateRegionRgnData(
		regionData, size, &nativeRegion);
}

 Region::Region(HRGN hrgn) : nativeRegion(NULL), lastStatus(Ok)
{
	lastStatus = DLLExports::GdipCreateRegionHrgn(hrgn, &nativeRegion);
}

 Region::~Region()
{
	DLLExports::GdipDeleteRegion(nativeRegion);
}

 Region* Region::Clone() const
{
	GpRegion* cloneRegion = NULL;
	Status status = updateStatus(DLLExports::GdipCloneRegion(
		nativeRegion, &cloneRegion));
	if (status == GpStatus::Ok) {
		Region* result = new Region(cloneRegion, lastStatus);
		if (!result) {
			DLLExports::GdipDeleteRegion(cloneRegion);
			lastStatus = OutOfMemory;
		}
		return result;
	}
	else {
		return NULL;
	}
}

 Status Region::Complement(const RectF& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRect(
		nativeRegion, &rect, CombineModeComplement));
}

 Status Region::Complement(const Rect& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRectI(
		nativeRegion, &rect, CombineModeComplement));
}

 Status Region::Complement(const Region* region)
{
	return updateStatus(DLLExports::GdipCombineRegionRegion(
		nativeRegion, region ? region->nativeRegion : NULL,
		CombineModeComplement));
}

 Status Region::Complement(const GraphicsPath* path)
{
	return updateStatus(DLLExports::GdipCombineRegionPath(
		nativeRegion, path ? path->nativePath : NULL,
		CombineModeComplement));
}

 BOOL Region::Equals(const Region* region, const Graphics* graphics)
const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsEqualRegion(
		nativeRegion, region ? region->nativeRegion : NULL,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 Status Region::Exclude(const RectF& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRect(
		nativeRegion, &rect, CombineModeExclude));
}

 Status Region::Exclude(const Rect& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRectI(
		nativeRegion, &rect, CombineModeExclude));
}

 Status Region::Exclude(const Region* region)
{
	return updateStatus(DLLExports::GdipCombineRegionRegion(
		nativeRegion, region ? region->nativeRegion : NULL,
		CombineModeExclude));
}

 Status Region::Exclude(const GraphicsPath* path)
{
	return updateStatus(DLLExports::GdipCombineRegionPath(
		nativeRegion, path ? path->nativePath : NULL,
		CombineModeExclude));
}

 Status Region::GetBounds(RectF* rect, const Graphics* graphics) const
{
	return updateStatus(DLLExports::GdipGetRegionBounds(nativeRegion,
		graphics ? graphics->nativeGraphics : NULL, rect));
}

/*WASM: NOT SUPPORTED
 Status Region::GetBounds(Rect *rect, const Graphics *graphics) const
{
	return updateStatus(DLLExports::GdipGetRegionBoundsI(nativeRegion,
			graphics ? graphics->nativeGraphics : NULL, rect));
}*/

 Status Region::GetData(BYTE* buffer, UINT bufferSize,
	UINT* sizeFilled) const
{
	return updateStatus(DLLExports::GdipGetRegionData(
		nativeRegion, buffer, bufferSize, sizeFilled));
}

 UINT Region::GetDataSize() const
{
	UINT result = 0;
	updateStatus(DLLExports::GdipGetRegionDataSize(nativeRegion, &result));
	return result;
}

 HRGN Region::GetHRGN(const Graphics* graphics) const
{
	HRGN result = NULL;
	updateStatus(DLLExports::GdipGetRegionHRgn(nativeRegion,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 Status Region::GetRegionScans(const Matrix* matrix,
	RectF* rects, INT* count) const
{
	return updateStatus(DLLExports::GdipGetRegionScans(
		nativeRegion, rects, count,
		matrix ? matrix->nativeMatrix : NULL));
}

 Status Region::GetRegionScans(const Matrix* matrix,
	Rect* rects, INT* count) const
{
	return updateStatus(DLLExports::GdipGetRegionScansI(
		nativeRegion, rects, count,
		matrix ? matrix->nativeMatrix : NULL));
}

 UINT Region::GetRegionScansCount(const Matrix* matrix) const
{
	UINT result = 0;
	updateStatus(DLLExports::GdipGetRegionScansCount(
		nativeRegion, &result,
		matrix ? matrix->nativeMatrix : NULL));
	return result;
}

 Status Region::Intersect(const RectF& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRect(
		nativeRegion, &rect, CombineModeIntersect));
}

 Status Region::Intersect(const Rect& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRectI(
		nativeRegion, &rect, CombineModeIntersect));
}

 Status Region::Intersect(const Region* region)
{
	return updateStatus(DLLExports::GdipCombineRegionRegion(
		nativeRegion, region ? region->nativeRegion : NULL,
		CombineModeIntersect));
}

 Status Region::Intersect(const GraphicsPath* path)
{
	return updateStatus(DLLExports::GdipCombineRegionPath(
		nativeRegion, path ? path->nativePath : NULL,
		CombineModeIntersect));
}

 BOOL Region::IsEmpty(const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsEmptyRegion(nativeRegion,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsInfinite(const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsInfiniteRegion(nativeRegion,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(REAL x, REAL y,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionPoint(
		nativeRegion, x, y,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(INT x, INT y,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionPointI(
		nativeRegion, x, y,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(const PointF& point,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionPoint(
		nativeRegion, point.X, point.Y,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(const Point& point,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionPointI(
		nativeRegion, point.X, point.Y,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(REAL x, REAL y, REAL width, REAL height,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionRect(
		nativeRegion, x, y, width, height,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(INT x, INT y, INT width, INT height,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionRectI(
		nativeRegion, x, y, width, height,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(const RectF& rect,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionRect(
		nativeRegion, rect.X, rect.Y, rect.Width, rect.Height,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 BOOL Region::IsVisible(const Rect& rect,
	const Graphics* graphics) const
{
	BOOL result = FALSE;
	updateStatus(DLLExports::GdipIsVisibleRegionRectI(
		nativeRegion, rect.X, rect.Y, rect.Width, rect.Height,
		graphics ? graphics->nativeGraphics : NULL, &result));
	return result;
}

 Status Region::MakeEmpty()
{
	return updateStatus(DLLExports::GdipSetEmpty(nativeRegion));
}

 Status Region::MakeInfinite()
{
	return updateStatus(DLLExports::GdipSetInfinite(nativeRegion));
}

 Status Region::Transform(const Matrix* matrix)
{
	return updateStatus(DLLExports::GdipTransformRegion(
		nativeRegion, matrix ? matrix->nativeMatrix : NULL));
}

 Status Region::Translate(REAL dx, REAL dy)
{
	return updateStatus(DLLExports::GdipTranslateRegion(
		nativeRegion, dx, dy));
}

 Status Region::Translate(INT dx, INT dy)
{
	return updateStatus(DLLExports::GdipTranslateRegionI(
		nativeRegion, dx, dy));
}

 Status Region::Union(const RectF& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRect(
		nativeRegion, &rect, CombineModeUnion));
}

 Status Region::Union(const Rect& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRectI(
		nativeRegion, &rect, CombineModeUnion));
}

 Status Region::Union(const Region* region)
{
	return updateStatus(DLLExports::GdipCombineRegionRegion(
		nativeRegion, region ? region->nativeRegion : NULL,
		CombineModeUnion));
}

 Status Region::Union(const GraphicsPath* path)
{
	return updateStatus(DLLExports::GdipCombineRegionPath(
		nativeRegion, path ? path->nativePath : NULL,
		CombineModeUnion));
}

 Status Region::Xor(const RectF& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRect(
		nativeRegion, &rect, CombineModeXor));
}

 Status Region::Xor(const Rect& rect)
{
	return updateStatus(DLLExports::GdipCombineRegionRectI(
		nativeRegion, &rect, CombineModeXor));
}

 Status Region::Xor(const Region* region)
{
	return updateStatus(DLLExports::GdipCombineRegionRegion(
		nativeRegion, region ? region->nativeRegion : NULL,
		CombineModeXor));
}

 Status Region::Xor(const GraphicsPath* path)
{
	return updateStatus(DLLExports::GdipCombineRegionPath(
		nativeRegion, path ? path->nativePath : NULL,
		CombineModeXor));
}
}
