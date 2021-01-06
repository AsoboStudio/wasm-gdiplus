// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>

#ifndef __GDIPLUS_PIXELFORMATS_H
#define __GDIPLUS_PIXELFORMATS_H


typedef DWORD ARGB;
typedef INT PixelFormat;
namespace Gdiplus
{
#define ALPHA_SHIFT 24
#define RED_SHIFT   16
#define GREEN_SHIFT 8
#define BLUE_SHIFT  0
#define ALPHA_MASK  ((ARGB) 0xff << ALPHA_SHIFT)

#define    PixelFormatIndexed      0x00010000 // Indexes into a palette
#define    PixelFormatGDI          0x00020000 // Is a GDI-supported format
#define    PixelFormatAlpha        0x00040000 // Has an alpha component
#define    PixelFormatPAlpha       0x00080000 // Pre-multiplied alpha
#define    PixelFormatExtended     0x00100000 // Extended color 16 bits/channel
#define    PixelFormatCanonical    0x00200000 

#define    PixelFormatUndefined       0
#define    PixelFormatDontCare        0

#define    PixelFormat1bppIndexed     (1 | ( 1 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat4bppIndexed     (2 | ( 4 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat8bppIndexed     (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat16bppGrayScale  (4 | (16 << 8) | PixelFormatExtended)
#define    PixelFormat16bppRGB555     (5 | (16 << 8) | PixelFormatGDI)
#define    PixelFormat16bppRGB565     (6 | (16 << 8) | PixelFormatGDI)
#define    PixelFormat16bppARGB1555   (7 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI)
#define    PixelFormat24bppRGB        (8 | (24 << 8) | PixelFormatGDI)
#define    PixelFormat32bppRGB        (9 | (32 << 8) | PixelFormatGDI)
#define    PixelFormat32bppARGB       (10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)
#define    PixelFormat32bppPARGB      (11 | (32 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI)
#define    PixelFormat48bppRGB        (12 | (48 << 8) | PixelFormatExtended)
#define    PixelFormat64bppARGB       (13 | (64 << 8) | PixelFormatAlpha  | PixelFormatCanonical | PixelFormatExtended)
#define    PixelFormat64bppPARGB      (14 | (64 << 8) | PixelFormatAlpha  | PixelFormatPAlpha | PixelFormatExtended)
#define    PixelFormat32bppCMYK       (15 | (32 << 8))
#define    PixelFormatMax             16


	typedef enum PaletteFlags {
		PaletteFlagsHasAlpha = 1,
		PaletteFlagsGrayScale = 2,
		PaletteFlagsHalftone = 4
	} PaletteFlags;

	typedef enum PaletteType {
		PaletteTypeCustom = 0,
		PaletteTypeOptimal = 1,
		PaletteTypeFixedBW = 2,
		PaletteTypeFixedHalftone8 = 3,
		PaletteTypeFixedHalftone27 = 4,
		PaletteTypeFixedHalftone64 = 5,
		PaletteTypeFixedHalftone125 = 6,
		PaletteTypeFixedHalftone216 = 7,
		PaletteTypeFixedHalftone252 = 8,
		PaletteTypeFixedHalftone256 = 9
	} PaletteType;

	typedef struct ColorPalette {
		UINT Flags;
		UINT Count;
		ARGB Entries[1];
	} ColorPalette;

	static  UINT GetPixelFormatSize(PixelFormat pixfmt)
	{
		return (((UINT)pixfmt) & 0xff00U) >> 8;
	}

	static  BOOL IsAlphaPixelFormat(PixelFormat pixfmt)
	{
		return (pixfmt & PixelFormatAlpha) != 0;
	}

	static  BOOL IsCanonicalPixelFormat(PixelFormat pixfmt)
	{
		return (pixfmt & PixelFormatCanonical) != 0;
	}

	static  BOOL IsExtendedPixelFormat(PixelFormat pixfmt)
	{
		return (pixfmt & PixelFormatExtended) != 0;
	}

	static  BOOL IsIndexedPixelFormat(PixelFormat pixfmt)
	{
		return (pixfmt & PixelFormatIndexed) != 0;
	}
}
#endif /* __GDIPLUS_PIXELFORMATS_H */
