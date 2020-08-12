// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <MSFS\Render\GdiPlus\gdiplusbrush.h>

namespace Gdiplus {

	// Brush

	Color Brush::GetColor() const
	{
		return nvgRGB(0, 0, 0);
	}

	// SolidBrush

	SolidBrush::SolidBrush(IN const Color& _color)
	{
		color = _color;
	}

	void SolidBrush::GetColor(OUT Color* _color) const
	{
		*_color = color;
	}

	Color SolidBrush::GetColor() const
	{
		return color;
	}

	void SolidBrush::SetColor(IN const Color& _color)
	{
		color = _color;
	}

	int SolidBrush::GetType() const
	{
		return BrushTypeSolidColor;
	}

	// HatchBrush

	HatchBrush::HatchBrush(IN HatchStyle hatchStyle,
		IN const Color& foreColor,
		IN const Color& backColor)
	{
		this->style = hatchStyle;
		this->foreColor = foreColor;
		this->backColor = backColor;
		this->patternImage = 0;
	}

	int HatchBrush::GetType() const
	{
		return BrushTypeHatchFill;
	}

}