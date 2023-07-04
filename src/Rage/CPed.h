#pragma once

#include "pch.h"

class CPed
{
public:
	virtual ~CPed() = 0; //0x0000
	char pad_0008[8]; //0x0008
}; //Size: 0x0010
static_assert(sizeof(CPed) == 0x10);

class CPedFactory
{
public:
	virtual ~CPedFactory() = 0; //0x0000
	CPed* m_LocalPed; //0x0008

	static CPedFactory* Get();
	static CPed* GetLocalPed();
}; //Size: 0x0010
static_assert(sizeof(CPedFactory) == 0x10);
