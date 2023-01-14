#include "pch.h"
#include "Cutscene.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "rage/enums.h"
#include "rage/natives.h"

namespace Features
{
	void PlayDinoLadyCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@RCDIN_RSC2", 0, 0, false, true);

		Features::Teleport(208.423f, 1000.71f, 190.036f);

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;

		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_playerArthur", b_playerArthur, false);

		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, (b_playerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);

		Ped dinoLady = Features::SpawnPed(CS_DINOBONESLADY);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_DINOBONESLADY", dinoLady, 0);

		Features::RequestModel(joaat("P_STICK04X"));
		Object stick = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("P_STICK04X"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_stick04x", stick, 0);

		Features::RequestModel(joaat("w_melee_knife03"));
		Object knife = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("w_melee_knife03"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "w_melee_knife03", knife, 0);

		Features::RequestModel(joaat("p_eme_barn_door3"));
		Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(160425541, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
			ENTITY::SET_ENTITY_VISIBLE(rdoor1, false);

		Object door1 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_eme_barn_door3"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_eme_barn_door3", door1, 0);

		Object rdoor2 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1127035680, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor2))
			ENTITY::SET_ENTITY_VISIBLE(rdoor2, false);

		Object door2 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_eme_barn_door3"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_eme_barn_door3^1", door2, 0);

		// Activate interior
		Interior stage = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
		INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			Features::YieldThread();
		}

		// Deactivate interior
		INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", true);

		// Delete objects and characters
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1)) ENTITY::SET_ENTITY_VISIBLE(rdoor1, true);
		OBJECT::DELETE_OBJECT(&door1);
		if (ENTITY::DOES_ENTITY_EXIST(rdoor2)) ENTITY::SET_ENTITY_VISIBLE(rdoor2, true);
		OBJECT::DELETE_OBJECT(&door2);
		OBJECT::DELETE_OBJECT(&knife);
		OBJECT::DELETE_OBJECT(&stick);
		PED::DELETE_PED(&dinoLady);
	}

	void PlayFishCollectorCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@"
			"RCFSH_RSC2" // RCFSH_RSC2 // RCFSH_RSC3 // RCFSH_EXT
			, 0, 0, false, true);

		// Load in the map
		Features::Teleport(347.43f, -659.89f, 42.00f);
		
		//const char* playList = "pl_leadin";
		// "pl_base";
		// "pl_leadin";
		// "pl_leadin_John";
		// "pl_IG2_FishingCatfish_ConvoArtLT";
		// "pl_IG2_FishingCatfish_ConvoArtRT";
		// "pl_IG2_FishingCatfish_Eaten";
		// "pl_IG2_FishingCatfish_SoThenISaid";
		// "pl_leadout";

		//while (!ANIMSCENE::_IS_ANIM_SCENE_PLAYBACK_LIST_PHASE_LOADED(scene, playList))
		//{
		//	ANIMSCENE::REQUEST_ANIM_SCENE_PLAY_LIST(scene, playList);
		//	Features::YieldThread();
		//}
		//ANIMSCENE::SET_ANIM_SCENE_PLAY_LIST(scene, playList, true);
		//ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_breakout", false, false);

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_playerArthur", b_playerArthur, false);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, (b_playerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);

		Ped fishCollector = Features::SpawnPed(CS_FISHCOLLECTOR);
		int id = PED::_REQUEST_METAPED_OUTFIT(CS_FISHCOLLECTOR, 0x0E3AA86D);
		while (!PED::_HAS_METAPED_OUTFIT_LOADED(id))
			Features::YieldThread();
		PED::_APPLY_PED_METAPED_OUTFIT(id, fishCollector, true, false);
		PED::_RELEASE_METAPED_OUTFIT_REQUEST(id);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_FISHCOLLECTOR", fishCollector, 0);

		Features::RequestModel(joaat("P_CAMERABOX01X"));
		Object camera = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("P_CAMERABOX01X"), -5044.987f, -4090.398f, -29.0651f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "P_CAMERABOX01X", camera, 0);

		//Ped fishCollector2 = Features::SpawnPed(IG_FISHCOLLECTOR);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_FISHCOLLECTOR", fishCollector, 0);
		//PED::SET_PED_RANDOM_COMPONENT_VARIATION(fishCollector, 0);

		Features::RequestModel(joaat("p_fishingpole01x"));
		Object fishingPole = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_fishingpole01x"), 347.43f, -659.89f, 42.00f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_fishingpole01x", fishingPole, 0);

		Features::RequestModel(joaat("p_cs_fishbag01x"));
		Object fishBag = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_cs_fishbag01x"), 347.43f, -659.89f, 42.00f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_cs_fishbag01x", fishBag, 0);

		Features::RequestModel(joaat("p_door04x"));
		Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1056329818, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
			ENTITY::SET_ENTITY_VISIBLE(rdoor1, false);

		Object door1 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_door04x"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_door04x", door1, 0);

		//Object rdoor2 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1127035680, 0));
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor2))
		//	ENTITY::SET_ENTITY_VISIBLE(rdoor2, false);
		//
		//Object door2 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_eme_barn_door3"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_eme_barn_door3^1", door2, 0);

		// Activate interior
		//Interior stage = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
		//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			Features::YieldThread();
		}

		// Deactivate interior
		//INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", true);

		// Delete objects and characters
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1)) ENTITY::SET_ENTITY_VISIBLE(rdoor1, true);
		OBJECT::DELETE_OBJECT(&door1);
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor2)) ENTITY::SET_ENTITY_VISIBLE(rdoor2, true);
		//OBJECT::DELETE_OBJECT(&door2);
		OBJECT::DELETE_OBJECT(&camera);
		OBJECT::DELETE_OBJECT(&fishBag);
		OBJECT::DELETE_OBJECT(&fishingPole);
		PED::DELETE_PED(&fishCollector);
	}

	void PlayIndustryCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@ind1_mcs_1", 0, 0, false, true);

		// Load in the map
		Features::Teleport(2543.44f, -1169.15f, 52.31f);
		
		// Create characters and objects
		//bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		Ped arthur = Features::SpawnPed(PLAYER_ZERO);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", arthur, 0);
		PED::_EQUIP_META_PED_OUTFIT_PRESET(arthur, 4, false);

		Ped dutch = Features::SpawnPed(CS_DUTCH);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "Dutch", dutch, 0);

		Ped hosea = Features::SpawnPed(CS_HOSEAMATTHEWS);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "HoseaMatthews", hosea, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			Features::YieldThread();
		}

		// Delete objects and characters
		PED::DELETE_PED(&hosea);
		PED::DELETE_PED(&dutch);
		PED::DELETE_PED(&arthur);
	}

	void PlayAnnesburgBreakoutCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@JBO6_EXT", 0, 0, false, true);

		Features::Teleport(2912.49f, 1296.00f, 44.20f);

		// Create characters and objects
		Ped charles = Features::SpawnPed(CS_CHARLESSMITH);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CharlesSmith", charles, 0);

		Ped arthur = Features::SpawnPed(PLAYER_ZERO);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", arthur, 0);

		Ped lawman1 = Features::SpawnPed(S_M_M_AMBIENTLAWRURAL_01);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "S_M_M_AMBIENTLAWRURAL_01", lawman1, 0);

		Ped lawman2 = Features::SpawnPed(S_M_M_AMBIENTLAWRURAL_01);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "S_M_M_AMBIENTLAWRURAL_01^1", lawman2, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			Features::YieldThread();
		}

		// Delete objects and characters
		PED::DELETE_PED(&lawman2);
		PED::DELETE_PED(&lawman1);
		PED::DELETE_PED(&arthur);
		PED::DELETE_PED(&charles);
	}

	void PlayCharlesLeavingCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@ntv0_int", 0, 0, false, true);

		Features::Teleport(1883.39f, -1828.63f, 42.35f);

		// Create characters and objects
		Ped charles = Features::SpawnPed(CS_CHARLESSMITH);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CharlesSmith", charles, 0);

		Ped arthur = Features::SpawnPed(PLAYER_ZERO);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", arthur, 0);

		Ped horse = Features::SpawnPed(A_C_HORSE_GANG_CHARLES);
		int id = PED::_REQUEST_METAPED_OUTFIT(A_C_HORSE_GANG_CHARLES, 0x4B96E611);
		while (!PED::_HAS_METAPED_OUTFIT_LOADED(id))
			Features::YieldThread();
		PED::_APPLY_PED_METAPED_OUTFIT(id, horse, true, false);
		PED::_RELEASE_METAPED_OUTFIT_REQUEST(id);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "Horse_01", horse, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			Features::YieldThread();
		}

		// Delete objects and characters
		Features::EndSpawnPed(horse);
		PED::DELETE_PED(&horse);
		Features::EndSpawnPed(arthur);
		PED::DELETE_PED(&arthur);
		Features::EndSpawnPed(charles);
		PED::DELETE_PED(&charles);
	}
}
