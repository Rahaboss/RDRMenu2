#include "pch.h"
#include "fwBasePool.h"
#include "Memory/Pointers.h"

namespace rage
{
	bool fwBasePool::Full() const
	{
		return m_Size - (m_FreeSlotIndex & 0x3FFFFFFF) <= 256;
	}

	int64_t fwBasePool::GetNumFreeSlots() const
	{
		return static_cast<int64_t>(m_Size) - static_cast<int>((m_FreeSlotIndex * 4) >> 2);
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
		if (Pointers::PedPoolEncryption->m_IsSet)
		{
			uint64_t x = _rotl64(Pointers::PedPoolEncryption->m_Second, 30);
			return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(x ^ Pointers::PedPoolEncryption->m_First, ((x & 0xFF) & 31) + 3), 32));
		}

		return nullptr;
	}

	fwBasePool* fwBasePool::GetObjectPool()
	{
		if (Pointers::ObjectPoolEncryption->m_IsSet)
		{
			uint64_t x = _rotl64(Pointers::ObjectPoolEncryption->m_Second, 30);
			return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(x ^ Pointers::ObjectPoolEncryption->m_First, 32), ((x & 0xFF) & 31) + 2));
		}

		return nullptr;
	}

	fwBasePool* fwBasePool::GetVehiclePool()
	{
		if (Pointers::VehiclePoolEncryption->m_IsSet)
		{
			uint64_t x = _rotl64(Pointers::VehiclePoolEncryption->m_Second, 31);
			return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(x ^ Pointers::VehiclePoolEncryption->m_First, 32), ((x & 0xFF) & 31) + 4));
		}

		return nullptr;
	}

	fwBasePool* fwBasePool::GetPickupPool()
	{
		if (Pointers::PickupPoolEncryption->m_IsSet)
		{
			uint64_t x = _rotl64(Pointers::PickupPoolEncryption->m_Second, 30);
			return reinterpret_cast<fwBasePool*>(~_rotl64(_rotl64(Pointers::PickupPoolEncryption->m_First ^ x, 32), ((x & 0xFF) & 31) + 2));
		}

		return nullptr;
	}
}
