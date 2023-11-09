#include "pch.h"
#include "Lists.h"
#include "Config.h"
#include "Script/Spawning.h"
#include "Rage/natives.h"
#include "Memory/Pointers.h"
#include "Rage/joaat.h"

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

	for (const auto& a : j)
	{
		const std::string& Name = a.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::AmmoList[Pointers::GetStringFromHashKey(Model)] = Model;
		Lists::HashNameList[Model] = Pointers::GetStringFromHashKey(Model);
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

	for (const auto& o : j)
	{
		const std::string& Name = o.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		// Don't perform any checks as some objects like water vfx don't pass for some reason

		Lists::ObjectList[Name] = Model;
		Lists::HashNameList[Model] = Name;
	}
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

	for (const auto& p : j)
	{
		const std::string& Name = p.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		if (!Script::IsModelValid(Model))
		{
			LOG_TEXT("%s: %s is invalid!", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::PedList[Name] = Model;
		Lists::HashNameList[Model] = Name;
	}
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

	for (const auto& p : j)
	{
		const std::string& Name = p.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::PickupList[Name] = Model;
		Lists::HashNameList[Model] = Name;
	}
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

	for (const auto& w : j)
	{
		const std::string& Name = w.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::WeaponList[Pointers::GetStringFromHashKey(Model)] = Model;
		Lists::HashNameList[Model] = Name;
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

	for (const auto& v : j)
	{
		const std::string& Name = v.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		if (!Script::IsModelValid(Model))
		{
			LOG_TEXT("%s: %s is invalid!", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::VehicleList[Name] = Model;
		Lists::HashNameList[Model] = Name;
	}
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

static void InitConsumableList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Consumables.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const auto& c : j)
	{
		const std::string& Name = c.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::ConsumableList[Pointers::GetStringFromHashKey(Model)] = Model;
		Lists::HashNameList[Model] = Name;
	}
}

static void InitDocumentList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Documents.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const auto& d : j)
	{
		const std::string& Name = d.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::DocumentList[Pointers::GetStringFromHashKey(Model)] = Model;
		Lists::HashNameList[Model] = Name;
	}
}

static void InitProvisionList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Provisions.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const auto& p : j)
	{
		const std::string& Name = p.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

		Lists::ProvisionList[Pointers::GetStringFromHashKey(Model)] = Model;
		Lists::HashNameList[Model] = Name;
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
	InitConsumableList();
	InitDocumentList();
	InitProvisionList();
}

const std::string& Lists::GetHashName(Hash h)
{
	if (const auto it = HashNameList.find(h); it != HashNameList.end())
		return it->second;

	// Don't return temporary variable as reference
	static const std::string s{};
	return s;
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
