#pragma once

#include "pch.h"

namespace Menu
{
#if 0
	bool RenderButton(const char* Text, bool Disabled = false);
	bool RenderToggle(const char* Text, bool& Toggle, bool Disabled = false, const char* EnabledText = "true", const char* DisabledText = "false");
	bool UpdateMenuState();
	bool IsGameWindowActive();
	void RenderMenu();
#else
	void RenderMenu();
#endif
}
