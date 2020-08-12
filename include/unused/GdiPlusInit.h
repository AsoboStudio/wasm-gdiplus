/*
 * Copyright (C) 2020 Asobo Studio (https://www.asobostudio.com/)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _GDIPLUSINIT_H
#define _GDIPLUSINIT_H

namespace Gdiplus
{/**** NOT SUPPORTED YET***\
    enum DebugEventLevel
    {
        DebugEventLevelFatal,
        DebugEventLevelWarning
    };

    // Callback function that GDI+ can call, on debug builds, for assertions
    // and warnings.

    typedef VOID(WINAPI* DebugEventProc)(DebugEventLevel level, CHAR* message);

    // Notification functions which the user must call appropriately if
    // "SuppressBackgroundThread" (below) is set.

    typedef Status(WINAPI* NotificationHookProc)(OUT ULONG_PTR* token);
    typedef VOID(WINAPI* NotificationUnhookProc)(ULONG_PTR token);

    // Input structure for GdiplusStartup()

    struct GdiplusStartupInput
    {
        UINT32 GdiplusVersion;             // Must be 1  (or 2 for the Ex version)
        DebugEventProc DebugEventCallback; // Ignored on free builds
        BOOL SuppressBackgroundThread;     // FALSE unless you're prepared to call 
                                           // the hook/unhook functions properly
        BOOL SuppressExternalCodecs;       // FALSE unless you want GDI+ only to use
                                           // its internal image codecs.

        GdiplusStartupInput(
            DebugEventProc debugEventCallback = NULL,
            BOOL suppressBackgroundThread = FALSE,
            BOOL suppressExternalCodecs = FALSE)
        {
            GdiplusVersion = 1;
            DebugEventCallback = debugEventCallback;
            SuppressBackgroundThread = suppressBackgroundThread;
            SuppressExternalCodecs = suppressExternalCodecs;
        }
    };

#if (GDIPVER >= 0x0110)
    struct GdiplusStartupInputEx : GdiplusStartupInput
    {
        INT StartupParameters;  // Do we not set the FPU rounding mode

        GdiplusStartupInputEx(
            INT startupParameters = 0,
            DebugEventProc debugEventCallback = NULL,
            BOOL suppressBackgroundThread = FALSE,
            BOOL suppressExternalCodecs = FALSE)
        {
            GdiplusVersion = 2;
            DebugEventCallback = debugEventCallback;
            SuppressBackgroundThread = suppressBackgroundThread;
            SuppressExternalCodecs = suppressExternalCodecs;
            StartupParameters = startupParameters;
        }
    };

    enum GdiplusStartupParams
    {
        GdiplusStartupDefault = 0,
        GdiplusStartupNoSetRound = 1,
        GdiplusStartupSetPSValue = 2,
        GdiplusStartupTransparencyMask = 0xFF000000
    };

#endif


    // Output structure for GdiplusStartup()

    struct GdiplusStartupOutput
    {
        // The following 2 fields are NULL if SuppressBackgroundThread is FALSE.
        // Otherwise, they are functions which must be called appropriately to
        // replace the background thread.
        //
        // These should be called on the application's main message loop - i.e.
        // a message loop which is active for the lifetime of GDI+.
        // "NotificationHook" should be called before starting the loop,
        // and "NotificationUnhook" should be called after the loop ends.

        NotificationHookProc NotificationHook;
        NotificationUnhookProc NotificationUnhook;
    };

    // GDI+ initialization. Must not be called from DllMain - can cause deadlock.
    //
    // Must be called before GDI+ API's or constructors are used.
    //
    // token  - may not be NULL - accepts a token to be passed in the corresponding
    //          GdiplusShutdown call.
    // input  - may not be NULL
    // output - may be NULL only if input->SuppressBackgroundThread is FALSE.

    extern "C" Status WINAPI GdiplusStartup(
        OUT ULONG_PTR * token,
        const GdiplusStartupInput * input,
        OUT GdiplusStartupOutput * output);

    // GDI+ termination. Must be called before GDI+ is unloaded. 
    // Must not be called from DllMain - can cause deadlock.
    //
    // GDI+ API's may not be called after GdiplusShutdown. Pay careful attention
    // to GDI+ object destructors.

    extern "C" VOID WINAPI GdiplusShutdown(ULONG_PTR token);
\**** NOT SUPPORTED YET***/
}

#endif

