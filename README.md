# GDI+ for Microsoft Flight Simulator 2020

Implementation of the GDI+ API as a wrapper of NanoVG, targeting WebAssembly for *Microsoft Flight Simulator 2020*.
The version of NanoVG in term uses the Low Level Vector Graphics API (**LLVG**) provided by *Microsoft Flight Simulator 2020*.

## Setup Instructions

This project is a mirror of the project present in the SDK for Microsoft Flight Simulator 2020 (**SDK**). 

* Download and install the latest **SDK** version : https://forums.flightsimulator.com/t/how-to-getting-started-with-the-sdk-dev-mode/123241

* Clone this repository (or a fork if you plan to do modifications) into $(MSFS_SDK)WASM\projects\gdiplus 

## Branches

* `master` : current development branch, changes in this branch will be available in the next **SDK** release
* `stable` : the version of GDI+ provided with the live version of the **SDK**

## Status

This library provides minimal implementation of legacy functionality commonly used in the creation of 
*Microsoft Flight Simulator X* (**FSX**) instruments. 

General status:
* General draw functions: supported
* Paths and figures: supported
* Non rectangular clipping: supported
* Fonts: supported via stb
* Bitmaps: supported with limitations (performance limited by the WebAssembly sandboxing, requires game changes of **LLVG**)
* String parameters: wchar* only
* Path gradient: not supported
* Texture brush: not supported
* Render textures: not supported (requires game-side changes of **LLVG**)

Missing functionality:
* GdipMeasureString() not implemented
* Custom Arrow and Pen caps not implemented

## Acknowledgements

This GDI+ implementation is based on:
* MinGW GDI+ headers : https://github.com/Alexpux/mingw-w64/tree/master/mingw-w64-headers/include/gdiplus
* LibGdiplus GDI+ Flat implementation : https://github.com/mono/libgdiplus/tree/master/src
* NanoVG : https://github.com/memononen/nanovg
* Images and fonts handled via stb (as part of the **SDK**): https://github.com/nothings/stb
