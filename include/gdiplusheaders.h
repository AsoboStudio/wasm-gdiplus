// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.


#ifndef __GDIPLUS_HEADERS_H
#define __GDIPLUS_HEADERS_H

#include <MSFS\Render\nanovg.h>
#include <string>

#include <MSFS\MSFS_WindowsTypes.h>
#include <MSFS\Render\GdiPlus\gdiplusbase.h>
#include <MSFS\Render\GdiPlus\gdiplustypes.h>
#include <MSFS\Render\GdiPlus\gdiplusenums.h>
#include <MSFS\Render\GdiPlus\gdiplusimaging.h>
#include <MSFS\Render\GdiPlus\gdipluspixelformats.h>
#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusheaders.h."
#endif
namespace Gdiplus {

	/*
	 * Note: Virtual inline functions (dtors, Clone()) are implemented here: If
	 * these were defined outside class scope, the compiler would always generate
	 * code for them (and the vtable), even if these classes were never used.
	 */

	class Bitmap;
	class Effect;
	class FontCollection;
	class FontFamily;
	class Graphics;
	class GraphicsPath;
	class Matrix;
	class Pen;

	class Image
	{
	public:
		UINT GetHeight();
		UINT GetWidth();

		/**** NOT SUPPORTED YET***\
		inline Image(IN const WCHAR* filename, IN BOOL useEmbeddedColorManagement);
		inline Image(IN IStream* stream, IN BOOL useEmbeddedColorManagement);
		inline Image* FromFile(IN const WCHAR* filename,IN BOOL useEmbeddedColorManagement);
		inline Image* FromStream(IN IStream* stream,IN BOOL useEmbeddedColorManagement);
		inline~Image();
		inline Image* Clone();
		inline UINT GetEncoderParameterListSize(IN const CLSID* clsidEncoder);
		inline Status GetEncoderParameterList(IN const CLSID* clsidEncoder, IN UINT size, OUT EncoderParameters* buffer);
		inline Status Save(IN const WCHAR* filename,IN const CLSID* clsidEncoder, IN const EncoderParameters *encoderParams);
		inline Status Save(IN IStream* stream, IN const CLSID* clsidEncoder, IN const EncoderParameters *encoderParams);
		inline Status SaveAdd(IN const EncoderParameters *encoderParams);
		inline Status SaveAdd(IN Image* newImage, IN const EncoderParameters *encoderParams);
		inline ImageType GetType() const;
		inline Status GetPhysicalDimension(OUT SizeF* size);
		inline Status GetBounds(OUT RectF *srcRect, OUT Unit *srcUnit);
		inline REAL GetHorizontalResolution();
		inline REAL GetVerticalResolution();
		inline UINT GetFlags();
		inline Status GetRawFormat(OUT GUID *format);
		inline PixelFormat GetPixelFormat();
		inline INT GetPaletteSize();
		inline Status GetPalette(OUT ColorPalette *palette, IN INT size);
		inline Status SetPalette(IN const ColorPalette *palette);
		inline Image* GetThumbnailImage(IN UINT thumbWidth, IN UINT thumbHeight, IN GetThumbnailImageAbort callback,IN VOID* callbackData);
		inline UINT GetFrameDimensionsCount();
		inline Status GetFrameDimensionsList(OUT GUID* dimensionIDs, IN UINT count);
		inline UINT GetFrameCount(IN const GUID* dimensionID);
		inline Status SelectActiveFrame(IN const GUID *dimensionID, IN UINT frameIndex);
		inline Status RotateFlip(IN RotateFlipType rotateFlipType);
		inline UINT GetPropertyCount();
		inline Status GetPropertyIdList(IN UINT numOfProperty, OUT PROPID* list);
		inline UINT GetPropertyItemSize(IN PROPID propId);
		inline Status GetPropertyItem(IN PROPID propId, IN UINT propSize, OUT PropertyItem* buffer);
		inline Status GetPropertySize(OUT UINT* totalBufferSize, OUT UINT* numProperties);
		inline Status GetAllPropertyItems(IN UINT totalBufferSize, IN UINT numProperties, OUT PropertyItem* allItems);
		inline Status RemovePropertyItem(IN PROPID propId);
		inline Status SetPropertyItem(IN const PropertyItem* item);
		inline Status FindFirstItem(IN ImageItemData* item);
		inline Status FindNextItem(IN ImageItemData* item);
		inline Status GetItemData(IN ImageItemData* item);
		inline Status GetLastStatus() const;
		inline Image(GpImage *nativeImage, Status status);
		VOID SetNativeImage(GpImage *nativeImage);
		\**** NOT SUPPORTED YET***/
	};

