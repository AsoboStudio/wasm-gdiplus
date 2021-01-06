// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_BRUSH_H
#define __GDIPLUS_BRUSH_H

#include "GdiPlusTypes.h"
#include "GdiPlusEnums.h"
#include "gdiplusheaders.h"
#include "GdiPlusMatrix.h"
#include "gdiplusbase.h"
#include "GdiPlusColor.h"
#include "gdiplusimageattributes.h"

#include "gdiplusflat.h"

namespace Gdiplus
{
	class Brush : public GdiplusBase
	{
		friend class HatchBrush;
		friend class LinearGradientBrush;
		friend class PathGradientBrush;
		friend class SolidBrush;
		friend class TextureBrush;
		friend class Graphics;
		friend class Pen;

	public:
		virtual ~Brush()
		{
			DLLExports::GdipDeleteBrush((GpBrush*)nativeBrush);
		}
		virtual Brush* Clone() const  // each subclass must implement this
		{
			lastStatus = NotImplemented;
			return NULL;
		}

		GpStatus GetLastStatus() const
		{
			GpStatus result = lastStatus;
			lastStatus = GpStatus::Ok;
			return result;
		}
		BrushType GetType() const
		{
			BrushType result = BrushTypeSolidColor;
			updateStatus(DLLExports::GdipGetBrushType((GpBrush*)nativeBrush, &result));
			return result;
		}

	protected:

		GpStatus updateStatus(GpStatus newStatus) const
		{
			if (newStatus != GpStatus::Ok) lastStatus = newStatus;
			return newStatus;
		}
	protected:
		Brush() : nativeBrush(NULL), lastStatus(GpStatus::Ok) {}
		Brush(GpBrush* brush, GpStatus status) :
			nativeBrush(brush), lastStatus(status) {}
		Brush(const Brush& brush);
		Brush& operator=(const Brush&);

		GpBrush* nativeBrush;
		mutable GpStatus lastStatus;
	};

	class HatchBrush : public Brush
	{
	public:

		HatchBrush(HatchStyle hatchStyle,
			const Color& foreColor,
			const Color& backColor = Color())
		{
			GpHatch* nativeHatch = NULL;
			lastStatus = DLLExports::GdipCreateHatchBrush(hatchStyle,
				foreColor.GetValue(), backColor.GetValue(),
				&nativeHatch);
			if (nativeHatch) {
				nativeBrush = &nativeHatch->base;
			}
			
		}
		/* WASM: NOT SUPPORTED
		virtual HatchBrush* Clone() const
		{
			GpBrush *cloneBrush = NULL;
			GpStatus status = updateStatus( DLLExports::GdipCloneBrush(
				nativeBrush, &cloneBrush));
			if (status == GpStatus::Ok) {
				HatchBrush *result =
					new HatchBrush(cloneBrush, lastStatus);
				if (!result) {
					 DLLExports::GdipDeleteBrush(cloneBrush);
					updateStatus(OutOfMemory);
				}
				return result;
			} else {
				return NULL;
			}
		}*/

		GpStatus GetBackgroundColor(Color* color) const
		{
			return updateStatus(DLLExports::GdipGetHatchBackgroundColor(
				(GpHatch*)nativeBrush->brush,
				color ? &color->Value : NULL));
		}
		GpStatus GetForegroundColor(Color* color) const
		{
			return updateStatus(DLLExports::GdipGetHatchForegroundColor(
				(GpHatch*)nativeBrush->brush,
				color ? &color->Value : NULL));
		}
		HatchStyle GetHatchStyle() const
		{
			HatchStyle result;
			updateStatus(DLLExports::GdipGetHatchStyle(
				(GpHatch*)nativeBrush->brush, &result));
			return result;
		}

	private:
		HatchBrush(GpBrush* brush, GpStatus status) : Brush(brush, status) {}
		HatchBrush(const HatchBrush& brush);
		HatchBrush& operator=(const HatchBrush&);
	};

