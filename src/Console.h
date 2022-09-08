#include "pch.h"

#define LOG_HEX(v) std::hex << std::uppercase << (uintptr_t)v << std::dec << std::nouppercase

namespace Console
{
	void Create();
	void Destroy();
}
