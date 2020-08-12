// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "GdiPlusTypes.h"

namespace Gdiplus {

	// SizeF

SizeF::SizeF()
{
	Width = Height = 0.0f;
}

SizeF::SizeF(IN const SizeF& size)
{
	Width = size.Width;
	Height = size.Height;
}

SizeF::SizeF(IN REAL width, IN REAL height)
{
	Width = width;
	Height = height;
}

SizeF SizeF::operator+(IN const SizeF& sz) const
{
	return SizeF(Width + sz.Width,
		Height + sz.Height);
}

SizeF SizeF::operator-(IN const SizeF& sz) const
{
	return SizeF(Width - sz.Width,
		Height - sz.Height);
}

BOOL SizeF::Equals(IN const SizeF& sz) const
{
	return (Width == sz.Width) && (Height == sz.Height);
}

BOOL SizeF::Empty() const
{
	return (Width == 0.0f && Height == 0.0f);
}

// Size

Size::Size()
{
	Width = Height = 0;
}

Size::Size(IN const Size& size)
{
	Width = size.Width;
	Height = size.Height;
}

Size::Size(IN INT width,
	IN INT height)
{
	Width = width;
	Height = height;
}

Size Size::operator+(IN const Size& sz) const
{
	return Size(Width + sz.Width,
		Height + sz.Height);
}

Size Size::operator-(IN const Size& sz) const
{
	return Size(Width - sz.Width,
		Height - sz.Height);
}

BOOL Size::Equals(IN const Size& sz) const
{
	return (Width == sz.Width) && (Height == sz.Height);
}

BOOL Size::Empty() const
{
	return (Width == 0 && Height == 0);
}

// PointF

PointF::PointF()
{
	X = Y = 0.0f;
}

PointF::PointF(IN const PointF& point)
{
	X = point.X;
	Y = point.Y;
}

PointF::PointF(IN const SizeF& size)
{
	X = size.Width;
	Y = size.Height;
}

PointF::PointF(IN REAL x, IN REAL y)
{
	X = x;
	Y = y;
}

PointF PointF::operator+(IN const PointF& point) const
{
	return PointF(X + point.X,
		Y + point.Y);
}

PointF PointF::operator-(IN const PointF& point) const
{
	return PointF(X - point.X,
		Y - point.Y);
}

BOOL PointF::Equals(IN const PointF& point)
{
	return (X == point.X) && (Y == point.Y);
}

//--------------------------------------------------------------------------
// Represents a location in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

Point::Point()
{
	X = Y = 0;
}

Point::Point(IN const Point& point)
{
	X = point.X;
	Y = point.Y;
}

Point::Point(IN const Size& size)
{
	X = size.Width;
	Y = size.Height;
}

Point::Point(IN INT x, IN INT y)
{
	X = x;
	Y = y;
}

Point Point::operator+(IN const Point& point) const
{
	return Point(X + point.X,
		Y + point.Y);
}

Point Point::operator-(IN const Point& point) const
{
	return Point(X - point.X,
		Y - point.Y);
}

BOOL Point::Equals(IN const Point& point)
{
	return (X == point.X) && (Y == point.Y);
}

//--------------------------------------------------------------------------
// Represents a rectangle in a 2D coordinate system (floating-point coordinates)
//--------------------------------------------------------------------------

RectF::RectF()
{
	X = Y = Width = Height = 0.0f;
}

RectF::RectF(IN REAL x, IN REAL y, IN REAL width, IN REAL height)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

RectF::RectF(IN const PointF& location, IN const SizeF& size)
{
	X = location.X;
	Y = location.Y;
	Width = size.Width;
	Height = size.Height;
}

RectF* RectF::Clone() const
{
	return new RectF(X, Y, Width, Height);
}

VOID RectF::GetLocation(OUT PointF* point) const
{
	point->X = X;
	point->Y = Y;
}

VOID RectF::GetSize(OUT SizeF* size) const
{
	size->Width = Width;
	size->Height = Height;
}

VOID RectF::GetBounds(OUT RectF* rect) const
{
	rect->X = X;
	rect->Y = Y;
	rect->Width = Width;
	rect->Height = Height;
}

REAL RectF::GetLeft() const
{
	return X;
}

REAL RectF::GetTop() const
{
	return Y;
}

REAL RectF::GetRight() const
{
	return X + Width;
}

REAL RectF::GetBottom() const
{
	return Y + Height;
}

BOOL RectF::IsEmptyArea() const
{
	return (Width <= REAL_EPSILON) || (Height <= REAL_EPSILON);
}

BOOL RectF::Equals(IN const RectF& rect) const
{
	return X == rect.X &&
		Y == rect.Y &&
		Width == rect.Width &&
		Height == rect.Height;
}

BOOL RectF::Contains(IN REAL x,
	IN REAL y) const
{
	return x >= X && x < X + Width &&
		y >= Y && y < Y + Height;
}

BOOL RectF::Contains(IN const PointF& pt) const
{
	return Contains(pt.X, pt.Y);
}

BOOL RectF::Contains(IN const RectF& rect) const
{
	return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
		(Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
}

VOID RectF::Inflate(IN REAL dx, IN REAL dy)
{
	X -= dx;
	Y -= dy;
	Width += 2 * dx;
	Height += 2 * dy;
}

VOID RectF::Inflate(IN const PointF& point)
{
	Inflate(point.X, point.Y);
}

BOOL RectF::Intersect(IN const RectF& rect)
{
	return Intersect(*this, *this, rect);
}

BOOL RectF::Intersect(OUT RectF& c, IN const RectF& a, IN const RectF& b)
{
	REAL right = min(a.GetRight(), b.GetRight());
	REAL bottom = min(a.GetBottom(), b.GetBottom());
	REAL left = max(a.GetLeft(), b.GetLeft());
	REAL top = max(a.GetTop(), b.GetTop());

	c.X = left;
	c.Y = top;
	c.Width = right - left;
	c.Height = bottom - top;
	return !c.IsEmptyArea();
}

BOOL RectF::IntersectsWith(IN const RectF& rect) const
{
	return (GetLeft() < rect.GetRight() &&
		GetTop() < rect.GetBottom() &&
		GetRight() > rect.GetLeft() &&
		GetBottom() > rect.GetTop());
}

BOOL RectF::Union(OUT RectF& c, IN const RectF& a, IN const RectF& b)
{
	REAL right = max(a.GetRight(), b.GetRight());
	REAL bottom = max(a.GetBottom(), b.GetBottom());
	REAL left = min(a.GetLeft(), b.GetLeft());
	REAL top = min(a.GetTop(), b.GetTop());

	c.X = left;
	c.Y = top;
	c.Width = right - left;
	c.Height = bottom - top;
	return !c.IsEmptyArea();
}

VOID RectF::Offset(IN const PointF& point)
{
	Offset(point.X, point.Y);
}

VOID RectF::Offset(IN REAL dx, IN REAL dy)
{
	X += dx;
	Y += dy;
}

//--------------------------------------------------------------------------
// Represents a rectangle in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

Rect::Rect()
{
	X = Y = Width = Height = 0;
}

Rect::Rect(IN INT x, IN INT y, IN INT width, IN INT height)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

Rect::Rect(IN const Point& location, IN const Size& size)
{
	X = location.X;
	Y = location.Y;
	Width = size.Width;
	Height = size.Height;
}

Rect* Rect::Clone() const
{
	return new Rect(X, Y, Width, Height);
}

VOID Rect::GetLocation(OUT Point* point) const
{
	point->X = X;
	point->Y = Y;
}

VOID Rect::GetSize(OUT Size* size) const
{
	size->Width = Width;
	size->Height = Height;
}

VOID Rect::GetBounds(OUT Rect* rect) const
{
	rect->X = X;
	rect->Y = Y;
	rect->Width = Width;
	rect->Height = Height;
}

INT Rect::GetLeft() const
{
	return X;
}

INT Rect::GetTop() const
{
	return Y;
}

INT Rect::GetRight() const
{
	return X + Width;
}

INT Rect::GetBottom() const
{
	return Y + Height;
}

BOOL Rect::IsEmptyArea() const
{
	return (Width <= 0) || (Height <= 0);
}

BOOL Rect::Equals(IN const Rect& rect) const
{
	return X == rect.X &&
		Y == rect.Y &&
		Width == rect.Width &&
		Height == rect.Height;
}

BOOL Rect::Contains(IN INT x, IN INT y) const
{
	return x >= X && x < X + Width &&
		y >= Y && y < Y + Height;
}

BOOL Rect::Contains(IN const Point& pt) const
{
	return Contains(pt.X, pt.Y);
}

BOOL Rect::Contains(IN Rect& rect) const
{
	return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
		(Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
}

VOID Rect::Inflate(IN INT dx, IN INT dy)
{
	X -= dx;
	Y -= dy;
	Width += 2 * dx;
	Height += 2 * dy;
}

VOID Rect::Inflate(IN const Point& point)
{
	Inflate(point.X, point.Y);
}

BOOL Rect::Intersect(IN const Rect& rect)
{
	return Intersect(*this, *this, rect);
}

BOOL Rect::Intersect(OUT Rect& c, IN const Rect& a, IN const Rect& b)
{
	INT right = min(a.GetRight(), b.GetRight());
	INT bottom = min(a.GetBottom(), b.GetBottom());
	INT left = max(a.GetLeft(), b.GetLeft());
	INT top = max(a.GetTop(), b.GetTop());

	c.X = left;
	c.Y = top;
	c.Width = right - left;
	c.Height = bottom - top;
	return !c.IsEmptyArea();
}

BOOL Rect::IntersectsWith(IN const Rect& rect) const
{
	return (GetLeft() < rect.GetRight() &&
		GetTop() < rect.GetBottom() &&
		GetRight() > rect.GetLeft() &&
		GetBottom() > rect.GetTop());
}

BOOL Rect::Union(OUT Rect& c, IN const Rect& a, IN const Rect& b)
{
	INT right = max(a.GetRight(), b.GetRight());
	INT bottom = max(a.GetBottom(), b.GetBottom());
	INT left = min(a.GetLeft(), b.GetLeft());
	INT top = min(a.GetTop(), b.GetTop());

	c.X = left;
	c.Y = top;
	c.Width = right - left;
	c.Height = bottom - top;
	return !c.IsEmptyArea();
}

VOID Rect::Offset(IN const Point& point)
{
	Offset(point.X, point.Y);
}

VOID Rect::Offset(IN INT dx,
	IN INT dy)
{
	X += dx;
	Y += dy;
}

}