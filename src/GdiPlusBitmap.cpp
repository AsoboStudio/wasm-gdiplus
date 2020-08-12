// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <MSFS\Render\GdiPlus\gdiplusheaders.h>
namespace Gdiplus {

	// Image

	UINT Image::GetHeight()
	{
		return 0;
	}
	UINT Image::GetWidth()
	{
		return 0;
	}


	// Bitmap

	Bitmap::Bitmap(INT width, INT height, Graphics* target)
	{

	}
	Bitmap::Bitmap(INT width, INT height, PixelFormat format)
	{

	}
	Status Bitmap::SetPixel(INT x, INT y, const Color& color)
	{
		return Ok;
	}
	Status Bitmap::LockBits(const Rect* rect, UINT flags, PixelFormat format, BitmapData* lockedBitmapData)
	{
		return Ok;
	}
	Status Bitmap::UnlockBits(BitmapData* lcokedBitmapData)
	{
		return Ok;
	}


}