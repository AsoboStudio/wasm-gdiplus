// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_PEN_H
#define __GDIPLUS_PEN_H

#include "GdiPlus.h"
#include "gdiplusflat.h"

namespace Gdiplus
{
	class Pen : public GdiplusBase
	{
		friend class Graphics;
		friend class GraphicsPath;
	private:
		GpPen* nativePen;
	public:
		Pen(const Color& color, REAL width = 1.0f) :
			nativePen(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreatePen1(
				color.GetValue(), width, UnitWorld,
				&nativePen);
		}
		Pen(const Brush* brush, REAL width = 1.0f) :
			nativePen(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreatePen2(
				brush ? brush->nativeBrush : NULL,
				width, UnitWorld, &nativePen);
		}
		~Pen()
		{
			DLLExports::GdipDeletePen(nativePen);
		}
		Pen* Clone() const
		{
			GpPen* clonePen = NULL;
			Status status = updateStatus(DLLExports::GdipClonePen(
				nativePen, &clonePen));
			if (status == GpStatus::Ok) {
				Pen* result = new Pen(clonePen, lastStatus);
				if (!result) {
					DLLExports::GdipDeletePen(clonePen);
					lastStatus = OutOfMemory;
				}
				return result;
			}
			else {
				return NULL;
			}
		}

