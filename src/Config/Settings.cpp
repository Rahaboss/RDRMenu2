#include "pch.h"
#include "Settings.h"
#include "Config.h"

static json s_DefaultSettings{
R"({
	"disable_black_borders": false,
	"disable_hud": false,
	"disable_invisible_snipers": false,
	"disable_pinkerton_patrols": false,
	"enable_dlcs": false,
	"esp": {
		"object": {
			"enable": false,
			"model": false
		},
		"ped": {
			"enable": false,
			"model": false
		},
		"pickup": {
			"enable": false,
			"model": false
		},
		"player": {
			"bone": false,
			"enable": false,
			"model": false
		},
		"vehicle": {
			"enable": false,
			"model": false
		}
	},
	"mount": {
		"clean": false,
		"godmode": false,
		"gold_cores": false,
		"no_ragdoll": false,
		"no_sliding": false,
		"super_run": false
	},
	"never_wanted": false,
	"player": {
		"clean": false,
		"godmode": false,
		"gold_cores": false,
		"no_ragdoll": false,
		"no_sliding": false,
		"super_jump": false,
		"super_run": false
	},
	"render_imgui_demo": false,
	"weapon": {
		"infinite_ammo": false,
		"rapid_fire": false,
		"rgb_electric_lantern": false
	}
})"_json
};

static void WriteDefaultSettings(const std::filesystem::path& FilePath)
{
	std::fstream File(FilePath, std::fstream::out | std::fstream::trunc);
	assert(File.good());
	File << s_DefaultSettings.dump(1, '\t') << std::endl;
	File.close();

	g_Settings.clear();
	g_Settings = s_DefaultSettings;
}

static void Save(const std::filesystem::path& FilePath)
{
	std::fstream File(FilePath, std::fstream::out | std::fstream::trunc);
	assert(File.good());
	File << g_Settings.dump(1, '\t') << std::endl;
	File.close();
}

static void AddMissingItemsInObject(json& Object, const json& DefaultObject, bool& HasChanged)
{
	for (const auto& e : DefaultObject.items())
	{
		// Set to default if missing or type doesn't match in loaded settings
		if (!Object.contains(e.key()) || e.value().type() != Object[e.key()].type())
		{
			HasChanged = true;
			Object[e.key()] = e.value();
		}
		// Recurse if type is object
		else if (e.value().is_object())
			AddMissingItemsInObject(Object[e.key()], DefaultObject[e.key()], HasChanged);
	}
}

void Settings::Create()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Settings.json") };
	std::ifstream File{ Path };

	if (!File)
	{
		WriteDefaultSettings(Path);
		File.open(Path, std::fstream::in);
	}

	assert(File.good());
	File >> g_Settings;

	bool UpdateFile = false;
	AddMissingItemsInObject(g_Settings, s_DefaultSettings, UpdateFile);

	if (UpdateFile)
	{
		LOG_TEXT("Updating settings.");
		Save(Path);
	}
}

void Settings::Destroy()
{
	std::filesystem::path Path{ Config::GetConfigPath().append("Settings.json") };
	Save(Path);
}
