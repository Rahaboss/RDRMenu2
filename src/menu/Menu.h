#pragma once

#include "pch.h"

namespace Menu
{
	bool RenderButton(const char* Text);
	bool RenderToggle(const char* Text, bool& Toggle, const char* EnabledText = "true", const char* DisabledText = "false");
	bool UpdateMenuState();
	void RenderMenu();
}
