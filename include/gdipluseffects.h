// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#ifndef __GDIPLUS_EFFECTS_H
#define __GDIPLUS_EFFECTS_H

#include <MSFS\MSFS_WindowsTypes.h>
#include "gdiplustypes.h"
#include "gdiplusheaders.h"

namespace Gdiplus {

	typedef enum CurveAdjustments {
		AdjustExposure = 0,
		AdjustDensity = 1,
		AdjustContrast = 2,
		AdjustHighlight = 3,
		AdjustShadow = 4,
		AdjustMidtone = 5,
		AdjustWhiteSaturation = 6,
		AdjustBlackSaturation = 7
	} CurveAdjustments;

	typedef enum CurveChannel {
		CurveChannelAll = 0,
		CurveChannelRed = 1,
		CurveChannelGreen = 2,
		CurveChannelBlue = 3
	} CurveChannel;

	typedef struct BlurParams {
		REAL radius;
		::BOOL expandEdge;
	} BlurParams;

	typedef struct BrightnessContrastParams {
		INT brightnessLevel;
		INT contrastLevel;
	} BrightnessContrastParams;

	typedef struct ColorBalanceParams {
		INT cyanRed;
		INT magentaGreen;
		INT yellowBlue;
	} ColorBalanceParams;

	typedef struct ColorCurveParams {
		CurveAdjustments adjustment;
		CurveChannel channel;
		INT adjustValue;
	} ColorCurveParams;

	typedef struct ColorLUTParams {
		ColorChannelLUT lutB;
		ColorChannelLUT lutG;
		ColorChannelLUT lutR;
		ColorChannelLUT lutA;
	} ColorLUTParams;

	typedef struct HueSaturationLightnessParams {
		INT hueLevel;
		INT saturationLevel;
		INT lightnessLevel;
	} HueSaturationLightnessParams;

	typedef struct LevelsParams {
		INT highlight;
		INT midtone;
		INT shadow;
	} LevelsParams;

	typedef struct RedEyeCorrectionParams {
		::UINT numberOfAreas;
		RECT* areas;
	} RedEyeCorrectionParams;

	typedef struct SharpenParams {
		REAL radius;
		REAL amount;
	} SharpenParams;

	typedef struct TintParams {
		INT hue;
		INT amount;
	} TintParams;

	extern const GUID BlurEffectGuid;                    /* ? */
	extern const GUID BrightnessContrastEffectGuid;      /* ? */
	extern const GUID ColorBalanceEffectGuid;            /* ? */
	extern const GUID ColorCurveEffectGuid;              /* ? */
	extern const GUID ColorLUTEffectGuid;                /* ? */
	extern const GUID ColorMatrixEffectGuid;             /* ? */
	extern const GUID HueSaturationLightnessEffectGuid;  /* ? */
	extern const GUID LevelsEffectGuid;                  /* ? */
	extern const GUID RedEyeCorrectionEffectGuid;        /* ? */
	extern const GUID SharpenEffectGuid;                 /* ? */
	extern const GUID TintEffectGuid;                    /* ? */

}

#endif /* __GDIPLUS_EFFECTS_H */
