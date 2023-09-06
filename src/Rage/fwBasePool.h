#pragma once

#include "pch.h"

namespace rage
{
	class fwBasePool
	{
	public:
		virtual ~fwBasePool() = 0; // 0x0000
		uintptr_t m_Entries;       // 0x0008
		uint8_t* m_Flags;          // 0x0010
		uint32_t m_Size;           // 0x0018
		uint32_t m_ItemSize;       // 0x001C
		uint32_t m_0020;           // 0x0020
		uint32_t m_0024;           // 0x0024
		uint32_t m_FreeSlotIndex;  // 0x0028

		bool Full() const;
		int64_t GetNumFreeSlots() const;
		int32_t GetScriptGuid(int32_t Index) const;
		int32_t GetIndex(int32_t ScriptGuid) const;
		bool IsValid(int32_t Index) const;

		template<typename T>
		inline T* GetAt(size_t Index)
		{
			if (m_Flags[Index])
				return reinterpret_cast<T*>(m_Entries + Index * m_ItemSize);

			return nullptr;
		}

		static fwBasePool* GetPedPool();
		static fwBasePool* GetObjectPool();
		static fwBasePool* GetVehiclePool();
		static fwBasePool* GetPickupPool();
	}; //Size: 0x0030
	static_assert(sizeof(fwBasePool) == 0x30);
}
