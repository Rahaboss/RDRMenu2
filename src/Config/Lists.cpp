#include "pch.h"
#include "Lists.h"
#include "Config.h"
#include "Script/Spawning.h"
#include "Rage/natives.h"

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

		if (!Script::IsModelValid(Model) || !STREAMING::_IS_MODEL_AN_OBJECT(Model))
		{
			LOG_TEXT("%s: %s is invalid!\n", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::ObjectList[Name] = Model;
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

		if (!Script::IsModelValid(Model) || !STREAMING::IS_MODEL_A_PED(Model))
		{
			LOG_TEXT("%s: %s is invalid!\n", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::PedList[Name] = Model;
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

		Lists::WeaponList[Name] = Model;
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

		if (!Script::IsModelValid(Model) || !STREAMING::IS_MODEL_A_VEHICLE(Model))
		{
			LOG_TEXT("%s: %s is invalid!\n", __FUNCTION__, Name.c_str());
			continue;
		}

		Lists::VehicleList[Name] = Model;
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
