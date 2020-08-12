// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <MSFS\Render\GdiPlus\GdiPlusPen.h>
namespace Gdiplus {

	// Pen

	Pen::Pen(Color _color, float _width)
	{
		color = _color;
		width = _width;
	}

	void Pen::GetColor(Color* _color)
	{
		*_color = color;
	}

	Status Pen::SetColor(const Color& _color)
	{
		color = _color;
		return Ok;
	}

	float Pen::GetWidth()
	{
		return width;
	}

	Status Pen::SetWidth(float _width)
	{
		width = _width;
		return Ok;
	}

	void Pen::SetAlignment(PenAlignment _Alignment)
	{

	}

	Status Pen::SetDashPattern(const REAL* dashArray, INT count)
	{
		return Ok;
	}
	Status Pen::SetDashStyle(DashStyle dashStyle)
	{
		return Ok;
	}

	Status Pen::SetEndCap(LineCap endCap)
	{
		return Ok;
	}
	Status Pen::SetCustomEndCap(const CustomLineCap* customCap)
	{
		return Ok;
	}
	Status Pen::SetCustomStartCap(const CustomLineCap* customCap)
	{
		return Ok;
	}

}