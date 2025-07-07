#include "Renderer.h"
#include "Menu/Menu.h"
#include "Memory/Pointers.h"
#include "Fonts.h"
#include "Config/Config.h"

static bool IsUsingD3D12()
{
	return Pointers::SwapChain && *Pointers::SwapChain && Pointers::CommandQueue && *Pointers::CommandQueue;
}

void Renderer::Create()
{
	CreateImGui();

	if (IsUsingD3D12())
	{
		LOG_TEXT("Creating D3D12 renderer.");
		RendererD3D12::Create();
	}
	else
	{
		LOG_TEXT("Creating Vulkan renderer.");
		RendererVulkan::Create();
	}
}

void Renderer::Destroy()
{
	if (Setup)
	{
		if (IsUsingD3D12())
		{
			LOG_TEXT("Destroying D3D12 renderer.");
			RendererD3D12::Destroy();
		}
		else
		{
			LOG_TEXT("Destroying Vulkan renderer.");
			RendererVulkan::Destroy();
		}
	}

	DestroyImGui();
}

void Renderer::CreateImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiStyle& Style = ImGui::GetStyle();

	Style.WindowTitleAlign.x = 0.5f;
	Style.WindowRounding = Style.ChildRounding = Style.FrameRounding = Style.PopupRounding =
		Style.ScrollbarRounding = Style.GrabRounding = Style.TabRounding = 4.0f;

	ImGuiIO& io = ImGui::GetIO();
	
	// Made static so it persists until end of the program
	static std::string IniPath(Config::GetConfigPath().append("imgui.ini").string());
	io.IniFilename = IniPath.c_str();

	ImFontConfig FontCfg{};
	FontCfg.FontDataOwnedByAtlas = false;

	// Main font
	strcpy_s(FontCfg.Name, "Chalet London 1960");
	io.FontDefault = io.Fonts->AddFontFromMemoryTTF(
		reinterpret_cast<void*>(const_cast<uint8_t*>(Fonts::ChaletLondon1960)), // C++ style (void*) cast lol
		sizeof(Fonts::ChaletLondon1960), 20.0f, &FontCfg);
	
#ifndef _DIST
	// Add ProggyClean font
	DefaultFont = io.Fonts->AddFontDefault();
#else
	DefaultFont = io.FontDefault;
#endif // _DIST

	io.Fonts->Build();

	hWnd = FindWindow(L"sgaWindow", NULL);
	WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcHook)));

	ImGui_ImplWin32_Init(hWnd);
}

void Renderer::DestroyImGui()
{
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static POINT CursorCoords{};
LRESULT CALLBACK Renderer::WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Setup)
	{
		if (uMsg == WM_KEYUP && wParam == VK_INSERT)
		{
			// Persist and restore the cursor position between menu instances
			if (Menu::IsOpen)
				GetCursorPos(&CursorCoords);
			else if (CursorCoords.x + CursorCoords.y != 0)
				SetCursorPos(CursorCoords.x, CursorCoords.y);

			Menu::IsOpen = !Menu::IsOpen;
		}

		if (Menu::IsOpen)
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	}

	// Always call the original event handler even if menu is open
	return CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam);
}
