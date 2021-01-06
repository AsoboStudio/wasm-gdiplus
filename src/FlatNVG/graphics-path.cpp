/*
 * graphics-path.cpp
 *
 * Copyright (C) 2003-2007, Novell Inc. (http://www.novell.com)
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
 *      Duncan Mak (duncan@ximian.com)
 *      Ravindra (rkumar@novell.com)
 *	    Sebastien Pouliot  <sebastien@ximian.com> 
 *		Sol Roo (sroo@asobostudio.com)
 *
 */
#include "FlatNVG\general-private.h"
#include "FlatNVG\graphics-path-private.h"
#include "FlatNVG\matrix-private.h"
#include "FlatNVG\font-private.h"
#include "FlatNVG\graphics-cairo-private.h"
#include "FlatNVG\fontfamily.h"
#include <math.h>

// TODO : clean
#define memcpy(s,d,size) wmemcpy((wchar_t*)s,(wchar_t*)d, size*sizeof(wchar_t)/sizeof(BYTE))
#define memmove(s,d,size) wmemmove((wchar_t*)s,(wchar_t*)d, size*sizeof(wchar_t)/sizeof(BYTE))
BOOL gdiplusInitialized = true;

BOOL
gdip_path_ensure_size(GpPath* path, int size)
{
	BYTE* new_types;
	GpPointF* new_points;

	if (path->size < size) {
		if (size < path->size + 64)
			size = path->size + 64;
		new_types = (BYTE*)gdip_realloc(path->types, size * sizeof(BYTE));
		if (!new_types)
			return FALSE;
		path->types = new_types;
		new_points = (GpPointF*)gdip_realloc(path->points, size * sizeof(GpPointF));
		if (!new_points)
			return FALSE;
		path->points = new_points;
		path->size = size;
	}

	return TRUE;
}

/* return TRUE if the specified path has (at least one) curves, FALSE otherwise */
BOOL
gdip_path_has_curve(GpPath* path)
{
	int i;

	if (!path)
		return FALSE;

	for (i = 0; i < path->count; i++) {
		if (path->types[i] == GpPathPointType::PathPointTypeBezier)
			return TRUE;
	}

	return FALSE;
}

BOOL
gdip_path_closed(GpPath* path)
{
	if (path->count < 1) {
		return FALSE;
	}

	GpPathPointType last_type = (GpPathPointType)path->types[path->count - 1];
	return (last_type & GpPathPointType::PathPointTypeCloseSubpath) == GpPathPointType::PathPointTypeCloseSubpath;
}

/*
 * Return the correct point type when adding a new shape to the path.
 */
static  GpPathPointType
gdip_get_first_point_type(GpPath* path)
{
	GpPathPointType type;

	//check for a new figure flag or an empty path s
	if (path->start_new_fig || (path->count == 0))
		return GpPathPointType::PathPointTypeStart;

	// check if the previous point is a closure
	type = (GpPathPointType)path->types[path->count - 1];
	if (type & GpPathPointType::PathPointTypeCloseSubpath)
		return GpPathPointType::PathPointTypeStart;
	else
		return GpPathPointType::PathPointTypeLine;
}

static VOID
append(GpPath* path, float x, float y, GpPathPointType type, BOOL compress)
{
	BYTE t = (BYTE)type;
	GpPointF pt;

	if (path->start_new_fig) {
		t = GpPathPointType::PathPointTypeStart;
	}
	else if (path->count > 0) {
		/* in some case we're allowed to compress identical points */
		if (compress) {
			/* points (X, Y) must be identical */
			GpPointF lastPoint = path->points[path->count - 1];
			if ((lastPoint.X == x) && (lastPoint.Y == y)) {
				/* types need not be identical but must handle closed subpaths */
				if (!gdip_path_closed(path)) {
					if ((type & GpPathPointType::PathPointTypeCloseSubpath) == GpPathPointType::PathPointTypeCloseSubpath)
						path->types[path->count - 1] |= GpPathPointType::PathPointTypeCloseSubpath;
					return;
				}
			}
		}

		/* if we closed a subpath, then start new figure and append */
		type = (GpPathPointType)path->types[path->count - 1];
		if (type & GpPathPointType::PathPointTypeCloseSubpath)
			t = GpPathPointType::PathPointTypeStart;
	}

	pt.X = x;
	pt.Y = y;

	/* all external APIs resize the buffers beforehand and fail gracefully */
	/* TODO
	if (!gdip_path_ensure_size(path, path->count + 1))
		g_assert(FALSE);
	*/
	path->points[path->count] = pt;
	path->types[path->count] = t;
	path->count++;
	path->start_new_fig = FALSE;
}

static VOID
append_point(GpPath* path, GpPointF pt, GpPathPointType type, BOOL compress)
{
	append(path, pt.X, pt.Y, type, compress);
}

static VOID
append_bezier(GpPath* path, float x1, float y1, float x2, float y2, float x3, float y3)
{
	append(path, x1, y1, GpPathPointType::PathPointTypeBezier3, FALSE);
	append(path, x2, y2, GpPathPointType::PathPointTypeBezier3, FALSE);
	append(path, x3, y3, GpPathPointType::PathPointTypeBezier3, FALSE);
}

static VOID
append_curve(GpPath* path, const GpPointF* points, GpPointF* tangents, int offset, int length, _CurveType type)
{
	int i;
	GpPathPointType ptype = ((type == CURVE_CLOSE) || (path->count == 0)) ? GpPathPointType::PathPointTypeStart : GpPathPointType::PathPointTypeLine;

	append_point(path, points[offset], ptype, TRUE);
	for (i = offset; i < offset + length; i++) {
		int j = i + 1;

		double x1 = points[i].X + tangents[i].X;
		double y1 = points[i].Y + tangents[i].Y;

		double x2 = points[j].X - tangents[j].X;
		double y2 = points[j].Y - tangents[j].Y;

		double x3 = points[j].X;
		double y3 = points[j].Y;

		append_bezier(path, x1, y1, x2, y2, x3, y3);
	}

	if (type == CURVE_CLOSE) {
		/* complete (close) the curve using the first point */
		double x1 = points[i].X + tangents[i].X;
		double y1 = points[i].Y + tangents[i].Y;

		double x2 = points[0].X - tangents[0].X;
		double y2 = points[0].Y - tangents[0].Y;

		double x3 = points[0].X;
		double y3 = points[0].Y;

		append_bezier(path, x1, y1, x2, y2, x3, y3);
		DLLExports::GdipClosePathFigure(path);
	}
}

static BOOL
gdip_validate_path_types(const BYTE* types, INT count)
{
	const BYTE* currentType = types;
	INT remaining = count;

	if (count == 1)
		return TRUE;

	while (TRUE) {
		remaining--;
		currentType++;

		// No more.
		if (remaining == 0)
			return FALSE;

		// Already started.
		if ((*currentType & GpPathPointType::PathPointTypePathTypeMask) == GpPathPointType::PathPointTypeStart)
			return FALSE;

		do {
			if ((*currentType & GpPathPointType::PathPointTypePathTypeMask) == GpPathPointType::PathPointTypeLine) {
				// No validation needed for lines. Move to the next type.
			}
			else if ((*currentType & GpPathPointType::PathPointTypePathTypeMask) == GpPathPointType::PathPointTypeBezier) {
				// A bezier path requires a start point (already handled) followed by 3 bezier
				// points.
				remaining--;
				currentType++;
				if (remaining == 0 || (*currentType & GpPathPointType::PathPointTypePathTypeMask) != GpPathPointType::PathPointTypeBezier)
					return FALSE;

				remaining--;
				currentType++;
				if (remaining == 0 || (*currentType & GpPathPointType::PathPointTypePathTypeMask) != GpPathPointType::PathPointTypeBezier)
					return FALSE;
			}
			else {
				// Unknown type.
				return FALSE;
			}

			BOOL endOfPath = (*currentType & GpPathPointType::PathPointTypeCloseSubpath) != 0;
			remaining--;
			currentType++;

			// End of an open path. There are no more subpaths.
			if (remaining == 0)
				return TRUE;
			// End of a closed path. There may be additional subpaths.
			if (endOfPath)
				break;
		} while ((*currentType & GpPathPointType::PathPointTypePathTypeMask) != GpPathPointType::PathPointTypeStart);
	}

	return TRUE;
}