	class Bitmap :public Image
	{
	public:
		Bitmap(INT width, INT height, Graphics* target);
		Bitmap(INT width, INT height, PixelFormat format = PixelFormat32bppARGB);
		Status SetPixel(INT x, INT y, const Color& color);
		Status LockBits(const Rect* rect, UINT flags, PixelFormat format, BitmapData* lockedBitmapData);
		Status UnlockBits(BitmapData* lcokedBitmapData);

		/**** NOT SUPPORTED YET***\
		/*
		inline
		Bitmap(
			IN const WCHAR *filename,
			IN BOOL useEmbeddedColorManagement
			);

		inline
		Bitmap(
			IN IStream *stream,
			IN BOOL useEmbeddedColorManagement
			);

		inline
		Bitmap(
			IN INT width,
			IN INT height,
			IN INT stride,
			IN PixelFormat format,
			IN BYTE *scan0
			);

		inline
		Bitmap(
			IN INT width,
			IN INT height,
			IN PixelFormat format
			);

		inline
		Bitmap(
			IN INT width,
			IN INT height,
			IN Graphics* target);

		inline
		Bitmap(
			IN IDirectDrawSurface7 * surface
			);

		inline
		Bitmap(
			IN const BITMAPINFO* gdiBitmapInfo,
			IN VOID* gdiBitmapData
			);

		inline
		Bitmap(
			IN HBITMAP hbm,
			IN HPALETTE hpal
			);

		inline
		Bitmap(
			IN HICON hicon
			);

		inline
		Bitmap(
			IN HINSTANCE hInstance,
			IN const WCHAR *bitmapName
			);


		inline Bitmap*
		FromFile(
			IN const WCHAR *filename,
			IN BOOL useEmbeddedColorManagement
			);

		inline Bitmap*
		FromStream(
			IN IStream *stream,
			IN BOOL useEmbeddedColorManagement
			);

		inline Bitmap*
		FromDirectDrawSurface7(
			IN IDirectDrawSurface7* surface
			);

		inline Bitmap*
		FromBITMAPINFO(
			IN const BITMAPINFO* gdiBitmapInfo,
			IN VOID* gdiBitmapData);

		inline Bitmap*
		FromHBITMAP(
			IN HBITMAP hbm,
			IN HPALETTE hpal
			);

		inline Bitmap*
		FromHICON(
			IN HICON hicon
			);

		inline Bitmap*
		FromResource(
			IN HINSTANCE hInstance,
			IN const WCHAR *bitmapName);

		inline Status
		GetHBITMAP(
			IN const Color& colorBackground,
			OUT HBITMAP* hbmReturn
			);

		inline Status
		GetHICON(
			OUT HICON* hiconReturn
			);

		inline Bitmap*
		Clone(
			IN const Rect& rect,
			IN PixelFormat format
			);

		inline Bitmap*
		Clone(
			IN INT x,
			IN INT y,
			IN INT width,
			IN INT height,
			IN PixelFormat format
			);

		inline Bitmap*
		Clone(
			IN const RectF& rect,
			IN PixelFormat format
			);

		inline Bitmap*
		Clone(
			IN REAL x,
			IN REAL y,
			IN REAL width,
			IN REAL height,
			IN PixelFormat format
			);

		inline Bitmap(GpBitmap *nativeBitmap);

		inline Status
		LockBits(
			IN const Rect* rect,
			IN UINT flags,
			IN PixelFormat format,
			OUT BitmapData* lockedBitmapData
		);

		inline Status
		UnlockBits(
			IN BitmapData* lockedBitmapData
			);

		inline Status
		GetPixel(
			IN INT x,
			IN INT y,
			OUT Color *color);

		inline Status
		SetPixel(
			IN INT x,
			IN INT y,
			IN const Color& color);

		inline Status SetAbort(GdiplusAbort *pIAbort);

		inline Status
		ConvertFormat(
			PixelFormat format,
			DitherType dithertype,
			PaletteType palettetype,
			ColorPalette *palette,
			REAL alphaThresholdPercent
			);

		inline Status
		InitializePalette(
			OUT ColorPalette *palette,   // output palette. must be allocated.
			PaletteType palettetype,     // palette enumeration type.
			INT optimalColors,           // how many optimal colors
			BOOL useTransparentColor,    // add a transparent color to the palette.
			Bitmap *bitmap               // optional bitmap for median cut.
			);

		inline Status
		ApplyEffect(
			IN  Bitmap **inputs,
			IN  INT numInputs,
			IN  Effect *effect,
			IN  RECT *ROI,                     // optional parameter.
			OUT RECT *outputRect,              // optional parameter.
			OUT Bitmap **output);
		inline Status ApplyEffect(Effect *effect, RECT *ROI);
		inline Status GetHistogram(IN HistogramFormat format, IN UINT NumberOfEntries, UINT *channel0, UINT *channel1, UINT *channel2, UINT *channel3);
		inline Status GetHistogramSize(IN HistogramFormat format, OUT UINT *NumberOfEntries);


		inline Status SetResolution(IN REAL xdpi, IN REAL ydpi);

		\**** NOT SUPPORTED YET***/
	};
	/*
	class CachedBitmap: public GdiplusBase
	{
		friend class Graphics;

	public:
		CachedBitmap(Bitmap *bitmap, Graphics *graphics);
		~CachedBitmap();

		Status GetLastStatus() const
		{
			return lastStatus;
		}

	private:
		CachedBitmap(const CachedBitmap&);
		CachedBitmap& operator=(const CachedBitmap&);

		GpCachedBitmap *nativeCachedBitmap;
		Status lastStatus;
	};

	class CustomLineCap//: public GdiplusBase
	{
		friend class AdjustableArrowCap;
		friend class Pen;

	public:
		CustomLineCap(const GraphicsPath *fillPath,
				const GraphicsPath *strokePath,
				LineCap baseCap = LineCapFlat,
				REAL baseInset = 0.0f);

		virtual ~CustomLineCap()
		{
			DllExports::GdipDeleteCustomLineCap(nativeCustomLineCap);
		}
		virtual CustomLineCap* Clone() const
		{
			GpCustomLineCap *cloneCustomLineCap = NULL;
			Status status = updateStatus(DllExports::GdipCloneCustomLineCap(
					nativeCustomLineCap, &cloneCustomLineCap));
			if (status == Ok) {
				CustomLineCap *result = new CustomLineCap(
						cloneCustomLineCap, lastStatus);
				if (!result) {
					DllExports::GdipDeleteCustomLineCap(cloneCustomLineCap);
					lastStatus = OutOfMemory;
				}
				return result;
			} else {
				return NULL;
			}
		}

		LineCap GetBaseCap() const;
		REAL GetBaseInset() const;
		Status GetStrokeCaps(LineCap *startCap, LineCap *endCap) const;
		LineJoin GetStrokeJoin() const;
		REAL GetWidthScale() const;
		Status SetBaseCap(LineCap baseCap);
		Status SetBaseInset(REAL inset);
		Status SetStrokeCap(LineCap strokeCap);
		Status SetStrokeCaps(LineCap startCap, LineCap endCap);
		Status SetStrokeJoin(LineJoin lineJoin);
		Status SetWidthScale(REAL widthScale);

		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}

	private:
		CustomLineCap(GpCustomLineCap *customLineCap, Status status):
			nativeCustomLineCap(customLineCap), lastStatus(status) {}
		CustomLineCap(const CustomLineCap&);
		CustomLineCap& operator=(const CustomLineCap&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != Ok) lastStatus = newStatus;
			return newStatus;
		}

		GpCustomLineCap *nativeCustomLineCap;
		mutable Status lastStatus;
	};*/
	class FontCollection
	{
		/**** NOT SUPPORTED YET***\
		inline
		FontCollection();

		inline
		~FontCollection();

		inline INT
		GetFamilyCount() const;

		inline Status
		GetFamilies(
			IN INT           numSought,
			OUT FontFamily * gpfamilies,
			OUT INT *        numFound
		) const;

		inline Status GetLastStatus () const;

		inline Status
		SetStatus(IN Status status) const;

		inline
		InstalledInstalledFontCollection();
		inline
		Installed~InstalledFontCollection();

		inline
		PrivatePrivateFontCollection();
		inline
		Private~PrivateFontCollection();

		inline Status
		PrivateAddFontFile(IN const WCHAR* filename);
		inline Status
		PrivateAddMemoryFont(IN const void* memory,
			IN INT length);
		\**** NOT SUPPORTED YET***/
	};
	class Font
	{
	public:
		mutable int fontid;
		double size;

