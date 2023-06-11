#include "pch.h"
#include "CPed.h"
#include "Pointers.h"

CPedFactory* CPedFactory::Get()
{
	if (Pointers::PedFactoryEncryption && Pointers::PedFactoryEncryption->m_IsSet)
	{
		uint64_t x = _rotl64(Pointers::PedFactoryEncryption->m_Second, 27);
		return reinterpret_cast<CPedFactory*>(~_rotl64(_rotl64(x ^ Pointers::PedFactoryEncryption->m_First, 32), (x & 0x1F) + 1));
	}

	return nullptr;
}

CPed* CPedFactory::GetLocalPed()
{
	if (CPedFactory* PedFactory = CPedFactory::Get())
		return PedFactory->m_LocalPed;

	return nullptr;
}
