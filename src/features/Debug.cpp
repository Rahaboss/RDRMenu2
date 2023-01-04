#include "pch.h"
#include "Debug.h"
#include "Features.h"
#include "rage/natives.h"
#include "PlayerInfo.h"

namespace Features
{
	void* GetBlipCollection()
	{
		uint64_t x = _rotl64(*Pointers::BlipBase, 29);
		return reinterpret_cast<void*>(~_rotl64(_rotl64(x ^ *Pointers::BlipHash, 32), (x & 0x1F) + 5));
	}

	CPedFactory* GetPedFactory()
	{
		uint64_t x = _rotl64(*Pointers::PedFactoryBase, 27);
		return reinterpret_cast<CPedFactory*>(~_rotl64(_rotl64(x ^ *Pointers::PedFactoryHash, 32), (x & 0x1F) + 1));
	}

	void PrintNativeHandlerAddress(const uint64_t& hash)
	{
		TRY
		{
			auto addr = (uintptr_t)NativeContext::GetHandler(hash);
			auto off = addr - g_BaseAddress;
			std::cout << LOG_HEX(hash) << " handler: RDR2.exe+" << LOG_HEX(off) << " (" << LOG_HEX(0x7FF73CAB0000 /*imagebase in ida*/ + off) << ").\n";
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void UnlockSPPreorderBonus()
	{
		constexpr Hash unlock_hash = RAGE_JOAAT("SP_GAME_CONTENT_PRE_ORDER");
		UNLOCK::UNLOCK_SET_UNLOCKED(unlock_hash, TRUE);
	}

	void PlayDinoLadyCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@RCDIN_RSC2", 0, 0, FALSE, TRUE);

		Features::Teleport(208.423f, 1000.71f, 190.036f);

		for (int i = 0; i < 10; i++)
		{
			for (int z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(208.423f, 1000.71f, (float)z);
				Features::YieldThread();
			}

			float groundZ;

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(208.423f, 1000.71f, 1000.f, &groundZ, FALSE))
				break;

			Features::YieldThread();
		}

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;

		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_playerArthur", b_playerArthur, FALSE);

		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, (b_playerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);

		Ped dinoLady = Features::SpawnPed(CS_DINOBONESLADY);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_DINOBONESLADY", dinoLady, 0);

		Features::RequestModel(joaat("P_STICK04X"));
		Object stick = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("P_STICK04X"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_stick04x", stick, 0);

		Features::RequestModel(joaat("w_melee_knife03"));
		Object knife = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("w_melee_knife03"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "w_melee_knife03", knife, 0);

		Features::RequestModel(joaat("p_eme_barn_door3"));
		Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(160425541, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
			ENTITY::SET_ENTITY_VISIBLE(rdoor1, FALSE);

		Object door1 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_eme_barn_door3"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_eme_barn_door3", door1, 0);

		Object rdoor2 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1127035680, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor2))
			ENTITY::SET_ENTITY_VISIBLE(rdoor2, FALSE);

