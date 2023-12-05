#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/joaat.h"

namespace Script
{
	// Top left
	void NotifyHelp(const char* Message);
	void NotifyHelpFormat(_Printf_format_string_ const char* const Text, ...);

	// Lower-middle right
	void NotifyFeed(const char* Message);

	// Bottom middle
	void NotifyObjective(const char* Message);

	// Top middle
	void NotifyLocation(const char* Message);

	// Upper middle
	void NotifyToast(const char* Message, const char* Description);

	// Fullscreen
	void NotifyError(const char* Message, const char* Description);
	void NotifyDeath(const char* Message);
	void NotifyWarning(const char* Message, const char* Description);
}
