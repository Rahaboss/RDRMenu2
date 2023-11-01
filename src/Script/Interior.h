#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void ToggleIPL(Hash IPLHash, bool Toggle);
	void ToggleInteriorSet(Interior InteriorID, const char* EntitySet, bool Toggle);
	void ToggleInterior(int iParam0, bool Toggle);
	void ToggleArtInterior(bool Toggle);
	void ToggleDinoBarnInterior(bool Toggle);
	void ToggleSaintDenisHideoutInterior(bool Toggle);
	void ToggleGrandKorringanInterior(bool Toggle);
	void ToggleGrandKorringanDockedInterior(bool Toggle);
	void ToggleWinter1Interior(bool Toggle);
}
