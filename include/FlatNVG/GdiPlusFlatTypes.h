/*
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

 * Copyright (C) 2003-2007 Novell, Inc (http://www.novell.com)
 * Copyright (C) 2020 Asobo Studio.
 */
#pragma once

#include <MSFS\MSFS_WindowsTypes.h>
#include "..\win32structs.h"
#include "..\GdiPlusTypes.h"
#include <MSFS\Render\nanovg.h>

#include "..\GdiPlusColor.h"
#include "..\GdiPlusEnums.h"
#include "..\GdiPlusImaging.h"
#include "..\GdiPlusPixelFormats.h"

#include "..\gdiplusmetaheader.h"

#ifndef _COM_interface
#define _COM_interface struct
#endif

#ifndef NULL
#define NULL 0
#endif

typedef _COM_interface IStream IStream;
typedef _COM_interface IDirectDrawSurface7 IDirectDrawSurface7;

typedef gint32 PROPID;

// shared types between gdiplus and flat
#ifdef __cplusplus
using Gdiplus::GpStatus;
using Gdiplus::GpRect;
using Gdiplus::GpRectF;
using Gdiplus::GpPoint;
using Gdiplus::GpPointF;
using Gdiplus::CharacterRange;
using Gdiplus::MetafileHeader;
using Gdiplus::WmfPlaceableFileHeader;
using Gdiplus::ColorMatrix;
using Gdiplus::ColorMap;
using Gdiplus::Color;
#endif

// Forward definition of flat data structures
typedef struct _AdjustableArrowCap GpAdjustableArrowCap;
typedef struct _CustomLineCap GpCustomLineCap;
typedef struct _Font GpFont;
typedef struct _FontCollection GpFontCollection;
typedef struct _FontFamily GpFontFamily;
typedef struct _Graphics GpGraphics;
typedef struct _Brush GpBrush;
typedef struct _Hatch GpHatch;
typedef struct _SolidFill GpSolidFill;
typedef struct _Image GpImage;
typedef struct _Image GpBitmap;
typedef struct _ImageAttributes GpImageAttributes;
typedef struct _LineGradient GpLineGradient;
typedef struct _Metafile  GpMetafile;
typedef struct _Path GpPath;
typedef struct _PathIterator GpPathIterator;
typedef struct _PathGradient GpPathGradient;
typedef struct _Pen GpPen;
typedef struct _Region GpRegion;
typedef struct _StringFormat GpStringFormat;
typedef struct _Texture GpTexture;


// renaming of Gdiplus structures
typedef Gdiplus::GraphicsContainer GpGraphicsContainer;
typedef Gdiplus::BitmapData GpBitmapData;
typedef PixelFormat GpPixelFormat;
typedef Gdiplus::EncoderParameters GpEncoderParameters;
typedef struct Gdiplus::ColorPalette GpColorPalette;
typedef struct Gdiplus::PropertyItem GpPropertyItem;
typedef struct Gdiplus::EncoderParameters GpEncoderParameters;

