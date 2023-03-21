#include "pch.h"

namespace ImGui
{
	void StyleColorsCherry();
	bool InputU32(const char* label, unsigned int* v, unsigned int step = 1, unsigned int step_fast = 100, ImGuiInputTextFlags flags = 0);
	bool InputU64(const char* label, uint64_t* v, uint64_t step = 1, uint64_t step_fast = 100, ImGuiInputTextFlags flags = 0);
}
