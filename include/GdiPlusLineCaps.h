// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_LINECAPS_H
#define __GDIPLUS_LINECAPS_H

#include <MSFS\MSFS_WindowsTypes.h>
#include "GdiPlusTypes.h"
#include "GdiPlusEnums.h"
#include "GdiPlusPath.h"

#ifndef __cplusplus
#error "A C++ compiler is required to include gdipluslinecaps.h."
#endif
namespace Gdiplus {

	class CustomLineCap
	{
	public:
		CustomLineCap(const GraphicsPath* fillPath, const GraphicsPath* strokePath, LineCap baseCap = LineCapFlat, REAL baseInset = 0.0f);

		/**** NOT SUPPORTED YET***\
		inline
		CustomLineCap::CustomLineCap();

		inline
		CustomLineCap::~CustomLineCap();

		inline Status
		CustomLineCap::SetStrokeCaps(
			IN LineCap startCap,
			IN LineCap endCap);

		inline Status
		CustomLineCap::GetStrokeCaps(
			OUT LineCap* startCap,
			OUT LineCap* endCap) const;

		inline Status
		CustomLineCap::SetStrokeJoin(
			IN LineJoin lineJoin);

		inline LineJoin
		CustomLineCap::GetStrokeJoin() const;

		inline Status
		CustomLineCap::SetBaseCap(IN LineCap baseCap);

		inline LineCap
		CustomLineCap::GetBaseCap() const;

		inline Status
		CustomLineCap::SetBaseInset(IN REAL inset);

		inline REAL
		CustomLineCap::GetBaseInset() const;

		inline Status
		CustomLineCap::SetWidthScale(IN REAL widthScale);
		inline REAL
		CustomLineCap::GetWidthScale() const;

		inline CustomLineCap*
		CustomLineCap::Clone() const;

		inline Status
		CustomLineCap::GetLastStatus() const;

		class AdjustableArrowCap : public CustomLineCap
		{
		public:

			AdjustableArrowCap(
				IN REAL height,
				IN REAL width,
				IN BOOL isFilled = TRUE
				);
			Status SetHeight(IN REAL height);

			REAL GetHeight() const;

			Status SetWidth(IN REAL width);
			REAL GetWidth() const;

			Status SetMiddleInset(IN REAL middleInset);

			REAL GetMiddleInset() const;

			Status SetFillState(IN BOOL isFilled);

			BOOL IsFilled() const;

		private:
			AdjustableArrowCap(const AdjustableArrowCap &);
			AdjustableArrowCap& operator=(const AdjustableArrowCap &);
	\**** NOT SUPPORTED YET***/
	};
}
#endif /* __GDIPLUS_LINECAPS_H */
