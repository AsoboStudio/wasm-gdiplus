// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers.

#include "GdiPlus.h"
#include "gdiplusflat.h"

namespace Gdiplus {

	// Image

	Image* Image::FromFile(const WCHAR* filename,
		BOOL useEmbeddedColorManagement)
	{
		return new Image(filename, useEmbeddedColorManagement);
	}

	/*WASM: NOT SUPPORTED
	Image* Image::FromStream(IStream *stream,
			BOOL useEmbeddedColorManagement)
	{
		return new Image(stream, useEmbeddedColorManagement);
	}*/

	Image::Image(const WCHAR* filename, BOOL useEmbeddedColorManagement) :
		nativeImage(NULL), lastStatus(Ok)
	{
		if (useEmbeddedColorManagement) {
			lastStatus = DLLExports::GdipLoadImageFromFileICM(
				filename, &nativeImage);
		}
		else {
			lastStatus = DLLExports::GdipLoadImageFromFile(
				filename, &nativeImage);
		}
	}

	/*WASM: NOT SUPPORTED
	Image::Image(IStream *stream, BOOL useEmbeddedColorManagement):
		nativeImage(NULL), lastStatus(Ok)
	{
		if (useEmbeddedColorManagement) {
			lastStatus = DLLExports::GdipLoadImageFromStreamICM(
					stream, &nativeImage);
		} else {
			lastStatus = DLLExports::GdipLoadImageFromStream(
					stream, &nativeImage);
		}
	}*/

	/*WASM: NOT SUPPORTED
	Status Image::FindFirstItem(ImageItemData *item)
	{
		// FIXME: can't test GdipFindFirstImageItem b/c it isn't exported in 1.0
		return updateStatus(DLLExports::GdipFindFirstImageItem(
				nativeImage, item));

	}
	*/
	/*WASM: NOT SUPPORTED
	Status Image::FindNextItem(ImageItemData *item)
	{
		// FIXME: can't test GdipFindFirstImageItem b/c it isn't exported in 1.0
		return updateStatus(DLLExports::GdipFindNextImageItem(
				nativeImage, item));
	}
	*/
	Status Image::GetAllPropertyItems(UINT totalBufferSize,
		UINT numProperties, PropertyItem* allItems)
	{
		return updateStatus(DLLExports::GdipGetAllPropertyItems(
			nativeImage, totalBufferSize, numProperties, allItems));
	}

	Status Image::GetBounds(RectF* srcRect, Unit* srcUnit)
	{
		return updateStatus(DLLExports::GdipGetImageBounds(
			nativeImage, srcRect, srcUnit));
	}

	Status Image::GetEncoderParameterList(const CLSID* clsidEncoder,
		UINT size, EncoderParameters* buffer)
	{
		return updateStatus(DLLExports::GdipGetEncoderParameterList(
			nativeImage, clsidEncoder, size, buffer));
	}

