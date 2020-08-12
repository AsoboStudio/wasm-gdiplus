// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "GdiPlusStringFormat.h"

namespace Gdiplus {

	// StringFormat

	Status StringFormat::SetAlignment(StringAlignment align)
	{
		alignment = align;
		return Ok;
	}

	StringAlignment StringFormat::GetAlignment() const
	{
		return alignment;
	}

	Status StringFormat::SetLineAlignment(StringAlignment align)
	{
		line_alignment = align;
		return Ok;
	}

	StringAlignment StringFormat::GetLineAlignment() const
	{
		return line_alignment;
	}
}
