#include "pch.h"
#include "CPed.h"
#include "Pointers.h"

CPedFactory* CPedFactory::Get()
{
	if (Pointers::PedFactoryBase && Pointers::PedFactoryHash)
	{
		uint64_t x = _rotl64(*Pointers::PedFactoryBase, 27);
		return reinterpret_cast<CPedFactory*>(~_rotl64(_rotl64(x ^ *Pointers::PedFactoryHash, 32), (x & 0x1F) + 1));
	}

	return nullptr;
}

CPed* CPedFactory::GetLocalPed()
{
	if (CPedFactory* PedFactory = Get())
		return PedFactory->m_LocalPed;

	return nullptr;
}