	class LinearGradientBrush : public Brush
	{
	public:
		LinearGradientBrush(const PointF& point1, const PointF& point2,
			const Color& color1, const Color& color2)
		{
			GpLineGradient* nativeLineGradient = NULL;
			lastStatus = DLLExports::GdipCreateLineBrush(
				&point1, &point2,
				color1.GetValue(), color2.GetValue(),
				WrapModeTile, &nativeLineGradient);
			if (nativeLineGradient != NULL)
			{
				nativeBrush = &nativeLineGradient->base;
			}
		}
		LinearGradientBrush(const Point& point1, const Point& point2,
			const Color& color1, const Color& color2)
		{
			GpLineGradient* nativeLineGradient = NULL;
			lastStatus = DLLExports::GdipCreateLineBrushI(
				&point1, &point2,
				color1.GetValue(), color2.GetValue(),
				WrapModeTile, &nativeLineGradient);
			if (nativeLineGradient != NULL)
			{
				nativeBrush = &nativeLineGradient->base;
			}
		}
		LinearGradientBrush(const RectF& rect, const Color& color1,
			const Color& color2, LinearGradientMode mode)
		{
			GpLineGradient* nativeLineGradient = NULL;
			lastStatus = DLLExports::GdipCreateLineBrushFromRect(
				&rect, color1.GetValue(), color2.GetValue(),
				mode, WrapModeTile, &nativeLineGradient);
			if (nativeLineGradient != NULL)
			{
				nativeBrush = &nativeLineGradient->base;
			}
		}
		LinearGradientBrush(const Rect& rect, const Color& color1,
			const Color& color2, LinearGradientMode mode)
		{
			GpLineGradient* nativeLineGradient = NULL;
			lastStatus = DLLExports::GdipCreateLineBrushFromRectI(
				&rect, color1.GetValue(), color2.GetValue(),
				mode, WrapModeTile, &nativeLineGradient);
			if (nativeLineGradient != NULL)
			{
				nativeBrush = &nativeLineGradient->base;
			}
		}
		LinearGradientBrush(const RectF& rect, const Color& color1,
			const Color& color2, REAL angle,
			BOOL isAngleScalable = FALSE)
		{
			GpLineGradient* nativeLineGradient = NULL;
			lastStatus = DLLExports::GdipCreateLineBrushFromRectWithAngle(
				&rect, color1.GetValue(), color2.GetValue(),
				angle, isAngleScalable, WrapModeTile,
				&nativeLineGradient);
			if (nativeLineGradient != NULL)
			{
				nativeBrush = &nativeLineGradient->base;
			}
		}
		LinearGradientBrush(const Rect& rect, const Color& color1,
			const Color& color2, REAL angle,
			BOOL isAngleScalable = FALSE)
		{
			GpLineGradient* nativeLineGradient = NULL;
			lastStatus = DLLExports::GdipCreateLineBrushFromRectWithAngleI(
				&rect, color1.GetValue(), color2.GetValue(),
				angle, isAngleScalable, WrapModeTile,
				&nativeLineGradient);
			if (nativeLineGradient != NULL)
			{
				nativeBrush = &nativeLineGradient->base;
			}
		}
		virtual LinearGradientBrush* Clone() const
		{
			GpBrush* cloneBrush = NULL;
			GpStatus status = updateStatus(DLLExports::GdipCloneBrush(
				(GpBrush*)nativeBrush, &cloneBrush));
			if (status == GpStatus::Ok) {
				LinearGradientBrush* result =
					new LinearGradientBrush(cloneBrush, lastStatus);
				if (!result) {
					DLLExports::GdipDeleteBrush(cloneBrush);
					updateStatus(OutOfMemory);
				}
				return result;
			}
			else {
				return NULL;
			}
		}

