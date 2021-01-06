// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>

#ifndef __GDIPLUS_LINECAPS_H
#define __GDIPLUS_LINECAPS_H

namespace Gdiplus
{
	class AdjustableArrowCap : public CustomLineCap
	{
	private:
		GpAdjustableArrowCap* nativeCustomLineCap;
	public:
		AdjustableArrowCap(REAL height, REAL width, BOOL isFilled) :
			CustomLineCap(NULL, GpStatus::Ok)
		{
			GpAdjustableArrowCap* nativeAdjustableArrowCap = NULL;
			lastStatus = DLLExports::GdipCreateAdjustableArrowCap(
				height, width, isFilled,
				&nativeAdjustableArrowCap);
			nativeCustomLineCap = nativeAdjustableArrowCap;
		}
		virtual ~AdjustableArrowCap()
		{
		}
		/* WASM: NOT SUPPORTED
		virtual AdjustableArrowCap* Clone() const
		{
			GpCustomLineCap *cloneCustomLineCap = NULL;
			Status status = updateStatus(DLLExports::GdipCloneCustomLineCap(
					nativeCustomLineCap, &cloneCustomLineCap));
			if (status == GpStatus::Ok) {
				AdjustableArrowCap *result = new AdjustableArrowCap(
						cloneCustomLineCap, lastStatus);
				if (!result) {
					DLLExports::GdipDeleteCustomLineCap(
							cloneCustomLineCap);
					lastStatus = OutOfMemory;
				}
				return result;
			} else {
				return NULL;
			}
		}*/

		REAL GetHeight() const
		{
			REAL result = 0.0f;
			updateStatus(DLLExports::GdipGetAdjustableArrowCapHeight(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				&result));
			return result;
		}
		REAL GetMiddleInset() const
		{
			REAL result = 0.0f;
			updateStatus(DLLExports::GdipGetAdjustableArrowCapMiddleInset(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				&result));
			return result;
		}
		REAL GetWidth() const
		{
			REAL result = 0.0f;
			updateStatus(DLLExports::GdipGetAdjustableArrowCapWidth(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				&result));
			return result;
		}
		BOOL IsFilled() const
		{
			BOOL result = FALSE;
			updateStatus(DLLExports::GdipGetAdjustableArrowCapFillState(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				&result));
			return result;
		}
		Status SetFillState(BOOL isFilled)
		{
			return updateStatus(DLLExports::GdipSetAdjustableArrowCapFillState(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				isFilled));
		}
		Status SetHeight(REAL height)
		{
			return updateStatus(DLLExports::GdipSetAdjustableArrowCapHeight(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				height));
		}
		Status SetMiddleInset(REAL middleInset)
		{
			return updateStatus(DLLExports::GdipSetAdjustableArrowCapMiddleInset(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				middleInset));
		}
		Status SetWidth(REAL width)
		{
			return updateStatus(DLLExports::GdipSetAdjustableArrowCapWidth(
				(GpAdjustableArrowCap*)nativeCustomLineCap,
				width));
		}

	private:
		AdjustableArrowCap(GpCustomLineCap* customLineCap, Status status) :
			CustomLineCap(customLineCap, status) {}
		AdjustableArrowCap(const AdjustableArrowCap&);
		AdjustableArrowCap& operator=(const AdjustableArrowCap&);
	};
}
#endif /* __GDIPLUS_LINECAPS_H */
