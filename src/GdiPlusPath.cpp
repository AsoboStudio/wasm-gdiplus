// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "gdipluspath.h"
#include "gdiplusgraphics.h"

namespace Gdiplus {

	// GraphicsPath

	GraphicsPath::GraphicsPath(Graphics* context, FillMode fillMode)
	{
		m_context = context;
		m_fillmode = fillMode;
		m_pathIndex = 1;
		Reset();
	}
	GraphicsPath::GraphicsPath(Graphics* context, const PointF* points, const BYTE* types, INT count, FillMode fillMode)
		: GraphicsPath(context, fillMode)
	{
		AddPolygon(points, count);
	}
	GraphicsPath::GraphicsPath(Graphics* context, const Point* points, const BYTE* types, INT count, FillMode fillMode)
		: GraphicsPath(context, fillMode)
	{
		AddPolygon(points, count);
	}
	Status GraphicsPath::AddEllipse(REAL x, REAL y, REAL width, REAL height)
	{
		//TODO: use stack for paths
		int prevPath = nvgCurrentPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, m_pathIndex);
		nvgEllipse(m_context->ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f);
		nvgSelectPath(m_context->ctx, prevPath);
		m_empty = false;
		return Ok;
	}
	Status GraphicsPath::AddEllipse(INT x, INT y, INT width, INT height)
	{
		return AddEllipse((REAL)x, (REAL)y, (REAL)width, (REAL)height);
	}
	Status GraphicsPath::AddEllipse(const RectF& rect)
	{
		return AddEllipse(rect.X, rect.Y, rect.Width, rect.Height);
	}
	Status GraphicsPath::AddEllipse(const Rect& rect)
	{
		return AddEllipse((REAL)rect.X, (REAL)rect.Y, (REAL)rect.Width, (REAL)rect.Height);
	}
	Status GraphicsPath::AddArc(REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
		//TODO: use stack for paths
		int prevPath = nvgCurrentPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, m_pathIndex);
		nvgEllipticalArc(m_context->ctx, x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, nvgDegToRad(startAngle), nvgDegToRad(startAngle + sweepAngle), (sweepAngle < 0 ? NVG_CCW : NVG_CW));
		nvgSelectPath(m_context->ctx, prevPath);
		m_empty = false;
		return Ok;
	}
	Status GraphicsPath::AddArc(INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return AddArc((REAL)x, (REAL)y, (REAL)width, (REAL)height, startAngle, sweepAngle);
	}
	Status GraphicsPath::AddArc(const RectF& rect, REAL startAngle, REAL sweepAngle)
	{
		return AddArc(rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle);
	}
	Status GraphicsPath::AddArc(const Rect& rect, REAL startAngle, REAL sweepAngle)
	{
		return AddArc((REAL)rect.X, (REAL)rect.Y, (REAL)rect.Width, (REAL)rect.Height, startAngle, sweepAngle);
	}
	Status GraphicsPath::AddPie(REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status GraphicsPath::AddPie(INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status GraphicsPath::AddPie(const RectF& rect, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status GraphicsPath::AddPie(const Rect& rect, REAL startAngle, REAL sweepAngle)
	{
		return Ok;
	}
	Status GraphicsPath::AddRectangle(REAL x, REAL y, REAL width, REAL height)
	{
		//TODO: use stack for paths
		int prevPath = nvgCurrentPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, m_pathIndex);
		nvgRect(m_context->ctx, x, y, width, height);
		nvgSelectPath(m_context->ctx, prevPath);
		m_empty = false;
		return Ok;
	}
	Status GraphicsPath::AddRectangle(const RectF& rect)
	{
		return AddRectangle(rect.X, rect.Y, rect.Width, rect.Height);
	}
	Status GraphicsPath::AddRectangle(const Rect& rect)
	{
		return AddRectangle((REAL)rect.X, (REAL)rect.Y, (REAL)rect.Width, (REAL)rect.Height);
	}
	Status GraphicsPath::AddPolygon(const PointF* points, INT count)
	{
		//TODO: use stack for paths
		int prevPath = nvgCurrentPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, m_pathIndex);

		nvgMoveTo(m_context->ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(m_context->ctx, points[i].X, points[i].Y);
		nvgClosePath(m_context->ctx);

		nvgSelectPath(m_context->ctx, prevPath);
		m_empty = false;
		return Ok;
	}
	Status GraphicsPath::AddPolygon(const Point* points, INT count)
	{
		//TODO: use stack for paths
		int prevPath = nvgCurrentPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, m_pathIndex);

		nvgMoveTo(m_context->ctx, points[0].X, points[0].Y);
		for (int i = 1; i < count; i++)
			nvgLineTo(m_context->ctx, points[i].X, points[i].Y);
		nvgClosePath(m_context->ctx);

		nvgSelectPath(m_context->ctx, prevPath);
		m_empty = false;
		return Ok;
	}

	Status GraphicsPath::Reset() {
		//TODO: use stack for paths
		int prevPath = nvgCurrentPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, m_pathIndex);
		nvgBeginPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, prevPath);
		m_empty = true;
		return Ok;
	}
	Status GraphicsPath::CloseFigure() {
		//TODO: use stack for paths
		int prevPath = nvgCurrentPath(m_context->ctx);
		nvgSelectPath(m_context->ctx, m_pathIndex);
		nvgClosePath(m_context->ctx);
		nvgSelectPath(m_context->ctx, prevPath);
		m_empty = false;
		return Ok;
	}

}