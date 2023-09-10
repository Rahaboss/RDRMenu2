#include "pch.h"
#include "Config.h"

static std::filesystem::path s_ConfigPath;
void Config::Create()
{
	LOG_TEXT("Creating configuration.");

	// Get Windows %APPDATA% environment variable (C:\Users\...\AppData\Roaming)
	char* Buffer = nullptr;
	size_t BufferCount = 0;
	_dupenv_s(&Buffer, &BufferCount, "APPDATA");
	assert(Buffer); // Fix _dupenv_s warning
	s_ConfigPath = Buffer;
	delete Buffer;

	// Change path to %APPDATA%\RDRMenu2
	s_ConfigPath.append("RDRMenu2");

	// Create folder at path if it doesn't exist
	if (!std::filesystem::exists(s_ConfigPath))
	{
		std::filesystem::create_directory(s_ConfigPath);
	}
	// If a file not a folder at that path exists, replace with folder
	else if (!std::filesystem::is_directory(s_ConfigPath))
	{
		std::filesystem::remove(s_ConfigPath);
		std::filesystem::create_directory(s_ConfigPath);
	}
}

std::filesystem::path Config::GetConfigPath()
{
	return s_ConfigPath;
}

Hash Config::HashFromJSON(const json& j)
{
	TRY
	{
		if (j.is_string())
			return rage::joaat(j.get_ref<const std::string&>());

		return j.get<Hash>();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return 0;
}