		std::string name;
		std::string path;

		Font(const WCHAR* familyName, REAL emSize, INT style = FontStyleRegular, Unit unit = UnitPoint, const FontCollection* fontCollection = NULL);
		bool Load(NVGcontext* _ctx) const;


		/**** NOT SUPPORTED YET***\
		inline
		Font(IN HDC hdc);

		inline
		Font(IN HDC hdc,
				   IN const HFONT hfont);

		inline
		Font(IN HDC hdc,
				   IN const LOGFONTW* logfont);

		inline
		Font(IN HDC hdc,
				   IN const LOGFONTA* logfont);

		inline
		Font(
			 IN const FontFamily * family,
			 IN REAL         emSize,
			 IN INT          style,
			 IN Unit         unit
		);

		inline Status
		GetLogFontA(IN const Graphics *g,
						  OUT LOGFONTA *logfontA) const;
		inline Status
		GetLogFontW(IN const Graphics *g,
						  OUT LOGFONTW *logfontW) const;


		inline Font*
		Clone() const;

		inline
		~Font();

		// Operations

		inline BOOL
		IsAvailable() const;

		inline Status
		GetFamily(OUT FontFamily *family) const;

		inline INT
		GetStyle() const;

		inline REAL
		GetSize() const;

		inline Unit
		GetUnit() const;

		inline REAL
		GetHeight(IN const Graphics *graphics) const;


		inline REAL
		GetHeight(IN REAL dpi) const;

		inline
		Font(IN GpFont* font,
				   IN Status status);

		inline VOID
		SetNativeFont(GpFont *Font);

		inline Status
		GetLastStatus(void) const;

		inline Status
		SetStatus(IN Status status) const;
		\**** NOT SUPPORTED YET***/

	};