typedef Gdiplus::PathData GpPathData;
typedef enum Gdiplus::FontStyle GpFontStyle;
typedef enum Gdiplus::ColorChannelFlags GpColorChannelFlags;
typedef enum Gdiplus::BrushType GpBrushType;
typedef enum Gdiplus::FillMode GpFillMode;
typedef enum Gdiplus::FlushIntention GpFlushIntention;
typedef enum Gdiplus::LineCap GpLineCap;
typedef enum Gdiplus::LineJoin GpLineJoin;
typedef enum Gdiplus::MatrixOrder GpMatrixOrder;
typedef enum Gdiplus::CoordinateSpace GpCoordinateSpace;
typedef enum Gdiplus::HatchStyle GpHatchStyle;
typedef enum Gdiplus::Unit GpUnit;
typedef enum Gdiplus::DashCap GpDashCap;
typedef enum Gdiplus::DashStyle GpDashStyle;
typedef enum Gdiplus::PenAlignment GpPenAlignment;
typedef enum Gdiplus::PenType GpPenType;
typedef enum Gdiplus::WrapMode GpWrapMode;
typedef enum Gdiplus::ImageFlags GpImageFlags;
typedef enum Gdiplus::ImageType GpImageType;
typedef enum Gdiplus::StringAlignment GpStringAlignment;
typedef enum Gdiplus::CustomLineCapType GpCustomLineCapType;
typedef enum Gdiplus::RotateFlipType GpRotateFlipType;
typedef Gdiplus::ColorAdjustType GpColorAdjustType;
typedef Gdiplus::ColorMatrixFlags GpColorMatrixFlags;
typedef enum Gdiplus::LinearGradientMode GpLinearGradientMode;
typedef enum Gdiplus::WarpMode GpWarpMode;
typedef enum Gdiplus::SmoothingMode GpSmoothingMode;
typedef Gdiplus::PathPointType GpPathPointType;
typedef enum Gdiplus::InterpolationMode GpInterpolationMode;
typedef enum Gdiplus::TextRenderingHint GpTextRenderingHint;
typedef enum Gdiplus::CompositingQuality GpCompositingQuality;
typedef enum Gdiplus::PixelOffsetMode GpPixelOffsetMode;
typedef enum Gdiplus::EmfType GpEmfType;
typedef enum Gdiplus::EmfPlusRecordType GpEmfPlusRecordType;
typedef enum Gdiplus::MetafileFrameUnit GpMetafileFrameUnit;
typedef Gdiplus::CompositingMode GpCompositingMode;
typedef Gdiplus::StringFormatFlags GpStringFormatFlags;
typedef Gdiplus::StringTrimming GpStringTrimming;
typedef Gdiplus::StringDigitSubstitute GpStringDigitSubstitute;
typedef Gdiplus::BitmapData GpBitmatData;
typedef Gdiplus::CombineMode GpCombineMode;
typedef Gdiplus::HotkeyPrefix GpHotkeyPrefix;
typedef Gdiplus::ImageFormat GpImageFormat;
typedef Gdiplus::GraphicsState GpGraphicsState;
typedef Gdiplus::ImageCodecInfo GpImageCodecInfo;


// forward definition of cairo structures for future replacement
typedef struct { float m[6]; } GpMatrix;
typedef void* GpPattern;
typedef void* GpSurface;
typedef int cairo_status_t;
typedef int cairo_font_face_t;

// TODO: fix, _Brush is declared here as libGdi+ uses vtables for inheritance
typedef struct _BrushClass {
	// Brush virtual functions 
	GpBrushType type;
	GpStatus(*setup) (GpGraphics* graphics, GpBrush* brush);
	GpStatus(*clone_brush) (GpBrush* brush, GpBrush** clonedBrush);
	GpStatus(*destroy) (GpBrush* brush);
} BrushClass;

typedef struct _Brush {
	BrushClass* vtable;
	void* brush;
	BOOL		changed;
} _Brush;

typedef struct _Hatch {
	GpBrush	base;
	GpHatchStyle	hatchStyle;
	ARGB		foreColor;
	ARGB		backColor;
	//cairo_pattern_t	*pattern;
	BOOL		alpha;

	// nanoVG
	int patternImage = -1;
} _Hatch;

typedef struct _SolidFill {
	GpBrush base;
	int color;
	/* We cache the color components */
	double A;
	double R;
	double G;
	double B;
} _SolidFill;

typedef struct _Texture {
	GpBrush		base;
	GpImage* image;
	GpMatrix	matrix;
	GpRect		rectangle;
	GpWrapMode	wrapMode;
	GpPattern* pattern;
} Texture;

typedef struct {
	float* factors;
	float* positions;
	int count;
} Blend;

typedef struct _LineGradient {
	GpBrush			base;
	ARGB			lineColors[2];
	GpPointF		points[2];
	GpRectF			rectangle;
	GpMatrix		matrix;
	GpWrapMode		wrapMode;
	float			angle;
	Blend* blend;
	//InterpolationColors	*presetColors;
	GpPattern* pattern;
	BOOL			isAngleScalable;
	BOOL			gammaCorrection; /* FIXME: Not used */
} LineGradient;


typedef BOOL(*ImageAbort) (void*);
typedef ImageAbort DrawImageAbort;
typedef ImageAbort GetThumbnailImageAbort;

typedef BOOL(*EnumerateMetafileProc) (/*EmfPlusRecordType, UINT, UINT, const BYTE*, VOID**/);

/* Callback function types */
/* FIXME: need a correct definition for these function pointer types */
typedef void* DebugEventProc;
