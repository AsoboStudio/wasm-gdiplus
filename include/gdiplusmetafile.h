// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>

#ifndef __GDIPLUS_METAFILE_H
#define __GDIPLUS_METAFILE_H

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusmetafile.h."
#endif

#include "GdiPlus.h"
#include "gdiplusflat.h"
namespace Gdiplus
{
	class Metafile : public Image
	{
		// WASM: NOT SUPPORTED
#if 0
	private:
		GpMetafile* nativeImage;
	public:
		static UINT EmfToWmfBits(HENHMETAFILE hEmf,
			UINT cbData16, LPBYTE pData16,
			INT iMapMode = MM_ANISOTROPIC,
			EmfToWmfBitsFlags eFlags = EmfToWmfBitsFlagsDefault)
		{
			return DLLExports::GdipEmfToWmfBits(hEmf,
				cbData16, pData16, iMapMode, eFlags);
		}
		static Status GetMetafileHeader(const WCHAR* filename,
			MetafileHeader* header)
		{
			return DLLExports::GdipGetMetafileHeaderFromFile(
				filename, header);
		}
		static Status GetMetafileHeader(IStream* stream, MetafileHeader* header)
		{
			return DLLExports::GdipGetMetafileHeaderFromStream(
				stream, header);
		}
		////TODO: Metafile::GetMetafileHeader
		//static Status GetMetafileHeader(HMETAFILE hWmf,
		//		const WmfPlaceableFileHeader *wmfPlaceableFileHeader,
		//		MetafileHeader *header)
		//{
		//	// WTF: No flat API to do this.
		//	return GpStatus::NotImplemented;
		//}
		static Status GetMetafileHeader(HENHMETAFILE hEmf,
			MetafileHeader* header)
		{
			return DLLExports::GdipGetMetafileHeaderFromEmf(hEmf, header);
		}

