#include "pch.h"
#include "Settings.h"
#include "Config.h"

static json s_DefaultSettings{
R"({
	"add_cutscene_info_automatically": false,
	"disable_black_borders": false,
	"disable_hud": false,
	"disable_invisible_snipers": false,
	"disable_pinkerton_patrols": false,
	"enable_dlcs": false,
	"esp": {
		"animal": {
			"enable": false,
			"ignore_dead": false,
			"model": false
		},
		"object": {
			"enable": false,
			"model": false
		},
		"ped": {
			"bone": false,
			"enable": false,
			"ignore_dead": false,
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
	"log_animscene": false,
	"mount": {
		"clean": false,
		"godmode": false,
		"gold_cores": false,
		"invisible": false,
		"no_ragdoll": false,
		"no_sliding": false,
		"super_run": false
	},
	"never_wanted": false,
	"player": {
		"clean": false,
		"godmode": false,
		"gold_cores": false,
		"invisible": false,
		"no_ragdoll": false,
		"no_sliding": false,
		"super_jump": false,
		"super_run": false
	},
	"render_imgui_demo": false,
	"render_overlay": false,
	"teleport_to_cutscene": true,
	"weapon": {
		"infinite_ammo": false,
		"rapid_fire": false,
		"rgb_electric_lantern": false
	}
})"_json
};

static void Save(const std::filesystem::path& FilePath)
{
	std::ofstream File{ FilePath };

	if (File)
		File << g_Settings.dump(1, '\t') << std::endl;
}

static void WriteDefaultSettings(const std::filesystem::path& FilePath)
{
	g_Settings = s_DefaultSettings;
	Save(FilePath);
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
