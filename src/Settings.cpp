#include "pch.h"
#include "Settings.h"
#include "Features.h"

namespace Settings
{
	static json s_DefaultSettings{
	R"({
		"clean_player": false,
		"disable_default_cutscenes": false,
		"disable_west_elizabeth_pinkertons": false,
		"enable_imgui_demo": false,
		"enable_overlay": false,
		"esp": {
			"human_bone": false,
			"human_box": false,
			"human_health": false,
			"human_ignore_dead": false,
			"human_model": false,
			"object_model": false,
			"ped_health": false,
			"ped_ignore_dead": false,
			"ped_model": false,
			"pickup_model": false,
			"player_bone": false,
			"player_bone_debug": false,
			"player_box": false,
			"player_health": false,
			"player_model": false,
			"vehicle_model": false
		},
		"freeze_player": false,
		"god_mode": false,
		"gold_cores": false,
		"infinite_ammo": false,
		"logging": {
			"added_cutscene_entity": false,
			"added_inventory_item": false,
			"created_cutscene": false,
			"set_decor": false,
			"spawned_human": false,
			"spawned_object": false,
			"spawned_ped": false,
			"spawned_vehicle": false
		},
		"mount_god_mode": false,
		"mount_gold_cores": false,
		"never_wanted": false,
		"no_black_borders": false,
		"no_ragdoll": false,
		"no_sliding": false,
		"no_snipers": false,
		"rapid_fire": false,
		"render_clothing_menu": false,
		"render_model_debug_gun": false,
		"rgb_elec_lantern": false,
		"super_jump": false,
		"super_run": false
	})"_json
	};

	void WriteDefaultSettings(const std::filesystem::path& FilePath)
	{
		std::fstream File(FilePath, std::fstream::out | std::fstream::trunc);
		assert(File.good());
		File << s_DefaultSettings.dump(4);
		File.close();

		g_Settings.clear();
		g_Settings = s_DefaultSettings;
	}

	void Save(const std::filesystem::path& FilePath)
	{
		std::fstream File(FilePath, std::fstream::out | std::fstream::trunc);
		assert(File.good());
		File << g_Settings.dump(4);
		File.close();
	}

	void Create()
	{
		std::filesystem::path Path(Features::GetConfigPath());

		if (!std::filesystem::exists(Path))
		{
			std::filesystem::create_directory(Path);
		}
		else if (!std::filesystem::is_directory(Path))
		{
			std::filesystem::remove(Path);
			std::filesystem::create_directory(Path);
		}

		Path.append("Settings.json");

		std::fstream File(Path, std::fstream::in);

		if (!File.good())
		{
			WriteDefaultSettings(Path);
			File.open(Path, std::fstream::in);
		}

		assert(File.good());
		File >> g_Settings;
		File.close();

		if (g_Settings.size() != s_DefaultSettings.size())
		{
			bool UpdateFile = false;
			for (const auto& e : s_DefaultSettings.items())
			{
				if (!g_Settings.contains(e.key()))
				{
					UpdateFile = true;
					g_Settings[e.key()] = e.value();
				}
			}

			if (UpdateFile)
			{
				LOG_TO_CONSOLE("Updating settings.\n");
				Save(Path);
			}
		}
	}

	void Destroy()
	{
		std::filesystem::path Path(Features::GetConfigPath());
		Path.append("Settings.json");
		Save(Path);
	}
}
