// Copyright (C) Asobo Studio. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "gdiplusheaders.h"

namespace Gdiplus {

	// Font
	Font::Font(const WCHAR* familyName, REAL emSize, INT style, Unit unit, const FontCollection* fontCollection)
	{
		fontid = -1;
		size = emSize;
		if (wcscmp(familyName, L"Arial") == 0)
		{
			name = "Arial";
			path = "./Fonts/arial.ttf";
		}
		else if (wcscmp(familyName, L"Arial Black") == 0)
		{
			name = "Arial Black";
			path = "./Fonts/ariblk.ttf";
		}
		else if (wcscmp(familyName, L"Arial Rounded MT") == 0)
		{
			name = "Arial Rounded MT";
			path = "./Fonts/arial.ttf";
		}
		else if (wcscmp(familyName, L"DACRJDisp") == 0)
		{
			name = "DACRJDisp";
			path = "./Fonts/DACRJDisp.ttf";
		}
		else if (wcscmp(familyName, L"DA FMS Font") == 0)
		{
			name = "DA FMS Font";
			path = "./Fonts/DACRJ.ttf";
		}
		else if (wcscmp(familyName, L"DA FMS Font Small") == 0)
		{
			name = "DA FMS Font Small";
			path = "./Fonts/DACRJS.ttf";
		}
		else if (wcscmp(familyName, L"DACRJHGS") == 0)
		{
			name = "DACRJHGS";
			path = "./Fonts/DACRJHGS.ttf";
		}
		else
		{
			name = "Arial";
			path = "./Fonts/arial.ttf";
		}

	}

	bool Font::Load(NVGcontext* _ctx) const
	{
		if (fontid == -1)
			fontid = nvgCreateFont(_ctx, name.c_str(), path.c_str());
		return (fontid != -1);
	}


}