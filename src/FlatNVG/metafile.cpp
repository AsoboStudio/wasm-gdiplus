/*
 * Copyright (C) 2007 Novell, Inc (http://www.novell.com)
 * Copyright (C) 2020 Asobo Studio. 
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
 * Authors:
 *	Sebastien Pouliot  <sebastien@ximian.com>
 *	Sol Roo (sroo@asobostudio.com)
 *
 */
#include "gdiplusflat.h"

namespace DLLExports {
	GpStatus
		GdipCreateMetafileFromFile(GDIPCONST WCHAR* file, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipCreateMetafileFromStream(void* stream, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}

	/*
	 * instead we'll use delegates to load the metafile header with this function
	 */
	GpStatus GdipCreateMetafileFromDelegate_linux(GetHeaderDelegate getHeaderFunc, GetBytesDelegate getBytesFunc,
		PutBytesDelegate putBytesFunc, SeekDelegate seekFunc, CloseDelegate closeFunc, SizeDelegate sizeFunc,
		GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipCreateMetafileFromEmf(HENHMETAFILE hEmf, BOOL deleteEmf, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipCreateMetafileFromWmf(HMETAFILE hWmf, BOOL deleteWmf, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipGetMetafileHeaderFromWmf(HMETAFILE hWmf, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, MetafileHeader* header) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipGetMetafileHeaderFromEmf(HENHMETAFILE hEmf, MetafileHeader* header) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipGetMetafileHeaderFromFile(GDIPCONST WCHAR* filename, MetafileHeader* header) {
		return GpStatus::NotImplemented;
	}


	/*
	 * GdipGetMetafileHeaderFromStream will never be implemented, as 'stream' is a COM IStream ...
	 */
	GpStatus
		GdipGetMetafileHeaderFromStream(void* stream, MetafileHeader* header) {
		return GpStatus::NotImplemented;
	}

	/*
	 * instead we'll use delegates to load the metafile header with this function
	 */
	GpStatus
		GdipGetMetafileHeaderFromDelegate_linux(GetHeaderDelegate getHeaderFunc, GetBytesDelegate getBytesFunc,
			PutBytesDelegate putBytesFunc, SeekDelegate seekFunc, CloseDelegate closeFunc, SizeDelegate sizeFunc,
			MetafileHeader* header) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipGetMetafileHeaderFromMetafile(GpMetafile* metafile, MetafileHeader* header) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipGetHemfFromMetafile(GpMetafile* metafile, HENHMETAFILE* hEmf) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipGetMetafileDownLevelRasterizationLimit(GpMetafile* metafile, UINT* metafileRasterizationLimitDpi) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipSetMetafileDownLevelRasterizationLimit(GpMetafile* metafile, UINT metafileRasterizationLimitDpi) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipPlayMetafileRecord(GDIPCONST GpMetafile* metafile, GpEmfPlusRecordType recordType, UINT flags, UINT dataSize, GDIPCONST BYTE* data) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipRecordMetafile(HDC referenceHdc, GpEmfType type, GDIPCONST GpRectF* frameRect, GpMetafileFrameUnit frameUnit,
			GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipRecordMetafileI(HDC referenceHdc, GpEmfType type, GDIPCONST GpRect* frameRect, GpMetafileFrameUnit frameUnit,
			GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipRecordMetafileFileName(GDIPCONST WCHAR* fileName, HDC referenceHdc, GpEmfType type, GDIPCONST GpRectF* frameRect,
			GpMetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipRecordMetafileFileNameI(GDIPCONST WCHAR* fileName, HDC referenceHdc, GpEmfType type, GDIPCONST GpRect* frameRect,
			GpMetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	/*
	 * GdipRecordMetafileStream and GdipRecordMetafileStreamI will never be implemented, as 'stream' is a COM IStream ...
	 */
	GpStatus
		GdipRecordMetafileStream(void /* IStream */* stream, HDC referenceHdc, GpEmfType type, GDIPCONST GpRectF* frameRect,
			GpMetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipRecordMetafileStreamI(void /* IStream */* stream, HDC referenceHdc, GpEmfType type, GDIPCONST GpRect* frameRect,
			GpMetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}

	/*
	 * instead we'll use delegates to create the metafile header with these functions
	 */
	GpStatus
		GdipRecordMetafileFromDelegate_linux(GetHeaderDelegate getHeaderFunc, GetBytesDelegate getBytesFunc,
			PutBytesDelegate putBytesFunc, SeekDelegate seekFunc, CloseDelegate closeFunc, SizeDelegate sizeFunc,
			HDC referenceHdc, GpEmfType type, GDIPCONST GpRectF* frameRect,
			GpMetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}


	GpStatus
		GdipRecordMetafileFromDelegateI_linux(GetHeaderDelegate getHeaderFunc, GetBytesDelegate getBytesFunc,
			PutBytesDelegate putBytesFunc, SeekDelegate seekFunc, CloseDelegate closeFunc, SizeDelegate sizeFunc,
			HDC referenceHdc, GpEmfType type, GDIPCONST GpRect* frameRect,
			GpMetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile) {
		return GpStatus::NotImplemented;
	}

}