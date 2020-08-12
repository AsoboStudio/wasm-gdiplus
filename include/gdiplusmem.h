// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_MEM_H
#define __GDIPLUS_MEM_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif
namespace Gdiplus {

#ifdef __cplusplus
    namespace DllExports {
        extern "C" {
#endif

            VOID* WINGDIPAPI GdipAlloc(size_t);
            VOID WINGDIPAPI GdipFree(VOID*);

#ifdef __cplusplus
        }  /* extern "C" */
    }  /* namespace DllExports */
#endif
}
#endif /* __GDIPLUS_MEM_H */
