/* -*- Mode: c; c-basic-offset: 4; indent-tabs-mode: t; tab-width: 4; -*-
 *
 * image.cpp
 *
 * Copyright (c) 2003 Alexandre Pigolkine
 * Copyright (C) 2007 Novell, Inc (http://www.novell.com)
 * Copyright (C) 2020 Asobo Studio.
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
 *   Alexandre Pigolkine(pigolkine@gmx.de)
 *   Sanjay Gupta (gsanjay@novell.com)
 *   Vladimir Vukicevic (vladimir@pobox.com)
 *   Jordi Mas (jordi@ximian.com)
 *   Jonathan Gilbert (logic@deltaq.org)
 *   Sebastien Pouliot  <sebastien@ximian.com>
 *	 Sol Roo (sroo@asobostudio.com)
 */

#include "gdiplusflat.h"
#include "FlatNVG\general-private.h"
#include "FlatNVG\bitmap-private.h"
#include "FlatNVG\graphics-private.h"

// WASM TODO: optimize this method. Reloading the image is not ideal.
inline bool gdip_load_image(GpGraphics* graphics, GpBitmap* bitmap) {

	if (!bitmap->bStatic) {
		if (bitmap->ctx == NULL)
		{
			bitmap->ctx = graphics->ctx;
		}
		else if (bitmap->ctx != graphics->ctx && bitmap->hImage != -1) {
			nvgDeleteImage(bitmap->ctx, bitmap->hImage);
			bitmap->hImage = -1;
			bitmap->ctx = graphics->ctx;
		}

		if (bitmap->path != NULL)
		{
			if (bitmap->hImage == -1) {
				bitmap->hImage = nvgCreateImage(graphics->ctx, bitmap->path->c_str(), 0);
				nvgImageSize(graphics->ctx, bitmap->hImage, &bitmap->iWidth, &bitmap->iHeight);
			}
		}
		else if (bitmap->scan0 != NULL) {

			if (bitmap->hImage == -1) {
				bitmap->hImage = nvgCreateImageRGBA(bitmap->ctx, bitmap->iWidth, bitmap->iHeight, 0, bitmap->scan0);
			}
			else if (bitmap->bDirty) {
				nvgUpdateImage(bitmap->ctx, bitmap->hImage, bitmap->scan0);
				bitmap->bDirty = false;
			}
		}
	}
	return (bitmap->hImage != -1);
}

inline void releaseImage(GpBitmap* bitmap) {
	if (bitmap->scan0) {
		DLLExports::GdipFree(bitmap->scan0);
	}
	if (bitmap->path) {
		delete (bitmap->path);
	}
	//WASM TODO: fix: this should be called before context destruction
	/*if (bitmap->ctx != NULL && bitmap->hImage != -1) {
		nvgDeleteImage(bitmap->ctx, bitmap->hImage);
	}*/
}


namespace DLLExports {

	GpStatus WINGDIPAPI
		GdipLoadImage(GpGraphics* graphics, GpImage* image) {

		if (!image || !image->ctx || image->hImage == -1)
			return GpStatus::InvalidParameter;

		if (gdip_load_image(graphics, image)) {
			image->bStatic = TRUE;
			return GpStatus::Ok;
		}
		else {
			return GpStatus::InvalidParameter;
		}
	}
	GpStatus WINGDIPAPI
		GdipDisposeImage(GpImage* image)
	{
		if (!image || !image->ctx || image->hImage == -1)
			return GpStatus::InvalidParameter;

		releaseImage(image);
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetImageGraphicsContext(GpImage* image, GpGraphics** graphics) {
		*graphics = NULL;
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDrawImageI(GpGraphics* graphics, GpImage* image, INT x, INT y) {
		return GdipDrawImage(graphics, image, x, y);
	}

	GpStatus WINGDIPAPI
		GdipDrawImage(GpGraphics* graphics, GpImage* image, REAL x, REAL y) {
#ifdef GDIFLAT_SAFE
		if (!graphics || !image)
			return GpStatus::InvalidParameter;
#endif
		//if (graphics->state == GraphicsStateBusy)
		//	return ObjectBusy;

		return GdipDrawImageRect(graphics, image, x, y, image->iWidth, image->iHeight);
	}

	GpStatus WINGDIPAPI
		GdipDrawImageRectI(GpGraphics* graphics, GpImage* image, INT x, INT y, INT width, INT height) {

		return GdipDrawImageRect(graphics, image, x, y, width, height);
	}

	GpStatus WINGDIPAPI
		GdipDrawImageRect(GpGraphics* graphics, GpImage* image, REAL x, REAL y, REAL width, REAL height) {
#ifdef GDIFLAT_SAFE
		if (!graphics || !image)
			return GpStatus::InvalidParameter;
#endif
		//if (graphics->state == GraphicsStateBusy)
		//	return ObjectBusy;

		gdip_load_image(graphics, image);

		if (image->hImage == -1) {
			// image not loaded
			return GpStatus::InvalidParameter;
		}
		NVGpaint imgPaint = nvgImagePattern(graphics->ctx, x, y, width, height, 0.0f, image->hImage, 1.0f);
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, x, y, width, height);
		nvgFillPaint(graphics->ctx, imgPaint);
		nvgFill(graphics->ctx);

#ifdef DEBUG_IMAGE_CONTOUR
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, x, y, width, height);
		nvgStrokeColor(graphics->ctx, nvgRGBA(255, 0, 0, 255));
		nvgStroke(graphics->ctx);
#endif
		return GpStatus::Ok;
	}

