#include "pch.h"
#include "Cutscene.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Rage/enums.h"
#include "Rage/natives.h"
#include "JobQueue.h"
#include "CutsceneHelper.h"

namespace Features
{
	void PlayDinoLadyCutscene()
	{
		Interior stage = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
		INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", 0);

		CutsceneHelper Cutscene("cutscene@rcdin_rsc2");
		Cutscene.PlayAutomatically();

		INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", true);
	}

	void PlayFishCollectorCutscene()
	{
		// First cutscene
		CutsceneHelper Cutscene1("cutscene@rcfsh_rsc2");
		Cutscene1.AddPeds();
		Cutscene1.AddObjects();
		Cutscene1.LoadCutscene();
		Cutscene1.TeleportToOrigin();

		Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1056329818, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
			ENTITY::SET_ENTITY_VISIBLE(rdoor1, false);

		Cutscene1.PlayCutscene();
		Cutscene1.WaitForCutsceneEnd();
		Cutscene1.CleanupCutscene();

		if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
			ENTITY::SET_ENTITY_VISIBLE(rdoor1, true);
	}

	void PlayIndustryCutscene()
	{
		CutsceneHelper Cutscene("cutscene@ind1_mcs_1");
		Cutscene.PlayAutomatically();
	}

	void PlayAnnesburgBreakoutCutscene()
	{
		constexpr Hash tags[]{
			joaat("GUNBELTS"),
			joaat("HATS"),
			joaat("HOLSTERS_KNIFE"),
			joaat("HOLSTERS_LEFT"),
			joaat("HOLSTERS_RIGHT"),
			joaat("SATCHELS"),
			joaat("SATCHEL_STRAPS")
		};

		CutsceneHelper Cutscene("cutscene@jbo6_ext");
		if (IsArthurModel())
		{
			for (const auto& t : tags)
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, t, 1);
			PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			Cutscene.AddLocalPlayer();
		}
		else
		{
			Ped arthur = SpawnPed(PLAYER_ZERO);
			PED::_EQUIP_META_PED_OUTFIT_PRESET(arthur, 0, false);
			for (const auto& t : tags)
				PED::REMOVE_TAG_FROM_META_PED(arthur, t, 1);
			PED::_UPDATE_PED_VARIATION(arthur, false, true, true, true, true);
			Cutscene.AddPedExisting(arthur, "ARTHUR");
		}
		Cutscene.PlayAutomatically();
	}

	void PlayCharlesLeavingCutscene()
	{
		CutsceneHelper Cutscene("cutscene@ntv0_int");
		Cutscene.PlayAutomatically();
	}

	void PlayJackCutscene()
	{
		CutsceneHelper Cutscene("cutscene@rjck1_rsc5");
		Cutscene.PlayAutomatically();
	}

	void PlayDebtCollectorCutscene()
	{
		CutsceneHelper Cutscene("cutscene@rdown_rsc_2");
		Cutscene.PlayAutomatically();
	}

	void PlayBeechersHopeCutscene()
	{
		CutsceneHelper Cutscene("cutscene@rbch5_rsc4");
		Cutscene.PlayAutomatically();
	}

	void PlayCutsceneFromJson(const nlohmann::json& JsonObject)
	{
		QUEUE_JOB(&)
		{
			[&]() {
				CutsceneHelper Cutscene(JsonObject);
				Cutscene.PlayAutomatically();
			}();
		}
		END_JOB()
	}
}