		GpStatus GetBlend(REAL* blendFactors, REAL* blendPositions,
			INT count) const
		{
			return updateStatus(DLLExports::GdipGetLineBlend(
				(GpLineGradient*)nativeBrush,
				blendFactors, blendPositions, count));
		}
		INT GetBlendCount() const
		{
			INT result = 0;
			updateStatus(DLLExports::GdipGetLineBlendCount(
				(GpLineGradient*)nativeBrush, &result));
			return result;
		}
		BOOL GetGammaCorrection() const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipGetLineGammaCorrection(
				(GpLineGradient*)nativeBrush, &result));
			return result;
		}
		INT GetInterpolationColorCount() const
		{
			INT result = 0;
			updateStatus(DLLExports::GdipGetLinePresetBlendCount(
				(GpLineGradient*)nativeBrush, &result));
			return result;
		}
		GpStatus GetInterpolationColors(Color* presetColors,
			REAL* blendPositions, INT count) const
		{
			if (!presetColors || count <= 0)
				return lastStatus = GpStatus::InvalidParameter;

			ARGB* presetArgb = new ARGB[count];
			if (!presetArgb)
				return lastStatus = OutOfMemory;

			GpStatus status = updateStatus(DLLExports::GdipGetLinePresetBlend(
				(GpLineGradient*)nativeBrush, presetArgb,
				blendPositions, count));
			for (INT i = 0; i < count; ++i) {
				presetColors[i].SetValue(presetArgb[i]);
			}
			delete[] (presetArgb);
			return status;
		}
		GpStatus GetLinearColors(Color* colors) const
		{
			if (!colors) return lastStatus = GpStatus::InvalidParameter;

			ARGB colorsArgb[2];
			GpStatus status = updateStatus(DLLExports::GdipGetLineColors(
				(GpLineGradient*)nativeBrush, colorsArgb));
			colors[0].SetValue(colorsArgb[0]);
			colors[1].SetValue(colorsArgb[1]);
			return status;
		}
		GpStatus GetRectangle(RectF* rect) const
		{
			return updateStatus(DLLExports::GdipGetLineRect(
				(GpLineGradient*)nativeBrush, rect));
		}
		GpStatus GetRectangle(Rect* rect) const
		{
			return updateStatus(DLLExports::GdipGetLineRectI(
				(GpLineGradient*)nativeBrush, rect));
		}
		GpStatus GetTransform(Matrix* matrix) const
		{
			return updateStatus(DLLExports::GdipGetLineTransform(
				(GpLineGradient*)nativeBrush,
				matrix ? matrix->nativeMatrix : NULL));
		}
		WrapMode GetWrapMode() const
		{
			WrapMode wrapMode = WrapModeTile;
			updateStatus(DLLExports::GdipGetLineWrapMode(
				(GpLineGradient*)nativeBrush, &wrapMode));
			return wrapMode;
		}
		GpStatus MultiplyTransform(const Matrix* matrix,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipMultiplyLineTransform(
				(GpLineGradient*)nativeBrush,
				matrix ? matrix->nativeMatrix : NULL, order));
		}
		GpStatus ResetTransform()
		{
			return updateStatus(DLLExports::GdipResetLineTransform(
				(GpLineGradient*)nativeBrush));
		}
		GpStatus RotateTranform(REAL angle, MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipRotateLineTransform(
				(GpLineGradient*)nativeBrush, angle, order));
		}
		GpStatus ScaleTransform(REAL sx, REAL sy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipScaleLineTransform(
				(GpLineGradient*)nativeBrush, sx, sy, order));
		}
		GpStatus SetBlend(const REAL* blendFactors,
			const REAL* blendPositions, INT count)
		{
			return updateStatus(DLLExports::GdipSetLineBlend(
				(GpLineGradient*)nativeBrush,
				blendFactors, blendPositions, count));
		}
		GpStatus SetBlendBellShape(REAL focus, REAL scale = 1.0f)
		{
			return updateStatus(DLLExports::GdipSetLineSigmaBlend(
				(GpLineGradient*)nativeBrush,
				focus, scale));
		}
		GpStatus SetBlendTriangularShape(REAL focus, REAL scale = 1.0f)
		{
			return updateStatus(DLLExports::GdipSetLineLinearBlend(
				(GpLineGradient*)nativeBrush,
				focus, scale));
		}
		GpStatus SetGammaCorrection(BOOL useGammaCorrection)
		{
			return updateStatus(DLLExports::GdipSetLineGammaCorrection(
				(GpLineGradient*)nativeBrush,
				useGammaCorrection));
		}
		GpStatus SetInterpolationColors(const Color* presetColors,
			const REAL* blendPositions, INT count)
		{
			if (!presetColors || count < 0)
				return lastStatus = GpStatus::InvalidParameter;

			ARGB* presetArgb = new ARGB[count];
			if (!presetArgb)
				return lastStatus = OutOfMemory;
			for (INT i = 0; i < count; ++i) {
				presetArgb[i] = presetColors[i].GetValue();
			}

			GpStatus status = updateStatus(DLLExports::GdipSetLinePresetBlend(
				(GpLineGradient*)nativeBrush,
				presetArgb, blendPositions, count));
			delete[](presetArgb);
			return status;
		}
		GpStatus SetLinearColors(const Color& color1, const Color& color2)
		{
			return updateStatus(DLLExports::GdipSetLineColors(
				(GpLineGradient*)nativeBrush,
				color1.GetValue(), color2.GetValue()));
		}
		GpStatus SetTransform(const Matrix* matrix)
		{
			return updateStatus(DLLExports::GdipSetLineTransform(
				(GpLineGradient*)nativeBrush,
				matrix ? matrix->nativeMatrix : NULL));
		}
		GpStatus SetWrapMode(WrapMode wrapMode)
		{
			return updateStatus(DLLExports::GdipSetLineWrapMode(
				(GpLineGradient*)nativeBrush, wrapMode));
		}
		GpStatus TranslateTransform(REAL dx, REAL dy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipTranslateLineTransform(
				(GpLineGradient*)nativeBrush, dx, dy, order));
		}

	private:
		LinearGradientBrush(GpBrush* brush, GpStatus status) : Brush(brush, status) {}
		LinearGradientBrush(const LinearGradientBrush& brush);
		LinearGradientBrush& operator=(const LinearGradientBrush&);
	};

	class SolidBrush : public Brush
	{
	public:
		SolidBrush(const Color& color)
		{
			GpSolidFill* nativeSolidFill = NULL;
			lastStatus = DLLExports::GdipCreateSolidFill(
				color.GetValue(), &nativeSolidFill);
			if (nativeSolidFill != NULL)
			{
				nativeBrush = &nativeSolidFill->base;
			}
		}
		// WASM not supported
		/*
		virtual SolidBrush* Clone() const
		{
			GpBrush *cloneBrush = NULL;
			GpStatus status = updateStatus( DLLExports::GdipCloneBrush(
					nativeBrush, &cloneBrush));
			if (status == GpStatus::Ok) {
				SolidBrush *result =
					new SolidBrush(cloneBrush, lastStatus);
				if (!result) {
					 DLLExports::GdipDeleteBrush(cloneBrush);
					updateStatus(OutOfMemory);
				}
				return result;
			} else {
				return NULL;
			}
		}*/

		GpStatus GetColor(Color* color) const
		{
			return updateStatus(DLLExports::GdipGetSolidFillColor(
				(GpSolidFill*)nativeBrush,
				color ? &color->Value : NULL));
		}
		GpStatus SetColor(const Color& color)
		{
			return updateStatus(DLLExports::GdipSetSolidFillColor(
				(GpSolidFill*)nativeBrush, color.GetValue()));
		}

	private:
		SolidBrush(GpBrush* brush, GpStatus status) : Brush(brush, status) {}
		SolidBrush(const SolidBrush&);
		SolidBrush& operator=(const SolidBrush&);
	};

	class TextureBrush : public Brush
	{
	public:
		TextureBrush(Image* image, WrapMode wrapMode = WrapModeTile)
		{
			GpTexture* nativeTexture = NULL;
			lastStatus = DLLExports::GdipCreateTexture(
				image ? image->nativeImage : NULL,
				wrapMode, &nativeTexture);
			if (nativeTexture != NULL)
			{
				nativeBrush = &nativeTexture->base;
			}
		}
		TextureBrush(Image* image, WrapMode wrapMode,
			REAL dstX, REAL dstY, REAL dstWidth, REAL dstHeight)
		{
			GpTexture* nativeTexture = NULL;
			lastStatus = DLLExports::GdipCreateTexture2(
				image ? image->nativeImage : NULL,
				wrapMode, dstX, dstY, dstWidth, dstHeight,
				&nativeTexture);
			if (nativeTexture != NULL)
			{
				nativeBrush = &nativeTexture->base;
			}
		}
		TextureBrush(Image* image, WrapMode wrapMode,
			INT dstX, INT dstY, INT dstWidth, INT dstHeight)
		{
			GpTexture* nativeTexture = NULL;
			lastStatus = DLLExports::GdipCreateTexture2I(
				image ? image->nativeImage : NULL,
				wrapMode, dstX, dstY, dstWidth, dstHeight,
				&nativeTexture);
			if (nativeTexture != NULL)
			{
				nativeBrush = &nativeTexture->base;
			}
		}
		TextureBrush(Image* image, WrapMode wrapMode, const RectF& dstRect)
		{
			GpTexture* nativeTexture = NULL;
			lastStatus = DLLExports::GdipCreateTexture2(
				image ? image->nativeImage : NULL, wrapMode,
				dstRect.X, dstRect.Y,
				dstRect.Width, dstRect.Height, &nativeTexture);
			if (nativeTexture != NULL)
			{
				nativeBrush = &nativeTexture->base;
			}
		}
		TextureBrush(Image* image, WrapMode wrapMode, const Rect& dstRect)
		{
			GpTexture* nativeTexture = NULL;
			lastStatus = DLLExports::GdipCreateTexture2I(
				image ? image->nativeImage : NULL, wrapMode,
				dstRect.X, dstRect.Y,
				dstRect.Width, dstRect.Height, &nativeTexture);
			if (nativeTexture != NULL)
			{
				nativeBrush = &nativeTexture->base;
			}
		}
		TextureBrush(Image* image, const RectF& dstRect,
			ImageAttributes* imageAttributes = NULL)
		{
			GpTexture* nativeTexture = NULL;
			lastStatus = DLLExports::GdipCreateTextureIA(
				image ? image->nativeImage : NULL,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
				dstRect.X, dstRect.Y,
				dstRect.Width, dstRect.Height, &nativeTexture);
			if (nativeTexture != NULL)
			{
				nativeBrush = &nativeTexture->base;
			}
		}
		TextureBrush(Image* image, const Rect& dstRect,
			ImageAttributes* imageAttributes = NULL)
		{
			GpTexture* nativeTexture = NULL;
			lastStatus = DLLExports::GdipCreateTextureIAI(
				image ? image->nativeImage : NULL,
				imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
				dstRect.X, dstRect.Y,
				dstRect.Width, dstRect.Height, &nativeTexture);
			if (nativeTexture != NULL)
			{
				nativeBrush = &nativeTexture->base;
			}
		}
		/* WASM: NOT SUPPORTED
		virtual TextureBrush* Clone() const
		{
			GpBrush *cloneBrush = NULL;
			GpStatus status = updateStatus( DLLExports::GdipCloneBrush(
					(TextureBrush*)nativeBrush, &cloneBrush));
			if (status == GpStatus::Ok) {
				TextureBrush *result =
					new TextureBrush(cloneBrush, lastStatus);
				if (!result) {
					 DLLExports::GdipDeleteBrush(cloneBrush);
					updateStatus(OutOfMemory);
				}
				return result;
			} else {
				return NULL;
			}
		}*/

		//TODO: implement TextureBrush::GetImage()
		//Image *GetImage() const
		//{
		//	// where is the Image allocated (static,member,new,other)?
		//	//  DLLExports::GdipGetTextureImage just returns a GpImage*
		//	updateStatus(NotImplemented);
		//	return NULL;
		//}
		GpStatus GetTransfrom(Matrix* matrix) const
		{
			return updateStatus(DLLExports::GdipGetTextureTransform(
				(GpTexture*)nativeBrush,
				matrix ? matrix->nativeMatrix : NULL));
		}
		WrapMode GetWrapMode() const
		{
			WrapMode result = WrapModeTile;
			updateStatus(DLLExports::GdipGetTextureWrapMode(
				(GpTexture*)nativeBrush, &result));
			return result;
		}
		GpStatus MultiplyTransform(const Matrix* matrix,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipMultiplyTextureTransform(
				(GpTexture*)nativeBrush,
				matrix ? matrix->nativeMatrix : NULL, order));
		}
		GpStatus ResetTransform()
		{
			return updateStatus(DLLExports::GdipResetTextureTransform(
				(GpTexture*)nativeBrush));
		}
		GpStatus RotateTransform(REAL angle,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipRotateTextureTransform(
				(GpTexture*)nativeBrush, angle, order));
		}
		GpStatus ScaleTransform(REAL sx, REAL sy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipScaleTextureTransform(
				(GpTexture*)nativeBrush, sx, sy, order));
		}
		GpStatus SetTransform(const Matrix* matrix)
		{
			return updateStatus(DLLExports::GdipSetTextureTransform(
				(GpTexture*)nativeBrush,
				matrix ? matrix->nativeMatrix : NULL));
		}
		GpStatus SetWrapMode(WrapMode wrapMode)
		{
			return updateStatus(DLLExports::GdipSetTextureWrapMode(
				(GpTexture*)nativeBrush, wrapMode));
		}
		GpStatus TranslateTransform(REAL dx, REAL dy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipTranslateTextureTransform(
				(GpTexture*)nativeBrush, dx, dy, order));
		}

	private:
		TextureBrush(GpBrush* brush, GpStatus status) : Brush(brush, status) {}
		TextureBrush(const TextureBrush&);
		TextureBrush& operator=(const TextureBrush&);
	};
}
#endif /* __GDIPLUS_BRUSH_H */