static VOID
append_arc(GpPath* path, BOOL start, float x, float y, float width, float height, float startAngle, float endAngle)
{
	float delta, bcp;
	double sin_alpha, sin_beta, cos_alpha, cos_beta;

	float rx = width / 2;
	float ry = height / 2;

	/* center */
	float cx = x + rx;
	float cy = y + ry;

	/* angles in radians */
	float alpha = startAngle * PI / 180;
	float beta = endAngle * PI / 180;

	/* adjust angles for ellipses */
	alpha = atan2(rx * sin(alpha), ry * cos(alpha));
	beta = atan2(rx * sin(beta), ry * cos(beta));

	if (fabs(beta - alpha) > M_PI) {
		if (beta > alpha)
			beta -= 2 * PI;
		else
			alpha -= 2 * PI;
	}

	delta = beta - alpha;
	// http://www.stillhq.com/ctpfaq/2001/comp.text.pdf-faq-2001-04.txt (section 2.13)
	bcp = 4.0 / 3 * (1 - cos(delta / 2)) / sin(delta / 2);

	sin_alpha = sin(alpha);
	sin_beta = sin(beta);
	cos_alpha = cos(alpha);
	cos_beta = cos(beta);

	/* move to the starting point if we're not continuing a curve */
	if (start) {
		/* starting point */
		double sx = cx + rx * cos_alpha;
		double sy = cy + ry * sin_alpha;
		append(path, sx, sy, GpPathPointType::PathPointTypeLine, FALSE);
	}

	append_bezier(path,
		cx + rx * (cos_alpha - bcp * sin_alpha),
		cy + ry * (sin_alpha + bcp * cos_alpha),
		cx + rx * (cos_beta + bcp * sin_beta),
		cy + ry * (sin_beta - bcp * cos_beta),
		cx + rx * cos_beta,
		cy + ry * sin_beta);
}

static int
count_arcs_points(GpPath* path, float x, float y, float width, float height, float startAngle, float sweepAngle)
{
	int i;
	float drawn = 0;
	int increment;
	float endAngle;
	int count = 1;

	if (fabs(sweepAngle) >= 360)
		return 13;

	endAngle = startAngle + sweepAngle;
	increment = (endAngle < startAngle) ? -90 : 90;

	/* i is the number of sub-arcs drawn, each sub-arc can be at most 90 degrees.*/
	/* there can be no more then 4 subarcs, ie. 90 + 90 + 90 + (something less than 90) */
	for (i = 0; i < 4; i++) {
		float current = startAngle + drawn;
		float additional;

		additional = endAngle - current; /* otherwise, add the remainder */
		if (fabs(additional) > 90) {
			additional = increment;
		}
		else {
			/* a near zero value will introduce bad artefact in the drawing (#78999) */
			if (gdip_near_zero(additional))
				return count;
			count += 3;
			return count;
		}

		count += 3;
		drawn += additional;
	}

	return count;
}


static VOID
append_arcs(GpPath* path, float x, float y, float width, float height, float startAngle, float sweepAngle)
{
	int i;
	float drawn = 0;
	int increment;
	float endAngle;
	BOOL enough = FALSE;

	if (fabs(sweepAngle) >= 360) {
		DLLExports::GdipAddPathEllipse(path, x, y, width, height);
		return;
	}

	endAngle = startAngle + sweepAngle;
	increment = (endAngle < startAngle) ? -90 : 90;

	/* i is the number of sub-arcs drawn, each sub-arc can be at most 90 degrees.*/
	/* there can be no more then 4 subarcs, ie. 90 + 90 + 90 + (something less than 90) */
	for (i = 0; i < 4; i++) {
		float current = startAngle + drawn;
		float additional;

		if (enough)
			return;

		additional = endAngle - current; /* otherwise, add the remainder */
		if (fabs(additional) > 90) {
			additional = increment;
		}
		else {
			/* a near zero value will introduce bad artefact in the drawing (#78999) */
			if (gdip_near_zero(additional))
				return;

			enough = TRUE;
		}

		append_arc(path, (i == 0),           /* only move to the starting pt in the 1st iteration */
			x, y, width, height,      /* bounding rectangle */
			current, current + additional);
		drawn += additional;
	}
}
static GpStatus
gdip_prepare_path(GpPath* path, GpMatrix* matrix, float flatness)
{
	/* convert any curve into lines */
	if (gdip_path_has_curve(path)) {
		/* this will apply the matrix too (before flattening) */
		return DLLExports::GdipFlattenPath(path, matrix, flatness);
	}
	else if (!gdip_is_matrix_empty(matrix)) {
		/* no curve, but we still have a matrix to apply... */
		return DLLExports::GdipTransformPath(path, matrix);
	}

	/* no preparation required */
	return GpStatus::Ok;
}

void gdip_apply_path(GpGraphics* graphics, GpPath* path) {

	int iPointIdx = 0;
	for (int pathEntry = 0; pathEntry < path->count; ++pathEntry)
	{
		int type = path->types[pathEntry];
		if (type == GpPathPointType::PathPointTypeStart)
		{
			nvgMoveTo(graphics->ctx, path->points[iPointIdx].X, path->points[iPointIdx].Y);
			++iPointIdx;
		}

		if ((type & GpPathPointType::PathPointTypeLine) == GpPathPointType::PathPointTypeLine)
		{
			nvgLineTo(graphics->ctx, path->points[iPointIdx].X, path->points[iPointIdx].Y);
			++iPointIdx;
		}
		else if ((type & GpPathPointType::PathPointTypeBezier3) == GpPathPointType::PathPointTypeBezier3)
		{
			nvgBezierTo(graphics->ctx, path->points[iPointIdx].X, path->points[iPointIdx].Y, path->points[iPointIdx + 1].X, path->points[iPointIdx + 1].Y, path->points[iPointIdx + 2].X, path->points[iPointIdx + 2].Y);
			iPointIdx += 3;
			pathEntry += 2; //skip the 2 points used for the bezier
		}
		/*
		else if ((type & GpPathPointType::PathPointTypePathTypeMask) == GpPathPointType::PathPointTypePathTypeMask)
		{
			// not supported
		}
		else if ((type & GpPathPointType::PathPointTypeDashMode) == GpPathPointType::PathPointTypeDashMode)
		{
			// not supported
		}
		else if ((type & GpPathPointType::PathPointTypePathMarker) == GpPathPointType::PathPointTypePathMarker)
		{
			// not supported
		}*/

		if ((type & GpPathPointType::PathPointTypeCloseSubpath) == GpPathPointType::PathPointTypeCloseSubpath)
		{
			nvgClosePath(graphics->ctx);
		}
	}
}
namespace DLLExports {


	/* coverity[+alloc : arg-*1] */
	GpStatus WINGDIPAPI
		GdipCreatePath(GpFillMode fillMode, GpPath** path)
	{
		GpPath* result;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;

#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		result = (GpPath*)GdipAlloc(sizeof(GpPath));
		if (!result)
			return GpStatus::OutOfMemory;

		result->fill_mode = fillMode;
		result->size = 0;
		result->points = NULL;
		result->types = NULL;
		result->count = 0;
		result->start_new_fig = TRUE;

		*path = result;
		return GpStatus::Ok;
	}

	/* coverity[+alloc : arg-*4] */
	GpStatus WINGDIPAPI
		GdipCreatePath2(GDIPCONST GpPointF* points, GDIPCONST BYTE* types, INT count, GpFillMode fillMode, GpPath** path)
	{
		GpPath* result;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;
#ifdef GDIFLAT_SAFE
		if (!path || !points || !types)
			return GpStatus::InvalidParameter;
#endif
		if (count <= 0 || fillMode > GpFillMode::FillModeWinding)
			return GpStatus::OutOfMemory;

		// Match GDI+ behaviour and set the path to empty if it is invalid.
		if (!gdip_validate_path_types(types, count))
			return GdipCreatePath(fillMode, path);

		result = (GpPath*)GdipAlloc(sizeof(GpPath));
		if (!result)
			return GpStatus::OutOfMemory;

		result->fill_mode = fillMode;
		result->count = count;
		result->size = (count + 63) & ~63;
		result->points = (GpPointF*)GdipAlloc(sizeof(GpPointF) * result->size);
		if (!result->points) {
			GdipFree(result);
			return GpStatus::OutOfMemory;
		}

		result->types = (BYTE*)GdipAlloc(sizeof(BYTE) * result->size);
		if (!result->types) {
			GdipFree(result->points);
			GdipFree(result);
			return GpStatus::OutOfMemory;
		}

		memcpy(result->points, points, sizeof(GpPointF) * count);
		memcpy(result->types, types, sizeof(BYTE) * count);

		// Match GDI+ behaviour by normalizing the start of the type array.
		result->types[0] = GpPathPointType::PathPointTypeStart;

		*path = result;
		return GpStatus::Ok;
	}

