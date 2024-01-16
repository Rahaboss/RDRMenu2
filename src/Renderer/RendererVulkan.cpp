#include "pch.h"
#include "RendererVulkan.h"
#include "Renderer.h"
#include "Menu/Menu.h"
#include "Util/Timer.h"

void RendererVulkan::Create()
{
}

void RendererVulkan::Destroy()
{
}

void RendererVulkan::Present()
{
	TRY
	{
		Timer RenderThreadTimer;

		if (Renderer::Setup)
		{
			NewFrame();
			Menu::Render();
			EndFrame();
		}

		Timer::s_RenderThreadTime = RenderThreadTimer.GetMillis();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void RendererVulkan::NewFrame()
{
	ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void RendererVulkan::EndFrame()
{
	ImGui::Render();
	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ...);
}
