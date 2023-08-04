#include "pch.h"
#include "Lists.h"
#include "Features.h"
#include "CutsceneHelper.h"

namespace Lists
{
	void InitConsumableList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		json j;
		File >> j;
		File.close();

		for (const auto& c : j)
		{
			Hash hash = rage::joaat(c.get<std::string>());
			std::string name = std::string(HUD::GET_STRING_FROM_HASH_KEY(hash));

			if (!Features::IsStringValid(name))
			{
				LOG_TO_CONSOLE("%s: %s is invalid!\n", __FUNCTION__, c.get<std::string>().c_str());
				continue;
			}

			g_ConsumableList[name] = hash;
		}
	}

	void InitCutscenesList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		File >> g_Cutscenes;
		File.close();
	}

	void InitDocumentList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		json j;
		File >> j;
		File.close();

		for (const auto& d : j)
		{
			Hash hash = rage::joaat(d.get<std::string>());
			std::string name = std::string(HUD::GET_STRING_FROM_HASH_KEY(hash));

			if (!Features::IsStringValid(name))
			{
				LOG_TO_CONSOLE("%s: %s is invalid!\n", __FUNCTION__, d.get<std::string>().c_str());
				continue;
			}

			g_DocumentList[name] = hash;
		}
	}

	void InitPedList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		json j;
		File >> j;
		File.close();

		for (const auto& p : j)
		{
			auto name = p.get<std::string>();
			Hash model = rage::joaat(name);

			if (!Features::IsModelValid(model) && STREAMING::IS_MODEL_A_PED(model))
			{
				LOG_TO_CONSOLE("%s: %s is invalid!\n", __FUNCTION__, name.c_str());
				continue;
			}

			g_PedList[name] = model;
			g_PedModelNameList[model] = name;
		}
	}

	void InitProvisionList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}
		
		json j;
		File >> j;
		File.close();
		
		for (const auto& p : j)
		{
			Hash hash = rage::joaat(p.get<std::string>());
			std::string name = std::string(HUD::GET_STRING_FROM_HASH_KEY(hash));

			if (!Features::IsStringValid(name))
			{
				LOG_TO_CONSOLE("%s: %s is invalid!\n", __FUNCTION__, p.get<std::string>().c_str());
				continue;
			}

			g_ProvisionList[name] = hash;
		}
	}

	void InitWeaponList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		json j;
		File >> j;
		File.close();

		for (const auto& w : j)
		{
			Hash model = rage::joaat(w.get<std::string>());
			auto name = HUD::GET_STRING_FROM_HASH_KEY(model);

			if (!Features::IsStringValid(name))
			{
				LOG_TO_CONSOLE("%s: %s is invalid!\n", __FUNCTION__, w.get<std::string>().c_str());
				continue;
			}

			g_WeaponList[name] = model;
		}
	}

	void InitVehicleList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		json j;
		File >> j;
		File.close();

		for (const auto& v : j)
		{
			auto name = v.get<std::string>();
			Hash model = rage::joaat(name);

			if (!Features::IsModelValid(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
			{
				LOG_TO_CONSOLE("%s: %s is invalid!\n", __FUNCTION__, name.c_str());
				continue;
			}

			g_VehicleList[name] = model;
			g_VehicleModelNameList[model] = name;
		}
	}

	void InitObjectList(const std::string& FileName)
	{
		std::filesystem::path Path(Features::GetConfigPath().append(FileName));
		std::fstream File(Path, std::fstream::in);
		if (!File.good())
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", Path.filename().string().c_str());
			return;
		}

		json j;
		File >> j;
		File.close();

		for (const auto& o : j)
		{
			auto name = o.get<std::string>();
			Hash model = rage::joaat(name);

			if (!Features::IsModelValid(model) && STREAMING::_IS_MODEL_AN_OBJECT(model))
			{
				LOG_TO_CONSOLE("%s: %s is invalid!\n", __FUNCTION__, name.c_str());
				continue;
			}

			g_ObjectList[name] = model;
			g_ObjectModelNameList[model] = name;
		}
	}

	void InitScenarioList(const std::string& FileName)
	{
		std::filesystem::path FilePath(Features::GetConfigPath().append(FileName));
		std::ifstream File(FilePath);

		if (!File)
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", FilePath.filename().string().c_str());
			return;
		}

		json ScenarioList;
		File >> ScenarioList;
		File.close();

		for (const json& Scenario : ScenarioList)
		{
			const std::string& Name = Scenario.get_ref<const std::string&>();
			Hash Model = rage::joaat(Name);

			g_ScenarioList[Name] = Model;
		}
	}
	
	void InitMetaPedOutfitList(const std::string& FileName)
	{
		std::filesystem::path FilePath(Features::GetConfigPath().append(FileName));
		std::ifstream File(FilePath);

		if (!File)
		{
			LOG_TO_CONSOLE("Can't find file: %s.\n", FilePath.filename().string().c_str());
			return;
		}

		File >> g_MetaPedOutfits;
		File.close();
	}

	void Create()
	{
		LOG_TO_CONSOLE("Creating lists.\n");

		InitConsumableList("Consumables.json");
		InitCutscenesList("Cutscenes.json");
		InitDocumentList("Documents.json");
		InitPedList("Peds.json");
		InitProvisionList("Provisions.json");
		InitWeaponList("Weapons.json");
		InitVehicleList("Vehicles.json");
		InitObjectList("Objects.json");
		InitScenarioList("Scenarios.json");
		InitMetaPedOutfitList("MetaPedOutfits.json");
	}

	void Reload()
	{
		LOG_TO_CONSOLE("Clearing lists.\n");

		g_ObjectList.clear();
		g_ObjectModelNameList.clear();
		g_VehicleList.clear();
		g_VehicleModelNameList.clear();
		g_ProvisionList.clear();
		g_DocumentList.clear();
		g_ConsumableList.clear();
		g_PedList.clear();
		g_PedModelNameList.clear();
		g_WeaponList.clear();
		g_Cutscenes.clear();
		g_ScenarioList.clear();
		g_MetaPedOutfits.clear();
		Create();
	}
	
	void ReloadCutscenesList()
	{
		LOG_TO_CONSOLE("Reloading cutscenes.\n");

		g_Cutscenes.clear();
		InitCutscenesList("Cutscenes.json");
	}
}