	/* coverity[+alloc : arg-*4] */
	GpStatus WINGDIPAPI
		GdipCreatePath2I(GDIPCONST GpPoint* points, GDIPCONST BYTE* types, INT count, GpFillMode fillMode, GpPath** path)
	{
		GpPointF* pt;
		GpStatus s;

		if (!gdiplusInitialized)
			return GpStatus::GdiplusNotInitialized;
#ifdef GDIFLAT_SAFE
		if (!points || !types || !path)
			return GpStatus::InvalidParameter;
#endif
		if (count < 0)
			return GpStatus::OutOfMemory;

		pt = convert_points(points, count);
		if (!pt)
			return GpStatus::OutOfMemory;

		s = GdipCreatePath2(pt, types, count, fillMode, path);

		GdipFree(pt);

		return s;
	}

	/* coverity[+alloc : arg-*1] */
	GpStatus WINGDIPAPI
		GdipClonePath(GpPath* path, GpPath** clonePath)
	{
		GpPath* result;
#ifdef GDIFLAT_SAFE
		if (!path || !clonePath)
			return GpStatus::InvalidParameter;
#endif
		result = (GpPath*)GdipAlloc(sizeof(GpPath));
		if (!result)
			return GpStatus::OutOfMemory;

		result->fill_mode = path->fill_mode;
		result->count = path->count;
		result->size = path->size;

		if (path->points) {
			result->points = (GpPointF*)GdipAlloc(sizeof(GpPointF) * result->size);
			if (!result->points) {
				GdipFree(result);
				return GpStatus::OutOfMemory;
			}

			memcpy(result->points, path->points, sizeof(GpPointF) * path->count);
		}
		else {
			result->points = NULL;
		}

		if (path->types) {
			result->types = (BYTE*)GdipAlloc(sizeof(BYTE) * result->size);
			if (!result->types) {
				GdipFree(result->points);
				GdipFree(result);
				return GpStatus::OutOfMemory;
			}

			memcpy(result->types, path->types, sizeof(BYTE) * path->count);
		}
		else {
			result->types = NULL;
		}

		result->start_new_fig = path->start_new_fig;

		*clonePath = result;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDeletePath(GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (path == NULL)
			return GpStatus::InvalidParameter;
#endif
		if (path->points != NULL)
			GdipFree(path->points);
		path->points = NULL;

		if (path->types != NULL)
			GdipFree(path->types);
		path->types = NULL;

		GdipFree(path);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipResetPath(GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (path == NULL)
			return GpStatus::InvalidParameter;
#endif
		path->count = 0;
		path->fill_mode = GpFillMode::FillModeAlternate;
		path->start_new_fig = TRUE;
		path->size = 0;
		if (path->points != NULL)
			GdipFree(path->points);
		if (path->types != NULL)
			GdipFree(path->types);
		path->points = NULL;
		path->types = NULL;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPointCount(GpPath* path, int* count)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !count)
			return GpStatus::InvalidParameter;
#endif
		*count = path->count;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPathTypes(GpPath* path, BYTE* types, INT count)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !types || count <= 0)
			return GpStatus::InvalidParameter;
#endif
		if (count < path->count)
			return GpStatus::InsufficientBuffer;

		memcpy(types, path->types, path->count * sizeof(BYTE));
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPathPoints(GpPath* path, GpPointF* points, int count)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !points || count <= 0)
			return GpStatus::InvalidParameter;
#endif
		if (count < path->count)
			return GpStatus::InsufficientBuffer;

		memcpy(points, path->points, path->count * sizeof(GpPointF));
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPathPointsI(GpPath* path, GpPoint* points, int count)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !points || count <= 0)
			return GpStatus::InvalidParameter;
#endif
		if (count < path->count)
			return GpStatus::InsufficientBuffer;

		for (int i = 0; i < path->count; i++)
			gdip_Point_from_PointF(path->points + i, points + i);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPathFillMode(GpPath* path, GpFillMode* fillMode)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !fillMode)
			return GpStatus::InvalidParameter;
#endif
		*fillMode = path->fill_mode;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPathFillMode(GpPath* path, GpFillMode fillMode)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		path->fill_mode = fillMode;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPathData(GpPath* path, GpPathData* pathData)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !pathData || !pathData->Points || !pathData->Types || pathData->Count < 0)
			return GpStatus::InvalidParameter;
