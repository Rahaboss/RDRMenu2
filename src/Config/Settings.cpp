#include "pch.h"
#include "Settings.h"
#include "Config.h"

static json s_DefaultSettings{
R"({
	"disable_pinkerton_patrols": false,
	"enable_dlcs": false,
	"infinite_ammo": false
})"_json
};

static void WriteDefaultSettings(const std::filesystem::path& FilePath)
{
	std::fstream File(FilePath, std::fstream::out | std::fstream::trunc);
	assert(File.good());
	File << s_DefaultSettings.dump(1, '\t');
	File.close();

	g_Settings.clear();
	g_Settings = s_DefaultSettings;
}

static void Save(const std::filesystem::path& FilePath)
{
	std::fstream File(FilePath, std::fstream::out | std::fstream::trunc);
	assert(File.good());
	File << g_Settings.dump(1, '\t');
	File.close();
}

void Settings::Create()
{
	std::filesystem::path Path(Config::GetConfigPath().append("Settings.json"));
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
		if (!g_Settings.contains(e.key()))
		{
			UpdateFile = true;
			g_Settings[e.key()] = e.value();
		}
	}

	if (UpdateFile)
	{
		LOG_TEXT("Updating settings.\n");
		Save(Path);
	}
}

void Settings::Destroy()
{
	std::filesystem::path Path(Config::GetConfigPath().append("Settings.json"));
	Save(Path);
}
