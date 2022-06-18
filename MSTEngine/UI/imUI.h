#pragma once

#include "mstglm.h"

namespace ui
{
	void AddText(const char* Text, v2f& Position, const Color& Colour);
	void AddText(const char* Text, v2f&& Position, const Color& Colour);
	bool AddButton(const char* Label, const v2f& Position, const Color& Colour);

	void InitUI();
	void Present();
}
