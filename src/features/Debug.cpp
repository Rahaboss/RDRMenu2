#include "pch.h"
#include "Features.h"

namespace Features
{
	void* GetBlipCollection()
	{
		uint64_t x = _rotl64(*Pointers::BlipBase, 29);
		return reinterpret_cast<void*>(~_rotl64(_rotl64(x ^ *Pointers::BlipHash, 32), (x & 0x1F) + 5));
	}

	CPedFactory* GetPedFactory()
	{
		uint64_t x = _rotl64(*Pointers::PedFactoryBase, 27);
		return reinterpret_cast<CPedFactory*>(~_rotl64(_rotl64(x ^ *Pointers::PedFactoryHash, 32), (x & 0x1F) + 1));
	}

	void PrintNativeHandlerAddress(const uint64_t& hash)
	{
		TRY
		{
			auto addr = (uintptr_t)g_NativeContext.GetHandler(hash);
			auto off = addr - g_BaseAddress;
			std::cout << LOG_HEX(hash) << " handler: RDR2.exe+" << LOG_HEX(off) << " (" << LOG_HEX(0x7FF73CAB0000 /*imagebase in ida*/ + off) << ").\n";
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void UnlockSPPreorderBonus()
	{
		constexpr Hash unlock_hash = RAGE_JOAAT("SP_GAME_CONTENT_PRE_ORDER");
		UNLOCK::UNLOCK_SET_UNLOCKED(unlock_hash, TRUE);
	}
}
