// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#pragma once

#ifndef _MSFS_WASM_PROXY_GDIPLUS
#define _MSFS_WASM_PROXY_GDIPLUS
#endif

#include <MSFS\Render\nanovg.h>

#include "gdiplusflat.h"
#include <math.h>

#include "GdiPlusEnums.h"
#include "GdiPlusTypes.h"
#include "GdiPlusImaging.h"
#include "gdiplusinit.h"
#include "gdiplusmetaheader.h"
#include "GdiPlusPixelFormats.h"
#include "GdiPlusColor.h"
#include "gdipluseffects.h"
#include "gdiplusimagecodec.h"

#ifdef __cplusplus
#include "gdiplusbase.h"
#include "gdiplusheaders.h"
#include "gdiplusimageattributes.h"
#include "GdiPlusMatrix.h"
#include "GdiPlusBrush.h"
#include "gdiplusmetafile.h"
#include "GdiPlusPen.h"
#include "GdiPlusStringFormat.h"
#include "GdiPlusPath.h"
#include "GdiPlusGraphics.h"
#include "GdiPlusLineCaps.h"
#include "gdiplusimpl.h"

#endif /* __cplusplus */

