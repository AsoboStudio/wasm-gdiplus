// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_H
#define __GDIPLUS_H

#include <MSFS\MSFS_WindowsTypes.h>
#include <string.h>
#include "MSFS\Render\nanovg.h"

#ifndef _COM_interface
#define _COM_interface struct
#endif

typedef _COM_interface IStream IStream;
typedef _COM_interface IDirectDrawSurface7 IDirectDrawSurface7;

typedef float REAL;
typedef SHORT INT16;
typedef WORD UINT16;

#include "gdiplusenums.h"
#include "gdiplustypes.h"
#include "gdiplusgpstubs.h"
#include "gdiplusimaging.h"
#include "gdiplusmem.h"
#include "gdipluspixelformats.h"
#include "gdipluscolor.h"
#include "gdipluscolormatrix.h"
#include "gdipluseffects.h"


#ifdef __cplusplus
#include "gdiplusbase.h"
#include "gdiplusheaders.h"
#include "gdiplusmatrix.h"
#include "gdiplusbrush.h"
#include "gdipluspen.h"
#include "gdiplusstringformat.h"
#include "gdipluspath.h"
#include "gdiplusgraphics.h"
#include "gdipluslinecaps.h"


#endif /* __cplusplus */


#endif /* __GDIPLUS_H */