	/* points are upper left, upper right, lower left. fourth point is extrapolated. */
	GpStatus WINGDIPAPI
		GdipDrawImagePoints(GpGraphics* graphics, GpImage* image, GDIPCONST GpPointF* dstPoints, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDrawImagePointsI(GpGraphics* graphics, GpImage* image, GDIPCONST GpPoint* dstPoints, INT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipDrawImagePointRect(GpGraphics* graphics, GpImage* image,
			REAL x, REAL y, REAL srcx, REAL srcy,
			REAL srcwidth, REAL srcheight,
			GpUnit srcUnit) {

		return GdipDrawImageRectRect(graphics, image,
			x, y, srcwidth, srcheight,
			srcx, srcy, srcwidth, srcheight,
			srcUnit, NULL, NULL, NULL);
	}

	GpStatus WINGDIPAPI
		GdipDrawImagePointRectI(GpGraphics* graphics, GpImage* image,
			INT x, INT y, INT srcx, INT srcy,
			INT srcwidth, INT srcheight,
			GpUnit srcUnit) {
		return GdipDrawImagePointRect(graphics, image, x, y, srcx, srcy,
			srcwidth, srcheight, srcUnit);
	}

	GpStatus WINGDIPAPI
		GdipDrawImageRectRect(GpGraphics* graphics, GpImage* image,
			REAL dstx, REAL dsty, REAL dstwidth, REAL dstheight,
			REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight,
			GpUnit srcUnit,
			GDIPCONST GpImageAttributes* imageAttributes,
			DrawImageAbort callback, void* callbackData) {
#ifdef GDIFLAT_SAFE
		if (!graphics || !image)
			return GpStatus::InvalidParameter;
#endif
		//if (graphics->state == GraphicsStateBusy)
		//	return ObjectBusy;
		gdip_load_image(graphics, image);

		if (image->hImage == -1 ) {
			// image not loaded
			return GpStatus::InvalidParameter;
		}

		NVGpaint imgPaint = nvgImagePattern(graphics->ctx, dstx - srcx, dsty - srcy, srcwidth, srcheight, 0.0f, image->hImage, 1.0f);
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, dstx, dsty, dstwidth, dstheight);
		nvgFillPaint(graphics->ctx, imgPaint);
		nvgFill(graphics->ctx);

#ifdef DEBUG_IMAGE_CONTOUR
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, dstx, dsty, dstwidth, dstheight);
		nvgStrokeColor(graphics->ctx, nvgRGBA(255, 0, 0, 255));
		nvgStroke(graphics->ctx);
		nvgBeginPath(graphics->ctx);
		nvgRect(graphics->ctx, srcx + dstx, srcy + dsty, srcwidth, srcheight);
		nvgStrokeColor(graphics->ctx, nvgRGBA(0, 0, 255, 255));
		nvgStroke(graphics->ctx);
#endif
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipDrawImageRectRectI(GpGraphics* graphics, GpImage* image, INT dstx, INT dsty, INT dstwidth, INT dstheight,
			INT srcx, INT srcy, INT srcwidth, INT srcheight, GpUnit srcUnit, GDIPCONST GpImageAttributes* imageAttributes,
			DrawImageAbort callback, void* callbackData) {
		return GdipDrawImageRectRect(graphics, image,
			dstx, dsty, dstwidth, dstheight,
			srcx, srcy, srcwidth, srcheight,
			srcUnit, imageAttributes,
			callback, callbackData);
	}

	GpStatus WINGDIPAPI
		GdipDrawImagePointsRect(GpGraphics* graphics, GpImage* image, GDIPCONST GpPointF* points, INT count, REAL srcx, REAL srcy,
			REAL srcwidth, REAL srcheight, GpUnit srcUnit, GDIPCONST GpImageAttributes* imageAttributes, DrawImageAbort callback,
			void* callbackData) {

		GpRectF rect;
		GpStatus status;
		GpMatrix* matrix = NULL;

		if (!graphics || !points || count <= 0)
			return GpStatus::InvalidParameter;
		if (graphics->state == GraphicsStateBusy)
			return GpStatus::ObjectBusy;
		if (!image || (count != 3 && count != 4))
			return GpStatus::InvalidParameter;

		switch (srcUnit) {
		case GpUnit::UnitPixel:
			break;
		case GpUnit::UnitPoint:
		case GpUnit::UnitInch:
		case GpUnit::UnitDocument:
		case GpUnit::UnitMillimeter:
			//if (graphics->type != gtPostScript)
			//	return NotImplemented; /* GDI+ returns the same */
			break;
		case GpUnit::UnitWorld:
		case GpUnit::UnitDisplay:
		default:
			return GpStatus::InvalidParameter;
		}

		if (count == 4)
			return GpStatus::NotImplemented;

		/* Short circuit empty destination rectangle to avoid creating non-invertible matrix */
		if (points[2].X + points[1].X - points[0].X - points[0].X == 0 &&
			points[2].Y + points[1].Y - points[0].Y - points[0].Y == 0) {
			return GpStatus::Ok;
		}

		rect.Width = image->iWidth;
		rect.Height = image->iHeight;


		status = GdipCreateMatrix3(&rect, points, &matrix);
		if (status != GpStatus::Ok) {
			GdipDeleteMatrix(matrix);
			return status;
		}

		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, matrix->m[0], matrix->m[1], matrix->m[2], matrix->m[3], matrix->m[4], matrix->m[5]);

		status = GdipDrawImageRectRect(graphics, image, rect.X, rect.Y, rect.Width, rect.Height, srcx, srcy,
			srcwidth, srcheight, srcUnit, imageAttributes, callback, callbackData);

		nvgResetTransform(graphics->ctx);
		nvgTransform(graphics->ctx, graphics->copy_of_ctm->m[0], graphics->copy_of_ctm->m[1], graphics->copy_of_ctm->m[2], graphics->copy_of_ctm->m[3], graphics->copy_of_ctm->m[4], graphics->copy_of_ctm->m[5]);

		GdipDeleteMatrix(matrix);
		return status;
	}

