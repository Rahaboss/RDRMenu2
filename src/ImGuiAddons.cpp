#include "pch.h"
#include "ImGuiAddons.h"

namespace ImGui
{
	void StyleColorsCherry()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.90f);
		colors[ImGuiCol_Border] = ImVec4(0.54f, 0.48f, 0.25f, 0.16f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.23f, 0.20f, 0.27f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.86f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.46f, 0.20f, 0.30f, 0.76f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.86f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.46f, 0.20f, 0.30f, 0.76f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.46f, 0.20f, 0.30f, 0.43f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
	}
	
	bool InputU32(const char* label, unsigned int* v, unsigned int step, unsigned int step_fast, ImGuiInputTextFlags flags)
	{
		const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%u";
		return InputScalar(label, ImGuiDataType_U32, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
	}
	
	bool InputU64(const char* label, uint64_t* v, uint64_t step, uint64_t step_fast, ImGuiInputTextFlags flags)
	{
		const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%llX" : "%llu";
		return InputScalar(label, ImGuiDataType_U64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
	}
}
