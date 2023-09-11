#pragma once

#include "pch.h"
#include "joaat.h"
#include "scrVector.h"

namespace rage
{
	class scrNativeCallContext
	{
	public:
		void* m_ReturnValue; //0x0000
		uint32_t m_ArgCount; //0x0008
		uint64_t* m_Args; //0x0010
		uint32_t m_DataCount; //0x0018
		scrVector* m_OutVectors[4]; //0x0020
		scrVec3N m_InVectors[4]; //0x0040
		uint8_t pad[96]; //0x0080

		template <typename T>
		inline T GetArg(size_t Index)
		{
			return *(T*)(&(m_Args[Index]));
		}

		template <typename T>
		inline T GetRet()
		{
			return *(T*)(m_ReturnValue);
		}

		template <typename T>
		inline void SetRet(T Ret)
		{
			*(T*)(m_ReturnValue) = Ret;
		}
	}; //Size: 0x00E0
	static_assert(sizeof(scrNativeCallContext) == 0xE0);

	typedef void (*scrNativeHandler)(scrNativeCallContext*);

	typedef uint64_t scrNativeHash;
}

typedef int32_t Any;
typedef int32_t Blip;
typedef int32_t Cam;
typedef int32_t Entity;
typedef int32_t FireId;
typedef rage::joaat_t Hash;
typedef int32_t Interior;
typedef int32_t ItemSet;
typedef Entity Object;
typedef Entity Ped;
typedef Object Pickup;
typedef int32_t Player;
typedef Entity ScrHandle;
typedef Entity Vehicle;

typedef int32_t AnimScene;
typedef int32_t PersChar;
typedef int32_t PopZone;
typedef int32_t Prompt;
typedef int32_t PropSet;
typedef int32_t Volume;

typedef rage::scrVector Vector3;
