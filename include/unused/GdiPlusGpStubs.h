/*
 * Copyright (C) 2020 Asobo Studio (https://www.asobostudio.com/)
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
 */

#ifndef _GDIPLUSGPSTUBS_H
#define _GDIPLUSGPSTUBS_H
#include <MSFS\Render\GdiPlus\GdiPlus.h>
namespace Gdiplus
{
//---------------------------------------------------------------------------
// GDI+ classes for forward reference
//---------------------------------------------------------------------------
using namespace Gdiplus;

class Graphics;
class Pen;
class Brush;
class Matrix;
class Bitmap;
class Metafile;
class GraphicsPath;
class PathIterator;
class Region;
class Image;
class TextureBrush;
class HatchBrush;
class SolidBrush;
class LinearGradientBrush;
class PathGradientBrush;
class Font;
class FontFamily;
class FontCollection;
class InstalledFontCollection;
class PrivateFontCollection;
class ImageAttributes;
class CachedBitmap;

//---------------------------------------------------------------------------
// Private GDI+ classes for internal type checking
//---------------------------------------------------------------------------
class GpGraphics {};

class GpBrush {};
class GpTexture : public GpBrush {};
class GpSolidFill : public GpBrush {};
class GpLineGradient : public GpBrush {};
class GpPathGradient : public GpBrush {};
class GpHatch : public GpBrush {};

class GpPen {};
class GpCustomLineCap {};
class GpAdjustableArrowCap : public GpCustomLineCap {};

class GpImage {};
class GpBitmap : public GpImage {};
class GpMetafile : public GpImage {};
class GpImageAttributes {};

class GpPath {};
class GpRegion {};
class GpPathIterator {};

class GpFontFamily {};
class GpFont {};
class GpStringFormat {};
class GpFontCollection {};
class GpInstalledFontCollection : public GpFontCollection {};
class GpPrivateFontCollection : public GpFontCollection {};

class GpCachedBitmap;

typedef Status GpStatus;
typedef FillMode GpFillMode;
typedef WrapMode GpWrapMode;
typedef Unit GpUnit;
typedef CoordinateSpace GpCoordinateSpace;
typedef PointF GpPointF;
typedef Point GpPoint;
typedef RectF GpRectF;
typedef Rect GpRect;
typedef SizeF GpSizeF;
typedef HatchStyle GpHatchStyle;
typedef DashStyle GpDashStyle;
typedef LineCap GpLineCap;
typedef DashCap GpDashCap;


typedef PenAlignment GpPenAlignment;

typedef LineJoin GpLineJoin;
typedef PenType GpPenType;


typedef BrushType GpBrushType;
typedef MatrixOrder GpMatrixOrder;
typedef FlushIntention GpFlushIntention;

/**** NOT SUPPORTED YET***\
typedef Matrix GpMatrix;
typedef PathData GpPathData;
\**** NOT SUPPORTED YET***/
}
#endif  // !_GDIPLUSGPSTUBS.HPP