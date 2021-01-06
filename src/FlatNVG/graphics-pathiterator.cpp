/*
 * graphics-pathiterator.cpp
 *
 * Copyright (C) 2004,2006-2007 Novell Inc.
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
 *
 * AuthorS: 
 *		Ravindra (rkumar@novell.com)
 *		Sol Roo (sroo@asobostudio.com)
 *
 */
#include "gdiplusflat.h"

namespace DLLExports {
	// coverity[+alloc : arg-*0]
	GpStatus
		GdipCreatePathIter(GpPathIterator** iterator, GpPath* path) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterGetCount(GpPathIterator* iterator, int* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterGetSubpathCount(GpPathIterator* iterator, int* count) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipDeletePathIter(GpPathIterator* iterator) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterCopyData(GpPathIterator* iterator, int* resultCount, GpPointF* points, BYTE* types, int startIndex, int endIndex) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterEnumerate(GpPathIterator* iterator, int* resultCount, GpPointF* points, BYTE* types, int count) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterHasCurve(GpPathIterator* iterator, BOOL* curve) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterNextMarkerPath(GpPathIterator* iterator, int* resultCount, GpPath* path) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterNextMarker(GpPathIterator* iterator, int* resultCount, int* startIndex, int* endIndex) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterNextPathType(GpPathIterator* iterator, int* resultCount, BYTE* pathType, int* startIndex, int* endIndex) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterNextSubpathPath(GpPathIterator* iterator, int* resultCount, GpPath* path, BOOL* isClosed) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterNextSubpath(GpPathIterator* iterator, int* resultCount, int* startIndex, int* endIndex, BOOL* isClosed) {
		return GpStatus::NotImplemented;
	}

	GpStatus
		GdipPathIterRewind(GpPathIterator* iterator) {
		return GpStatus::NotImplemented;
	}

}