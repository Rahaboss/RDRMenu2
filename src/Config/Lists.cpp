#include "pch.h"
#include "Lists.h"
#include "Config.h"
#include "Script/Spawning.h"
#include "Rage/natives.h"
#include "Memory/Pointers.h"

static void InitObjectList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Objects.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.\n", Path.string().c_str());
		return;
	}

	json j;
	File >> j;

	for (const auto& o : j)
	{
		const std::string& Name = o.get_ref<const std::string&>();
		Hash Model = rage::joaat(Name);

#if 0
		if (!Script::IsModelValid(Model))
		{
			LOG_TEXT("%s: %s is invalid!\n", __FUNCTION__, Name.c_str());
			continue;
		}
#endif

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
		LOG_TEXT("Can't open file: %s.\n", Path.string().c_str());
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
			LOG_TEXT("%s: %s is invalid!\n", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::PedList[Name] = Model;
		Lists::HashNameList[Model] = Name;
	}
}

static void InitWeaponList()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Weapons.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		LOG_TEXT("Can't open file: %s.\n", Path.string().c_str());
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
		LOG_TEXT("Can't open file: %s.\n", Path.string().c_str());
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
			LOG_TEXT("%s: %s is invalid!\n", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::VehicleList[Name] = Model;
		Lists::HashNameList[Model] = Name;
	}
}

void Lists::Create()
{
	LOG_TEXT("Creating lists.\n");
	InitObjectList();
	InitPedList();
	InitWeaponList();
	InitVehicleList();
}

std::string Lists::GetHashName(Hash h)
{
	//const char* str = Pointers::GetStringFromHashKey(h);
	//if (str[0])
	//	return std::string{ str };

	const auto it = HashNameList.find(h);
	if (it != HashNameList.end())
		return it->second;

	return std::string{};
}