#endif
		if (pathData->Count < path->count)
			return GpStatus::OutOfMemory;

		memcpy(pathData->Points, path->points, path->count * sizeof(GpPointF));
		memcpy(pathData->Types, path->types, path->count * sizeof(BYTE));
		pathData->Count = path->count;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipStartPathFigure(GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		path->start_new_fig = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipClosePathFigure(GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		// Close the last figure.
		if (path->count > 1)
			path->types[path->count - 1] |= GpPathPointType::PathPointTypeCloseSubpath;

		// Start a new figure.
		path->start_new_fig = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipClosePathFigures(GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		if (path->count > 1) {
			// Close the last figure.
			path->types[path->count - 1] |= GpPathPointType::PathPointTypeCloseSubpath;

			// Close each open figure.
			for (int index = 1; index < path->count; index++) {
				if (path->types[index] == GpPathPointType::PathPointTypeStart) {
					path->types[index - 1] |= GpPathPointType::PathPointTypeCloseSubpath;
				}
			}
		}

		// Start a new figure.
		path->start_new_fig = TRUE;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipSetPathMarker(GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		if (path->count > 1)
			path->types[path->count - 1] |= GpPathPointType::PathPointTypePathMarker;

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipClearPathMarkers(GpPath* path)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		for (int i = 0; i < path->count; i++)
			path->types[i] &= ~GpPathPointType::PathPointTypePathMarker;

		return GpStatus::Ok;
	}

	/*
	 * Append old_types[start, end] to new_types, adjusting flags.
	 */
	static void
		reverse_subpath_adjust_flags(int start, int end, BYTE* types, BOOL* prev_had_marker)
	{
		BYTE prev_last;

		/* Copy all but GpPathPointType::PathPointTypeStart */
		if (end != start)
			memmove(types + start, types + start + 1, (end - start) * sizeof(BYTE));

		/* Append GpPathPointType::PathPointTypeStart */
		prev_last = types[end];
		types[end] = GpPathPointType::PathPointTypeStart;

		/* Remove potential flags from our future start point */
		if (end != start)
			types[end - 1] &= GpPathPointType::PathPointTypePathTypeMask;
		/* Set the flags on our to-be-last point */
		types[start] |= prev_last & (GpPathPointType::PathPointTypeDashMode | GpPathPointType::PathPointTypeCloseSubpath);

		/* If the last point of the previous subpath had a marker, we inherit it */
		if (*prev_had_marker)
			types[start] |= GpPathPointType::PathPointTypePathMarker;
		else
			types[start] &= ~GpPathPointType::PathPointTypePathMarker;

		*prev_had_marker = ((prev_last & GpPathPointType::PathPointTypePathMarker) == GpPathPointType::PathPointTypePathMarker);
	}

	GpStatus WINGDIPAPI
		GdipReversePath(GpPath* path)
	{
		int length, i;
		int start = 0;
		BOOL prev_had_marker = FALSE;

#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		length = path->count;
		/* shortcut */
		if (length <= 1)
			return GpStatus::Ok;

		/* PathTypes reversal */

		/* First adjust the flags for each subpath */
		for (i = 1; i < length; i++) {
			BYTE t = path->types[i];
			if ((t & GpPathPointType::PathPointTypePathTypeMask) == GpPathPointType::PathPointTypeStart) {
				reverse_subpath_adjust_flags(start, i - 1, path->types, &prev_had_marker);
				start = i;
			}
		}
		if (start < length - 1)
			reverse_subpath_adjust_flags(start, length - 1, path->types, &prev_had_marker);

		/* Then reverse the resulting array */
		for (i = 0; i < (length >> 1); i++) {
			BYTE* a = path->types + i;
			BYTE* b = path->types + length - i - 1;
			BYTE temp = *a;
			*a = *b;
			*b = temp;
		}

		/* PathPoints reversal
		 * note: if length is odd then the middle point doesn't need to switch side
		 */
		for (i = 0; i < (length >> 1); i++) {
			GpPointF* first = path->points + i;
			GpPointF* last = path->points + length - i - 1;

			GpPointF temp;
			temp.X = first->X;
			temp.Y = first->Y;
			first->X = last->X;
			first->Y = last->Y;
			last->X = temp.X;
			last->Y = temp.Y;
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPathLastPoint(GpPath* path, GpPointF* lastPoint)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !lastPoint || (path->count <= 0))
			return GpStatus::InvalidParameter;
#endif
		*lastPoint = path->points[path->count - 1];
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathLine(GpPath* path, float x1, float y1, float x2, float y2)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + 2))
			return GpStatus::OutOfMemory;

		/* only the first point can be compressed (i.e. removed if identical to previous) */
		append(path, x1, y1, GpPathPointType::PathPointTypeLine, TRUE);
		append(path, x2, y2, GpPathPointType::PathPointTypeLine, FALSE);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathLine2(GpPath* path, const GpPointF* points, int count)
	{
		int i;
		GpPointF* tmp;

#ifdef GDIFLAT_SAFE
		if (!path || !points || (count < 0))
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + count))
			return GpStatus::OutOfMemory;

		/* only the first point can be compressed (i.e. removed if identical to previous) */
		for (i = 0, tmp = (GpPointF*)points; i < count; i++, tmp++)
			append(path, tmp->X, tmp->Y, GpPathPointType::PathPointTypeLine, (i == 0));

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathArc(GpPath* path, REAL x, REAL y,
			REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
		int point_count;
#ifdef GDIFLAT_SAFE
		if (!path || width <= 0 || height <= 0)
			return GpStatus::InvalidParameter;
#endif
		point_count = count_arcs_points(path, x, y, width, height, startAngle, sweepAngle);
		if (!gdip_path_ensure_size(path, path->count + point_count))
			return GpStatus::OutOfMemory;

		/* draw the arcs */
		append_arcs(path, x, y, width, height, startAngle, sweepAngle);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathBezier(GpPath* path,
			float x1, float y1, float x2, float y2,
			float x3, float y3, float x4, float y4)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + 4))
			return GpStatus::OutOfMemory;

		append(path, x1, y1, GpPathPointType::PathPointTypeLine, TRUE);
		append_bezier(path, x2, y2, x3, y3, x4, y4);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathBeziers(GpPath* path, const GpPointF* points, int count)
	{
		int i;
		GpPointF* tmp = (GpPointF*)points;

#ifdef GDIFLAT_SAFE
		if (!path || !points)
			return GpStatus::InvalidParameter;
#endif
		/* first bezier requires 4 points, other 3 more points */
		if ((count < 4) || ((count % 3) != 1))
			return GpStatus::InvalidParameter;

		if (!gdip_path_ensure_size(path, path->count + count))
			return GpStatus::OutOfMemory;

		append_point(path, *tmp, GpPathPointType::PathPointTypeLine, TRUE);
		tmp++;

		for (i = 1; i < count; i++, tmp++)
			append_point(path, *tmp, GpPathPointType::PathPointTypeBezier3, FALSE);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathCurve(GpPath* path, const GpPointF* points, int count)
	{
		return GdipAddPathCurve2(path, points, count, 0.5);
	}

	GpStatus WINGDIPAPI
		GdipAddPathCurve2(GpPath* path, const GpPointF* points, int count, float tension)
	{
		GpPointF* tangents;
#ifdef GDIFLAT_SAFE
		/* special case, here we support a curve with 2 points */
		if (!path || !points || (count < 2))
			return GpStatus::InvalidParameter;
#endif
		tangents = gdip_open_curve_tangents(CURVE_MIN_TERMS, points, count, tension);
		if (!tangents)
			return GpStatus::OutOfMemory;

		if (!gdip_path_ensure_size(path, path->count + (3 * (count - 1)) + 1)) {
			GdipFree(tangents);
			return GpStatus::OutOfMemory;
		}

		append_curve(path, points, tangents, 0, count - 1, CURVE_OPEN);

		GdipFree(tangents);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathCurve3(GpPath* path, const GpPointF* points, int count,
			int offset, int numberOfSegments, float tension)
	{
		GpPointF* tangents;
#ifdef GDIFLAT_SAFE
		if (!path || !points || (numberOfSegments < 1))
			return GpStatus::InvalidParameter;
#endif
		/* we need 3 points for the first curve, 2 more for each curves */
		/* and it's possible to use a point prior to the offset (to calculate) */
		if ((offset == 0) && (numberOfSegments == 1) && (count < 3))
			return GpStatus::InvalidParameter;
		else if (numberOfSegments >= count - offset)
			return GpStatus::InvalidParameter;

		tangents = gdip_open_curve_tangents(CURVE_MIN_TERMS, points, count, tension);
		if (!tangents)
			return GpStatus::OutOfMemory;

		if (!gdip_path_ensure_size(path, path->count + (3 * numberOfSegments) + 1)) {
			GdipFree(tangents);
			return GpStatus::OutOfMemory;
		}

		append_curve(path, points, tangents, offset, numberOfSegments, CURVE_OPEN);

		GdipFree(tangents);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathClosedCurve(GpPath* path, const GpPointF* points, int count)
	{
		return GdipAddPathClosedCurve2(path, points, count, 0.5);
	}

	GpStatus WINGDIPAPI
		GdipAddPathClosedCurve2(GpPath* path, const GpPointF* points, int count, float tension)
	{
		GpPointF* tangents;
#ifdef GDIFLAT_SAFE
		if (!path || !points || (count < 3))
			return GpStatus::InvalidParameter;
#endif
		tangents = gdip_closed_curve_tangents(CURVE_MIN_TERMS, points, count, tension);
		if (!tangents)
			return GpStatus::OutOfMemory;

		if (!gdip_path_ensure_size(path, path->count + (3 * count) + 1)) {
			GdipFree(tangents);
			return GpStatus::OutOfMemory;
		}

		append_curve(path, points, tangents, 0, count - 1, CURVE_CLOSE);

		/* close the path */
		GdipClosePathFigure(path);
		GdipFree(tangents);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathRectangle(GpPath* path, float x, float y, float width, float height)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		if ((width <= 0.0) || (height <= 0.0))
			return GpStatus::Ok;

		if (!gdip_path_ensure_size(path, path->count + 4))
			return GpStatus::OutOfMemory;

		append(path, x, y, GpPathPointType::PathPointTypeStart, FALSE);
		append(path, x + width, y, GpPathPointType::PathPointTypeLine, FALSE);
		append(path, x + width, y + height, GpPathPointType::PathPointTypeLine, FALSE);
		append(path, x, y + height, (GpPathPointType)(GpPathPointType::PathPointTypeLine | GpPathPointType::PathPointTypeCloseSubpath), FALSE);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathRectangles(GpPath* path, const GpRectF* rects, int count)
	{
		int i;
#ifdef GDIFLAT_SAFE
		if (!path || !rects)
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + (4 * count)))
			return GpStatus::OutOfMemory;

		for (i = 0; i < count; i++) {
			float x = rects[i].X;
			float y = rects[i].Y;
			float width = rects[i].Width;
			float height = rects[i].Height;
			GdipAddPathRectangle(path, x, y, width, height);
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathEllipse(GpPath* path, float x, float y, float width, float height)
	{
		double rx = width / 2;
		double ry = height / 2;
		double cx = x + rx;
		double cy = y + ry;
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + 13))
			return GpStatus::OutOfMemory;

		/* origin */
		append(path, cx + rx, cy, GpPathPointType::PathPointTypeStart, FALSE);

		/* quadrant I */
		append_bezier(path,
			cx + rx, cy - C1 * ry,
			cx + C1 * rx, cy - ry,
			cx, cy - ry);

		/* quadrant II */
		append_bezier(path,
			cx - C1 * rx, cy - ry,
			cx - rx, cy - C1 * ry,
			cx - rx, cy);

		/* quadrant III */
		append_bezier(path,
			cx - rx, cy + C1 * ry,
			cx - C1 * rx, cy + ry,
			cx, cy + ry);

		/* quadrant IV */
		append_bezier(path,
			cx + C1 * rx, cy + ry,
			cx + rx, cy + C1 * ry,
			cx + rx, cy);

		/* close the path */
		GdipClosePathFigure(path);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathPie(GpPath* path, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
		int point_count;

		float sin_alpha, cos_alpha;
#ifdef GDIFLAT_SAFE
		if (!path || width <= 0 || height <= 0)
			return GpStatus::InvalidParameter;
#endif
		float rx = width / 2;
		float ry = height / 2;

		/* center */
		int cx = x + rx;
		int cy = y + ry;

		/* angles in radians */
		float alpha = startAngle * PI / 180;

		/* adjust angle for ellipses */
		alpha = atan2(rx * sin(alpha), ry * cos(alpha));

		sin_alpha = sin(alpha);
		cos_alpha = cos(alpha);

		point_count = count_arcs_points(path, x, y, width, height, startAngle, sweepAngle) + 1;
		if (fabs(sweepAngle) < 360)
			point_count += 2;
		if (!gdip_path_ensure_size(path, path->count + point_count))
			return GpStatus::OutOfMemory;

		/* move to center */
		append(path, cx, cy, GpPathPointType::PathPointTypeStart, FALSE);

		/* draw pie edge */
		if (fabs(sweepAngle) < 360)
			append(path, cx + rx * cos_alpha, cy + ry * sin_alpha, GpPathPointType::PathPointTypeLine, FALSE);

		/* draw the arcs */
		append_arcs(path, x, y, width, height, startAngle, sweepAngle);

		/* draw pie edge */
		if (fabs(sweepAngle) < 360)
			append(path, cx, cy, GpPathPointType::PathPointTypeLine, FALSE);

		/* close the path */
		return GdipClosePathFigure(path);
	}

	GpStatus WINGDIPAPI
		GdipAddPathPolygon(GpPath* path, const GpPointF* points, int count)
	{
		int i;
		GpPointF* tmp = (GpPointF*)points;
#ifdef GDIFLAT_SAFE
		if (!path || !points || (count < 3))
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + count + 1))
			return GpStatus::OutOfMemory;

		/* note: polygon points are never compressed (i.e. removed if identical) */

		append_point(path, *tmp, GpPathPointType::PathPointTypeStart, FALSE);
		tmp++;

		for (i = 1; i < count; i++, tmp++)
			append_point(path, *tmp, GpPathPointType::PathPointTypeLine, FALSE);

		/*
		 * Add a line from the last point back to the first point if
		 * they're not the same
		 */
		if (points[0].X != points[count - 1].X && points[0].Y != points[count - 1].Y)
			append_point(path, points[0], GpPathPointType::PathPointTypeLine, FALSE);

		/* close the path */
		return GdipClosePathFigure(path);
	}

	GpStatus WINGDIPAPI
		GdipAddPathPath(GpPath* path, GDIPCONST GpPath* addingPath, BOOL connect)
	{
#ifdef GDIFLAT_SAFE
		if (!path || !addingPath)
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + addingPath->count))
			return GpStatus::OutOfMemory;

		memcpy(path->types + path->count, addingPath->types, sizeof(BYTE) * addingPath->count);
		memcpy(path->points + path->count, addingPath->points, sizeof(GpPointF) * addingPath->count);

		/* We can connect only open figures. If first figure is closed
		 * it can't be connected.
		 */
		path->types[path->count] = connect ? gdip_get_first_point_type(path) : GpPathPointType::PathPointTypeStart;
		path->start_new_fig = FALSE;
		path->count += addingPath->count;

		return GpStatus::Ok;
	}

	/* MonoTODO - deal with layoutRect, format... */
	GpStatus WINGDIPAPI
		GdipAddPathString(GpPath* path, GDIPCONST WCHAR* string, int length,
			GDIPCONST GpFontFamily* family, int style, float emSize,
			GDIPCONST GpRectF* layoutRect, GDIPCONST GpStringFormat* format)
	{
		return GpStatus::NotImplemented;
#if 0
		cairo_surface_t* cs;
		cairo_t* cr;
		cairo_path_t* cp;
		GpFont* font = NULL;
		GpStatus status;
		BYTE* utf8 = NULL;

		if (!path || !string || length < -1 || !family || !layoutRect)
			return GpStatus::InvalidParameter;

		if (length == 0) {
			return GpStatus::Ok;
		}
		else if (length == -1) {
			const WCHAR* ptr = string;
			length = 0;
			while (*ptr != 0) {
				length++;
				ptr++;
			}
		}

		if (emSize == 0)
			return GpStatus::GenericError;

		cs = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
		if (cairo_surface_status(cs) != CAIRO_STATUS_SUCCESS) {
			cairo_surface_destroy(cs);
			return GpStatus::OutOfMemory;
		}

		cr = cairo_create(cs);
		if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
			cairo_destroy(cr);
			cairo_surface_destroy(cs);
			return GpStatus::OutOfMemory;
		}

		status = gdip_create_font_without_validation(family, fabsf(emSize), style, UnitPixel, &font);
		if (status != GpStatus::Ok) {
			GdipFree(utf8);
			cairo_destroy(cr);
			cairo_surface_destroy(cs);
			return status;
		}

