#include "pch.h"
#include "RendererVulkan.h"
#include "Renderer.h"
#include "Menu/Menu.h"
#include "Util/Timer.h"
#include "Memory/Pointers.h"
#include "Memory/Hooking.h"

static void CheckVkResult(VkResult err)
{
	if (err == VK_SUCCESS)
		return;

	LOG_TEXT("Vulkan error: %d", err);
	assert(err > 0);
}

void RendererVulkan::Create()
{
	Hooking::vkQueuePresentKHR.Create(Pointers::vkQueuePresentKHR, Hooking::vkQueuePresentKHRHook);
	Hooking::vkQueueSubmit.Create(Pointers::vkQueueSubmit, Hooking::vkQueueSubmitHook);

	return;

	ImGui_ImplVulkan_InitInfo InitInfo{};
	InitInfo.Instance = *Pointers::vkInstance;
	InitInfo.PhysicalDevice = *Pointers::vkPhysicalDevice;
	InitInfo.Device = *Pointers::vkDevice;
	//InitInfo.QueueFamily = g_QueueFamily;
	InitInfo.Queue = *Pointers::vkQueue;
	//InitInfo.PipelineCache = g_PipelineCache;
	//InitInfo.DescriptorPool = g_DescriptorPool;
	//InitInfo.Subpass = 0;
	//InitInfo.MinImageCount = g_MinImageCount;
	//InitInfo.ImageCount = wd->ImageCount;
	//InitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	//InitInfo.Allocator = g_Allocator;
	InitInfo.CheckVkResultFn = CheckVkResult;

	//ImGui_ImplVulkan_Init(&InitInfo, ...);
	
	Renderer::Setup = true;
}

void RendererVulkan::Destroy()
{
	Renderer::Setup = false;

	Hooking::vkQueueSubmit.Destroy();
	Hooking::vkQueuePresentKHR.Destroy();
}

void RendererVulkan::Present(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
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