	UINT Image::GetEncoderParameterListSize(const CLSID* clsidEncoder)
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipGetEncoderParameterListSize(
			nativeImage, clsidEncoder, &result));
		return result;
	}

	UINT Image::GetFlags()
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipGetImageFlags(nativeImage, &result));
		return result;
	}

	UINT Image::GetFrameCount(const GUID* dimensionID)
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipImageGetFrameCount(
			nativeImage, dimensionID, &result));
		return result;
	}

	UINT Image::GetFrameDimensionsCount()
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipImageGetFrameDimensionsCount(
			nativeImage, &result));
		return result;
	}

	Status Image::GetFrameDimensionsList(GUID* dimensionIDs, UINT count)
	{
		return updateStatus(DLLExports::GdipImageGetFrameDimensionsList(
			nativeImage, dimensionIDs, count));
	}

	UINT Image::GetHeight()
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipGetImageHeight(nativeImage, &result));
		return result;
	}

	REAL Image::GetHorizontalResolution()
	{
		REAL result = 0.0f;
		updateStatus(DLLExports::GdipGetImageHorizontalResolution(
			nativeImage, &result));
		return result;
	}

	/*WASM: NOT SUPPORTED
	Status Image::GetItemData(ImageItemData *item)
	{
		// FIXME: can't test GdipGetImageItemData b/c it isn't exported in 1.0
		return updateStatus(DLLExports::GdipGetImageItemData(
				nativeImage, item));

	}*/

	Status Image::GetPalette(ColorPalette* palette, INT size)
	{
		return updateStatus(DLLExports::GdipGetImagePalette(
			nativeImage, palette, size));
	}

	INT Image::GetPaletteSize()
	{
		INT result = 0;
		updateStatus(DLLExports::GdipGetImagePaletteSize(nativeImage, &result));
		return result;
	}

	Status Image::GetPhysicalDimension(SizeF* size)
	{
		if (!size) return lastStatus = GpStatus::InvalidParameter;
		return updateStatus(DLLExports::GdipGetImageDimension(
			nativeImage, &size->Width, &size->Height));
	}

	PixelFormat Image::GetPixelFormat()
	{
		PixelFormat result = (PixelFormat)0;
		updateStatus(DLLExports::GdipGetImagePixelFormat(nativeImage, &result));
		return result;
	}

	UINT Image::GetPropertyCount()
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipGetPropertyCount(nativeImage, &result));
		return result;
	}

	Status Image::GetPropertyIdList(UINT numOfProperty, PROPID* list)
	{
		return updateStatus(DLLExports::GdipGetPropertyIdList(
			nativeImage, numOfProperty, list));
	}

	Status Image::GetPropertyItem(PROPID propId, UINT propSize,
		PropertyItem* buffer)
	{
		return updateStatus(DLLExports::GdipGetPropertyItem(
			nativeImage, propId, propSize, buffer));
	}

	UINT Image::GetPropertyItemSize(PROPID propId)
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipGetPropertyItemSize(
			nativeImage, propId, &result));
		return result;
	}

	Status Image::GetPropertySize(
		UINT* totalBufferSize, UINT* numProperties)
	{
		return updateStatus(DLLExports::GdipGetPropertySize(
			nativeImage, totalBufferSize, numProperties));
	}

	Status Image::GetRawFormat(GUID* format)
	{
		return updateStatus(DLLExports::GdipGetImageRawFormat(
			nativeImage, format));
	}

	Image* Image::GetThumbnailImage(UINT thumbWidth, UINT thumbHeight,
		GetThumbnailImageAbort callback, VOID* callbackData)
	{
		GpImage* thumbImage = NULL;
		Status status = updateStatus(DLLExports::GdipGetImageThumbnail(
			nativeImage, thumbWidth, thumbHeight, &thumbImage,
			callback, callbackData));

		if (status == GpStatus::Ok) {
			Image* result = new Image(thumbImage, GpStatus::Ok);
			if (!result) {
				DLLExports::GdipDisposeImage(thumbImage);
				lastStatus = OutOfMemory;
			}
			return result;
		}
		else {
			return NULL;
		}
	}

	ImageType Image::GetType() const
	{
		ImageType result = ImageTypeUnknown;
		updateStatus(DLLExports::GdipGetImageType(nativeImage, &result));
		return result;
	}

	REAL Image::GetVerticalResolution()
	{
		REAL result = 0.0f;
		updateStatus(DLLExports::GdipGetImageVerticalResolution(
			nativeImage, &result));
		return result;
	}

	UINT Image::GetWidth()
	{
		UINT result = 0;
		updateStatus(DLLExports::GdipGetImageWidth(nativeImage, &result));
		return result;
	}

	Status Image::RemovePropertyItem(PROPID propId)
	{
		return updateStatus(DLLExports::GdipRemovePropertyItem(
			nativeImage, propId));
	}

	Status Image::RotateFlip(RotateFlipType rotateFlipType)
	{
		return updateStatus(DLLExports::GdipImageRotateFlip(
			nativeImage, rotateFlipType));
	}

	Status Image::Save(const WCHAR* filename, const CLSID* clsidEncoder,
		const EncoderParameters* encoderParams)
	{
		return updateStatus(DLLExports::GdipSaveImageToFile(
			nativeImage, filename, clsidEncoder, encoderParams));
	}

	/*WASM: NOT SUPPORTED
	Status Image::Save(IStream *stream, const CLSID *clsidEncoder,
			const EncoderParameters *encoderParams)
	{
		return updateStatus(DLLExports::GdipSaveImageToStream(
				nativeImage, stream, clsidEncoder, encoderParams));
	}*/

	Status Image::SaveAdd(const EncoderParameters* encoderParams)
	{
		return updateStatus(DLLExports::GdipSaveAdd(
			nativeImage, encoderParams));
	}

	Status Image::SaveAdd(Image* newImage,
		const EncoderParameters* encoderParams)
	{
		return updateStatus(DLLExports::GdipSaveAddImage(
			nativeImage,
			newImage ? newImage->nativeImage : NULL,
			encoderParams));
	}

	Status Image::SelectActiveFrame(
		const GUID* dimensionID, UINT frameIndex)
	{
		return updateStatus(DLLExports::GdipImageSelectActiveFrame(
			nativeImage, dimensionID, frameIndex));
	}

	Status Image::SetAbort(GdiplusAbort* pIAbort)
	{
		// FIXME: can't test GdipImageSetAbort because it isn't exported in 1.0
		return GpStatus::Ok;
		/*WASM: NOT SUPPORTED
		return updateStatus(DLLExports::GdipImageSetAbort(
				nativeImage, pIAbort));
				*/
	}

	Status Image::SetPalette(const ColorPalette* palette)
	{
		return updateStatus(DLLExports::GdipSetImagePalette(
			nativeImage, palette));
	}

	Status Image::SetPropertyItem(const PropertyItem* item)
	{
		return updateStatus(DLLExports::GdipSetPropertyItem(nativeImage, item));
	}


	// Bitmap

	Bitmap* Bitmap::FromBITMAPINFO(const BITMAPINFO* gdiBitmapInfo,
		VOID* gdiBitmapData)
	{
		return new Bitmap(gdiBitmapInfo, gdiBitmapData);
	}

	/*WASM: NOT SUPPORTED
	Bitmap* Bitmap::FromDirectDrawSurface7(IDirectDrawSurface7 *surface)
	{
		return new Bitmap(surface);
	}*/

	Bitmap* Bitmap::FromFile(const WCHAR* filename,
		BOOL useEmbeddedColorManagement)
	{
		return new Bitmap(filename, useEmbeddedColorManagement);
	}

	Bitmap* Bitmap::FromHBITMAP(HBITMAP hbm, HPALETTE hpal)
	{
		return new Bitmap(hbm, hpal);
	}

	Bitmap* Bitmap::FromHICON(HICON icon)
	{
		return new Bitmap(icon);
	}

	Bitmap* Bitmap::FromResource(
		HINSTANCE hInstance, const WCHAR* bitmapName)
	{
		return new Bitmap(hInstance, bitmapName);
	}
	/*WASM: NOT SUPPORTED
	Bitmap* Bitmap::FromStream(IStream *stream,
			BOOL useEmbeddedColorManagement)
	{
		return GpStatus::Ok;
		return new Bitmap(stream, useEmbeddedColorManagement);

	}
	*/
	//TODO: [GDI+ 1.1] Bitmap::ApplyEffect
	//Status Bitmap::ApplyEffect(Bitmap **inputs, INT numInputs,
	//		Effect *effect, RECT *ROI, RECT *outputRect, Bitmap **output)
	//{
	//	return GpStatus::NotImplemented;
	//}

	/*WASM: NOT SUPPORTED
	Status Bitmap::InitializePalette(ColorPalette *palette,
			PaletteType paletteType, INT optimalColors,
			BOOL useTransparentColor, Bitmap *bitmap)
	{
		// FIXME: can't test GdipInitializePalette b/c it isn't exported in 1.0
		return DLLExports::GdipInitializePalette(palette, paletteType,
				optimalColors, useTransparentColor,
				bitmap ? (GpBitmap*) bitmap->nativeImage : NULL);
	}*/

	/*WASM: NOT SUPPORTED
	Bitmap::Bitmap(const BITMAPINFO *gdiBitmapInfo, VOID *gdiBitmapData):
		Image(NULL, GpStatus::Ok)
	{
		GpBitmap *nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromGdiDib(
				gdiBitmapInfo, gdiBitmapData, &nativeBitmap);
		nativeImage = nativeBitmap;
	}*/

	/*WASM: NOT SUPPORTED
	Bitmap::Bitmap(IDirectDrawSurface7 *surface): Image(NULL, GpStatus::Ok)
	{
		GpBitmap *nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromDirectDrawSurface(
				surface, &nativeBitmap);
		nativeImage = nativeBitmap;
	}*/
