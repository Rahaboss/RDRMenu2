#pragma once

#include "pch.h"

namespace RendererVulkan
{
	void Create();
	void Destroy();
	void Present();
	void NewFrame();
	void EndFrame();
}
