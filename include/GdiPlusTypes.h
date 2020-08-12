// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_TYPES_H
#define __GDIPLUS_TYPES_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#include <MSFS/MSFS_WindowsTypes.h>

#if defined(_ARM_)
#define WINGDIPAPI
#else
#define WINGDIPAPI __stdcall
#endif
#define GDIPCONST const

namespace Gdiplus {

	// foward declaration
	struct Size;
	struct SizeF;
	struct Point;
	struct PointF;
	struct Rect;
	struct RectF;
	class CharacterRange;

	struct Graphics;
	struct GraphicsPath;
	struct CustomLineCap;
	struct Pen;
	struct Font;
	struct Image;
	class Color;

	typedef enum GpStatus {
		Ok = 0,
		GenericError = 1,
		InvalidParameter = 2,
		OutOfMemory = 3,
		ObjectBusy = 4,
		InsufficientBuffer = 5,
		NotImplemented = 6,
		Win32Error = 7,
		WrongState = 8,
		Aborted = 9,
		FileNotFound = 10,
		ValueOverflow = 11,
		AccessDenied = 12,
		UnknownImageFormat = 13,
		FontFamilyNotFound = 14,
		FontStyleNotFound = 15,
		NotTrueTypeFont = 16,
		UnsupportedGdiplusVersion = 17,
		GdiplusNotInitialized = 18,
		PropertyNotFound = 19,
		PropertyNotSupported = 20,
		ProfileNotFound = 21
	} GpStatus;

#ifdef __cplusplus
	typedef GpStatus Status;
#endif

	typedef struct Size {
	public:
		Size();
		Size(IN const Size& size);
		Size(IN INT width, IN INT height);
		Size operator+(IN const Size& sz) const;
		Size operator-(IN const Size& sz) const;
		BOOL Equals(IN const Size& sz) const;
		BOOL Empty() const;

	public:

		INT Width;
		INT Height;
	} Size;

	typedef struct SizeF {
	public:
		SizeF();
		SizeF(IN const SizeF& size);
		SizeF(IN REAL width, IN REAL height);
		SizeF operator+(IN const SizeF& sz) const;
		SizeF operator-(IN const SizeF& sz) const;
		BOOL Equals(IN const SizeF& sz) const;
		BOOL Empty() const;

	public:

		REAL Width;
		REAL Height;
	} SizeF;

	typedef struct Point {
	public:
		Point();
		Point(IN const Point& point);
		Point(IN const Size& size);
		Point(IN INT x, IN INT y);
		Point operator+(IN const Point& point) const;
		Point operator-(IN const Point& point) const;
		BOOL Equals(IN const Point& point);

	public:

		INT X;
		INT Y;
	} Point;

	typedef struct PointF {
	public:
		PointF();
		PointF(IN const PointF& point);
		PointF(IN const SizeF& size);
		PointF(IN REAL x, IN REAL y);
		PointF operator+(IN const PointF& point) const;
		PointF operator-(IN const PointF& point) const;
		BOOL Equals(IN const PointF& point);

	public:

		REAL X;
		REAL Y;
	} PointF;

	typedef struct Rect {

	public:
#ifdef __cplusplus
		Rect();
		Rect(IN INT x, IN INT y, IN INT width, IN INT height);
		Rect(IN const Point& location, IN const Size& size);
		Rect* Clone() const;
		VOID GetLocation(OUT Point* point) const;
		VOID GetSize(OUT Size* size) const;
		VOID GetBounds(OUT Rect* rect) const;
		INT GetLeft() const;
		INT GetTop() const;
		INT GetRight() const;
		INT GetBottom() const;
		BOOL IsEmptyArea() const;
		BOOL Equals(IN const Rect& rect) const;
		BOOL Contains(IN INT x, IN INT y) const;
		BOOL Contains(IN const Point& pt) const;
		BOOL Contains(IN Rect& rect) const;
		VOID Inflate(IN INT dx, IN INT dy);
		VOID Inflate(IN const Point& point);
		BOOL Intersect(IN const Rect& rect);
		static BOOL Intersect(OUT Rect& c, IN const Rect& a, IN const Rect& b);
		BOOL IntersectsWith(IN const Rect& rect) const;
		static BOOL Union(OUT Rect& c, IN const Rect& a, IN const Rect& b);
		VOID Offset(IN const Point& point);
		VOID Offset(IN INT dx, IN INT dy);

	public:

		INT X;
		INT Y;
		INT Width;
		INT Height;

#endif /* __cplusplus */
	} Rect;
	typedef Rect RECT;

