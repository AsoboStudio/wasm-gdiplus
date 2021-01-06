// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>

#ifndef __GDIPLUS_MEM_H
#define __GDIPLUS_MEM_H

#ifdef __cplusplus
namespace DLLExports {
extern "C" {
#endif

// WASM TODO: replace with new / delete
//VOID* WINGDIPAPI GdipAlloc(size_t);
//VOID WINGDIPAPI GdipFree(VOID*);

#ifdef __cplusplus
}  /* extern "C" */
}  /* namespace DLLExports */
#endif

#endif /* __GDIPLUS_MEM_H */
