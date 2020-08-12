// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_BRUSH_H
#define __GDIPLUS_BRUSH_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusbrush.h."
#endif

#include <MSFS\MSFS_WindowsTypes.h>
#include "gdiplusheaders.h"
#include "gdipluscolor.h"
namespace Gdiplus {

	class Brush//: public GdiplusBase
	{
		friend class HatchBrush;
		friend class LinearGradientBrush;
		friend class PathGradientBrush;
		friend class SolidBrush;
		friend class TextureBrush;
		friend class Graphics;
		friend class Pen;

	public:
		virtual int GetType() const = 0;
		virtual Color GetColor() const;

		/**** NOT SUPPORTED YET***\
	public:
		virtual ~Brush();
		virtual Brush* Clone() const;
		Status GetLastStatus() const;

	protected:
		Brush();
	private:
		Brush(const Brush& brush);
		Brush& operator=(const Brush& brush);
	protected:
		Brush(GpBrush* nativeBrush, Status status);
		VOID SetNativeBrush(GpBrush* nativeBrush);
		Status SetStatus(Status status) const;
		GpBrush* nativeBrush;
		mutable Status lastResult;
		\**** NOT SUPPORTED YET***/
	};

	class HatchBrush : public Brush
	{
	public:
		friend class Pen;
		friend Graphics;

		HatchBrush(IN HatchStyle hatchStyle,
			IN const Color& foreColor,
			IN const Color& backColor = Color());
		virtual int GetType() const;

		HatchStyle style;
		Color foreColor;
		Color backColor;
		int patternImage;

		/**** NOT SUPPORTED YET***\
		HatchStyle GetHatchStyle() const;
		Status GetForegroundColor(OUT Color* color) const;
		Status GetBackgroundColor(OUT Color* color) const;
	private:
		HatchBrush(const HatchBrush&);
		HatchBrush& operator=(const HatchBrush&);

	protected:

		HatchBrush();
		\**** NOT SUPPORTED YET***/
	};

	class SolidBrush : public Brush
	{
	public:
		friend class Pen;
	public:
		Color color;

		SolidBrush(IN const Color& color);
		void GetColor(OUT Color* _color) const;
		virtual Color GetColor() const;
		void SetColor(IN const Color& color);
		virtual int GetType() const;

		/**** NOT SUPPORTED YET***\

	private:
		SolidBrush(const SolidBrush &);
		SolidBrush& operator=(const SolidBrush &);

	protected:
		SolidBrush();
		\**** NOT SUPPORTED YET***/

	};
	/**** NOT SUPPORTED YET***\
		class TextureBrush : public Brush
		{
		public:
			friend class Pen;

			TextureBrush(IN Image* image,
				IN WrapMode wrapMode = WrapModeTile);

			// When creating a texture brush from a metafile image, the dstRect
			// is used to specify the size that the metafile image should be
			// rendered at in the device units of the destination graphics.
			// It is NOT used to crop the metafile image, so only the width
			// and height values matter for metafiles.

			TextureBrush(IN Image* image,
				IN WrapMode wrapMode,
				IN const RectF& dstRect);

			TextureBrush(IN Image* image,
				IN const RectF& dstRect,
				IN const ImageAttributes* imageAttributes = NULL);

			TextureBrush(IN Image* image,
				IN const Rect& dstRect,
				IN const ImageAttributes* imageAttributes = NULL);

			TextureBrush(
				IN Image* image,
				IN WrapMode wrapMode,
				const IN Rect& dstRect
			);
			TextureBrush(IN Image* image,
				IN WrapMode wrapMode,
				IN REAL dstX,
				IN REAL dstY,
				IN REAL dstWidth,
				IN REAL dstHeight);

			TextureBrush(IN Image* image,
				IN WrapMode wrapMode,
				IN INT dstX,
				IN INT dstY,
				IN INT dstWidth,
				IN INT dstHeight);

			Status SetTransform(IN const Matrix* matrix);

			Status GetTransform(OUT Matrix* matrix) const;

			Status ResetTransform();

			Status MultiplyTransform(IN const Matrix* matrix,
				IN MatrixOrder order = MatrixOrderPrepend);

			Status TranslateTransform(IN REAL dx,
				IN REAL dy,
				IN MatrixOrder order = MatrixOrderPrepend);

			Status ScaleTransform(IN REAL sx,
				IN REAL sy,
				IN MatrixOrder order = MatrixOrderPrepend);

			Status RotateTransform(IN REAL angle,
				IN MatrixOrder order = MatrixOrderPrepend);

			Status SetWrapMode(IN WrapMode wrapMode);

			WrapMode GetWrapMode() const;

			Image* GetImage() const;

		private:
			TextureBrush(const TextureBrush&);
			TextureBrush& operator=(const TextureBrush&);

		protected:

			TextureBrush();
		};
	}
		\**** NOT SUPPORTED YET***/