#ifdef USE_PANGO_RENDERING
		{
			GpRectF box;
			GpPointF box_offset;
			PangoLayout* layout;
			GpStringFormat* string_format;

			if (format == NULL) {
				status = GdipCreateStringFormat(StringFormatFlagsNoClip, 0, &string_format);
			}
			else if (!(format->formatFlags & StringFormatFlagsNoClip)) {
				status = GdipCloneStringFormat(format, &string_format);
				if (status == GpStatus::Ok)
					string_format->formatFlags |= StringFormatFlagsNoClip;
			}
			else {
				status = GpStatus::Ok;
				string_format = (GpStringFormat*)format;
			}

			if (status != GpStatus::Ok) {
				GdipDeleteFont(font);
				GdipFree(utf8);
				cairo_destroy(cr);
				cairo_surface_destroy(cs);
				return status;
			}

			layout = gdip_pango_setup_layout(cr, string, length, font, layoutRect, &box, &box_offset, string_format, NULL);
			cairo_move_to(cr, layoutRect->X + box_offset.X, layoutRect->Y + box_offset.Y);
			pango_cairo_layout_path(cr, layout);
			g_object_unref(layout);

			if (string_format != format)
				GdipDeleteStringFormat(string_format);

			// If our Cairo context had a current point before laying out the path, Pango will have moved us back there.
			// We don't want that when we process the path below, so clear it if set.
			if (cairo_has_current_point(cr))
				cairo_new_sub_path(cr);

		}
#else
		{
			BYTE* utf8 = (BYTE*)utf16_to_utf8(string, length);
			if (!utf8) {
				GdipDeleteFont(font);
				cairo_destroy(cr);
				cairo_surface_destroy(cs);
				return GpStatus::OutOfMemory;
			}

			cairo_move_to(cr, layoutRect->X, layoutRect->Y + font->sizeInPixels);

			cairo_set_font_face(cr, gdip_get_cairo_font_face(font));
			cairo_set_font_size(cr, font->sizeInPixels);
			/* TODO - deal with layoutRect, format... ideally we would be calling a subset
			   of GdipDrawString that already does everything *and* preserve the whole path */
			cairo_text_path(cr, (const char*)utf8);

			GdipFree(utf8);
		}
