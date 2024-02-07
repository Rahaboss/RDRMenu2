#pragma once

#include "pch.h"
#include "RendererD3D12.h"
#include "RendererVulkan.h"

namespace Renderer
{
	void Create();
	void Destroy();
	void CreateImGui();
	void DestroyImGui();

	// Set to true by renderer backend (D3D12 or Vulkan) on successful creation
	// and then to false before destroying
	inline bool Setup = false;

	inline ImFont* DefaultFont{};
	inline HWND hWnd{};
	inline WNDPROC WndProc{};
	LRESULT CALLBACK WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}
