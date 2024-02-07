#pragma once

#include "pch.h"

namespace RendererVulkan
{
	void Create();
	void Destroy();
	void Present(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
	void NewFrame();
	void EndFrame();
}
