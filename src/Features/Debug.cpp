#include "pch.h"
#include "Debug.h"
#include "Features.h"
#include "Rage/natives.h"
#include "PlayerInfo.h"

namespace Features
{
	void* GetBlipCollection()
	{
		uint64_t x = _rotl64(*Pointers::BlipBase, 29);
		return reinterpret_cast<void*>(~_rotl64(_rotl64(x ^ *Pointers::BlipHash, 32), (x & 0x1F) + 5));
	}

	void PrintNativeHandlerAddress(uint64_t hash)
	{
		TRY
		{
			uint64_t addr = (uintptr_t)NativeContext::GetHandler(hash);
			uint64_t off = addr - g_BaseAddress;
			LOG_TO_CONSOLE("0x%llX handler: RDR2.exe+0x%llX (0x%llX).\n", hash, off, (off + 0x7FF73CAB0000 /*imagebase in ida*/));
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void UnlockSPPreorderBonus()
	{
		constexpr Hash unlock_hash = RAGE_JOAAT("SP_GAME_CONTENT_PRE_ORDER");
		UNLOCK::UNLOCK_SET_UNLOCKED(unlock_hash, true);
	}

	bool IsStringValid(const char* String)
	{
		return String && String[0];
	}

	bool IsStringValid(const std::string& String)
	{
		return !String.empty();
	}

	std::filesystem::path GetConfigPath()
	{
		// Get Windows %APPDATA% environment variable
		char* Buffer = nullptr;
		size_t BufferCount = 0;
		_dupenv_s(&Buffer, &BufferCount, "APPDATA");
		assert(Buffer); // Fix _dupenv_s warning
		std::filesystem::path Path(Buffer);
		delete Buffer;

		// Change path to %APPDATA%\RDRMenu2
		Path.append("RDRMenu2");

		return Path;
	}

	void SetClipboardText(const char* Text)
	{
		ImGui::SetClipboardText(Text);
	}

	void SetClipboardText(const std::string& Text)
	{
		SetClipboardText(Text.c_str());
	}
	
	Hash GetHashFromJson(const nlohmann::json& JsonObject)
	{
		if (JsonObject.is_string())
			return joaat(JsonObject.get_ref<const std::string&>());
		
		return JsonObject.get<Hash>();
	}
	
	void* GetPedPool()
	{
		if (auto pool = Pointers::PedPoolEncryption; pool->m_IsSet)
		{
			uint64_t x = _rotl64(pool->m_Second, 30);
			return reinterpret_cast<void*>(~_rotl64(_rotl64(x ^ pool->m_First, ((x & 0xFF) & 31) + 3), 32));
		}

		return nullptr;
	}

	bool IsPedValid(void* pool, Ped ped)
	{
		uint8_t* flags = *(uint8_t**)(((uintptr_t)pool) + 0x10);
		return !((flags[ped]) & 0x80);
	}

	uint64_t* GetAt(void* pool, Ped ped)
	{
		uint8_t* flags = *(uint8_t**)(((uintptr_t)pool) + 0x10);
		uintptr_t entries = *(uintptr_t*)(((uintptr_t)pool) + 0x8);
		uint32_t itemsize = *(uint32_t*)(((uintptr_t)pool) + 0x1C);

		if (flags[ped])
			return (uint64_t*)(entries + ((uintptr_t)(ped * itemsize)));
		return NULL;
	}

	std::vector<Ped> GetAllPeds()
	{
		std::vector<Ped> result{};
		const auto pool = GetPedPool();

		if (!pool)
			return result;

		for (int i = 0; i < *(int*)(((uintptr_t)pool) + 0x18); i++)
		{
			if (IsPedValid(pool, i))
			{
				if (auto obj = GetAt(pool, i))
					result.push_back(Pointers::FwScriptGuidCreateGuid(obj));
			}
		}

		return result;
	}
}
