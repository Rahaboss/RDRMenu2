#include "pch.h"
#include "fwBasePool.h"
#include "Pointers.h"

bool fwBasePool::Full() const
{
	return m_Size - (m_FreeSlotIndex & 0x3FFFFFFF) <= 256;
}

int64_t fwBasePool::GetNumFreeSlots() const
{
	return static_cast<int64_t>(m_Size) - static_cast<signed int>((m_FreeSlotIndex * 4) >> 2);
}

int32_t fwBasePool::GetScriptGuid(int32_t Index) const
{
	return (Index << 8) + m_Flags[Index];
}

int32_t fwBasePool::GetIndex(int32_t ScriptGuid) const
{
	return ScriptGuid >> 8;
}

bool fwBasePool::IsValid(int32_t Index) const
{
	return !(m_Flags[Index] & 0x80);
}

fwBasePool* fwBasePool::GetPedPool()
{
	if (auto pool = Pointers::PedPoolEncryption; pool->m_IsSet)
	{
		uint64_t x = _rotl64(pool->m_Second, 30);
		return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(x ^ pool->m_First, ((x & 0xFF) & 31) + 3), 32));
	}

	return nullptr;
}

fwBasePool* fwBasePool::GetObjectPool()
{
	if (auto pool = Pointers::ObjectPoolEncryption; pool->m_IsSet)
	{
		uint64_t x = _rotl64(pool->m_Second, 30);
		return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(x ^ pool->m_First, 32), ((x & 0xFF) & 31) + 2));
	}

	return nullptr;
}

fwBasePool* fwBasePool::GetVehiclePool()
{
	if (auto pool = Pointers::VehiclePoolEncryption; pool->m_IsSet)
	{
		uint64_t x = _rotl64(pool->m_Second, 31);
		return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(x ^ pool->m_First, 32), ((x & 0xFF) & 31) + 4));
	}

	return nullptr;
}

fwBasePool* fwBasePool::GetPickupPool()
{
	if (auto pool = Pointers::PickupPoolEncryption; pool->m_IsSet)
	{
		uint64_t x = _rotl64(pool->m_Second, 30);
		return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(pool->m_First ^ x, 32), ((x & 0xFF) & 31) + 2));
	}

	return nullptr;
}
