#pragma once

#include "pch.h"

namespace Console
{
	void Create();
	void Destroy();
}

#define LOG_HEX(v) std::hex << std::uppercase << (uintptr_t)v << std::dec << std::nouppercase
