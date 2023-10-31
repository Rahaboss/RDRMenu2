#pragma once

#include "pch.h"

namespace ImGui
{
	bool InputU64(const char* label, uint64_t* v, uint64_t step = 1, uint64_t step_fast = 100, ImGuiInputTextFlags flags = 0);
	bool InputU32(const char* label, uint32_t* v, uint32_t step = 1, uint32_t step_fast = 100, ImGuiInputTextFlags flags = 0);
}
