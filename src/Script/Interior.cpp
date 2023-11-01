#include "Interior.h"
#include "Rage/natives.h"
#include "Rage/ScriptGlobal.h"

void Script::ToggleIPL(Hash IPLHash, bool Toggle)
{
	if (Toggle)
		STREAMING::REQUEST_IPL_HASH(IPLHash);
	else
		STREAMING::REMOVE_IPL_HASH(IPLHash);
}

void Script::ToggleInteriorSet(Interior InteriorID, const char* EntitySet, bool Toggle)
{
	if (Toggle)
		INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(InteriorID, EntitySet, 0);
	else
		INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(InteriorID, EntitySet, true);
}

// Forcing the game to toggle the interior sets itself, instead of using ToggleInteriorSet
void Script::ToggleInterior(int iParam0, bool Toggle)
{
	int iVar1 = iParam0 / 31;
	int iVar2 = iParam0 % 31;
	auto p = ScriptGlobal(1934765).At(21).At(iVar1, 1).Get<int*>();

	if (Toggle)
		MISC::SET_BIT(p, iVar2);
	else
		MISC::CLEAR_BIT(p, iVar2);
}

void Script::ToggleArtInterior(bool Toggle)
{
	// WS_NBX_ART_EXHIBIT_OPEN
	ToggleIPL(-205116461, Toggle);
	ToggleIPL(-1490034522, Toggle);
	ToggleIPL(-1473336090, !Toggle);
	ToggleIPL(-1013403664, !Toggle);

	ToggleInterior(18, Toggle); // new_art_photos_pre_RC_Mason
	ToggleInterior(19, !Toggle); // new_art_photos_post_RC_Mason
	ToggleInterior(154, Toggle); // new_art_exhibit_RC_Art3
	ToggleInterior(155, !Toggle); // new_art_exhibit_post_RC_Art3
	ToggleInterior(156, Toggle); // new_forMyArt_paintings
	ToggleInterior(157, Toggle); // new_forMyArt_gallery
	ToggleInterior(158, !Toggle); // st_denis_art_int_closed
	ToggleInterior(159, Toggle); // st_denis_art_int_open
	ToggleInterior(168, true); // ForMyArt_nobarprops
}

void Script::ToggleDinoBarnInterior(bool Toggle)
{
	Interior InteriorID = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
	ToggleInteriorSet(InteriorID, "din_barn_dinoSP", Toggle);
}

void Script::ToggleSaintDenisHideoutInterior(bool Toggle)
{
	// WS_NBD1_ABANDONED_BUILDING_INTERIOR
	ToggleIPL(1628286919, Toggle);
	ToggleIPL(RAGE_JOAAT("NEW_COM_07_NOHOTEL"), !Toggle);
	ToggleIPL(RAGE_JOAAT("NEW_COM_07_WIN"), !Toggle);
}

void Script::ToggleGrandKorringanInterior(bool Toggle)
{
	ToggleIPL(RAGE_JOAAT("ser_01_grandkor"), Toggle);
	ToggleIPL(RAGE_JOAAT("ser_01_grandkor2"), Toggle);
	ToggleIPL(RAGE_JOAAT("ser_01_grandkor2_lod"), Toggle);
	ToggleIPL(RAGE_JOAAT("ser_01_grandkor3"), Toggle);
	ToggleIPL(RAGE_JOAAT("ser_01_grandkor3_lod"), Toggle);
	ToggleIPL(RAGE_JOAAT("ser_01_grandkor_lod"), Toggle);

	Interior InteriorID = INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(3285.792f, -1325.603f, 43.08399f, RAGE_JOAAT("SER_GRANDKOR_L1_INT"));
	ToggleInteriorSet(InteriorID, "korrigan_props_poker", Toggle);
}

void Script::ToggleGrandKorringanDockedInterior(bool Toggle)
{
	ToggleIPL(RAGE_JOAAT("new_boats_01_grandkor"), Toggle);
	ToggleIPL(RAGE_JOAAT("new_boats_01_grandkor2"), Toggle);
	ToggleIPL(RAGE_JOAAT("new_boats_01_grandkor2_lod"), Toggle);
	ToggleIPL(RAGE_JOAAT("new_boats_01_grandkor3"), Toggle);
	ToggleIPL(RAGE_JOAAT("new_boats_01_grandkor3_lod"), Toggle);
	ToggleIPL(RAGE_JOAAT("new_boats_01_grandkor_lod"), Toggle);

	Interior InteriorID = INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(2880.0f, -1416.0f, 45.3f, RAGE_JOAAT("SER_GRANDKOR_L1_INT"));
	ToggleInteriorSet(InteriorID, "korrigan_props_default", Toggle);
}

void Script::ToggleWinter1Interior(bool Toggle)
{
	ToggleIPL(RAGE_JOAAT("win1_intro"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_placement"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_cutscene"), Toggle);
	ToggleIPL(RAGE_JOAAT("winter1_cabin_fire"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_trees_1"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_trees_0"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_slod_0"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_placement_strm_0"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_grass_1"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_grass_0"), Toggle);
	ToggleIPL(RAGE_JOAAT("win1_intro_cutscene_lod"), Toggle);
	ToggleIPL(RAGE_JOAAT("win_01_"), Toggle);
	ToggleIPL(RAGE_JOAAT("win_snow"), Toggle);

	STREAMING::SET_MAPDATACULLBOX_ENABLED("Win_Intro", Toggle);
	STREAMING::SET_MAPDATACULLBOX_ENABLED("Main_World", !Toggle);
}
