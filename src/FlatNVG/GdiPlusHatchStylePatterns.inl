// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


namespace Gdiplus {

unsigned char patternHatchStyleHorizontal[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyleVertical[8 * 8]{
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0 };

unsigned char patternHatchStyleForwardDiagonal[8 * 8]{
	255,128,  0,  0,  0,  0,  0,128,
	128,255,128,  0,  0,  0,  0,  0,
	  0,128,255,128,  0,  0,  0,  0,
	  0,  0,128,255,128,  0,  0,  0,
	  0,  0,  0,128,255,128,  0,  0,
	  0,  0,  0,  0,128,255,128,  0,
	  0,  0,  0,  0,  0,128,255,128,
	128,  0,  0,  0,  0,  0,128,255 };

unsigned char patternHatchStyleBackwardDiagonal[8 * 8]{
	128,  0,  0,  0,  0,  0,128,255,
	  0,  0,  0,  0,  0,128,255,128,
	  0,  0,  0,  0,128,255,128,  0,
	  0,  0,  0,128,255,128,  0,  0,
	  0,  0,128,255,128,  0,  0,  0,
	  0,128,255,128,  0,  0,  0,  0,
	128,255,128,  0,  0,  0,  0,  0,
	255,128,  0,  0,  0,  0,  0,128 };


unsigned char patternHatchStyleCross[8 * 8]{
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0 };

unsigned char patternHatchStyleDiagonalCross[8 * 8]{
	255,128,  0,  0,  0,  0,128,255,
	128,255,128,  0,  0,128,255,128,
	  0,128,255,128,128,255,128,  0,
	  0,  0,128,255,255,128,  0,  0,
	  0,  0,128,255,255,128,  0,  0,
	  0,128,255,128,128,255,128,  0,
	128,255,128,  0,  0,128,255,128,
	255,128,  0,  0,  0,  0,128,255 };

unsigned char patternHatchStyle05Percent[8 * 8]{
	255,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyle10Percent[8 * 8]{
	  0,  0,  0,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,255,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,255,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyle20Percent[8 * 8]{
	255,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyle25Percent[8 * 8]{
	255,  0,  0,  0,255,  0,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0 };

unsigned char patternHatchStyle30Percent[8 * 8]{
	255,  0,  0,  0,255,  0,  0,  0,
	  0,255,  0,255,  0,255,  0,255,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,  0,  0,255,  0,  0,  0,
	  0,255,  0,255,  0,255,  0,255,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255 };

unsigned char patternHatchStyle40Percent[8 * 8]{
	255,  0,  0,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,255,  0,255,  0,  0,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255 };

unsigned char patternHatchStyle50Percent[8 * 8]{
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255 };

unsigned char patternHatchStyle60Percent[8 * 8]{
	255,  0,255,  0,255,  0,255,  0,
	255,255,  0,255,255,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,255,255,  0,255,255,255,
	255,  0,255,  0,255,  0,255,  0,
	255,255,  0,255,255,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,255,255,  0,255,255,255 };

unsigned char patternHatchStyle70Percent[8 * 8]{
	  0,255,255,255,  0,255,255,255,
	255,  0,255,  0,255,  0,255,  0,
	255,255,  0,255,255,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,255,255,  0,255,255,255,
	255,  0,255,  0,255,  0,255,  0,
	255,255,  0,255,255,255,  0,255,
	255,  0,255,  0,255,  0,255,  0 };

unsigned char patternHatchStyle75Percent[8 * 8]{
	  0,255,255,255,  0,255,255,255,
	255,255,  0,255,255,255,  0,255,
	  0,255,255,255,  0,255,255,255,
	255,255,  0,255,255,255,  0,255,
	  0,255,255,255,  0,255,255,255,
	255,255,  0,255,255,255,  0,255,
	  0,255,255,255,  0,255,255,255,
	255,255,  0,255,255,255,  0,255 };

unsigned char patternHatchStyle80Percent[8 * 8]{
	255,255,255,255,255,255,  0,255,
	255,255,255,255,255,255,255,255,
	255,255,  0,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,  0,255,
	255,255,255,255,255,255,255,255,
	255,255,  0,255,255,255,255,255,
	255,255,255,255,255,255,255,255 };

unsigned char patternHatchStyle90Percent[8 * 8]{
	  0,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,  0,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255 };

unsigned char patternHatchStyleLightUpwardDiagonal[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,255,
	  0,  0,  0,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,255,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,255,  0,  0,  0,  0,  0,
	  0,255,  0,  0,  0,  0,  0,  0,
	255,  0,  0,  0,  0,  0,  0,  0 };



unsigned char patternHatchStyleLightDownwardDiagonal[8 * 8]{
	255, 0,  0, 0, 0, 0, 0, 0,
	  0,255, 0, 0, 0, 0, 0, 0,
	  0, 0,255, 0, 0, 0, 0, 0,
	  0, 0, 0,255, 0, 0, 0, 0,
	  0, 0, 0, 0,255, 0, 0, 0,
	  0, 0, 0, 0, 0,255, 0, 0,
	  0, 0, 0, 0, 0, 0,255, 0,
	  0, 0, 0, 0, 0, 0, 0,255 };

unsigned char patternHatchStyleDarkUpwardDiagonal[8 * 8]{
	  0,  0,  0,  0,  0,  0,255,255,
	  0,  0,  0,  0,  0,255,255,  0,
	  0,  0,  0,  0,255,255,  0,  0,
	  0,  0,  0,255,255,  0,  0,  0,
	  0,  0,255,255,  0,  0,  0,  0,
	  0,255,255,  0,  0,  0,  0,  0,
	255,255,  0,  0,  0,  0,  0,  0,
	255,  0,  0,  0,  0,  0,  0,  255 };

unsigned char patternHatchStyleDarkDownwardDiagonal[8 * 8]{
	255,255, 0, 0, 0, 0, 0,255,
	  0,255,255, 0, 0, 0, 0, 0,
	  0, 0,255,255, 0, 0, 0, 0,
	  0, 0, 0,255,255, 0, 0, 0,
	  0, 0, 0, 0,255,255, 0, 0,
	  0, 0, 0, 0, 0,255,255, 0,
	  0, 0, 0, 0, 0, 0,255,255,
	255, 0, 0, 0, 0, 0, 0,255, };

unsigned char patternHatchStyleWideDownwardDiagonal[8 * 8]{
	255,255,  0,  0,  0,  0,  0,255,
	255,255,255,  0,  0,  0,  0,  0,
	  0,255,255,255,  0,  0,  0,  0,
	  0,  0,255,255,255,  0,  0,  0,
	  0,  0,  0,255,255,255,  0,  0,
	  0,  0,  0,  0,255,255,255,  0,
	  0,  0,  0,  0,  0,255,255,255,
	255,  0,  0,  0,  0,  0,255,255 };

unsigned char patternHatchStyleWideUpwardDiagonal[8 * 8]{
	255,  0,  0,  0,  0,  0,255,255,
	  0,  0,  0,  0,  0,255,255,255,
	  0,  0,  0,  0,255,255,255,  0,
	  0,  0,  0,255,255,255,  0,  0,
	  0,  0,255,255,255,  0,  0,  0,
	  0,255,255,255,  0,  0,  0,  0,
	255,255,255,  0,  0,  0,  0,  0,
	255,255,  0,  0,  0,  0,  0,255 };

unsigned char patternHatchStyleLightHorizontal[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255 };

unsigned char patternHatchStyleLightVertical[8 * 8]{
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,255 };

unsigned char patternHatchStyleNarrowHorizontal[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255 };

unsigned char patternHatchStyleNarrowVertical[8 * 8]{
	  0,255,  0,255,  0,255,  0,255,
	  0,255,  0,255,  0,255,  0,255,
	  0,255,  0,255,  0,255,  0,255,
	  0,255,  0,255,  0,255,  0,255,
	  0,255,  0,255,  0,255,  0,255,
	  0,255,  0,255,  0,255,  0,255,
	  0,255,  0,255,  0,255,  0,255,
	  0,255,  0,255,  0,255,  0,255 };

unsigned char patternHatchStyleDarkHorizontal[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255 };

unsigned char patternHatchStyleDarkVertical[8 * 8]{
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255 };

unsigned char patternHatchStyleDashedDownwardDiagonal[8 * 8]{
	255,  0,  0,  0,255,  0,  0,  0,
	  0,255,  0,  0,  0,255,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyleDashedUpwardDiagonal[8 * 8]{
	  0,  0,  0,255,  0,  0,  0,255,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,255,  0,  0,  0,255,  0,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyleDashedHorizontal[8 * 8]{
	255,255,255,255,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,255,255,255,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyleDashedVertical[8 * 8]{
	255,  0,  0,  0,  0,  0,  0,  0,
	255,  0,  0,  0,  0,  0,  0,  0,
	255,  0,  0,  0,  0,  0,  0,  0,
	255,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0 };

unsigned char patternHatchStyleSmallConfetti[8 * 8]{
	255,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,255,  0,  0,
	  0,  0,255,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,255,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,255,  0,
	  0,255,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0 };

unsigned char patternHatchStyleLargeConfetti[8 * 8]{
	255,255,  0,  0,  0,  0,  0,  0,
	255,255,  0,  0,  0,255,255,  0,
	  0,  0,255,255,  0,255,255,  0,
	  0,  0,255,255,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,255,255,
	  0,255,255,  0,  0,  0,255,255,
	  0,255,255,  0,255,255,  0,  0,
	  0,  0,  0,  0,255,255,  0,  0 };

unsigned char patternHatchStyleZigZag[8 * 8]{
	  0,  0,  0,255,255,  0,  0,  0,
	  0,  0,255,  0,  0,255,  0,  0,
	  0,255,  0,  0,  0,  0,255,  0,
	255,  0,  0,  0,  0,  0,  0,255,
	  0,  0,  0,255,255,  0,  0,  0,
	  0,  0,255,  0,  0,255,  0,  0,
	  0,255,  0,  0,  0,  0,255,  0,
	255,  0,  0,  0,  0,  0,  0,255 };

unsigned char patternHatchStyleWave[8 * 8]{
	  0,  0,  0,  0,255,255,  0,  0,
	255,  0,  0,255,  0,  0,255,  0,
	  0,255,255,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,255,  0,  0,
	255,  0,  0,255,  0,  0,255,  0,
	  0,255,255,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };


unsigned char patternHatchStyleDiagonalBrick[8 * 8]{
	255,  0,  0,  0,  0,  0,  0,255,
	  0,255,  0,  0,  0,  0,255,  0,
	  0,  0,255,  0,  0,255,  0,  0,
	  0,  0,  0,255,255,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,255,  0,  0,  0,  0,  0,
	  0,255,  0,  0,  0,  0,  0,  0,
	255,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyleHorizontalBrick[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,255,
	  0,  0,  0,  0,  0,  0,  0,255,
	  0,  0,  0,  0,  0,  0,  0,255,
	255,255,255,255,255,255,255,255,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	255,255,255,255,255,255,255,255 };

unsigned char patternHatchStyleWeave[8 * 8]{
	255,  0,  0,  0,  0,  0,255,  0,
	  0,255,  0,  0,  0,255,  0,  0,
	  0,  0,255,  0,255,  0,255,  0,
	  0,  0,  0,255,  0,  0,  0,255,
	255,  0,  0,  0,255,  0,255,  0,
	  0,255,  0,  0,  0,255,  0,  0,
	255,  0,255,  0,255,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,255 };

unsigned char patternHatchStylePlaid[8 * 8]{
	255,255,255,255,  0,  0,  0,  0,
	255,255,255,255,  0,  0,  0,  0,
	255,255,255,255,  0,  0,  0,  0,
	255,255,255,255,  0,  0,  0,  0,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255,
	255,  0,255,  0,255,  0,255,  0,
	  0,255,  0,255,  0,255,  0,255 };

unsigned char patternHatchStyleDivot[8 * 8]{
	  0,255,  0,  0,  0,  0,  0,  0,
	  0,  0,255,  0,  0,  0,  0,  0,
	  0,255,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,255,  0,  0,
	  0,  0,  0,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };

unsigned char patternHatchStyleDottedGrid[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	255,  0,255,  0,255,  0,255,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,255,  0,  0,  0 };

unsigned char patternHatchStyleDottedDiamond[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,255,  0,  0,  0,255,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,255,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,255,  0,  0,  0,255,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,255 };

unsigned char patternHatchStyleShingle[8 * 8]{
	255,  0,  0,  0,  0,255,  0,  0,
	  0,255,  0,  0,255,  0,  0,  0,
	  0,  0,255,255,  0,  0,  0,  0,
	  0,  0,  0,  0,255,255,  0,  0,
	  0,  0,  0,  0,  0,  0,255,  0,
	  0,  0,  0,  0,  0,  0,  0,255,
	  0,  0,  0,  0,  0,  0,  0,255,
	  0,  0,  0,  0,  0,  0,255,255 };

unsigned char patternHatchStyleTrellis[8 * 8]{
	255,255,255,255,255,255,255,255,
	  0,  0,255,255,  0,  0,255,255,
	255,255,255,255,255,255,255,255,
	255,255,  0,  0,255,255,  0,  0,
	255,255,255,255,255,255,255,255,
	  0,  0,255,255,  0,  0,255,255,
	255,255,255,255,255,255,255,255,
	255,255,  0,  0,255,255,  0,  0 };


unsigned char patternHatchStyleSphere[8 * 8]{
	  0,255,255,255,  0,255,255,255,
	255,  0,  0,255,255,  0,  0,  0,
	255,255,255,255,255,  0,  0,  0,
	255,255,255,255,255,  0,  0,  0,
	  0,255,255,255,  0,255,255,255,
	255,  0,  0,  0,255,  0,  0,  0,
	255,  0,  0,  0,255,255,255,255,
	255,  0,  0,  0,255,255,255,255 };


unsigned char patternHatchStyleSmallGrid[8 * 8]{
	255,  0,  0,  0,255,  0,  0,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	255,255,255,255,255,255,255,255,
	255,  0,  0,  0,255,  0,  0,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	255,  0,  0,  0,255,  0,  0,  0,
	255,255,255,255,255,255,255,255 };


unsigned char patternHatchStyleSmallCheckerBoard[8 * 8]{
	255,255,  0,  0,255,255,  0,  0,
	255,255,  0,  0,255,255,  0,  0,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255,
	255,255,  0,  0,255,255,  0,  0,
	255,255,  0,  0,255,255,  0,  0,
	  0,  0,255,255,  0,  0,255,255,
	  0,  0,255,255,  0,  0,255,255 };


unsigned char patternHatchStyleLargeCheckerBoard[8 * 8]{
	255,255,255,255,  0,  0,  0,  0,
	255,255,255,255,  0,  0,  0,  0,
	255,255,255,255,  0,  0,  0,  0,
	255,255,255,255,  0,  0,  0,  0,
	  0,  0,  0,  0,255,255,255,255,
	  0,  0,  0,  0,255,255,255,255,
	  0,  0,  0,  0,255,255,255,255,
	  0,  0,  0,  0,255,255,255,255 };



unsigned char patternHatchStyleOutlinedDiamond[8 * 8]{
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,255,  0,255,  0,  0,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,255,  0,  0,  0,  0,  0,255,
	255,  0,  0,  0,  0,  0,  0,  0,
	  0,255,  0,  0,  0,  0,  0,255,
	  0,  0,255,  0,  0,  0,255,  0,
	  0,  0,  0,255,  0,255,  0,  0 };


unsigned char patternHatchStyleSolidDiamond[8 * 8]{
	  0,  0,  0,  0,255,  0,  0,  0,
	  0,  0,  0,255,255,255,  0,  0,
	  0,  0,255,255,255,255,255,  0,
	  0,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	  0,255,255,255,255,255,255,255,
	  0,  0,255,255,255,255,255,  0,
	  0,  0,  0,255,255,255,  0,  0 };


/*
unsigned char patternHatpatternHatchStyleTotal[8 * 8]{
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255 };
*/

/*
unsigned char patternHatchStyleEmpty[8 * 8]{
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0 };
	*/

unsigned char data[8 * 8 * 4]{
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

// Generate brush textures based on https://docs.microsoft.com/en-us/windows/win32/api/gdiplusenums/ne-gdiplusenums-hatchstyle
int nvgGenerateBrushPattern(NVGcontext* ctx, HatchStyle hatchStyle)
{
	if (!ctx) {
		return -1;
	}
	int w = 8, h = 8, d = 4;
	unsigned char* pattern = nullptr;
	switch (hatchStyle)
	{
	case HatchStyleHorizontal:
		pattern = patternHatchStyleHorizontal;
		break;
	case HatchStyleVertical:
		pattern = patternHatchStyleVertical;
		break;
	case HatchStyleForwardDiagonal:
		pattern = patternHatchStyleForwardDiagonal;
		break;
	case HatchStyleBackwardDiagonal:
		pattern = patternHatchStyleBackwardDiagonal;
		break;
	case HatchStyleCross:
		pattern = patternHatchStyleCross;
		break;
	case HatchStyleDiagonalCross:
		pattern = patternHatchStyleDiagonalCross;
		break;
	case HatchStyle05Percent:
		pattern = patternHatchStyle05Percent;
		break;
	case HatchStyle10Percent:
		pattern = patternHatchStyle10Percent;
		break;
	case HatchStyle20Percent:
		pattern = patternHatchStyle20Percent;
		break;
	case HatchStyle25Percent:
		pattern = patternHatchStyle25Percent;
		break;
	case HatchStyle30Percent:
		pattern = patternHatchStyle30Percent;
		break;
	case HatchStyle40Percent:
		pattern = patternHatchStyle40Percent;
		break;
	case HatchStyle50Percent:
		pattern = patternHatchStyle50Percent;
		break;
	case HatchStyle60Percent:
		pattern = patternHatchStyle60Percent;
		break;
	case HatchStyle70Percent:
		pattern = patternHatchStyle70Percent;
		break;
	case HatchStyle75Percent:
		pattern = patternHatchStyle75Percent;
		break;
	case HatchStyle80Percent:
		pattern = patternHatchStyle80Percent;
		break;
	case HatchStyle90Percent:
		pattern = patternHatchStyle90Percent;
		break;
	case HatchStyleLightDownwardDiagonal:
		pattern = patternHatchStyleLightDownwardDiagonal;
		break;
	case HatchStyleLightUpwardDiagonal:
		pattern = patternHatchStyleLightUpwardDiagonal;
		break;
	case HatchStyleDarkDownwardDiagonal:
		pattern = patternHatchStyleDarkDownwardDiagonal;
		break;
	case HatchStyleDarkUpwardDiagonal:
		pattern = patternHatchStyleDarkUpwardDiagonal;
		break;
	case HatchStyleWideDownwardDiagonal:
		pattern = patternHatchStyleWideDownwardDiagonal;
		break;
	case HatchStyleWideUpwardDiagonal:
		pattern = patternHatchStyleWideUpwardDiagonal;
		break;
	case HatchStyleLightVertical:
		pattern = patternHatchStyleLightVertical;
		break;
	case HatchStyleLightHorizontal:
		pattern = patternHatchStyleLightHorizontal;
		break;
	case HatchStyleNarrowVertical:
		pattern = patternHatchStyleNarrowVertical;
		break;
	case HatchStyleNarrowHorizontal:
		pattern = patternHatchStyleNarrowHorizontal;
		break;
	case HatchStyleDarkVertical:
		pattern = patternHatchStyleNarrowVertical;
		break;
	case HatchStyleDarkHorizontal:
		pattern = patternHatchStyleDarkHorizontal;
		break;
	case HatchStyleDashedDownwardDiagonal:
		pattern = patternHatchStyleDashedDownwardDiagonal;
		break;
	case HatchStyleDashedUpwardDiagonal:
		pattern = patternHatchStyleDashedUpwardDiagonal;
		break;
	case HatchStyleDashedHorizontal:
		pattern = patternHatchStyleDashedHorizontal;
		break;
	case HatchStyleDashedVertical:
		pattern = patternHatchStyleDashedVertical;
		break;
	case HatchStyleSmallConfetti:
		pattern = patternHatchStyleSmallConfetti;
		break;
	case HatchStyleLargeConfetti:
		pattern = patternHatchStyleLargeConfetti;
		break;
	case HatchStyleZigZag:
		pattern = patternHatchStyleZigZag;
		break;
	case HatchStyleWave:
		pattern = patternHatchStyleWave;
		break;
	case HatchStyleDiagonalBrick:
		pattern = patternHatchStyleDiagonalBrick;
		break;
	case HatchStyleHorizontalBrick:
		pattern = patternHatchStyleHorizontalBrick;
		break;
	case HatchStyleWeave:
		pattern = patternHatchStyleWeave;
		break;
	case HatchStylePlaid:
		pattern = patternHatchStylePlaid;
		break;
	case HatchStyleDivot:
		pattern = patternHatchStyleDivot;
		break;
	case HatchStyleDottedGrid:
		pattern = patternHatchStyleDottedGrid;
		break;
	case HatchStyleDottedDiamond:
		pattern = patternHatchStyleDottedDiamond;
		break;
	case HatchStyleShingle:
		pattern = patternHatchStyleShingle;
		break;
	case HatchStyleTrellis:
		pattern = patternHatchStyleTrellis;
		break;
	case HatchStyleSphere:
		pattern = patternHatchStyleSphere;
		break;
	case HatchStyleSmallGrid:
		pattern = patternHatchStyleSmallGrid;
		break;
	case HatchStyleSmallCheckerBoard:
		pattern = patternHatchStyleSmallCheckerBoard;
		break;
	case HatchStyleLargeCheckerBoard:
		pattern = patternHatchStyleLargeCheckerBoard;
		break;
	case HatchStyleOutlinedDiamond:
		pattern = patternHatchStyleOutlinedDiamond;
		break;
	case HatchStyleSolidDiamond:
		pattern = patternHatchStyleSolidDiamond;
		break;
	default:
		//case HatchStyleTotal:
		//case HatchStyleMin:
		//case HatchStyleMax:
		w = h = 0;
		pattern = nullptr;
		break;
	}
	if (pattern != nullptr) {
		// The pattern should be either :
		// * a unique image with single channel (lerp information between colors), or
		// * one image per brush, with the final colored pattern including the brush colors

		int dataIndex = 0;
		int pixelIndex = 0;
		for (int x = 0; x < w; ++x) {
			for (int y = 0; y < h; ++y) {
				unsigned char p = pattern[pixelIndex];
				for (int c = 0; c < 4; ++c) {
					data[dataIndex++] = p;
				}

				++pixelIndex;
			}
		}
		int img = nvgCreateImageRGBA(ctx, 8, 8, NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY, data);

		return img;
	}
	else {
		return -1;
	}
}
}