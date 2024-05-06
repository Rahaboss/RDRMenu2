#include "pch.h"
#include "Lists.h"
#include "Config.h"
#include "Script/Spawning.h"
#include "Rage/natives.h"
#include "Rage/joaat.h"

static void AddNameToHashList(Hash Model, const std::string& Name)
{
	if (const auto it = Lists::HashNameList.find(Model); it != Lists::HashNameList.cend())
		LOG_TEXT(__FUNCTION__": Hash collision between \"%s\" and \"%s\" (0x%X)!", it->second.c_str(), Name.c_str(), Model);

	Lists::HashNameList[Model] = Name;
}

static void InitAmmoList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Ammo.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& a : j)
	{
		const std::string& Name = a.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::AmmoList[Script::GetStringFromHashKey(Model)] = Model;
		AddNameToHashList(Model, Name);
	}
}

static void InitObjectList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Objects.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& o : j)
	{
		const std::string& Name = o.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		// Don't perform any checks as some objects like water vfx don't pass for some reason

		Lists::ObjectList[Name] = Model;
		AddNameToHashList(Model, Name);
	}

	Lists::ObjectListRaw.reserve(Lists::ObjectList.size());
	for (const auto& [Name, Model] : Lists::ObjectList)
		Lists::ObjectListRaw.push_back(Model);
}

static void InitPedList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Peds.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& p : j)
	{
		const std::string& Name = p.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		if (!Script::IsModelValid(Model))
		{
			LOG_TEXT("%s: %s is invalid!", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::PedList[Name] = Model;
		AddNameToHashList(Model, Name);
	}

	Lists::PedListRaw.reserve(Lists::PedList.size());
	for (const auto& [Name, Model] : Lists::PedList)
		Lists::PedListRaw.push_back(Model);
}

static void InitPickupList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Pickups.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& p : j)
	{
		const std::string& Name = p.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::PickupList[Name] = Model;
		AddNameToHashList(Model, Name);
	}

	Lists::PickupListRaw.reserve(Lists::PickupList.size());
	for (const auto& [Name, Model] : Lists::PickupList)
		Lists::PickupListRaw.push_back(Model);
}

static void InitWeaponList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Weapons.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& w : j)
	{
		const std::string& Name = w.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::WeaponList[Script::GetStringFromHashKey(Model)] = Model;
		AddNameToHashList(Model, Name);
	}
}

static void InitVehicleList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Vehicles.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& v : j)
	{
		const std::string& Name = v.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		if (!Script::IsModelValid(Model))
		{
			LOG_TEXT("%s: %s is invalid!", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::VehicleList[Name] = Model;
		AddNameToHashList(Model, Name);
	}

	Lists::VehicleListRaw.reserve(Lists::VehicleList.size());
	for (const auto& [Name, Model] : Lists::VehicleList)
		Lists::VehicleListRaw.push_back(Model);
}

void Lists::InitCutsceneList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Cutscenes.json") };
	std::ifstream File{ Path };

	if (File)
		File >> CutsceneList;
	else
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
}

static void InitOutfitList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("MetaPedOutfits.json") };
	std::ifstream File{ Path };

	if (File)
		File >> Lists::MetaPedOutfits;
	else
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
}

static void InitInventoryList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Inventory.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& d : j)
	{
		const std::string& Name = d.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::InventoryList[Script::GetStringFromHashKey(Model)] = Model;
		AddNameToHashList(Model, Name);
	}
}

static void InitScenarioList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Scenarios.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const json& s : j)
	{
		const std::string& Name = s.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::ScenarioList[Name] = Model;
		AddNameToHashList(Model, Name);
	}
}

void Lists::Create()
{
	LOG_TEXT("Creating lists.");
	InitAmmoList();
	InitObjectList();
	InitPedList();
	InitPickupList();
	InitWeaponList();
	InitVehicleList();
	InitCutsceneList();
	InitOutfitList();
	InitInventoryList();
	InitScenarioList();
}

void Lists::Destroy()
{
	LOG_TEXT("Destroying lists.");
	AmmoList.clear();
	ObjectList.clear();
	PedList.clear();
	PickupList.clear();
	WeaponList.clear();
	VehicleList.clear();
	CutsceneList.clear();
	MetaPedOutfits.clear();
	//ConsumableList.clear();
	//DocumentList.clear();
	//ProvisionList.clear();
	InventoryList.clear();
	
	HashNameList.clear();
}

const std::string& Lists::GetHashName(Hash h)
{
	if (const auto it = HashNameList.find(h); it != HashNameList.cend())
		return it->second;

	// Don't return temporary variable as reference
	static const std::string s{};
	return s;
}

std::string Lists::GetHashNameOrUint(Hash h)
{
	if (const auto it = HashNameList.find(h); it != HashNameList.cend())
		return it->second;

	return std::to_string(h);
}

Hash Lists::GetHashFromJSON(const json& Object)
{
	if (Object.is_string())
		return rage::joaat(Object.get_ref<const std::string&>());
	if (Object.is_number())
		return Object.get<Hash>();

	return 0;
}

json::iterator Lists::GetCutscene(const std::string& ID)
{
	json::iterator it = Lists::CutsceneList.begin();

	while (it < Lists::CutsceneList.end())
	{
		const json& Cutscene = *it;
		
		if (Cutscene["id"].get_ref<const std::string&>() == ID)
			break;

		it++;
	}

	return it;
}
