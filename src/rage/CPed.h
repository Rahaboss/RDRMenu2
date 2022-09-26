#pragma once

#include "pch.h"

class CPed
{
public:
	char pad_0000[8]; //0x0000
};
static_assert(sizeof(CPed) == 0x8);
