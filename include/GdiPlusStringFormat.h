// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
// based on the w32api implementation of the GDI+ wrappers: 
//
//   Created by Markus Koenig <markus@stber-koenig.de>


#ifndef __GDIPLUS_STRINGFORMAT_H
#define __GDIPLUS_STRINGFORMAT_H

namespace Gdiplus
{
	// FIXME: do StringFormat::GenericDefault() et al. need to be thread safe?
	// FIXME: maybe put this in gdiplus.c?

	extern "C" void* _GdipStringFormatCachedGenericDefault;
	extern "C" void* _GdipStringFormatCachedGenericTypographic;

	class StringFormat : public GdiplusBase
	{
		friend class Graphics;
		friend class GraphicsPath;

	public:

		StringFormat(INT formatFlags = 0, LANGID language = LANG_NEUTRAL) :
			nativeStringFormat(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCreateStringFormat(
				formatFlags, language, &nativeStringFormat);
		}
		StringFormat(const StringFormat* format) :
			nativeStringFormat(NULL), lastStatus(Ok)
		{
			lastStatus = DLLExports::GdipCloneStringFormat(
				format ? format->nativeStringFormat : NULL,
				&nativeStringFormat);
		}
		~StringFormat()
		{
			DLLExports::GdipDeleteStringFormat(nativeStringFormat);
		}
		StringFormat* Clone() const
		{
			GpStringFormat* cloneStringFormat = NULL;
			Status status = updateStatus(DLLExports::GdipCloneStringFormat(
				nativeStringFormat, &cloneStringFormat));
			if (status == GpStatus::Ok) {
				StringFormat* result = new StringFormat(
					cloneStringFormat, lastStatus);
				if (!result) {
					DLLExports::GdipDeleteStringFormat(cloneStringFormat);
					lastStatus = OutOfMemory;
				}
				return result;
			}
			else {
				return NULL;
			}
		}

