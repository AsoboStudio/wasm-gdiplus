// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>

#ifndef __GDIPLUS_BASE_H
#define __GDIPLUS_BASE_H

namespace Gdiplus
{
	class GdiplusBase
	{
	public:
		static void* operator new(size_t in_size)
		{
			return DLLExports::GdipAlloc(in_size);
		}
		static void* operator new[](size_t in_size)
		{
			return DLLExports::GdipAlloc(in_size);
		}
		static void operator delete(void *in_pVoid)
		{
			DLLExports::GdipFree(in_pVoid);
		}
		static void operator delete[](void *in_pVoid)
		{
			DLLExports::GdipFree(in_pVoid);
		}
	};
}
#endif /* __GDIPLUS_BASE_H */
