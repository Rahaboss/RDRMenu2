#include "pch.h"
#include "Debug.h"
#include "Features.h"
#include "rage/natives.h"
#include "PlayerInfo.h"

namespace Features
{
	void* GetBlipCollection()
	{
		uint64_t x = _rotl64(*Pointers::BlipBase, 29);
		return reinterpret_cast<void*>(~_rotl64(_rotl64(x ^ *Pointers::BlipHash, 32), (x & 0x1F) + 5));
	}

	void* GetScriptGlobal(uint32_t Index)
	{
		uintptr_t qword_18002D4F0 = g_BaseAddress + 0xB0000000;
		uint32_t y = *(uint32_t*)qword_18002D4F0;
		uint32_t dword_18002D4F8 = 0;
		
		if (y > 0x5DE8CC8B)
		{
			switch (y)
			{
			case 0x8B48D38B:
			{
				int v2 = *(int*)(qword_18002D4F0 + 0x143200000i64);
				if (v2 == 1837819906)
					dword_18002D4F8 = 6;
				if (v2 == 496633225)
					dword_18002D4F8 = 7;
				break;
			}
			case 0xD233FF87:
				dword_18002D4F8 = 4;
				break;
			case 0xD68B4808:
				dword_18002D4F8 = 10;
				break;
			case 0xE870290F:
				dword_18002D4F8 = 9;
				break;
			}
		}
		else
		{
			switch (y)
			{
			case 0x5DE8CC8Bu:
				dword_18002D4F8 = 1;
				break;
			case 0x2A5D3A4u:
				dword_18002D4F8 = 5;
				break;
			case 0x573F72Fu:
				dword_18002D4F8 = 8;
				break;
			case 0x100FFF94u:
				dword_18002D4F8 = 11;
				break;
			case 0x18488948u:
				dword_18002D4F8 = 2;
				break;
			case 0x39CCC328u:
				dword_18002D4F8 = 3;
				break;
			}
		}

		std::vector<uint64_t> qword_18002BB10{
			0,
			0x1457C3B00,
			0x1457CAEE0,
			0x1457D0040,
			0x1457D0170,
			0x1457D1330,
			0x14581D190,
			0x14581E2B0,
			0x145909370,
			0x1459E1040,
			0x145953A90,
			0x14598FDD0,
		};

		assert(dword_18002D4F8 > 0 && dword_18002D4F8 < 12);

		//uint64_t** GlobalPtr = (uint64_t**)(0x14598FDD0 + g_BaseAddress);
		//return GlobalPtr[(Index >> 0x12) & 0x3F] + (Index & 0x3FFFF);
		printf("Global Pointer: 0x%llX.\n", (qword_18002BB10[dword_18002D4F8]) + qword_18002D4F0);

		return (void*)(*((uint64_t*)qword_18002BB10[dword_18002D4F8] + 8i64 * ((Index >> 0x12) & 0x3F) + qword_18002D4F0) + 8i64 * (Index & 0x3FFFF));
	}

	void PrintNativeHandlerAddress(uint64_t hash)
	{
		TRY
		{
			uint64_t addr = (uintptr_t)NativeContext::GetHandler(hash);
			uint64_t off = addr - g_BaseAddress;
			printf("0x%llX handler: RDR2.exe+0x%llX (0x%llX).\n", hash, off, (off + 0x7FF73CAB0000 /*imagebase in ida*/));
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void UnlockSPPreorderBonus()
	{
		constexpr Hash unlock_hash = RAGE_JOAAT("SP_GAME_CONTENT_PRE_ORDER");
		UNLOCK::UNLOCK_SET_UNLOCKED(unlock_hash, true);
	}
}