#ifdef _MSFS_WASM_PROXY_GDIPLUS
	Bitmap::Bitmap(Graphics* graphics, const char* filename,
		BOOL useEmbeddedColorManagement) : Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		if (useEmbeddedColorManagement) {
			lastStatus = GpStatus::NotImplemented;
		}
		else {
			lastStatus = DLLExports::GdipCreateBitmapFromFileUTF8Linked(
				filename, &nativeBitmap, graphics? graphics->nativeGraphics : NULL);
		}
		nativeImage = nativeBitmap;
	}
	Bitmap::Bitmap(Graphics* graphics, const WCHAR* filename,
		BOOL useEmbeddedColorManagement) : Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		if (useEmbeddedColorManagement) {
			lastStatus = GpStatus::NotImplemented;
		}
		else {
			lastStatus = DLLExports::GdipCreateBitmapFromFileLinked(
				filename, &nativeBitmap, graphics ? graphics->nativeGraphics : NULL);
		}
		nativeImage = nativeBitmap;
	}

	Bitmap::Bitmap(const char* filename,
		BOOL useEmbeddedColorManagement) : Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		if (useEmbeddedColorManagement) {
			lastStatus = GpStatus::NotImplemented;
		}
		else {
			lastStatus = DLLExports::GdipCreateBitmapFromFileUTF8(
				filename, &nativeBitmap);
		}
		nativeImage = nativeBitmap;
	}
	Status Bitmap::Load(Graphics* gfx) {
		lastStatus = DLLExports::GdipLoadImage(gfx? gfx->nativeGraphics: NULL, nativeImage);
		return lastStatus;
	}
