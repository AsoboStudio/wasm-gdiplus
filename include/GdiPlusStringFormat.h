// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_STRINGFORMAT_H
#define __GDIPLUS_STRINGFORMAT_H

#include "GdiPlusTypes.h"
#include "GdiPlusEnums.h"


#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusstringformat.h."
#endif
namespace Gdiplus {

    class StringFormat//: public GdiplusBase
    {
        friend class Graphics;
        friend class GraphicsPath;

    public:
        StringAlignment alignment;
        StringAlignment line_alignment;

        Status SetAlignment(StringAlignment align);
        StringAlignment GetAlignment() const;
        Status SetLineAlignment(StringAlignment align);
        StringAlignment GetLineAlignment() const;
    };

    /**** NOT SUPPORTED YET***\
    extern "C" void *_GdipStringFormatCachedGenericDefault;
    extern "C" void *_GdipStringFormatCachedGenericTypographic;

    __inline__ const StringFormat* StringFormat::GenericDefault();

    __inline__ const StringFormat* StringFormat::GenericTypographic();
    \**** NOT SUPPORTED YET***/

}
#endif /* __GDIPLUS_STRINGFORMAT_H */
