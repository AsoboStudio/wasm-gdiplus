// (c)1998-2018 Codejock Software, All Rights Reserved.
//
// THIS CLASS IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
/*
//{{AFX_CODEJOCK_PRIVATE
#ifndef _XTP_GDIPLUS_H
#define _XTP_GDIPLUS_H
//}}AFX_CODEJOCK_PRIVATE

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include "GdiPlus.h"
#pragma comment(lib, "GdiPlus.lib")
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#ifndef GdipCreateFontFromLogfont
#ifdef _UNICODE
#define GdipCreateFontFromLogfont GdipCreateFontFromLogfontW
#else
#define GdipCreateFontFromLogfont GdipCreateFontFromLogfontA
#endif
#endif

#endif /*_XTP_GDIPLUS_H*/
*/