		Object door2 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_eme_barn_door3"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_eme_barn_door3^1", door2, 0);

		// Activate interior
		Interior stage = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
		INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, TRUE, FALSE))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, FALSE))
		{
			Features::YieldThread();
		}

		// Deactivate interior
		INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", TRUE);

		// Delete objects and characters
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1)) ENTITY::SET_ENTITY_VISIBLE(rdoor1, TRUE);
		OBJECT::DELETE_OBJECT(&door1);
		if (ENTITY::DOES_ENTITY_EXIST(rdoor2)) ENTITY::SET_ENTITY_VISIBLE(rdoor2, TRUE);
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
			, 0, 0, FALSE, TRUE);

		// Load in the map
		Features::Teleport(347.43f, -659.89f, 42.00f);
		for (int i = 0; i < 10; i++)
		{
			for (int z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(347.43f, -659.89f, (float)z);
				Features::YieldThread();
			}

			float groundZ;

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(347.43f, -659.89f, 1000.f, &groundZ, FALSE))
				break;

			Features::YieldThread();
		}

		const char* playList = "pl_leadin";
		// "pl_base";
		// "pl_leadin";
		// "pl_leadin_John";
		// "pl_IG2_FishingCatfish_ConvoArtLT";
		// "pl_IG2_FishingCatfish_ConvoArtRT";
		// "pl_IG2_FishingCatfish_Eaten";
		// "pl_IG2_FishingCatfish_SoThenISaid";
		// "pl_leadout";

		while (!ANIMSCENE::_IS_ANIM_SCENE_PLAYBACK_LIST_PHASE_LOADED(scene, playList))
		{
			ANIMSCENE::REQUEST_ANIM_SCENE_PLAY_LIST(scene, playList);
			Features::YieldThread();
		}
		ANIMSCENE::SET_ANIM_SCENE_PLAY_LIST(scene, playList, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_breakout", FALSE, FALSE);

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_playerArthur", b_playerArthur, FALSE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, (b_playerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);

		Ped fishCollector = Features::SpawnPed(CS_FISHCOLLECTOR);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_FISHCOLLECTOR", fishCollector, 0);
		PED::_RESET_PED_COMPONENTS(fishCollector);

		Features::RequestModel(joaat("P_CAMERABOX01X"));
		Object camera = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("P_CAMERABOX01X"), -5044.987f, -4090.398f, -29.0651f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "P_CAMERABOX01X", camera, 0);

		//Ped fishCollector2 = Features::SpawnPed(IG_FISHCOLLECTOR);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_FISHCOLLECTOR", fishCollector, 0);
		//PED::SET_PED_RANDOM_COMPONENT_VARIATION(fishCollector, 0);

		Features::RequestModel(joaat("p_fishingpole01x"));
		Object fishingPole = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_fishingpole01x"), 347.43f, -659.89f, 42.00f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_fishingpole01x", fishingPole, 0);

		Features::RequestModel(joaat("p_cs_fishbag01x"));
		Object fishBag = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_cs_fishbag01x"), 347.43f, -659.89f, 42.00f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_cs_fishbag01x", fishBag, 0);

		Features::RequestModel(joaat("p_door04x"));
		Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1056329818, 0));
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
			ENTITY::SET_ENTITY_VISIBLE(rdoor1, FALSE);
		
		Object door1 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_door04x"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_door04x", door1, 0);
		
		//Object rdoor2 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1127035680, 0));
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor2))
		//	ENTITY::SET_ENTITY_VISIBLE(rdoor2, FALSE);
		//
		//Object door2 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_eme_barn_door3"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_eme_barn_door3^1", door2, 0);

		// Activate interior
		//Interior stage = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
		//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, TRUE, FALSE))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, FALSE))
		{
			Features::YieldThread();
		}

		// Deactivate interior
		//INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", TRUE);

		// Delete objects and characters
		if (ENTITY::DOES_ENTITY_EXIST(rdoor1)) ENTITY::SET_ENTITY_VISIBLE(rdoor1, TRUE);
		OBJECT::DELETE_OBJECT(&door1);
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor2)) ENTITY::SET_ENTITY_VISIBLE(rdoor2, TRUE);
		//OBJECT::DELETE_OBJECT(&door2);
		OBJECT::DELETE_OBJECT(&camera);
		OBJECT::DELETE_OBJECT(&fishBag);
		OBJECT::DELETE_OBJECT(&fishingPole);
		PED::DELETE_PED(&fishCollector);
	}
	
	void PlayIndustryCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@ind1_mcs_1", 0, 0, FALSE, TRUE);

		// Load in the map
		Features::Teleport(2551.05f, -1178.40f, 53.31f);
		for (int i = 0; i < 10; i++)
		{
			for (int z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(2551.05f, -1178.40f, (float)z);
				Features::YieldThread();
			}

			float groundZ;

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(2551.05f, -1178.40f, 1000.f, &groundZ, FALSE))
				break;

			Features::YieldThread();
		}

		//ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_breakout", false, false);
		//ANIMSCENE::SET_ANIM_SCENE_PLAY_LIST(scene, "pl_IG33_AMB_WServesDrinks_BK", TRUE);

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", g_LocalPlayer.m_Entity, 0);

		Ped dutch = Features::SpawnPed(CS_DUTCH);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "Dutch", dutch, 0);

		Ped hosea = Features::SpawnPed(CS_HOSEAMATTHEWS);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "HoseaMatthews", hosea, 0);

		//Features::RequestModel(joaat("P_CAMERABOX01X"));
		//Object camera = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("P_CAMERABOX01X"), -5044.987f, -4090.398f, -29.0651f, TRUE, TRUE, FALSE, TRUE);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "P_CAMERABOX01X", camera, 0);
		//
		//Ped fishCollector2 = Features::SpawnPed(IG_FISHCOLLECTOR);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_FISHCOLLECTOR", fishCollector, 0);
		//PED::SET_PED_RANDOM_COMPONENT_VARIATION(fishCollector, 0);
		//
		//Features::RequestModel(joaat("p_fishingpole01x"));
		//Object fishingPole = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_fishingpole01x"), 347.43f, -659.89f, 42.00f, TRUE, TRUE, FALSE, TRUE);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_fishingpole01x", fishingPole, 0);
		//
		//Features::RequestModel(joaat("p_cs_fishbag01x"));
		//Object fishBag = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_cs_fishbag01x"), 347.43f, -659.89f, 42.00f, TRUE, TRUE, FALSE, TRUE);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_cs_fishbag01x", fishBag, 0);
		//
		//Features::RequestModel(joaat("p_door04x"));
		//Object rdoor1 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1056329818, 0));
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor1))
		//	ENTITY::SET_ENTITY_VISIBLE(rdoor1, FALSE);
		//
		//Object door1 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_door04x"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_door04x", door1, 0);
		//
		//Object rdoor2 = ENTITY::GET_OBJECT_INDEX_FROM_ENTITY_INDEX(ENTITY::_GET_ENTITY_BY_DOORHASH(-1127035680, 0));
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor2))
		//	ENTITY::SET_ENTITY_VISIBLE(rdoor2, FALSE);
		//
		//Object door2 = OBJECT::CREATE_OBJECT_NO_OFFSET(joaat("p_eme_barn_door3"), 196.386f, 988.0386f, 189.1274f, TRUE, TRUE, FALSE, TRUE);
		//ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "p_eme_barn_door3^1", door2, 0);
		//
		// Activate interior
		//Interior stage = INTERIOR::GET_INTERIOR_AT_COORDS(196.2653f, 985.5404f, 189.1246f);
		//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", 0);

		// Load, start, unload
		while (!ANIMSCENE::IS_ANIM_SCENE_LOADED(scene, TRUE, FALSE))
		{
			ANIMSCENE::LOAD_ANIM_SCENE(scene);
			Features::YieldThread();
		}
		ANIMSCENE::START_ANIM_SCENE(scene);
		while (!ANIMSCENE::HAS_ANIM_SCENE_EXITED(scene, FALSE))
		{
			Features::YieldThread();
		}

		// Deactivate interior
		//INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(stage, "din_barn_dinoSP", TRUE);

		// Delete objects and characters
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor1)) ENTITY::SET_ENTITY_VISIBLE(rdoor1, TRUE);
		//OBJECT::DELETE_OBJECT(&door1);
		//if (ENTITY::DOES_ENTITY_EXIST(rdoor2)) ENTITY::SET_ENTITY_VISIBLE(rdoor2, TRUE);
		//OBJECT::DELETE_OBJECT(&door2);
		//OBJECT::DELETE_OBJECT(&camera);
		//OBJECT::DELETE_OBJECT(&fishBag);
		//OBJECT::DELETE_OBJECT(&fishingPole);
		PED::DELETE_PED(&hosea);
		PED::DELETE_PED(&dutch);
	}
}
