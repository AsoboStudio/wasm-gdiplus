// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_PEN_H
#define __GDIPLUS_PEN_H
#include <MSFS\MSFS_WindowsTypes.h>
#include <MSFS\Render\GdiPlus\GdiPlusTypes.h>
#include <MSFS\Render\GdiPlus\GdiPlusEnums.h>
#include <MSFS\Render\GdiPlus\GdiPlusColor.h>
#include <MSFS\Render\GdiPlus\GdiPlusLineCaps.h>

#ifndef __cplusplus
#error "A C++ compiler is required to include gdipluspen.h."
#endif
namespace Gdiplus {

	class Pen//: public GdiplusBase
	{
		friend class Graphics;
		friend class GraphicsPath;

	public:
		Color color;
		float width;

		Pen(Color _color, float _width = 1.0f);
		void GetColor(Color* _color);
		Status SetColor(const Color& _color);
		float GetWidth();
		Status SetWidth(float _width);
		void SetAlignment(PenAlignment _Alignment);
		Status SetDashPattern(const REAL* dashArray, INT count);
		Status SetDashStyle(DashStyle dashStyle);
		Status SetEndCap(LineCap endCap);
		Status SetCustomEndCap(const CustomLineCap* customCap);
		Status SetCustomStartCap(const CustomLineCap* customCap);

		/**** NOT SUPPORTED YET***\
		Pen(const Color& color, REAL width = 1.0f):
			nativePen(NULL), lastStatus(Ok);
		Pen(const Brush *brush, REAL width = 1.0f):
			nativePen(NULL), lastStatus(Ok);
		~Pen();
		Pen* Clone() const;
		Brush *GetBrush() const;
		Status GetColor(Color *color) const
		Status GetCompoundArray(REAL *compoundArray, INT count) const;
		INT GetCompoundArrayCount() const;
		Status GetCustomEndCap(CustomLineCap *customCap) const;
		Status GetCustomStartCap(CustomLineCap *customCap) const;
		DashCap GetDashCap() const;
		REAL GetDashOffset() const;
		Status GetDashPattern(REAL *dashArray, INT count) const;
		INT GetDashPatternCount() const;
		DashStyle GetDashStyle() const;
		LineCap GetEndCap() const;
		Status GetLastStatus() const;
		LineJoin GetLineJoin() const;
		REAL GetMiterLimit() const;
		PenType GetPenType() const;
		LineCap GetStartCap() const;
		Status GetTransform(Matrix *matrix) const;
		Status MultiplyTransform(const Matrix *matrix,
				MatrixOrder order = MatrixOrderPrepend);
		Status ResetTransform();
		Status RotateTransform(REAL angle,
				MatrixOrder order = MatrixOrderPrepend);
		Status ScaleTransform(REAL sx, REAL sy,
				MatrixOrder order = MatrixOrderPrepend);
		Status SetAlignment(PenAlignment penAlignment);
		Status SetBrush(const Brush *brush);
		Status SetColor(const Color& color);
		Status SetCompoundArray(const REAL *compoundArray, INT count);
		Status SetCustomEndCap(const CustomLineCap *customCap);
		Status SetCustomStartCap(const CustomLineCap *customCap);
		Status SetDashCap(DashCap dashCap);
		Status SetDashOffset(REAL dashOffset);
		Status SetDashPattern(const REAL *dashArray, INT count);
		Status SetDashStyle(DashStyle dashStyle);
		Status SetLineCap(LineCap startCap, LineCap endCap, DashCap dashCap);
		Status SetLineJoin(LineJoin lineJoin);
		Status SetMiterLimit(REAL miterLimit);
		Status SetStartCap(LineCap startCap);
		Status SetTransform(const Matrix *matrix);
		Status SetWidth(REAL width);
		Status TranslateTransform(REAL dx, REAL dy,
				MatrixOrder order = MatrixOrderPrepend);

	private:
		Pen(GpPen *pen, Status status): nativePen(pen), lastStatus(status);
		Pen(const Pen& pen);
		Pen& operator=(const Pen&);
		Status updateStatus(Status newStatus) const;
		\**** NOT SUPPORTED YET***/
	};
}
#endif /* __GDIPLUS_PEN_H */