#endif

		/* get the font data from the cairo path and translate it as a gdi+ path */
		status = GpStatus::Ok;
		cp = cairo_copy_path(cr);
		if (cp) {
			int i;
			int count = 0;
			for (i = 0; i < cp->num_data; i += cp->data[i].header.length) {
				cairo_path_data_t* data = &cp->data[i];
				switch (data->header.type) {
				case CAIRO_PATH_MOVE_TO: count++; break;
				case CAIRO_PATH_LINE_TO: count++; break;
				case CAIRO_PATH_CURVE_TO: count += 3; break;
				case CAIRO_PATH_CLOSE_PATH: break;
				}
			}

			if (gdip_path_ensure_size(path, path->count + count)) {
				for (i = 0; i < cp->num_data; i += cp->data[i].header.length) {
					GpPathPointType type = GpPathPointType::PathPointTypeStart;
					int dataLength = cp->data[i].header.length;
					cairo_path_data_t* data = &cp->data[i];

					if ((i < cp->num_data - dataLength) && (cp->data[i + dataLength].header.type == CAIRO_PATH_CLOSE_PATH))
						type |= GpPathPointType::PathPointTypeCloseSubpath;

					switch (data->header.type) {
					case CAIRO_PATH_MOVE_TO:
						append(path, data[1].point.x, data[1].point.y, type, FALSE);
						break;
					case CAIRO_PATH_LINE_TO:
						append(path, data[1].point.x, data[1].point.y, type | GpPathPointType::PathPointTypeLine, FALSE);
						break;
					case CAIRO_PATH_CURVE_TO:
						append(path, data[1].point.x, data[1].point.y, GpPathPointType::PathPointTypeBezier, FALSE);
						append(path, data[2].point.x, data[2].point.y, GpPathPointType::PathPointTypeBezier, FALSE);
						append(path, data[3].point.x, data[3].point.y, type | GpPathPointType::PathPointTypeBezier, FALSE);
						break;
					case CAIRO_PATH_CLOSE_PATH:
						break;
					}
				}
			}
			else {
				status = GpStatus::OutOfMemory;
			}

			cairo_path_destroy(cp);
		}

		if (font) {
			GdipDeleteFont(font);
		}
		GdipFree(utf8);
		cairo_destroy(cr);
		cairo_surface_destroy(cs);
		return status;
