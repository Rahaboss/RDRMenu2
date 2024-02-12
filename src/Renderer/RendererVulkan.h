#pragma once

#include "pch.h"

namespace RendererVulkan
{
	void Create();
	void Destroy();
#if ENABLE_VULKAN_RENDERER
	void Present(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
	void NewFrame();
	void EndFrame();
#endif // ENABLE_VULKAN_RENDERER
}
