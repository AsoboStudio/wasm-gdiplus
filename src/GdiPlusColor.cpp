// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "GdiPlusColor.h"
namespace Gdiplus {

// Color

Color::Color()
{

}
Color::Color(const NVGcolor& _color)
{
	r = _color.r;
	g = _color.g;
	b = _color.b;
	a = _color.a;
}
Color::Color(ARGB _color)
{
	r = (float)((BYTE)(_color >> 16)) / 255;
	g = (float)((BYTE)(_color >> 8)) / 255;
	b = (float)((BYTE)(_color)) / 255;
	a = (float)((BYTE)(_color >> 24)) / 255;
}
Color::Color(BYTE r, BYTE g, BYTE b)
{
	*this = nvgRGB(r, g, b);
}
Color::Color(BYTE a, BYTE r, BYTE g, BYTE b)
{
	*this = nvgRGBA(r, g, b, a);
}
BYTE Color::GetA() const
{
	return (BYTE)(a * 255);
}
BYTE Color::GetAlpha() const
{
	return (BYTE)(a * 255);
}
BYTE Color::GetB() const
{
	return (BYTE)(b * 255);
}
BYTE Color::GetBlue() const
{
	return (BYTE)(b * 255);
}
BYTE Color::GetG() const
{
	return (BYTE)(g * 255);
}
BYTE Color::GetGreen() const
{
	return (BYTE)(g * 255);
}
BYTE Color::GetR() const
{
	return (BYTE)(r * 255);
}
BYTE Color::GetRed() const
{
	return (BYTE)(r * 255);
}
Status Color::SetValue(const ARGB _color)
{
	r = (float)((BYTE)(_color >> 16)) / 255;
	g = (float)((BYTE)(_color >> 8)) / 255;
	b = (float)((BYTE)(_color)) / 255;
	a = (float)((BYTE)(_color >> 24)) / 255;
	return Ok;
}
ARGB Color::GetValue() const
{
	return MakeARGB((BYTE)(a * 255), (BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}
COLORREF  Color::ToCOLORREF() const
{
	return GetValue();
}
}