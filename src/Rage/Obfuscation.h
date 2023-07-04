#pragma once

#include "pch.h"

// lmao
// https://uspto.report/patent/grant/11273380

class VariableEncryption
{
public:
	bool m_IsSet; //0x0000
	uint64_t m_First; //0x0008
	uint64_t m_Second; //0x0010
}; //Size: 0x0018

static_assert(sizeof(VariableEncryption) == 0x18);
static_assert(offsetof(VariableEncryption, m_First) == 0x8);
static_assert(offsetof(VariableEncryption, m_Second) == 0x10);
