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
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@RCDIN_RSC2", 0, "MultiStart", false, true);

		Teleport(208.423f, 1000.71f, 190.036f);

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;

		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_playerArthur", b_playerArthur, false);

		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, (b_playerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);

		Ped dinoLady = SpawnPed(CS_DINOBONESLADY);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_DINOBONESLADY", dinoLady, 0);

		RequestModel(joaat("P_STICK04X"));
		Object stick = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("P_STICK04X"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_stick04x", stick, 0);

		RequestModel(joaat("w_melee_knife03"));
		Object knife = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("w_melee_knife03"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "w_melee_knife03", knife, 0);

		RequestModel(joaat("p_eme_barn_door3"));
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
			YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			YieldThread();
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
		DeletePed(dinoLady);
	}

	void PlayFishCollectorCutscene(int index)
	{
		// Load in the map
		switch (index)
		{
		case 0:
			Teleport(347.43f, -659.89f, 42.00f);
			break;
		case 1:
		case 2:
			Teleport(-5083.33f, -4119.60f, -28.63f);
			break;
		default:
			printf("%s: %d is not a valid cutscene index.\n", __FUNCTION__, index);
			return;
		}
		
		// Init anim scene (cutscene)
		constexpr char* animDicts[] = {
			"cutscene@RCFSH_RSC2",
			"cutscene@RCFSH_RSC3",
			"cutscene@RCFSH_EXT",
		};
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE(animDicts[index], 0, "MultiStart", false, true);

		// Create characters and objects
		bool b_PlayerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_PlayerArthur", b_PlayerArthur, false);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, (b_PlayerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);

		Ped fishCollector;
		if (index != 2)
		{
			fishCollector = SpawnPed(CS_FISHCOLLECTOR);
			int id = PED::_REQUEST_METAPED_OUTFIT(CS_FISHCOLLECTOR, 0xE3AA86D);
			while (!PED::_HAS_METAPED_OUTFIT_LOADED(id))
				YieldThread();
			PED::_APPLY_PED_METAPED_OUTFIT(id, fishCollector, true, false);
			PED::_RELEASE_METAPED_OUTFIT_REQUEST(id);
			ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_FISHCOLLECTOR", fishCollector, 0);
		}

		RequestModel(joaat("P_CAMERABOX01X"));
		Object camera = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("P_CAMERABOX01X"), -5044.987f, -4090.398f, -29.0651f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "P_CAMERABOX01X", camera, 0);

		RequestModel(joaat("p_fishingpole01x"));
		Object fishingPole = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_fishingpole01x"), 347.43f, -659.89f, 42.00f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_fishingpole01x", fishingPole, 0);

		RequestModel(joaat("p_cs_fishbag01x"));
		Object fishBag = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_cs_fishbag01x"), 347.43f, -659.89f, 42.00f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_cs_fishbag01x", fishBag, 0);

		RequestModel(joaat("p_door04x"));
		Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1056329818, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
			ENTITY::SET_ENTITY_VISIBLE(rdoor1, false);

		Object door1 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_door04x"), 196.386f, 988.0386f, 189.1274f, true, true, false, true);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_door04x", door1, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			YieldThread();
		}
		ANIMSCENE::_DELETE_ANIM_SCENE(scene);

		// Delete objects and characters
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1)) ENTITY::SET_ENTITY_VISIBLE(rdoor1, true);
		OBJECT::DELETE_OBJECT(&door1);
		OBJECT::DELETE_OBJECT(&camera);
		OBJECT::DELETE_OBJECT(&fishBag);
		OBJECT::DELETE_OBJECT(&fishingPole);
		if (index != 2)
			DeletePed(fishCollector);
	}

	void PlayIndustryCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@ind1_mcs_1", 0, "MultiStart", false, true);

		// Load in the map
		Teleport(2543.44f, -1169.15f, 52.31f);
		
		// Create characters and objects
		//bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		Ped arthur = SpawnPed(PLAYER_ZERO);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", arthur, 0);
		PED::_EQUIP_META_PED_OUTFIT_PRESET(arthur, 4, false);

		Ped dutch = SpawnPed(CS_DUTCH);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "Dutch", dutch, 0);

		Ped hosea = SpawnPed(CS_HOSEAMATTHEWS);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "HoseaMatthews", hosea, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			YieldThread();
		}

		// Delete objects and characters
		DeletePed(hosea);
		DeletePed(dutch);
		DeletePed(arthur);
	}

	void PlayAnnesburgBreakoutCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@JBO6_EXT", 0, "MultiStart", false, true);

		Teleport(2912.49f, 1296.00f, 44.20f);
		YieldThread();
		ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(Features::GetMainEntity(), true);

		// Create characters and objects
		Ped charles = SpawnPed(CS_CHARLESSMITH);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CharlesSmith", charles, 0);

		Ped arthur = g_LocalPlayer.m_Entity;
		if (ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) != PLAYER_ZERO)
			arthur = SpawnPed(PLAYER_ZERO);
		PED::_EQUIP_META_PED_OUTFIT_PRESET(arthur, 0, false);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", arthur, 0);
		constexpr Hash tags[]
		{
			joaat("GUNBELTS"),
			joaat("HATS"),
			joaat("HOLSTERS_KNIFE"),
			joaat("HOLSTERS_LEFT"),
			joaat("HOLSTERS_RIGHT"),
			joaat("SATCHELS"),
			joaat("SATCHEL_STRAPS"),
		};
		for (const auto& t : tags)
			PED::REMOVE_TAG_FROM_META_PED(arthur, t, 1);
		PED::_UPDATE_PED_VARIATION(arthur, false, true, true, true, true);
		
		Ped lawman1 = SpawnPed(S_M_M_AMBIENTLAWRURAL_01);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "S_M_M_AMBIENTLAWRURAL_01", lawman1, 0);

		Ped lawman2 = SpawnPed(S_M_M_AMBIENTLAWRURAL_01);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "S_M_M_AMBIENTLAWRURAL_01^1", lawman2, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			YieldThread();
		}

		// Delete objects and characters
		DeletePed(lawman2);
		DeletePed(lawman1);
		DeletePed(arthur);
		DeletePed(charles);
	}

	void PlayCharlesLeavingCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@ntv0_int", 0, "MultiStart", false, true);

		Teleport(1883.39f, -1828.63f, 42.35f);

		// Create characters and objects
		Ped charles = SpawnPed(CS_CHARLESSMITH);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CharlesSmith", charles, 0);
		WEAPON::REMOVE_ALL_PED_WEAPONS(charles, true, true);

		//Ped arthur = SpawnPed(PLAYER_ZERO);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", g_LocalPlayer.m_Entity, 0);

		Ped horse = SpawnPed(A_C_HORSE_GANG_CHARLES);
		int id = PED::_REQUEST_METAPED_OUTFIT(A_C_HORSE_GANG_CHARLES, 0x4B96E611);
		while (!PED::_HAS_METAPED_OUTFIT_LOADED(id))
			YieldThread();
		PED::_APPLY_PED_METAPED_OUTFIT(id, horse, true, false);
		PED::_RELEASE_METAPED_OUTFIT_REQUEST(id);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "Horse_01", horse, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			YieldThread();
		}

		// Delete objects and characters
		DeletePed(horse);
		DeletePed(charles);
	}

	void PlayJackCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@rjck1_rsc5", 0, "MultiStart", false, true);

		Teleport(-1654.05f, -1367.56f, 83.97f);

		// Create characters and objects
		Ped jack = SpawnPed(CS_JACKMARSTON_TEEN);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "JackMarston_Teen", jack, 0);

		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "JOHN", g_LocalPlayer.m_Entity, 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, true, false))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, false))
		{
			YieldThread();
		}

		// Delete objects and characters
		DeletePed(jack);
	}
}