#endif
	Bitmap::Bitmap(const WCHAR* filename,
		BOOL useEmbeddedColorManagement) : Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		if (useEmbeddedColorManagement) {
			lastStatus = DLLExports::GdipCreateBitmapFromFileICM(
				filename, &nativeBitmap);
		}
		else {
			lastStatus = DLLExports::GdipCreateBitmapFromFile(
				filename, &nativeBitmap);
		}
		nativeImage = nativeBitmap;
	}

	Bitmap::Bitmap(HBITMAP hbm, HPALETTE hpal) :
		Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromHBITMAP(
			hbm, hpal, &nativeBitmap);
		nativeImage = nativeBitmap;
	}

	Bitmap::Bitmap(HICON hicon) :
		Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromHICON(hicon, &nativeBitmap);
		nativeImage = nativeBitmap;
	}

	Bitmap::Bitmap(HINSTANCE hInstance, const WCHAR* bitmapName) :
		Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromResource(
			hInstance, bitmapName, &nativeBitmap);
		nativeImage = nativeBitmap;
	}

	/*WASM: NOT SUPPORTED
	Bitmap::Bitmap(IStream *stream, BOOL useEmbeddedColorManagement):
		Image(NULL, GpStatus::Ok)
	{
		GpBitmap *nativeBitmap = NULL;
		if (useEmbeddedColorManagement) {
			lastStatus = DLLExports::GdipCreateBitmapFromStreamICM(
					stream, &nativeBitmap);
		} else {
			lastStatus = DLLExports::GdipCreateBitmapFromStream(
					stream, &nativeBitmap);
		}
		nativeImage = nativeBitmap;
	}*/

	Bitmap::Bitmap(INT width, INT height, Graphics* target) :
		Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromGraphics(
			width, height, target ? target->nativeGraphics : NULL,
			&nativeBitmap);
		nativeImage = nativeBitmap;
	}

	Bitmap::Bitmap(INT width, INT height, PixelFormat format) :
		Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromScan0(
			width, height, 0, format, NULL, &nativeBitmap);
		nativeImage = nativeBitmap;
	}

	Bitmap::Bitmap(INT width, INT height, INT stride,
		PixelFormat format, BYTE* scan0) : Image(NULL, GpStatus::Ok)
	{
		GpBitmap* nativeBitmap = NULL;
		lastStatus = DLLExports::GdipCreateBitmapFromScan0(
			width, height, stride, format, scan0, &nativeBitmap);
		nativeImage = nativeBitmap;
	}

	Bitmap* Bitmap::Clone(const RectF& rect, PixelFormat format) const
	{
		return Clone(rect.X, rect.Y, rect.Width, rect.Height, format);
	}

	Bitmap* Bitmap::Clone(const Rect& rect, PixelFormat format) const
	{
		return Clone(rect.X, rect.Y, rect.Width, rect.Height, format);
	}

	Bitmap* Bitmap::Clone(REAL x, REAL y, REAL width, REAL height,
		PixelFormat format) const
	{
		GpBitmap* cloneBitmap = NULL;
		Status status = updateStatus(DLLExports::GdipCloneBitmapArea(
			x, y, width, height, format,
			(GpBitmap*)nativeImage, &cloneBitmap));
		if (status == GpStatus::Ok) {
			Bitmap* result = new Bitmap(cloneBitmap, lastStatus);
			if (!result) {
				DLLExports::GdipDisposeImage(cloneBitmap);
				lastStatus = OutOfMemory;
			}
			return result;
		}
		else {
			return NULL;
		}
	}

	Bitmap* Bitmap::Clone(INT x, INT y, INT width, INT height,
		PixelFormat format) const
	{
		GpBitmap* cloneBitmap = NULL;
		Status status = updateStatus(DLLExports::GdipCloneBitmapAreaI(
			x, y, width, height, format,
			(GpBitmap*)nativeImage, &cloneBitmap));
		if (status == GpStatus::Ok) {
			Bitmap* result = new Bitmap(cloneBitmap, lastStatus);
			if (!result) {
				DLLExports::GdipDisposeImage(cloneBitmap);
				lastStatus = OutOfMemory;
			}
			return result;
		}
		else {
			return NULL;
		}
	}

	//TODO: [GDI+ 1.1] Bitmap::ApplyEffect
	//Status Bitmap::ApplyEffect(Effect *effect, RECT *ROI)
	//{
	//	return GpStatus::NotImplemented;
	//}

	/*WASM: NOT SUPPORTED
	Status Bitmap::ConvertFormat(PixelFormat format,
			DitherType ditherType, PaletteType paletteType,
			ColorPalette *palette, REAL alphaThresholdPercent)
	{
		// FIXME: can't test GdipBitmapConvertFormat b/c it isn't exported in 1.0
		return updateStatus(DLLExports::GdipBitmapConvertFormat(
				(GpBitmap*) nativeImage, format, ditherType,
				paletteType, palette, alphaThresholdPercent));
	}*/

	Status Bitmap::GetHBITMAP(
		const Color& colorBackground, HBITMAP* hbmReturn) const
	{
		return updateStatus(DLLExports::GdipCreateHBITMAPFromBitmap(
			(GpBitmap*)nativeImage, hbmReturn,
			colorBackground.GetValue()));
	}

	Status Bitmap::GetHICON(HICON* icon) const
	{
		return updateStatus(DLLExports::GdipCreateHICONFromBitmap(
			(GpBitmap*)nativeImage, icon));
	}

	/*WASM: NOT SUPPORTED
	Status Bitmap::GetHistogram(HistogramFormat format,
			UINT numberOfEntries,
			UINT *channel0, UINT *channel1,
			UINT *channel2, UINT *channel3) const
	{
		return updateStatus(DLLExports::GdipBitmapGetHistogram(
				(GpBitmap*) nativeImage, format, numberOfEntries,
				channel0, channel1, channel2, channel3));
	}*/

	/*WASM: NOT SUPPORTED
	Status Bitmap::GetHistogramSize(HistogramFormat format,
			UINT *numberOfEntries) const
	{
		return updateStatus(DLLExports::GdipBitmapGetHistogramSize(
				format, numberOfEntries));
	}*/

	Status Bitmap::GetPixel(INT x, INT y, Color* color) const
	{
		return updateStatus(DLLExports::GdipBitmapGetPixel(
			(GpBitmap*)nativeImage, x, y,
			color ? &color->Value : NULL));
	}

	Status Bitmap::LockBits(const Rect* rect, UINT flags,
		PixelFormat format, BitmapData* lockedBitmapData)
	{
		return updateStatus(DLLExports::GdipBitmapLockBits(
			(GpBitmap*)nativeImage, rect, flags, format,
			lockedBitmapData));
	}

	Status Bitmap::SetPixel(INT x, INT y, const Color& color)
	{
		return updateStatus(DLLExports::GdipBitmapSetPixel(
			(GpBitmap*)nativeImage, x, y, color.GetValue()));
	}

	Status Bitmap::SetResolution(REAL xdpi, REAL ydpi)
	{
		return updateStatus(DLLExports::GdipBitmapSetResolution(
			(GpBitmap*)nativeImage, xdpi, ydpi));
	}

	Status Bitmap::UnlockBits(BitmapData* lockedBitmapData)
	{
		return updateStatus(DLLExports::GdipBitmapUnlockBits(
			(GpBitmap*)nativeImage, lockedBitmapData));
	}


	// CachedBitmap
	CachedBitmap::CachedBitmap(Bitmap *bitmap, Graphics *graphics)//:nativeCachedBitmap(NULL), lastStatus(Ok)
	{

		/*lastStatus = DLLExports::GdipCreateCachedBitmap(
				bitmap ? ((GpBitmap*) bitmap->nativeImage) : NULL,
				graphics ? graphics->nativeGraphics : NULL,
				&nativeCachedBitmap);*/

	}

	CachedBitmap::~CachedBitmap()
	{
		//DLLExports::GdipDeleteCachedBitmap(nativeCachedBitmap);
	}
};