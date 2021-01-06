// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_METAHEADER_H
#define __GDIPLUS_METAHEADER_H

#ifdef __cplusplus
namespace Gdiplus
{
#endif
	/*
	 * FIXME: is 1 the correct value for GDIP_EMFPLUSFLAGS_DISPLAY? This number
	 * has been determined by calling Metafile::GetMetafileHeader() on a EMF+
	 * metafile which was recorded on a display device context (SampleMetafile.emf).
	 */
#ifdef __cplusplus
	const UINT GDIP_EMFPLUSFLAGS_DISPLAY = 1;
#else
#define GDIP_EMFPLUSFLAGS_DISPLAY ((UINT) 1)
#endif

	typedef struct tagENHMETAHEADER3 {
		DWORD iType;
		DWORD nSize;
		RECTL rclBounds;
		RECTL rclFrame;
		DWORD dSignature;
		DWORD nVersion;
		DWORD nBytes;
		DWORD nRecords;
		WORD nHandles;
		WORD sReserved;
		DWORD nDescription;
		DWORD offDescription;
		DWORD nPalEntries;
		SIZEL szlDevice;
		SIZEL szlMillimeters;
	} ENHMETAHEADER3, * LPENHMETAHEADER3;

	typedef struct PWMFRect16 {
		INT16 Left;
		INT16 Top;
		INT16 Right;
		INT16 Bottom;
	} PWMFRect16;

	typedef struct WmfPlaceableFileHeader {
		UINT32 Key;
		INT16 Hmf;
		PWMFRect16 BoundingBox;
		INT16 Inch;
		UINT32 Reserved;
		INT16 Checksum;
	} WmfPlaceableFileHeader;


	typedef struct MetafileHeader {
		MetafileType Type;
		UINT Size;
		UINT Version;
		UINT EmfPlusFlags;
		REAL DpiX;
		REAL DpiY;
		INT X;
		INT Y;
		INT Width;
		INT Height;
		__extension__ union {
			METAHEADER WmfHeader;
			ENHMETAHEADER3 EmfHeader;
		};
		INT EmfPlusHeaderSize;
		INT LogicalDpiX;
		INT LogicalDpiY;

#ifdef __cplusplus
	public:
		void GetBounds(Rect* rect) const
		{
			if (rect)
			{
				rect->X = X;
				rect->Y = Y;
				rect->Width = Width;
				rect->Height = Height;
			}
		}
		REAL GetDpiX() const
		{
			return DpiX;
		}
		REAL GetDpiY() const
		{
			return DpiY;
		}
		const ENHMETAHEADER3* GetEmfHeader() const
		{
			if (Type == MetafileTypeEmf
				|| Type == MetafileTypeEmfPlusOnly
				|| Type == MetafileTypeEmfPlusDual)
			{
				return &EmfHeader;
			}
			else
			{
				return NULL;
			}
		}
		UINT GetEmfPlusFlags() const
		{
			return EmfPlusFlags;
		}
		UINT GetMetafileSize() const
		{
			return Size;
		}
		MetafileType GetType() const
		{
			return Type;
		}
		UINT GetVersion() const
		{
			return Version;
		}
		const METAHEADER* GetWmfHeader() const
		{
			if (Type == MetafileTypeWmf || Type == MetafileTypeWmfPlaceable)
			{
				return &WmfHeader;
			}
			else
			{
				return NULL;
			}
		}
		BOOL IsDisplay() const
		{
			return EmfPlusFlags == GDIP_EMFPLUSFLAGS_DISPLAY;
		}
		BOOL IsEmf() const
		{
			return Type == MetafileTypeEmf;
		}
		BOOL IsEmfOrEmfPlus() const
		{
			return Type == MetafileTypeEmf
				|| Type == MetafileTypeEmfPlusOnly
				|| Type == MetafileTypeEmfPlusDual;
		}
		BOOL IsEmfPlus() const
		{
			return Type == MetafileTypeEmfPlusOnly
				|| Type == MetafileTypeEmfPlusDual;
		}
		BOOL IsEmfPlusDual() const
		{
			return Type == MetafileTypeEmfPlusDual;
		}
		BOOL IsEmfPlusOnly() const
		{
			return Type == MetafileTypeEmfPlusOnly;
		}
		BOOL IsWmf() const
		{
			return Type == MetafileTypeWmf
				|| Type == MetafileTypeWmfPlaceable;
		}
		BOOL IsWmfPlaceable() const
		{
			return Type == MetafileTypeWmfPlaceable;
		}
#endif
	} MetafileHeader;
#ifdef __cplusplus
}
#endif
#endif /* __GDIPLUS_METAHEADER_H */