		Metafile(HMETAFILE hWmf,
			const WmfPlaceableFileHeader* wmfPlaceableFileHeader,
			BOOL deleteWmf = FALSE) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipCreateMetafileFromWmf(
				hWmf, deleteWmf, wmfPlaceableFileHeader,
				&nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(HENHMETAFILE hEmf, BOOL deleteEmf = FALSE) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipCreateMetafileFromEmf(
				hEmf, deleteEmf, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(const WCHAR* filename) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipCreateMetafileFromFile(
				filename, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(const WCHAR* filename,
			const WmfPlaceableFileHeader* wmfPlaceableFileHeader) :
			Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipCreateMetafileFromWmfFile(
				filename, wmfPlaceableFileHeader,
				&nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(IStream* stream) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipCreateMetafileFromStream(
				stream, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(HDC referenceHdc, EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafile(
				referenceHdc, type, NULL, MetafileFrameUnitGdi,
				description, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(HDC referenceHdc, const RectF& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafile(
				referenceHdc, type, &frameRect, frameUnit,
				description, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(HDC referenceHdc, const Rect& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafileI(
				referenceHdc, type, &frameRect, frameUnit,
				description, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(const WCHAR* filename, HDC referenceHdc,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafileFileName(
				filename, referenceHdc, type, NULL,
				MetafileFrameUnitGdi, description,
				&nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(const WCHAR* filename, HDC referenceHdc,
			const RectF& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafileFileName(
				filename, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(const WCHAR* filename, HDC referenceHdc,
			const Rect& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafileFileNameI(
				filename, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(IStream* stream, HDC referenceHdc,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafileStream(
				stream, referenceHdc, type, NULL,
				MetafileFrameUnitGdi, description,
				&nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(IStream* stream, HDC referenceHdc, const RectF& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafileStream(
				stream, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		Metafile(IStream* stream, HDC referenceHdc, const Rect& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR* description = NULL) : Image(NULL, GpStatus::Ok)
		{
			GpMetafile* nativeMetafile = NULL;
			lastStatus = DLLExports::GdipRecordMetafileStreamI(
				stream, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
			nativeImage = nativeMetafile;
		}
		virtual ~Metafile()
		{
		}
		virtual Metafile* Clone() const
		{
			GpImage* cloneImage = NULL;
			Status status = updateStatus(DLLExports::GdipCloneImage(
				nativeImage, &cloneImage));
			if (status == GpStatus::Ok) {
				Metafile* result = new Metafile(cloneImage, lastStatus);
				if (!result) {
					DLLExports::GdipDisposeImage(cloneImage);
					lastStatus = OutOfMemory;
				}
				return result;
			}
			else {
				return NULL;
			}
		}

		////TODO: [GDI+ 1.1] Metafile::ConvertToEmfPlus
		//Status ConvertToEmfPlus(const Graphics *refGraphics,
		//		BOOL *conversionSuccess = NULL,
		//		EmfType emfType = EmfTypeEmfPlusOnly,
		//		const WCHAR *description = NULL)
		//{
		//	// FIXME: can't test GdipConvertToEmfPlus because it isn't exported in 1.0
		//	return updateStatus(DLLExports::GdipConvertToEmfPlus(
		//			refGraphics ? refGraphics->nativeGraphics : NULL,
		//			(GpMetafile*) nativeImage,
		//			conversionSuccess, emfType, description, ???));
		//}
		////TODO: [GDI+ 1.1] Metafile::ConvertToEmfPlus
		//Status ConvertToEmfPlus(const Graphics *refGraphics,
		//		const WCHAR *filename,
		//		BOOL *conversionSuccess = NULL,
		//		EmfType emfType = EmfTypeEmfPlusOnly,
		//		const WCHAR *description = NULL)
		//{
		//	// FIXME: can't test GdipConvertToEmfPlusToFile because it isn't exported in 1.0
		//	return updateStatus(DLLExports::GdipConvertToEmfPlusToFile(
		//			refGraphics ? refGraphics->nativeGraphics : NULL,
		//			(GpMetafile*) nativeImage, conversionSuccess,
		//			filename, emfType, description, ???));
		//}
		////TODO: [GDI+ 1.1] Metafile::ConvertToEmfPlus
		//Status ConvertToEmfPlus(const Graphics *refGraphics,
		//		IStream *stream,
		//		BOOL *conversionSuccess = NULL,
		//		EmfType emfType = EmfTypeEmfPlusOnly,
		//		const WCHAR *description = NULL)
		//{
		//	// FIXME: can't test GdipConvertToEmfPlusToStream because it isn't exported in 1.0
		//	return updateStatus(DLLExports::GdipConvertToEmfPlusToStream(
		//			refGraphics ? refGraphics->nativeGraphics : NULL,
		//			(GpMetafile*) nativeImage, conversionSuccess,
		//			stream, emfType, description, ???));
		//}
		UINT GetDownLevelRasterizationLimit() const
		{
			UINT result = 0;
			updateStatus(DLLExports::GdipGetMetafileDownLevelRasterizationLimit(
				(GpMetafile*)nativeImage, &result));
			return result;
		}
		HENHMETAFILE GetHENHMETAFILE()
		{
			HENHMETAFILE result = NULL;
			updateStatus(DLLExports::GdipGetHemfFromMetafile(
				(GpMetafile*)nativeImage, &result));
			return result;
		}
		Status GetMetafileHeader(MetafileHeader* header) const
		{
			return updateStatus(DLLExports::GdipGetMetafileHeaderFromMetafile(
				(GpMetafile*)nativeImage, header));
		}
		Status PlayRecord(EmfPlusRecordType recordType, UINT flags,
			UINT dataSize, const BYTE* data) const
		{
			return updateStatus(DLLExports::GdipPlayMetafileRecord(
				(GpMetafile*)nativeImage,
				recordType, flags, dataSize, data));
		}
		Status SetDownLevelRasterizationLimit(UINT limitDpi)
		{
			return updateStatus(DLLExports::GdipSetMetafileDownLevelRasterizationLimit(
				(GpMetafile*)nativeImage, limitDpi));
		}

	private:
		Metafile(GpImage* image, Status status) : Image(image, status) {}
		Metafile(const Metafile&);
		Metafile& operator=(const Metafile&);
#endif
	};
}
#endif /* __GDIPLUS_METAFILE_H */
