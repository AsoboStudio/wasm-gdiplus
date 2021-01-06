/*
 * Copyright (C) 2007 Novell, Inc (http://www.novell.com)
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
 * Authors:
 *          Sebastien Pouliot  <sebastien@ximian.com>
 */

 /*
  * This is *the* public header to be used by C/C++ consumers of GDI+
  * and has the same name as MS provided header for the "flat API"
  */

#ifndef _FLATAPI_H
#define _FLATAPI_H

// Debug defines
// shows clipping mask contour
// #define DEBUG_CLIP_MASK
// shows Text containing bounds and pivot
// #define DEBUG_TEXT
// shows the image drawing contour
// #define DEBUG_IMAGE_CONTOUR
// check for valid parameters on each call. This is safer, but slower and it can hide parameter issues, recommended for RELEASE but not for DEBUG.
// #define GDIFLAT_SAFE

#include "GdiPlusFlatTypes.h"

#include "general.h"

#include "adjustablearrowcap.h"
#include "bitmap.h"
#include "codecs.h"
#include "brush.h"
#include "customlinecap.h"
#include "font.h"
#include "fontcollection.h"
#include "fontfamily.h"
#include "graphics.h"
#include "graphics-path.h"
#include "graphics-pathiterator.h"
#include "hatchbrush.h"
#include "image.h"
#include "imageattributes.h"
#include "lineargradientbrush.h"
#include "matrix.h"
#include "metafile.h"
#include "pathgradientbrush.h"
#include "pen.h"
#include "region.h"
#include "solidbrush.h"
#include "stringformat.h"
#include "text.h"
#include "texturebrush.h"

#endif