		PenAlignment GetAlignment() const
		{
			PenAlignment result = PenAlignmentCenter;
			updateStatus(DLLExports::GdipGetPenMode(nativePen, &result));
			return result;
		}
		// TODO: implement Pen::GetBrush()
		//Brush *GetBrush() const
		//{
		//	// where is the pen brush allocated (static,member,new,other)?
		//	// GdipGetPenBrushFill just returns a GpBrush*
		//	updateStatus(NotImplemented);
		//	return NULL;
		//}
		Status GetColor(Color* color) const
		{
			return updateStatus(DLLExports::GdipGetPenColor(
				nativePen, color ? &color->Value : NULL));
		}
		Status GetCompoundArray(REAL* compoundArray, INT count) const
		{
			return updateStatus(DLLExports::GdipGetPenCompoundArray(
				nativePen, compoundArray, count));
		}
		INT GetCompoundArrayCount() const
		{
			INT result = 0;
			updateStatus(DLLExports::GdipGetPenCompoundCount(
				nativePen, &result));
			return result;
		}
		Status GetCustomEndCap(CustomLineCap* customCap) const
		{
			if (!customCap) return lastStatus = GpStatus::InvalidParameter;
			// FIXME: do we need to call GdipDeleteCustomLineCap first?
			return updateStatus(DLLExports::GdipGetPenCustomEndCap(
				nativePen, &customCap->nativeCustomLineCap));
		}
		Status GetCustomStartCap(CustomLineCap* customCap) const
		{
			if (!customCap) return lastStatus = GpStatus::InvalidParameter;
			// FIXME: do we need to call GdipDeleteCustomLineCap first?
			return updateStatus(DLLExports::GdipGetPenCustomStartCap(
				nativePen, &customCap->nativeCustomLineCap));
		}
		DashCap GetDashCap() const
		{
			DashCap result = DashCapFlat;
			updateStatus(DLLExports::GdipGetPenDashCap197819(
				nativePen, &result));
			return result;
		}
		REAL GetDashOffset() const
		{
			REAL result = 0.0f;
			updateStatus(DLLExports::GdipGetPenDashOffset(
				nativePen, &result));
			return result;
		}
		Status GetDashPattern(REAL* dashArray, INT count) const
		{
			return updateStatus(DLLExports::GdipGetPenDashArray(
				nativePen, dashArray, count));
		}
		INT GetDashPatternCount() const
		{
			INT result = 0;
			updateStatus(DLLExports::GdipGetPenDashCount(
				nativePen, &result));
			return result;
		}
		DashStyle GetDashStyle() const
		{
			DashStyle result = DashStyleSolid;
			updateStatus(DLLExports::GdipGetPenDashStyle(
				nativePen, &result));
			return result;
		}
		LineCap GetEndCap() const
		{
			LineCap result = LineCapFlat;
			updateStatus(DLLExports::GdipGetPenEndCap(nativePen, &result));
			return result;
		}
		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}
		LineJoin GetLineJoin() const
		{
			LineJoin result = LineJoinMiter;
			updateStatus(DLLExports::GdipGetPenLineJoin(
				nativePen, &result));
			return result;
		}
		REAL GetMiterLimit() const
		{
			REAL result = 10.0f;
			updateStatus(DLLExports::GdipGetPenMiterLimit(
				nativePen, &result));
			return result;
		}
		PenType GetPenType() const
		{
			PenType result = PenTypeUnknown;
			updateStatus(DLLExports::GdipGetPenFillType(
				nativePen, &result));
			return result;
		}
		LineCap GetStartCap() const
		{
			LineCap result = LineCapFlat;
			updateStatus(DLLExports::GdipGetPenStartCap(
				nativePen, &result));
			return result;
		}
		Status GetTransform(Matrix* matrix) const
		{
			return updateStatus(DLLExports::GdipGetPenTransform(
				nativePen,
				matrix ? matrix->nativeMatrix : NULL));
		}
		REAL GetWidth() const
		{
			REAL result = 1.0f;
			updateStatus(DLLExports::GdipGetPenWidth(nativePen, &result));
			return result;
		}
		Status MultiplyTransform(const Matrix* matrix,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipMultiplyPenTransform(
				nativePen,
				matrix ? matrix->nativeMatrix : NULL, order));
		}
		Status ResetTransform()
		{
			return updateStatus(DLLExports::GdipResetPenTransform(
				nativePen));
		}
		Status RotateTransform(REAL angle,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipRotatePenTransform(
				nativePen, angle, order));
		}
		Status ScaleTransform(REAL sx, REAL sy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipScalePenTransform(
				nativePen, sx, sy, order));
		}
		Status SetAlignment(PenAlignment penAlignment)
		{
			return updateStatus(DLLExports::GdipSetPenMode(
				nativePen, penAlignment));
		}
		Status SetBrush(const Brush* brush)
		{
			return updateStatus(DLLExports::GdipSetPenBrushFill(
				nativePen, brush ? brush->nativeBrush : NULL));
		}
		Status SetColor(const Color& color)
		{
			return updateStatus(DLLExports::GdipSetPenColor(
				nativePen, color.GetValue()));
		}
		Status SetCompoundArray(const REAL* compoundArray, INT count)
		{
			return updateStatus(DLLExports::GdipSetPenCompoundArray(
				nativePen, compoundArray, count));
		}
		Status SetCustomEndCap(const CustomLineCap* customCap)
		{
			return updateStatus(DLLExports::GdipSetPenCustomEndCap(
				nativePen,
				customCap ? customCap->nativeCustomLineCap : NULL));
		}
		Status SetCustomStartCap(const CustomLineCap* customCap)
		{
			return updateStatus(DLLExports::GdipSetPenCustomStartCap(
				nativePen,
				customCap ? customCap->nativeCustomLineCap : NULL));
		}
		Status SetDashCap(DashCap dashCap)
		{
			return updateStatus(DLLExports::GdipSetPenDashCap197819(
				nativePen, dashCap));
		}
		Status SetDashOffset(REAL dashOffset)
		{
			return updateStatus(DLLExports::GdipSetPenDashOffset(
				nativePen, dashOffset));
		}
		Status SetDashPattern(const REAL* dashArray, INT count)
		{
			return updateStatus(DLLExports::GdipSetPenDashArray(
				nativePen, dashArray, count));
		}
		Status SetDashStyle(DashStyle dashStyle)
		{
			return updateStatus(DLLExports::GdipSetPenDashStyle(
				nativePen, dashStyle));
		}
		Status SetEndCap(LineCap endCap)
		{
			return updateStatus(DLLExports::GdipSetPenEndCap(
				nativePen, endCap));
		}
		Status SetLineCap(LineCap startCap, LineCap endCap, DashCap dashCap)
		{
			return updateStatus(DLLExports::GdipSetPenLineCap197819(
				nativePen, startCap, endCap, dashCap));
		}
		Status SetLineJoin(LineJoin lineJoin)
		{
			return updateStatus(DLLExports::GdipSetPenLineJoin(
				nativePen, lineJoin));
		}
		Status SetMiterLimit(REAL miterLimit)
		{
			return updateStatus(DLLExports::GdipSetPenMiterLimit(
				nativePen, miterLimit));
		}
		Status SetStartCap(LineCap startCap)
		{
			return updateStatus(DLLExports::GdipSetPenStartCap(
				nativePen, startCap));
		}
		Status SetTransform(const Matrix* matrix)
		{
			return updateStatus(DLLExports::GdipSetPenTransform(
				nativePen,
				matrix ? matrix->nativeMatrix : NULL));
		}
		Status SetWidth(REAL width)
		{
			return updateStatus(DLLExports::GdipSetPenWidth(
				nativePen, width));
		}
		Status TranslateTransform(REAL dx, REAL dy,
			MatrixOrder order = MatrixOrderPrepend)
		{
			return updateStatus(DLLExports::GdipTranslatePenTransform(
				nativePen, dx, dy, order));
		}

	private:
		Pen(GpPen* pen, Status status) : nativePen(pen), lastStatus(status) {}
		Pen(const Pen& pen);
		Pen& operator=(const Pen&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != GpStatus::Ok) lastStatus = newStatus;
			return newStatus;
		}

		mutable Status lastStatus;
	};
}
#endif /* __GDIPLUS_PEN_H */
