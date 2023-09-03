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

	inline bool Setup = false;

	inline HWND hWnd{};
	inline WNDPROC WndProc{};
	LRESULT CALLBACK WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}