	GpStatus WINGDIPAPI
		GdipDrawImagePointsRectI(GpGraphics* graphics, GpImage* image, GDIPCONST GpPoint* points, INT count, INT srcx, INT srcy,
			INT srcwidth, INT srcheight, GpUnit srcUnit, GDIPCONST GpImageAttributes* imageAttributes, DrawImageAbort callback,
			void* callbackData) {

		GpPointF* pointsF;

		if (!points || count < 0)
			return GpStatus::InvalidParameter;

		pointsF = convert_points(points, count);
		if (!pointsF)
			return GpStatus::OutOfMemory;

		GpStatus status = GdipDrawImagePointsRect(graphics, image, pointsF, count, srcx, srcy, srcwidth, srcheight,
			srcUnit, imageAttributes, callback, callbackData);
		GdipFree(pointsF);
		return status;
	}

	/*
	 * These two will never be implemented, as 'stream' is a COM IStream
	 */
	GpStatus WINGDIPAPI
		GdipLoadImageFromStream(void* stream, GpImage** image) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipLoadImageFromStreamICM(void* stream, GpImage** image) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSaveImageToStream(GpImage* image, void* stream, GDIPCONST CLSID* encoderCLSID, GDIPCONST GpEncoderParameters* params) {
		return GpStatus::NotImplemented;
	}