		StringAlignment GetAlignment() const
		{
			StringAlignment result = StringAlignmentNear;
			updateStatus(DLLExports::GdipGetStringFormatAlign(
				nativeStringFormat, &result));
			return result;
		}
		LANGID GetDigitSubstitutionLanguage() const
		{
			LANGID result = 0;
			StringDigitSubstitute method;
			updateStatus(DLLExports::GdipGetStringFormatDigitSubstitution(
				nativeStringFormat, &result, &method));
			return result;
		}
		StringDigitSubstitute GetDigitSubstitutionMethod() const
		{
			LANGID language;
			StringDigitSubstitute result = StringDigitSubstituteUser;
			updateStatus(DLLExports::GdipGetStringFormatDigitSubstitution(
				nativeStringFormat, &language, &result));
			return result;
		}
		INT GetFormatFlags() const
		{
			INT result = 0;
			updateStatus(DLLExports::GdipGetStringFormatFlags(
				nativeStringFormat, &result));
			return result;
		}
		HotkeyPrefix GetHotkeyPrefix() const
		{
			HotkeyPrefix result = HotkeyPrefixNone;
			updateStatus(DLLExports::GdipGetStringFormatHotkeyPrefix(
				nativeStringFormat, (INT*)&result));
			return result;
		}
		Status GetLastStatus() const
		{
			Status result = lastStatus;
			lastStatus = Ok;
			return result;
		}
		StringAlignment GetLineAlignment() const
		{
			StringAlignment result = StringAlignmentNear;
			updateStatus(DLLExports::GdipGetStringFormatLineAlign(
				nativeStringFormat, &result));
			return result;
		}
		INT GetMeasurableCharacterRangeCount() const
		{
			INT result = 0;
			updateStatus(DLLExports::GdipGetStringFormatMeasurableCharacterRangeCount(
				nativeStringFormat, &result));
			return result;
		}
		INT GetTabStopCount() const
		{
			INT result = 0;
			updateStatus(DLLExports::GdipGetStringFormatTabStopCount(
				nativeStringFormat, &result));
			return result;
		}
		Status GetTabStops(INT count, REAL* firstTabOffset, REAL* tabStops)
		{
			return updateStatus(DLLExports::GdipGetStringFormatTabStops(
				nativeStringFormat, count,
				firstTabOffset, tabStops));
		}
		StringTrimming GetTrimming() const
		{
			StringTrimming result = StringTrimmingNone;
			updateStatus(DLLExports::GdipGetStringFormatTrimming(
				nativeStringFormat, &result));
			return result;
		}
		Status SetAlignment(StringAlignment align)
		{
			return updateStatus(DLLExports::GdipSetStringFormatAlign(
				nativeStringFormat, align));
		}
		Status SetDigitSubstitution(LANGID language,
			StringDigitSubstitute substitute)
		{
			return updateStatus(DLLExports::GdipSetStringFormatDigitSubstitution(
				nativeStringFormat, language, substitute));
		}
		Status SetFormatFlags(INT flags)
		{
			return updateStatus(DLLExports::GdipSetStringFormatFlags(
				nativeStringFormat, flags));
		}
		Status SetHotkeyPrefix(HotkeyPrefix hotkeyPrefix)
		{
			return updateStatus(DLLExports::GdipSetStringFormatHotkeyPrefix(
				nativeStringFormat, (INT)hotkeyPrefix));
		}
		Status SetLineAlignment(StringAlignment align)
		{
			return updateStatus(DLLExports::GdipSetStringFormatLineAlign(
				nativeStringFormat, align));
		}
		Status SetMeasurableCharacterRanges(INT rangeCount,
			const CharacterRange* ranges)
		{
			return updateStatus(DLLExports::GdipSetStringFormatMeasurableCharacterRanges(
				nativeStringFormat, rangeCount, ranges));
		}
		Status SetTabStops(REAL firstTabOffset, INT count, const REAL* tabStops)
		{
			return updateStatus(DLLExports::GdipSetStringFormatTabStops(
				nativeStringFormat, firstTabOffset,
				count, tabStops));
		}
		Status SetTrimming(StringTrimming trimming)
		{
			return updateStatus(DLLExports::GdipSetStringFormatTrimming(
				nativeStringFormat, trimming));
		}

	private:
		StringFormat(GpStringFormat* stringFormat, Status status) :
			nativeStringFormat(stringFormat), lastStatus(status) {}
		StringFormat(const StringFormat&);
		StringFormat& operator=(const StringFormat&);

		Status updateStatus(Status newStatus) const
		{
			if (newStatus != GpStatus::Ok) lastStatus = newStatus;
			return newStatus;
		}

		GpStringFormat* nativeStringFormat;
		mutable Status lastStatus;


	public:
		const StringFormat* GenericDefault()
		{
			if (!_GdipStringFormatCachedGenericDefault) {
				GpStringFormat* nativeStringFormat = 0;
				Status status = DLLExports::GdipStringFormatGetGenericDefault(
					&nativeStringFormat);
				if (status == Ok && nativeStringFormat) {
					_GdipStringFormatCachedGenericDefault = (void*)
						new StringFormat(nativeStringFormat, GpStatus::Ok);
				}
			}
			return (StringFormat*)_GdipStringFormatCachedGenericDefault;
		}

		const StringFormat* GenericTypographic()
		{
			if (!_GdipStringFormatCachedGenericTypographic) {
				GpStringFormat* nativeStringFormat = 0;
				Status status = DLLExports::GdipStringFormatGetGenericTypographic(
					&nativeStringFormat);
				if (status == Ok && nativeStringFormat) {
					_GdipStringFormatCachedGenericTypographic = (void*)
						new StringFormat(nativeStringFormat, GpStatus::Ok);
				}
			}
			return (StringFormat*)_GdipStringFormatCachedGenericTypographic;
		}
	};
}
#endif /* __GDIPLUS_STRINGFORMAT_H */
