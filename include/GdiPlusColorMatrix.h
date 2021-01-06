// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_COLORMATRIX_H
#define __GDIPLUS_COLORMATRIX_H

#include <MSFS\MSFS_WindowsTypes.h>
#include "gdipluscolor.h"

namespace Gdiplus {

	typedef enum ColorAdjustType {
		ColorAdjustTypeDefault = 0,
		ColorAdjustTypeBitmap = 1,
		ColorAdjustTypeBrush = 2,
		ColorAdjustTypePen = 3,
		ColorAdjustTypeText = 4,
		ColorAdjustTypeCount = 5,
		ColorAdjustTypeAny = 6
	} ColorAdjustType;

	typedef enum ColorMatrixFlags {
		ColorMatrixFlagsDefault = 0,
		ColorMatrixFlagsSkipGrays = 1,
		ColorMatrixFlagsAltGray = 2
	} ColorMatrixFlags;

	typedef enum HistogramFormat {
		HistogramFormatARGB = 0,
		HistogramFormatPARGB = 1,
		HistogramFormatRGB = 2,
		HistogramFormatGray = 3,
		HistogramFormatB = 4,
		HistogramFormatG = 5,
		HistogramFormatR = 6,
		HistogramFormatA = 7
	} HistogramFormat;

	typedef struct ColorMap {
		Color oldColor;
		Color newColor;
	} ColorMap;

	typedef struct ColorMatrix {
		REAL m[5][5];
	} ColorMatrix;

	typedef BYTE ColorChannelLUT[256];

#endif /* __GDIPLUS_COLORMATRIX_H */

}