#endif	
	}

	GpStatus WINGDIPAPI
		GdipAddPathStringI(GpPath* path, GDIPCONST WCHAR* string, int length,
			GDIPCONST GpFontFamily* family, int style, float emSize,
			GDIPCONST GpRect* layoutRect, GDIPCONST GpStringFormat* format)
	{
		GpRectF rect;
#ifdef GDIFLAT_SAFE
		if (!layoutRect)
			return GpStatus::InvalidParameter;
#endif
		return GpStatus::NotImplemented;
		/*gdip_RectF_from_Rect(layoutRect, &rect);
		return GdipAddPathString(path, string, length, family, style, emSize, &rect, format);
		*/
	}

	GpStatus WINGDIPAPI
		GdipAddPathLineI(GpPath* path, int x1, int y1, int x2, int y2)
	{
		return GdipAddPathLine(path, x1, y1, x2, y2);
	}

	GpStatus WINGDIPAPI
		GdipAddPathLine2I(GpPath* path, const GpPoint* points, int count)
	{
		GpPoint* tmp;
		int i;
#ifdef GDIFLAT_SAFE
		if (!path || !points || (count < 0))
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + count))
			return GpStatus::OutOfMemory;

		/* only the first point can be compressed (i.e. removed if identical to previous) */
		for (i = 0, tmp = (GpPoint*)points; i < count; i++, tmp++)
			append(path, tmp->X, tmp->Y, GpPathPointType::PathPointTypeLine, (i == 0));

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathArcI(GpPath* path, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return GdipAddPathArc(path, x, y, width, height, startAngle, sweepAngle);
	}

	GpStatus WINGDIPAPI
		GdipAddPathBezierI(GpPath* path, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		return GdipAddPathBezier(path, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	GpStatus WINGDIPAPI
		GdipAddPathBeziersI(GpPath* path, const GpPoint* points, int count)
	{
		GpPoint* tmp;
		int i;
#ifdef GDIFLAT_SAFE
		if (!path || !points)
			return GpStatus::InvalidParameter;

		/* first bezier requires 4 points, other 3 more points */
		if ((count < 4) || ((count % 3) != 1))
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + count))
			return GpStatus::OutOfMemory;

		tmp = (GpPoint*)points;
		append(path, tmp->X, tmp->Y, GpPathPointType::PathPointTypeLine, TRUE);
		tmp++;

		for (i = 1; i < count; i++, tmp++)
			append(path, tmp->X, tmp->Y, GpPathPointType::PathPointTypeBezier3, FALSE);

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathCurveI(GpPath* path, const GpPoint* points, int count)
	{
		return GdipAddPathCurve2I(path, points, count, 0.5);
	}

	GpStatus WINGDIPAPI
		GdipAddPathCurve2I(GpPath* path, const GpPoint* points, int count, float tension)
	{
		GpPointF* pt;
		GpStatus s;
#ifdef GDIFLAT_SAFE
		if (!points)
			return GpStatus::InvalidParameter;
#endif
		pt = convert_points(points, count);
		if (!pt)
			return GpStatus::OutOfMemory;

		/* here we must deal/accept curve with 2 points, GdipAddPathCurve3I doesn't */
		s = GdipAddPathCurve2(path, pt, count, tension);

		GdipFree(pt);
		return s;
	}

	GpStatus WINGDIPAPI
		GdipAddPathCurve3I(GpPath* path, const GpPoint* points, int count, int offset, int numberOfSegments, float tension)
	{
		GpPointF* pt;
		GpStatus s;
#ifdef GDIFLAT_SAFE
		if (!points)
			return GpStatus::InvalidParameter;
#endif
		pt = convert_points(points, count);
		if (!pt)
			return GpStatus::OutOfMemory;

		s = GdipAddPathCurve3(path, pt, count, offset, numberOfSegments, tension);

		GdipFree(pt);
		return s;
	}

	GpStatus WINGDIPAPI
		GdipAddPathClosedCurveI(GpPath* path, const GpPoint* points, int count)
	{
		return GdipAddPathClosedCurve2I(path, points, count, 0.5);
	}

	GpStatus WINGDIPAPI
		GdipAddPathClosedCurve2I(GpPath* path, const GpPoint* points, int count, float tension)
	{
		GpPointF* pt;
		GpStatus s;
#ifdef GDIFLAT_SAFE
		if (!path || !points)
			return GpStatus::InvalidParameter;
#endif
		pt = convert_points(points, count);
		if (!pt)
			return GpStatus::OutOfMemory;

		s = GdipAddPathClosedCurve2(path, pt, count, tension);

		GdipFree(pt);

		return s;
	}

	GpStatus WINGDIPAPI
		GdipAddPathRectangleI(GpPath* path, int x, int y, int width, int height)
	{
		return GdipAddPathRectangle(path, x, y, width, height);
	}

	GpStatus WINGDIPAPI
		GdipAddPathRectanglesI(GpPath* path, const GpRect* rects, int count)
	{
		int i;
#ifdef GDIFLAT_SAFE
		if (!path || !rects)
			return GpStatus::InvalidParameter;
#endif
		for (i = 0; i < count; i++) {
			float x = (float)rects[i].X;
			float y = (float)rects[i].Y;
			float width = (float)rects[i].Width;
			float height = (float)rects[i].Height;
			GdipAddPathRectangle(path, x, y, width, height);
		}

		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipAddPathEllipseI(GpPath* path, int x, int y, int width, int height)
	{
		return GdipAddPathEllipse(path, x, y, width, height);
	}

	GpStatus WINGDIPAPI
		GdipAddPathPieI(GpPath* path, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return GdipAddPathPie(path, x, y, width, height, startAngle, sweepAngle);
	}

	GpStatus WINGDIPAPI
		GdipAddPathPolygonI(GpPath* path, const GpPoint* points, int count)
	{
		int i;
		GpPoint* tmp;
#ifdef GDIFLAT_SAFE
		if (!path || !points || (count < 3))
			return GpStatus::InvalidParameter;
#endif
		if (!gdip_path_ensure_size(path, path->count + count + 1))
			return GpStatus::OutOfMemory;

		/* note: polygon points are never compressed (i.e. removed if identical) */

		tmp = (GpPoint*)points;
		append(path, tmp->X, tmp->Y, GpPathPointType::PathPointTypeStart, FALSE);
		tmp++;

		for (i = 1; i < count; i++, tmp++)
			append(path, tmp->X, tmp->Y, GpPathPointType::PathPointTypeLine, FALSE);

		/*
		 * Add a line from the last point back to the first point if
		 * they're not the same
		 */
		if (points[0].X != points[count - 1].X && points[0].Y != points[count - 1].Y)
			append(path, points[0].X, points[0].Y, GpPathPointType::PathPointTypeLine, FALSE);

		/* close the path */
		return GdipClosePathFigure(path);
	}

	/* nr_curve_flatten comes from Sodipodi's libnr (public domain) available from http://www.sodipodi.com/ */
	/* Mono changes: converted to float (from double), added recursion limit, use GArray */
	static BOOL
		nr_curve_flatten(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float flatness, int level, GpPath* flat_path)
	{
		float dx1_0, dy1_0, dx2_0, dy2_0, dx3_0, dy3_0, dx2_3, dy2_3, d3_0_2;
		float s1_q, t1_q, s2_q, t2_q, v2_q;
		float f2, f2_q;
		float x00t, y00t, x0tt, y0tt, xttt, yttt, x1tt, y1tt, x11t, y11t;

		dx1_0 = x1 - x0;
		dy1_0 = y1 - y0;
		dx2_0 = x2 - x0;
		dy2_0 = y2 - y0;
		dx3_0 = x3 - x0;
		dy3_0 = y3 - y0;
		dx2_3 = x3 - x2;
		dy2_3 = y3 - y2;
		f2 = flatness;
		d3_0_2 = dx3_0 * dx3_0 + dy3_0 * dy3_0;
		if (d3_0_2 < f2) {
			float d1_0_2, d2_0_2;
			d1_0_2 = dx1_0 * dx1_0 + dy1_0 * dy1_0;
			d2_0_2 = dx2_0 * dx2_0 + dy2_0 * dy2_0;
			if ((d1_0_2 < f2) && (d2_0_2 < f2)) {
				goto nosubdivide;
			}
			else {
				goto subdivide;
			}
		}
		f2_q = f2 * d3_0_2;
		s1_q = dx1_0 * dx3_0 + dy1_0 * dy3_0;
		t1_q = dy1_0 * dx3_0 - dx1_0 * dy3_0;
		s2_q = dx2_0 * dx3_0 + dy2_0 * dy3_0;
		t2_q = dy2_0 * dx3_0 - dx2_0 * dy3_0;
		v2_q = dx2_3 * dx3_0 + dy2_3 * dy3_0;
		if ((t1_q * t1_q) > f2_q) goto subdivide;
		if ((t2_q * t2_q) > f2_q) goto subdivide;
		if ((s1_q < 0.0) && ((s1_q * s1_q) > f2_q)) goto subdivide;
		if ((v2_q < 0.0) && ((v2_q * v2_q) > f2_q)) goto subdivide;
		if (s1_q >= s2_q) goto subdivide;

	nosubdivide:
		{
			append(flat_path, x3, y3, GpPathPointType::PathPointTypeLine, FALSE);
			return TRUE;
		}
	subdivide:
		/* things gets *VERY* memory intensive without a limit */
		if (level >= FLATTEN_RECURSION_LIMIT)
			return FALSE;

		x00t = (x0 + x1) * 0.5;
		y00t = (y0 + y1) * 0.5;
		x0tt = (x0 + 2 * x1 + x2) * 0.25;
		y0tt = (y0 + 2 * y1 + y2) * 0.25;
		x1tt = (x1 + 2 * x2 + x3) * 0.25;
		y1tt = (y1 + 2 * y2 + y3) * 0.25;
		x11t = (x2 + x3) * 0.5;
		y11t = (y2 + y3) * 0.5;
		xttt = (x0tt + x1tt) * 0.5;
		yttt = (y0tt + y1tt) * 0.5;

		if (!nr_curve_flatten(x0, y0, x00t, y00t, x0tt, y0tt, xttt, yttt, flatness, level + 1, flat_path)) return FALSE;
		if (!nr_curve_flatten(xttt, yttt, x1tt, y1tt, x11t, y11t, x3, y3, flatness, level + 1, flat_path)) return FALSE;
		return TRUE;
	}

	static BOOL
		gdip_convert_bezier_to_lines(GpPath* path, int index, float flatness, GpPath* flat_path)
	{
		GpPointF start, first, second, end;
		int saved_count;

		if ((index <= 0) || (index + 2 >= path->count))
			return FALSE; /* bad path data */

		start = path->points[index - 1];
		first = path->points[index];
		second = path->points[index + 1];
		end = path->points[index + 2];

		if (!gdip_path_ensure_size(path, path->count + (1 << FLATTEN_RECURSION_LIMIT)))
			return FALSE;

		saved_count = flat_path->count;
		if (!nr_curve_flatten(start.X, start.Y, first.X, first.Y, second.X, second.Y, end.X, end.Y, flatness, 0, flat_path)) {
			/* curved path is too complex (i.e. would result in too many points) to render as a polygon */
			flat_path->count = saved_count;
			return FALSE;
		}

		return TRUE;
	}

	GpStatus WINGDIPAPI
		GdipFlattenPath(GpPath* path, GpMatrix* matrix, float flatness)
	{
		GpStatus status = GpStatus::Ok;
		GpPath* flat_path;
		int i;
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		/* apply matrix before flattening (as there's less points at this stage) */
		if (matrix) {
			status = GdipTransformPath(path, matrix);
			if (status != GpStatus::Ok)
				return status;
		}

		/* if no bezier are present then the path doesn't need to be flattened */
		if (!gdip_path_has_curve(path))
			return status;

		status = GdipCreatePath(path->fill_mode, &flat_path);
		if (status != GpStatus::Ok)
			return status;

		/* Iterate the current path and replace each bezier with multiple lines */
		for (i = 0; i < path->count; i++) {
			GpPointF point = path->points[i];
			BYTE type = path->types[i];

			/* GpPathPointType::PathPointTypeBezier3 has the same value as GpPathPointType::PathPointTypeBezier */
			if ((type & GpPathPointType::PathPointTypeBezier) == GpPathPointType::PathPointTypeBezier) {
				if (!gdip_convert_bezier_to_lines(path, i, fabs(flatness), flat_path)) {
					/* uho, too much recursion - do not pass go, do not collect 200$ */

					/* free the the partial flat */
					GdipResetPath(flat_path);

					if (!gdip_path_ensure_size(flat_path, flat_path->count + 4)) {
						GdipDeletePath(flat_path);
						return GpStatus::OutOfMemory;
					}

					/* mimic MS behaviour when recursion becomes a problem */
					/* note: it's not really an empty rectangle as the last point isn't closing */

					append(flat_path, 0, 0, GpPathPointType::PathPointTypeStart, FALSE);
					append(flat_path, 0, 0, GpPathPointType::PathPointTypeLine, FALSE);
					append(flat_path, 0, 0, GpPathPointType::PathPointTypeLine, FALSE);
					append(flat_path, 0, 0, GpPathPointType::PathPointTypeLine, FALSE);
					break;
				}
				/* beziers have 4 points: the previous one, the current and the next two */
				i += 2;
			}
			else {
				if (!gdip_path_ensure_size(flat_path, flat_path->count + 1)) {
					GdipDeletePath(flat_path);
					return GpStatus::OutOfMemory;
				}

				/* no change required, just copy the point */
				append_point(flat_path, point, (GpPathPointType)type, FALSE);
			}
		}

		/* free original path points and types */
		if (path->points != NULL)
			GdipFree(path->points);
		if (path->types != NULL)
			GdipFree(path->types);

		/* transfer new path informations */
		path->points = flat_path->points;
		path->types = flat_path->types;
		path->count = flat_path->count;
		path->size = flat_path->size;
		GdipFree(flat_path);

		/* note: no error code is given for excessive recursion */
		return GpStatus::Ok;
	}

	/* MonoTODO - doesn't seems to be exposed in System.Drawing.dll */
	GpStatus WINGDIPAPI
		GdipWindingModeOutline(GpPath* path, GpMatrix* matrix, float flatness)
	{
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		/* quick out */
		if (path->count == 0)
			return GpStatus::Ok;

		status = gdip_prepare_path(path, matrix, flatness);
		if (status != GpStatus::Ok)
			return status;

		/* TODO */

		return GpStatus::NotImplemented;
	}

	/* MonoTODO */
	GpStatus WINGDIPAPI
		GdipWidenPath(GpPath* nativePath, GpPen* pen, GpMatrix* matrix, float flatness)
	{
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!nativePath || !pen)
			return GpStatus::InvalidParameter;
#endif
		/* (0) is deal within System.Drawing */
		/* (1) for compatibility with MS GDI+ (reported as FDBK49685) */
		if (nativePath->count <= 1)
			return GpStatus::OutOfMemory;

		status = gdip_prepare_path(nativePath, matrix, flatness);
		if (status != GpStatus::Ok)
			return status;

		/* TODO inner path (same number of points as the prepared path) */

		/* TODO outer path (twice the number of points as the prepared path) */

		return GpStatus::NotImplemented;
	}

	/* MonoTODO */
	GpStatus WINGDIPAPI
		GdipWarpPath(GpPath* path, GpMatrix* matrix, const GpPointF* points, int count,
			float srcx, float srcy, float srcwidth, float srcheight,
			GpWarpMode warpMode, float flatness)
	{
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!path || !points || (count < 1))
			return GpStatus::InvalidParameter;
#endif
		/* quick out */
		if (path->count == 0)
			return GpStatus::Ok;

		/* an invalid warp mode resets the current path */
		/* a path with a single point will reset it too */
		if (((warpMode != GpWarpMode::WarpModePerspective) && (warpMode != GpWarpMode::WarpModeBilinear)) || (path->count == 1))
			return GdipResetPath(path);

		status = gdip_prepare_path(path, matrix, flatness);
		if (status != GpStatus::Ok)
			return status;

		/* TODO */

		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipTransformPath(GpPath* path, GpMatrix* matrix)
	{
#ifdef GDIFLAT_SAFE
		if (!path)
			return GpStatus::InvalidParameter;
#endif
		if (path->count == 0)
			return GpStatus::Ok; /* GdipTransformMatrixPoints would fail */

		/* avoid allocation/free/calculation for null/identity matrix */
		if (gdip_is_matrix_empty(matrix))
			return GpStatus::Ok;

		return GdipTransformMatrixPoints(matrix, path->points, path->count);
	}

	GpStatus WINGDIPAPI
		GdipGetPathWorldBounds(GpPath* path, GpRectF* bounds, const GpMatrix* matrix, const GpPen* pen)
	{
		GpStatus status;
		GpPath* workpath;
		GpPointF points;
#ifdef GDIFLAT_SAFE
		if (!path || !bounds)
			return GpStatus::InvalidParameter;
#endif
		// Paths with zero or one points are empty.
		if (path->count <= 1) {
			bounds->X = 0.0f;
			bounds->Y = 0.0f;
			bounds->Width = 0.0f;
			bounds->Height = 0.0f;
			return GpStatus::Ok;
		}

		status = GdipClonePath(path, &workpath);
		if (status != GpStatus::Ok)
			return status;

		/* We don't need a very precise flat value to get the bounds (GDI+ isn't, big time) -
		 * however flattening helps by removing curves, making the rest of the algorithm a
		 * lot simpler.
		 */

		 /* note: only the matrix is applied if no curves are present in the path */
		status = GdipFlattenPath(workpath, (GpMatrix*)matrix, 25.0f);
		if (status != GpStatus::Ok) {
			GdipDeletePath(workpath);
			return status;
		}

		points = workpath->points[0];
		bounds->X = points.X;		/* keep minimum X here */
		bounds->Y = points.Y;		/* keep minimum Y here */
		if (workpath->count == 1) {
			/* special case #2 - Only one element */
			bounds->Width = 0.0f;
			bounds->Height = 0.0f;
			GdipDeletePath(workpath);
			return GpStatus::Ok;
		}

		bounds->Width = points.X;	/* keep maximum X here */
		bounds->Height = points.Y;	/* keep maximum Y here */

		for (int i = 1; i < workpath->count; i++) {
			points = workpath->points[i];
			if (points.X < bounds->X)
				bounds->X = points.X;
			if (points.Y < bounds->Y)
				bounds->Y = points.Y;
			if (points.X > bounds->Width)
				bounds->Width = points.X;
			if (points.Y > bounds->Height)
				bounds->Height = points.Y;
		}

		/* convert maximum values (width/height) as length */
		bounds->Width -= bounds->X;
		bounds->Height -= bounds->Y;

		if (pen) {
			/* in calculation the pen's width is at least 1.0 */
			float width = (pen->width < 1.0f) ? 1.0f : pen->width;
			float halfw = (width / 2);

			bounds->X -= halfw;
			bounds->Y -= halfw;
			bounds->Width += width;
			bounds->Height += width;
		}

		GdipDeletePath(workpath);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetPathWorldBoundsI(GpPath* path, GpRect* bounds, const GpMatrix* matrix, const GpPen* pen)
	{
		GpRectF rect;
		GpStatus status;
#ifdef GDIFLAT_SAFE
		if (!path || !bounds)
			return GpStatus::InvalidParameter;
#endif
		status = GdipGetPathWorldBounds(path, &rect, matrix, pen);
		if (status != GpStatus::Ok)
			return status;

		bounds->X = (int)rect.X;
		bounds->Y = (int)rect.Y;
		bounds->Width = (int)rect.Width;
		bounds->Height = (int)rect.Height;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipIsVisiblePathPoint(GpPath* path, float x, float y, GpGraphics* graphics, BOOL* result)
	{
		return GpStatus::NotImplemented;
#if 0
		GpStatus status = GpStatus::Ok;
		cairo_surface_t* s = NULL;
		GpGraphics* g;
		GpUnit page_unit = UnitPixel;

		if (!path || !result)
			return GpStatus::InvalidParameter;

		if (graphics) {
			g = graphics;
			cairo_save(g->ct);
			page_unit = g->page_unit;
		}
		else {
			/* create a temporary context */
			s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1);
			g = gdip_graphics_new(s);
		}

		cairo_new_path(g->ct);
		/* unit tests shows that PageUnit isn't consireded (well x, y are probably considered to be the same unit format ) */
		g->page_unit = UnitPixel;
		status = gdip_plot_path(g, path, FALSE);
		if (status == GpStatus::Ok) {
			cairo_set_fill_rule(g->ct, gdip_convert_fill_mode(path->fill_mode));
			cairo_set_antialias(g->ct, CAIRO_ANTIALIAS_NONE);
			*result = cairo_in_fill(g->ct, x + 1.0 /* CAIRO_AA_OFFSET_X */, y + CAIRO_AA_OFFSET_Y);
		}
		else {
			*result = FALSE;
		}

		if (graphics) {
			/* restore GpGraphics to original state */
			cairo_restore(graphics->ct);
			g->page_unit = page_unit;
		}
		else {
			/* delete temporary context */
			cairo_surface_destroy(s);
			GdipDeleteGraphics(g);
		}

		return status;
#endif
	}

	GpStatus WINGDIPAPI
		GdipIsVisiblePathPointI(GpPath* path, int x, int y, GpGraphics* graphics, BOOL* result)
	{
		return GdipIsVisiblePathPoint(path, x, y, graphics, result);
	}

	GpStatus WINGDIPAPI
		GdipIsOutlineVisiblePathPoint(GpPath* path, float x, float y, GpPen* pen, GpGraphics* graphics, BOOL* result)
	{
		return GpStatus::NotImplemented;
#if 0
		GpStatus status = GpStatus::Ok;
		cairo_surface_t* s = NULL;
		GpGraphics* g;
		GpUnit page_unit = GpUnit::UnitPixel;

		if (!path || !pen || !result)
			return GpStatus::InvalidParameter;

		if (graphics) {
			g = graphics;
			cairo_save(graphics->ct);
			page_unit = g->page_unit;
		}
		else {
			/* create a temporary context */
			s = cairo_image_surface_create(CAIRO_FORMAT_A1, 1, 1);
			g = gdip_graphics_new(s);
		}

		cairo_new_path(g->ct);
		/* unit tests shows that PageUnit isn't consireded (well x, y are probably considered to be the same unit format ) */
		g->page_unit = UnitPixel;
		status = gdip_plot_path(g, path, FALSE);
		if (status == GpStatus::Ok) {
			/* we must fight around cairo AA */
			cairo_set_antialias(g->ct, CAIRO_ANTIALIAS_NONE);
			cairo_set_line_width(g->ct, pen->width - CAIRO_AA_OFFSET_Y);
			*result = cairo_in_stroke(g->ct, x, y);
		}
		else {
			*result = FALSE;
		}

		if (graphics) {
			/* restore GpGraphics to original state */
			cairo_restore(graphics->ct);
			g->page_unit = page_unit;
		}
		else {
			/* delete temporary context */
			cairo_surface_destroy(s);
			GdipDeleteGraphics(g);
		}

		return status;
#endif
	}

	GpStatus WINGDIPAPI
		GdipIsOutlineVisiblePathPointI(GpPath* path, int x, int y, GpPen* pen, GpGraphics* graphics, BOOL* result)
	{
		return GdipIsOutlineVisiblePathPoint(path, x, y, pen, graphics, result);
	}

}