	typedef struct RectF {
	public:
#ifdef __cplusplus
		RectF();
		RectF(IN REAL x, IN REAL y, IN REAL width, IN REAL height);
		RectF(IN const PointF& location, IN const SizeF& size);
		RectF* Clone() const;
		VOID GetLocation(OUT PointF* point) const;
		VOID GetSize(OUT SizeF* size) const;
		VOID GetBounds(OUT RectF* rect) const;
		REAL GetLeft() const;
		REAL GetTop() const;
		REAL GetRight() const;
		REAL GetBottom() const;
		BOOL IsEmptyArea() const;
		BOOL Equals(IN const RectF& rect) const;
		BOOL Contains(IN REAL x, IN REAL y) const;
		BOOL Contains(IN const PointF& pt) const;
		BOOL Contains(IN const RectF& rect) const;
		VOID Inflate(IN REAL dx, IN REAL dy);
		VOID Inflate(IN const PointF& point);
		BOOL Intersect(IN const RectF& rect);
		static BOOL Intersect(OUT RectF& c, IN const RectF& a, IN const RectF& b);
		BOOL IntersectsWith(IN const RectF& rect) const;
		static BOOL Union(OUT RectF& c, IN const RectF& a, IN const RectF& b);
		VOID Offset(IN const PointF& point);
		VOID Offset(IN REAL dx, IN REAL dy);

	public:

		REAL X;
		REAL Y;
		REAL Width;
		REAL Height;
#endif /* __cplusplus */
	} RectF;
	/*
	// FIXME: Are descendants of this class, when compiled with g++,
	// binary compatible with MSVC++ code (especially GDIPLUS.DLL of course)?
	#ifdef __cplusplus
	struct GdiplusAbort {
		virtual HRESULT __stdcall Abort(void) {}
	};
	#else
	typedef struct GdiplusAbort GdiplusAbort;  // incomplete type
	#endif

	typedef struct CharacterRange {
		INT First;
		INT Length;

		#ifdef __cplusplus
		CharacterRange(): First(0), Length(0) {}
		CharacterRange(INT first, INT length): First(first), Length(length) {}
		CharacterRange& operator=(const CharacterRange& rhs) {
			// This gracefully handles self-assignment
			First = rhs.First;
			Length = rhs.Length;
			return *this;
		}
		#endif // __cplusplus
	} CharacterRange;
	*/
	/**** NOT SUPPORTED YET***\
	typedef struct PathData {
		INT Count;
		PointF *Points;
		BYTE *Types;

		#ifdef __cplusplus
		friend class GraphicsPath;

		PathData(): Count(0), Points(NULL), Types(NULL) {}
		~PathData() {
			FreeArrays();
		}
	private:
		// used by GraphicsPath::GetPathData, defined in gdipluspath.h
		Status AllocateArrays(INT capacity);
		VOID FreeArrays();
		#endif // __cplusplus
	} PathData;

	// Callback function types
	// FIXME: need a correct definition for these function pointer types
	typedef void *DebugEventProc;
	typedef BOOL CALLBACK (*EnumerateMetafileProc)(EmfPlusRecordType,UINT,UINT,const BYTE*,VOID*);
	typedef void *DrawImageAbort;
	typedef void *GetThumbnailImageAbort;
	\**** NOT SUPPORTED YET***/
}
#endif /* __GDIPLUS_TYPES_H */
