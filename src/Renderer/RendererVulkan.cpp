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
}

void RendererVulkan::EndFrame()
{
}