		/**** NOT SUPPORTED YET***\
		class LinearGradientBrush : public Brush
		{
		public:
			friend class Pen;

			LinearGradientBrush(IN const PointF& point1,
				IN const PointF& point2,
				IN const Color& color1,
				IN const Color& color2);

			LinearGradientBrush(IN const Point& point1,
				IN const Point& point2,
				IN const Color& color1,
				IN const Color& color2);

			LinearGradientBrush(IN const RectF& rect,
				IN const Color& color1,
				IN const Color& color2,
				IN LinearGradientMode mode);

			LinearGradientBrush(IN const Rect& rect,
				IN const Color& color1,
				IN const Color& color2,
				IN LinearGradientMode mode);

			LinearGradientBrush(IN const RectF& rect,
				IN const Color& color1,
				IN const Color& color2,
				IN REAL angle,
				IN BOOL isAngleScalable = FALSE);

			LinearGradientBrush(IN const Rect& rect,
				IN const Color& color1,
				IN const Color& color2,
				IN REAL angle,
				IN BOOL isAngleScalable = FALSE);

			Status SetLinearColors(IN const Color& color1,
				IN const Color& color2);

			Status GetLinearColors(OUT Color* colors) const;

			Status GetRectangle(OUT RectF* rect) const;

			Status GetRectangle(OUT Rect* rect) const;

			Status SetGammaCorrection(IN BOOL useGammaCorrection);

			BOOL GetGammaCorrection() const;

			INT GetBlendCount() const;

			Status SetBlend(IN const REAL* blendFactors,
				IN const REAL* blendPositions,
				IN INT count);

			Status GetBlend(OUT REAL* blendFactors,
				OUT REAL* blendPositions,
				IN INT count) const;

			INT GetInterpolationColorCount() const;

			Status SetInterpolationColors(IN const Color* presetColors,
				IN const REAL* blendPositions,
				IN INT count);

			Status GetInterpolationColors(OUT Color* presetColors,
				OUT REAL* blendPositions,
				IN INT count) const;

			Status SetBlendBellShape(IN REAL focus,
				IN REAL scale = 1.0f);

			Status SetBlendTriangularShape(
				IN REAL focus,
				IN REAL scale = 1.0f
			);

			Status SetTransform(IN const Matrix* matrix);

			Status GetTransform(OUT Matrix* matrix) const;

			Status ResetTransform();

			Status MultiplyTransform(IN const Matrix* matrix,
				IN MatrixOrder order = MatrixOrderPrepend);

			Status TranslateTransform(IN REAL dx,
				IN REAL dy,
				IN MatrixOrder order = MatrixOrderPrepend);

			Status ScaleTransform(IN REAL sx,
				IN REAL sy,
				IN MatrixOrder order = MatrixOrderPrepend);
			Status RotateTransform(IN REAL angle,
				IN MatrixOrder order = MatrixOrderPrepend)
			{
				return SetStatus(DllExports::GdipRotateLineTransform((GpLineGradient*)nativeBrush,
					angle, order));
			}

			Status SetWrapMode(IN WrapMode wrapMode);

			WrapMode GetWrapMode() const;

		private:
			LinearGradientBrush(const LinearGradientBrush&);
			LinearGradientBrush& operator=(const LinearGradientBrush&);

		protected:

			LinearGradientBrush();
		};
	}
		\**** NOT SUPPORTED YET***/

}
#endif /* __GDIPLUS_BRUSH_H */
