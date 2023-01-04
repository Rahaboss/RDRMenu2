#include "pch.h"
#include "Lists.h"

namespace Lists
{
	std::filesystem::path GetSettingsFolderPath()
	{
		char* Buffer = nullptr;
		size_t BufferCount = 0;
		_dupenv_s(&Buffer, &BufferCount, "APPDATA");
		assert(Buffer);
		std::filesystem::path Path(Buffer);
		delete Buffer;
		Path.append("RDRMenu2");

		return Path;
	}

	bool IsModelValid(Hash model)
	{
		return STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model);
	}

	bool IsStringValid(const char* str)
	{
		return str && str[0];
	}

	bool IsStringValid(const std::string& str)
	{
		return !str.empty();
	}

	void InitVehicleList(const std::filesystem::path& Path)
	{
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			printf("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		nlohmann::json j;
		File >> j;
		File.close();

		for (const auto& v : j)
		{
			auto name = v.get<std::string>();
			Hash model = joaat(name);

			if (!IsModelValid(model))
			{
				printf("%s: %s is invalid!\n", __FUNCTION__, name.c_str());
				continue;
			}

			g_VehicleList[name] = model;
			g_VehicleModelNameList[model] = name;
		}
	}

	void InitProvisionList(const std::filesystem::path& Path)
	{
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			printf("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}
		
		nlohmann::json j;
		File >> j;
		File.close();
		
		for (const auto& p : j)
		{
			Hash hash = joaat(p.get<std::string>());
			std::string name = std::string(HUD::GET_STRING_FROM_HASH_KEY(hash));

			if (!IsStringValid(name))
			{
				printf("%s: %s is invalid!\n", __FUNCTION__, p.get<std::string>().c_str());
				continue;
			}

			g_ProvisionList[name] = hash;
		}
	}

	void InitDocumentList(const std::filesystem::path& Path)
	{
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			printf("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		nlohmann::json j;
		File >> j;
		File.close();

		for (const auto& d : j)
		{
			Hash hash = joaat(d.get<std::string>());
			std::string name = std::string(HUD::GET_STRING_FROM_HASH_KEY(hash));

			if (!IsStringValid(name))
			{
				printf("%s: %s is invalid!\n", __FUNCTION__, d.get<std::string>().c_str());
				continue;
			}

			g_DocumentList[name] = hash;
		}
	}

	void InitConsumableList(const std::filesystem::path& Path)
	{
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			printf("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		nlohmann::json j;
		File >> j;
		File.close();

		for (const auto& c : j)
		{
			Hash hash = joaat(c.get<std::string>());
			std::string name = std::string(HUD::GET_STRING_FROM_HASH_KEY(hash));

			if (!IsStringValid(name))
			{
				printf("%s: %s is invalid!\n", __FUNCTION__, c.get<std::string>().c_str());
				continue;
			}

			g_ConsumableList[name] = hash;
		}
	}

	void InitPedList(const std::filesystem::path& Path)
	{
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			printf("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		nlohmann::json j;
		File >> j;
		File.close();

		for (const auto& p : j)
		{
			auto name = p.get<std::string>();
			Hash model = joaat(name);

			if (!IsModelValid(model))
			{
				printf("%s: %s is invalid!\n", __FUNCTION__, name.c_str());
				continue;
			}

			g_PedList[name] = model;
			g_PedModelNameList[model] = name;
		}
	}

	void InitWeaponList(const std::filesystem::path& Path)
	{
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			printf("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		nlohmann::json j;
		File >> j;
		File.close();

		for (const auto& w : j)
		{
			Hash model = joaat(w.get<std::string>());
			auto name = HUD::GET_STRING_FROM_HASH_KEY(model);

			if (!IsStringValid(name))
			{
				printf("%s: %s is invalid!\n", __FUNCTION__, w.get<std::string>().c_str());
				continue;
			}

			g_WeaponList[name] = model;
		}
	}

	void Create()
	{
		printf("Creating lists.\n");

		auto Path = GetSettingsFolderPath();
		if (!std::filesystem::exists(Path))
		{
			std::filesystem::create_directory(Path);
		}
		else if (!std::filesystem::is_directory(Path))
		{
			std::filesystem::remove(Path);
			std::filesystem::create_directory(Path);
		}

		InitVehicleList(Path.append("Vehicles.json"));
		InitProvisionList(Path.parent_path().append("Provisions.json"));
		InitDocumentList(Path.parent_path().append("Documents.json"));
		InitConsumableList(Path.parent_path().append("Consumables.json"));
		InitPedList(Path.parent_path().append("Peds.json"));
		InitWeaponList(Path.parent_path().append("Weapons.json"));
	}
}