	/* coverity[+alloc : arg-*1] */
	GpStatus WINGDIPAPI
		GdipLoadImageFromFile(GDIPCONST WCHAR* file, GpImage** image) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipSaveImageToFile(GpImage* image, GDIPCONST WCHAR* file, GDIPCONST CLSID* encoderCLSID, GDIPCONST GpEncoderParameters* params) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageBounds(GpImage* image, GpRectF* rect, GpUnit* unit) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageDimension(GpImage* image, REAL* width, REAL* height) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageType(GpImage* image, GpImageType* type) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageWidth(GpImage* image, UINT* width) {
#ifdef GDIFLAT_SAFE
		if (!image) {
			return GpStatus::InvalidParameter;
		}
#endif
		* width = image->iWidth;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetImageHeight(GpImage* image, UINT* height) {
#ifdef GDIFLAT_SAFE
		if (!image) {
			return GpStatus::InvalidParameter;
		}
#endif
		* height = image->iHeight;
		return GpStatus::Ok;
	}

	GpStatus WINGDIPAPI
		GdipGetImageHorizontalResolution(GpImage* image, REAL* resolution) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageVerticalResolution(GpImage* image, REAL* resolution) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageFlags(GpImage* image, UINT* flags) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageRawFormat(GpImage* image, GUID* format) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImagePixelFormat(GpImage* image, PixelFormat* format) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipImageGetFrameDimensionsCount(GpImage* image, UINT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipImageGetFrameDimensionsList(GpImage* image, GUID* dimensionGUID, UINT count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipImageGetFrameCount(GpImage* image, GDIPCONST GUID* dimensionGUID, UINT* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipImageSelectActiveFrame(GpImage* image, GDIPCONST GUID* dimensionGUID, UINT index) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipImageRotateFlip(GpImage* image, GpRotateFlipType type) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImagePalette(GpImage* image, GpColorPalette* palette, INT size) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetImagePalette(GpImage* image, GDIPCONST GpColorPalette* palette) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImagePaletteSize(GpImage* image, INT* size) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPropertyCount(GpImage* image, UINT* propertyNumber) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPropertyIdList(GpImage* image, UINT propertyNumber, PROPID* list) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPropertyItemSize(GpImage* image, PROPID propID, UINT* size) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPropertyItem(GpImage* image, PROPID propID, UINT size, GpPropertyItem* buffer) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetPropertySize(GpImage* image, UINT* totalBufferSize, UINT* numProperties) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipRemovePropertyItem(GpImage* image, PROPID propID) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSetPropertyItem(GpImage* image, GDIPCONST GpPropertyItem* item) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipCloneImage(GpImage* image, GpImage** cloneImage) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipLoadImageFromDelegate_linux(GetHeaderDelegate getHeaderFunc,
			GetBytesDelegate getBytesFunc,
			PutBytesDelegate putBytesFunc,
			SeekDelegate seekFunc,
			CloseDelegate closeFunc,
			SizeDelegate sizeFunc,
			GpImage** image) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSaveImageToDelegate_linux(GpImage* image, GetBytesDelegate getBytesFunc, PutBytesDelegate putBytesFunc,
			SeekDelegate seekFunc, CloseDelegate closeFunc, SizeDelegate sizeFunc, GDIPCONST CLSID* encoderCLSID,
			GDIPCONST GpEncoderParameters* params) {
		return GpStatus::NotImplemented;
	}


	GpStatus WINGDIPAPI
		GdipGetImageDecodersSize(UINT* numDecoders, UINT* size) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageDecoders(UINT numDecoders, UINT size, GpImageCodecInfo* decoders) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageEncodersSize(UINT* numEncoders, UINT* size) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageEncoders(UINT numEncoders, UINT size, GpImageCodecInfo* encoders) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetAllPropertyItems(GpImage* image, UINT totalBufferSize, UINT numProperties, GpPropertyItem* allItems) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetEncoderParameterListSize(GpImage* image, GDIPCONST CLSID* clsidEncoder, UINT* size) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetEncoderParameterList(GpImage* image, GDIPCONST CLSID* clsidEncoder, UINT size, GpEncoderParameters* buffer) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipGetImageThumbnail(GpImage* image, UINT thumbWidth, UINT thumbHeight, GpImage** thumbImage, GetThumbnailImageAbort callback, VOID* callbackData) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipLoadImageFromFileICM(GDIPCONST WCHAR* filename, GpImage** image) {
		return GpStatus::NotImplemented;
	}

	/*
		GDI+ 1.0 only supports multiple frames on an image for the
		tiff format
	*/

	GpStatus WINGDIPAPI
		GdipSaveAdd(GpImage* image, GDIPCONST GpEncoderParameters* encoderParams) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipSaveAddImage(GpImage* image, GpImage* imageNew, GDIPCONST GpEncoderParameters* params) {
		return GpStatus::NotImplemented;
	}

	GpStatus WINGDIPAPI
		GdipImageForceValidation(GpImage* image) {
		return GpStatus::NotImplemented;
	}

}