	/*
	class FontFamily//: public GdiplusBase
	{
		friend class Font;
		friend class FontCollection;
		friend class GraphicsPath;

	public:
		static const FontFamily* GenericMonospace();
		static const FontFamily* GenericSansSerif();
		static const FontFamily* GenericSerif();

		FontFamily();
		FontFamily(const WCHAR *name,
				const FontCollection *fontCollection = NULL);
		~FontFamily();
		FontFamily* Clone() const;

		UINT16 GetCellAscent(INT style) const;
		UINT16 GetCellDescent(INT style) const;
		UINT16 GetEmHeight(INT style) const;
		Status GetFamilyName(WCHAR name[LF_FACESIZE],
				LANGID language = LANG_NEUTRAL) const;
		UINT16 GetLineSpacing(INT style) const;
		BOOL IsStyleAvailable(INT style) const;

		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}
		BOOL IsAvailable() const
		{
			return nativeFontFamily != NULL;
		}

	private:
		FontFamily(GpFontFamily *fontFamily, Status status):
			nativeFontFamily(fontFamily), lastStatus(status) {}
		FontFamily(const FontFamily&);
		FontFamily& operator=(const FontFamily&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != Ok) lastStatus = newStatus;
			return newStatus;
		}

		GpFontFamily *nativeFontFamily;
		mutable Status lastStatus;
	};

	class InstalledFontCollection: public FontCollection
	{
	public:
		InstalledFontCollection();
		virtual ~InstalledFontCollection() {}
	};

	class PrivateFontCollection: public FontCollection
	{
	public:
		PrivateFontCollection();

		virtual ~PrivateFontCollection()
		{
			DllExports::GdipDeletePrivateFontCollection(&nativeFontCollection);
		}

		Status AddFontFile(const WCHAR *filename);
		Status AddMemoryFont(const VOID *memory, INT length);
	};

	class Region: public GdiplusBase
	{
		friend class Graphics;

	public:
		static Region* FromHRGN(HRGN hrgn);

		Region();
		Region(const RectF& rect);
		Region(const Rect& rect);
		Region(const GraphicsPath *path);
		Region(const BYTE *regionData, INT size);
		Region(HRGN hrgn);
		~Region();
		Region* Clone() const;

		Status Complement(const RectF& rect);
		Status Complement(const Rect& rect);
		Status Complement(const Region *region);
		Status Complement(const GraphicsPath *path);
		BOOL Equals(const Region *region, const Graphics *graphics) const;
		Status Exclude(const RectF& rect);
		Status Exclude(const Rect& rect);
		Status Exclude(const Region *region);
		Status Exclude(const GraphicsPath *path);
		Status GetBounds(RectF *rect, const Graphics *graphics) const;
		Status GetBounds(Rect *rect, const Graphics *graphics) const;
		Status GetData(BYTE *buffer, UINT bufferSize, UINT *sizeFilled) const;
		UINT GetDataSize() const;
		HRGN GetHRGN(const Graphics *graphics) const;
		Status GetRegionScans(const Matrix *matrix,
				RectF *rects, INT *count) const;
		Status GetRegionScans(const Matrix *matrix,
				Rect *rects, INT *count) const;
		UINT GetRegionScansCount(const Matrix *matrix) const;
		Status Intersect(const RectF& rect);
		Status Intersect(const Rect& rect);
		Status Intersect(const Region *region);
		Status Intersect(const GraphicsPath *path);
		BOOL IsEmpty(const Graphics *graphics) const;
		BOOL IsInfinite(const Graphics *graphics) const;
		BOOL IsVisible(REAL x, REAL y,
				const Graphics *graphics = NULL) const;
		BOOL IsVisible(INT x, INT y,
				const Graphics *graphics = NULL) const;
		BOOL IsVisible(const PointF& point,
				const Graphics *graphics = NULL) const;
		BOOL IsVisible(const Point& point,
				const Graphics *graphics = NULL) const;
		BOOL IsVisible(REAL x, REAL y, REAL width, REAL height,
				const Graphics *graphics = NULL) const;
		BOOL IsVisible(INT x, INT y, INT width, INT height,
				const Graphics *graphics = NULL) const;
		BOOL IsVisible(const RectF& rect,
				const Graphics *graphics = NULL) const;
		BOOL IsVisible(const Rect& rect,
				const Graphics *graphics = NULL) const;
		Status MakeEmpty();
		Status MakeInfinite();
		Status Transform(const Matrix *matrix);
		Status Translate(REAL dx, REAL dy);
		Status Translate(INT dx, INT dy);
		Status Union(const RectF& rect);
		Status Union(const Rect& rect);
		Status Union(const Region *region);
		Status Union(const GraphicsPath *path);
		Status Xor(const RectF& rect);
		Status Xor(const Rect& rect);
		Status Xor(const Region *region);
		Status Xor(const GraphicsPath *path);

		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}

	private:
		Region(GpRegion *region, Status status):
			nativeRegion(region), lastStatus(status) {}
		Region(const Region&);
		Region& operator=(const Region&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != Ok) lastStatus = newStatus;
			return newStatus;
		}

		GpRegion *nativeRegion;
		mutable Status lastStatus;
	};
	*/
}

#endif /* __GDIPLUS_HEADERS_H */
