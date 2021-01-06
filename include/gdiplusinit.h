// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_INIT_H
#define __GDIPLUS_INIT_H

#include "GdiPlus.h"
#include "gdiplusflat.h"

#define GdiplusStartupInput DLLExports::GdiplusStartupInput
#define GdiplusStartupOutput DLLExports::GdiplusStartupOutput
#define NotificationHookProc DLLExports::NotificationHookProc
#define NotificationUnhookProc DLLExports::NotificationUnhookProc

#define GdiplusStartup DLLExports::GdiplusStartup
#define GdiplusShutdown DLLExports::GdiplusShutdown
#define GdiplusNotificationHook DLLExports:: GdiplusNotificationHook
#define GdiplusNotificationUnhook DLLExports::GdiplusNotificationUnhook

#if 0
typedef struct GdiplusStartupInput {
	UINT32 GdiplusVersion;
	DebugEventProc DebugEventCallback;
	BOOL SuppressBackgroundThread;
	BOOL SuppressExternalCodecs;

	#ifdef __cplusplus
	GdiplusStartupInput(DebugEventProc debugEventCallback = NULL,
	                    BOOL suppressBackgroundThread = FALSE,
	                    BOOL suppressExternalCodecs = FALSE):
		GdiplusVersion(1),
		DebugEventCallback(debugEventCallback),
		SuppressBackgroundThread(suppressBackgroundThread),
		SuppressExternalCodecs(suppressExternalCodecs) {}
	#endif /* __cplusplus */
} GdiplusStartupInput;

typedef GpStatus WINGDIPAPI (*NotificationHookProc)(ULONG_PTR *token);
typedef VOID WINGDIPAPI (*NotificationUnhookProc)(ULONG_PTR token);

typedef struct GdiplusStartupOutput {
	NotificationHookProc NotificationHook;
	NotificationUnhookProc NotificationUnhook;

	#ifdef __cplusplus
	GdiplusStartupOutput():
		NotificationHook(NULL),
		NotificationUnhook(NULL) {}
	#endif /* __cplusplus */
} GdiplusStartupOutput;

/*
#ifdef __cplusplus
extern "C" {
#endif

GpStatus WINGDIPAPI GdiplusStartup(ULONG_PTR*,GDIPCONST GdiplusStartupInput*,GdiplusStartupOutput*);
VOID WINGDIPAPI GdiplusShutdown(ULONG_PTR);
GpStatus WINGDIPAPI GdiplusNotificationHook(ULONG_PTR*);
VOID WINGDIPAPI GdiplusNotificationUnhook(ULONG_PTR);

#ifdef __cplusplus
}  // extern "C" 
#endif
*/
#endif

#endif /* __GDIPLUS_INIT_H */
