#include "pch.h"
#include "RendererVulkan.h"
#include "Renderer.h"
#include "Menu.h"

void RendererVulkan::Create()
{
}

void RendererVulkan::Destroy()
{
}

void RendererVulkan::Present()
{
	if (Renderer::Setup)
	{
		NewFrame();
		Menu::RenderMenu();
		EndFrame();
	}
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
