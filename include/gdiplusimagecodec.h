// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_IMAGECODEC_H
#define __GDIPLUS_IMAGECODEC_H

#include "GdiPlus.h"
#include "gdiplusflat.h"
namespace Gdiplus
{
	static  GpStatus GetImageDecoders(UINT numDecoders, UINT size,
		ImageCodecInfo* decoders)
	{
#ifdef __cplusplus
		return DLLExports::GdipGetImageDecoders(numDecoders, size, decoders);
#else
		return GdipGetImageDecoders(numDecoders, size, decoders);
#endif
	}

	static  GpStatus GetImageDecodersSize(UINT* numDecoders, UINT* size)
	{
#ifdef __cplusplus
		return DLLExports::GdipGetImageDecodersSize(numDecoders, size);
#else
		return GdipGetImageDecodersSize(numDecoders, size);
#endif
	}

	static  GpStatus GetImageEncoders(UINT numEncoders, UINT size,
		ImageCodecInfo* encoders)
	{
#ifdef __cplusplus
		return DLLExports::GdipGetImageEncoders(numEncoders, size, encoders);
#else
		return GdipGetImageEncoders(numEncoders, size, encoders);
#endif
	}

	static  GpStatus GetImageEncodersSize(UINT* numEncoders, UINT* size)
	{
#ifdef __cplusplus
		return DLLExports::GdipGetImageEncodersSize(numEncoders, size);
#else
		return GdipGetImageEncodersSize(numEncoders, size);
#endif
	}
}
#endif /* __GDIPLUS_IMAGECODEC_H */
