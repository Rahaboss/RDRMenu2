#include "pch.h"
#include "Cutscene.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Rage/Natives.h"
#include "JobQueue.h"
#include "CutsceneHelper.h"

namespace Features
{
	void PlayDinoLadyCutscene()
	{
		QUEUE_JOB(=)
		{
			[]() {
				Interior stage = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
				INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", 0);

				CutsceneHelper Cutscene("cutscene@rcdin_rsc2");
				Cutscene.PlayAutomatically();

				INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", true);
			}();
		}
		END_JOB()
	}

	void PlayFishCollectorCutscene()
	{
		QUEUE_JOB(=)
		{
			[]() {
				CutsceneHelper Cutscene1("cutscene@rcfsh_rsc2");
				Cutscene1.AddPeds();
				Cutscene1.AddObjects();
				Cutscene1.LoadCutscene();
				Cutscene1.TeleportToOrigin();

				Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(
					ENTITY::_GET_ENTITY_BY_DOORHASH(-1056329818, 0));
				if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
					ENTITY::SET_ENTITY_VISIBLE(rdoor1, false);

				Cutscene1.PlayCutscene();
				Cutscene1.WaitForCutsceneEnd();
				Cutscene1.CleanupCutscene();

				if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
					ENTITY::SET_ENTITY_VISIBLE(rdoor1, true);
			}();
		}
		END_JOB()
	}

	void PlayIndustryCutscene()
	{
		QUEUE_JOB(=)
		{
			PlayCutsceneFromID("cutscene@ind1_mcs_1");
		}
		END_JOB()
	}

	void PlayAnnesburgBreakoutCutscene()
	{
		QUEUE_JOB(=)
		{
			[]() {
				constexpr Hash tags[]{
					RAGE_JOAAT("GUNBELTS"),
					RAGE_JOAAT("HATS"),
					RAGE_JOAAT("HOLSTERS_KNIFE"),
					RAGE_JOAAT("HOLSTERS_LEFT"),
					RAGE_JOAAT("HOLSTERS_RIGHT"),
					RAGE_JOAAT("SATCHELS"),
					RAGE_JOAAT("SATCHEL_STRAPS")
				};

				CutsceneHelper Cutscene("cutscene@jbo6_ext");

				if (IsArthurModel())
				{
					for (const auto& t : tags)
						PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, t, 1);
					PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
				}
				else
				{
					Ped arthur = SpawnPed(RAGE_JOAAT("PLAYER_ZERO"));
					PED::_EQUIP_META_PED_OUTFIT_PRESET(arthur, 0, false);
					for (const auto& t : tags)
						PED::REMOVE_TAG_FROM_META_PED(arthur, t, 1);
					PED::_UPDATE_PED_VARIATION(arthur, false, true, true, true, true);
					Cutscene.AddPedExisting(arthur, "ARTHUR");
				}

				Cutscene.PlayAutomatically();
			}();
		}
		END_JOB()
	}

	void PlayCharlesLeavingCutscene()
	{
		QUEUE_JOB(=)
		{
			PlayCutsceneFromID("cutscene@ntv0_int");
		}
		END_JOB()
	}

	void PlayJackCutscene()
	{
		QUEUE_JOB(=)
		{
			PlayCutsceneFromID("cutscene@rjck1_rsc5");
		}
		END_JOB()
	}

	void PlayDebtCollectorCutscene()
	{
		QUEUE_JOB(=)
		{
			PlayCutsceneFromID("cutscene@rdown_rsc_2");
		}
		END_JOB()
	}

	void PlayBeechersHopeCutscene()
	{
		QUEUE_JOB(=)
		{
			PlayCutsceneFromID("cutscene@rbch5_rsc4");
		}
		END_JOB()
	}

	void PlayBareKnuckleFriendshipsCutscene()
	{
		QUEUE_JOB(=)
		{
			[]() {
				CutsceneHelper Cutscene("cutscene@mar6_rsc2");
				const Ped unc = SpawnPed(RAGE_JOAAT("CS_UNCLE"));
				WEAPON::REMOVE_ALL_PED_WEAPONS(unc, TRUE, TRUE);
				PED::_EQUIP_META_PED_OUTFIT_PRESET(unc, 5, FALSE);
				Cutscene.AddPedExisting(unc, "Uncle");
				Cutscene.PlayAutomatically();
			}();
		}
		END_JOB()
	}

	void PlayANewFutureImaginedCutscene()
	{
		QUEUE_JOB(=)
		{
			PlayCutsceneFromID("cutscene@rabi3_rsc2");
		}
		END_JOB()
	}

	void PlayEndingStrawberryCutscene()
	{
		QUEUE_JOB(=)
		{
			PlayCutsceneFromID("cutscene@fin2_ext_p7");
		}
		END_JOB()
	}

	void PlayCutsceneFromJson(const nlohmann::json& JsonObject)
	{
		CutsceneHelper Cutscene(JsonObject);
		Cutscene.PlayAutomatically();
	}
	
	void PlayCutsceneFromID(const char* animDict)
	{
		CutsceneHelper Cutscene(animDict);
		Cutscene.PlayAutomatically();
	}
}
