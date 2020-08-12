// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#ifndef __GDIPLUS_BASE_H
#define __GDIPLUS_BASE_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusbase.h."
#endif

namespace Gdiplus {

    class GdiplusBase
    {
    public:
        /*
        static void* operator new(size_t in_size);
        static void* operator new[](size_t in_size);
        static void operator delete(void *in_pVoid);
        static void operator delete[](void *in_pVoid);
        */
    };
}
#endif /* __GDIPLUS_BASE_H */
