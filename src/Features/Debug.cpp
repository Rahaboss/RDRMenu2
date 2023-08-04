#include "pch.h"
#include "Debug.h"
#include "Features.h"
#include "Rage/Natives.h"
#include "PlayerInfo.h"

namespace Features
{
	void* GetBlipCollection()
	{
		if (auto blip = Pointers::BlipEncryption; blip->m_IsSet)
		{
			uint64_t x = _rotl64(blip->m_Second, 29);
			return reinterpret_cast<void*>(~_rotl64(_rotl64(x ^ blip->m_First, 32), (x & 0x1F) + 5));
		}

		return nullptr;
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

	bool IsStringValid(const char* String)
	{
		return String && String[0];
	}

	bool IsStringValid(const std::string& String)
	{
		return !String.empty();
	}

	static std::filesystem::path s_ConfigPath;
	void CreateConfigPath()
	{
		// Get Windows %APPDATA% environment variable
		char* Buffer = nullptr;
		size_t BufferCount = 0;
		_dupenv_s(&Buffer, &BufferCount, "APPDATA");
		assert(Buffer); // Fix _dupenv_s warning
		s_ConfigPath = Buffer;
		delete Buffer;

		// Change path to %APPDATA%\RDRMenu2
		s_ConfigPath.append("RDRMenu2");

		// Create folder at path if it doesn't exist
		if (!std::filesystem::exists(s_ConfigPath))
		{
			std::filesystem::create_directory(s_ConfigPath);
		}
		// If a file not a folder at that path exists, replace with folder
		else if (!std::filesystem::is_directory(s_ConfigPath))
		{
			std::filesystem::remove(s_ConfigPath);
			std::filesystem::create_directory(s_ConfigPath);
		}
	}

	std::filesystem::path GetConfigPath()
	{
		return s_ConfigPath;
	}

	void SetClipboardText(const char* Text)
	{
		ImGui::SetClipboardText(Text);
	}

	void SetClipboardText(const std::string& Text)
	{
		SetClipboardText(Text.c_str());
	}
	
	Hash GetHashFromJson(const json& JsonObject)
	{
		if (JsonObject.is_string())
			return rage::joaat(JsonObject.get_ref<const std::string&>());
		
		return JsonObject.get<Hash>();
	}
	
	std::vector<Ped> GetAllPeds(bool CheckEntityValidity)
	{
		std::vector<Ped> result{};
		const auto pool = rage::fwBasePool::GetPedPool();

		if (!pool)
			return result;

		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (int64_t* obj = pool->GetAt<int64_t>(i))
				{
					Ped ped = Pointers::fwScriptGuidCreateGuid(obj);
					if (CheckEntityValidity)
					{
						if (!ENTITY::DOES_ENTITY_EXIST(ped) || !STREAMING::IS_MODEL_A_PED(ENTITY::GET_ENTITY_MODEL(ped)))
							continue;
					}
					
					result.push_back(ped);
				}
			}
		}

		return result;
	}
	
	std::vector<Object> GetAllObjects(bool CheckEntityValidity)
	{
		std::vector<Object> result{};
		const auto pool = rage::fwBasePool::GetObjectPool();

		if (!pool)
			return result;

		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (int64_t* obj = pool->GetAt<int64_t>(i))
				{
					Object object = Pointers::fwScriptGuidCreateGuid(obj);
					if (CheckEntityValidity)
					{
						if (!ENTITY::DOES_ENTITY_EXIST(object) || !STREAMING::_IS_MODEL_AN_OBJECT(ENTITY::GET_ENTITY_MODEL(object)))
							continue;
					}

					result.push_back(object);
				}
			}
		}

		return result;
	}
	
	std::vector<Vehicle> GetAllVehicles(bool CheckEntityValidity)
	{
		std::vector<Vehicle> result{};
		const auto pool = rage::fwBasePool::GetVehiclePool();

		if (!pool)
			return result;

		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (int64_t* obj = pool->GetAt<int64_t>(i))
				{
					Vehicle veh = Pointers::fwScriptGuidCreateGuid(obj);
					if (CheckEntityValidity)
					{
						if (!ENTITY::DOES_ENTITY_EXIST(veh) || !STREAMING::IS_MODEL_A_VEHICLE(ENTITY::GET_ENTITY_MODEL(veh)))
							continue;
					}

					result.push_back(veh);
				}
			}
		}

		return result;
	}
	
	std::vector<Pickup> GetAllPickups(bool CheckEntityValidity)
	{
		std::vector<Pickup> result{};
		const auto pool = rage::fwBasePool::GetPickupPool();

		if (!pool)
			return result;

		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (int64_t* obj = pool->GetAt<int64_t>(i))
				{
					Pickup pickup = Pointers::fwScriptGuidCreateGuid(obj);
					if (CheckEntityValidity)
					{
						if (!ENTITY::DOES_ENTITY_EXIST(pickup) || !STREAMING::_IS_MODEL_AN_OBJECT(ENTITY::GET_ENTITY_MODEL(pickup)))
							continue;
					}

					result.push_back(pickup);
				}
			}
		}

		return result;
	}
	
	void StringToLower(std::string& String)
	{
		std::transform(String.cbegin(), String.cend(), String.begin(), ::tolower);
	}
}
