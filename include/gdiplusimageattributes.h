// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>

#ifndef __GDIPLUS_IMAGEATTRIBUTES_H
#define __GDIPLUS_IMAGEATTRIBUTES_H

#include "GdiPlus.h"
#include "gdiplusflat.h"
namespace Gdiplus
{
	class ImageAttributes : public GdiplusBase
	{
		friend class Graphics;
		friend class TextureBrush;

	public:
		ImageAttributes() : nativeImageAttributes(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreateImageAttributes(
				&nativeImageAttributes);
		}
		~ImageAttributes()
		{
			DLLExports::GdipDisposeImageAttributes(nativeImageAttributes);
		}
		ImageAttributes* Clone() const
		{
			GpImageAttributes* cloneImageAttributes = NULL;
			Status status = updateStatus(DLLExports::GdipCloneImageAttributes(
				nativeImageAttributes, &cloneImageAttributes));
			if (status == GpStatus::Ok) {
				ImageAttributes* result = new ImageAttributes(
					cloneImageAttributes, lastStatus);
				if (!result) {
					DLLExports::GdipDisposeImageAttributes(cloneImageAttributes);
					lastStatus = OutOfMemory;
				}
				return result;
			}
			else {
				return NULL;
			}
		}

		Status ClearBrushRemapTable()
		{
			return updateStatus(DLLExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, ColorAdjustTypeBrush,
				FALSE, 0, NULL));
		}
		Status ClearColorKey(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesColorKeys(
				nativeImageAttributes, type, FALSE, 0, 0));
		}
		Status ClearColorMatrices(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, FALSE,
				NULL, NULL, ColorMatrixFlagsDefault));
		}
		Status ClearColorMatrix(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, FALSE,
				NULL, NULL, ColorMatrixFlagsDefault));
		}
		Status ClearGamma(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesGamma(
				nativeImageAttributes, type, FALSE, 1.0f));
		}
		Status ClearNoOp(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesNoOp(
				nativeImageAttributes, type, FALSE));
		}
		Status ClearOutputChannel(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesOutputChannel(
				nativeImageAttributes, type, FALSE,
				ColorChannelFlagsC));
		}
		Status ClearOutputChannelColorProfile(
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesOutputChannelColorProfile(
				nativeImageAttributes, type, FALSE, NULL));
		}
		Status ClearRemapTable(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, type, FALSE, 0, NULL));
		}
		Status ClearThreshold(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesThreshold(
				nativeImageAttributes, type, FALSE, 0.0));
		}
		Status GetAdjustedPalette(ColorPalette* colorPalette,
			ColorAdjustType type) const
		{
			return updateStatus(DLLExports::GdipGetImageAttributesAdjustedPalette(
				nativeImageAttributes, colorPalette, type));
		}
		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}
		Status Reset(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipResetImageAttributes(
				nativeImageAttributes, type));
		}
		Status SetBrushRemapTable(UINT mapSize, ColorMap* map)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, ColorAdjustTypeBrush,
				TRUE, mapSize, map));
		}
		Status SetColorKey(const Color& colorLow, const Color& colorHigh,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesColorKeys(
				nativeImageAttributes, type, TRUE,
				colorLow.GetValue(), colorHigh.GetValue()));
		}
		Status SetColorMatrices(const ColorMatrix* colorMatrix,
			const ColorMatrix* grayMatrix,
			ColorMatrixFlags mode = ColorMatrixFlagsDefault,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, TRUE,
				colorMatrix, grayMatrix, mode));
		}
		Status SetColorMatrix(const ColorMatrix* colorMatrix,
			ColorMatrixFlags mode = ColorMatrixFlagsDefault,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, TRUE,
				colorMatrix, NULL, mode));
		}
		Status SetGamma(REAL gamma,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesGamma(
				nativeImageAttributes, type, TRUE, gamma));
		}
		Status SetNoOp(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesNoOp(
				nativeImageAttributes, type, TRUE));
		}
		Status SetOutputChannel(ColorChannelFlags channelFlags,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesOutputChannel(
				nativeImageAttributes, type, TRUE,
				channelFlags));
		}
		Status SetOutputChannelColorProfile(const WCHAR* colorProfileFilename,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesOutputChannelColorProfile(
				nativeImageAttributes, type, TRUE,
				colorProfileFilename));
		}
		Status SetRemapTable(UINT mapSize, const ColorMap* map,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, type, TRUE,
				mapSize, map));
		}
		Status SetThreshold(REAL threshold,
			ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesThreshold(
				nativeImageAttributes, type, TRUE, threshold));
		}
		Status SetToIdentity(ColorAdjustType type = ColorAdjustTypeDefault)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesToIdentity(
				nativeImageAttributes, type));
		}
		Status SetWrapMode(WrapMode wrap, const Color& color = Color(),
			BOOL clamp = FALSE)
		{
			return updateStatus(DLLExports::GdipSetImageAttributesWrapMode(
				nativeImageAttributes, wrap,
				color.GetValue(), clamp));
		}

	private:
		ImageAttributes(GpImageAttributes* imageAttributes, Status status) :
			nativeImageAttributes(imageAttributes), lastStatus(status) {}
		ImageAttributes(const ImageAttributes&);
		ImageAttributes& operator=(const ImageAttributes&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != GpStatus::Ok) lastStatus = newStatus;
			return newStatus;
		}

		GpImageAttributes* nativeImageAttributes;
		mutable Status lastStatus;
	};

}
#endif /* __GDIPLUS_IMAGEATTRIBUTES_H */
