#include "pch.h"
#include "Settings.h"

namespace Settings
{
	static nlohmann::json s_DefaultSettings =
		R"({
			"enable_imgui_demo": false,
			"god_mode": false,
			"gold_cores": false,
			"infinite_ammo": false,
			"log_added_inventory_items": false,
			"log_human_spawning": false,
			"log_ped_spawning": false,
			"log_vehicle_spawning": false,
			"mount_god_mode": false,
			"mount_gold_cores": false,
			"never_wanted": false,
			"no_black_borders": false,
			"no_sliding": false,
			"no_snipers": false,
			"super_jump": false,
			"rgb_elec_lantern": false,
			"enable_overlay": false,
			"clean_player": false,
			"log_created_cutscenes": false
		})"_json;

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
		std::filesystem::path Path(GetSettingsFolderPath());

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

		bool UpdateFile = false;
		for (const auto& e : s_DefaultSettings.items())
		{
			if (g_Settings.count(e.key()) == 0)
			{
				UpdateFile = true;
				g_Settings[e.key()] = e.value();
			}
		}

		if (UpdateFile)
		{
			std::cout << "Updating settings.\n";
			Save(Path);
		}
	}

	void Destroy()
	{
		std::filesystem::path Path(GetSettingsFolderPath());
		Path.append("Settings.json");
		Save(Path);
	}
}