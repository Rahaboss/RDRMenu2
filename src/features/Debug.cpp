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

	void* GetScriptGlobal(uint32_t Index)
	{
		uintptr_t qword_18002D4F0 = g_BaseAddress + 0xB0000000;
		uint32_t y = *(uint32_t*)qword_18002D4F0;
		uint32_t dword_18002D4F8 = 0;
		
		if (y > 0x5DE8CC8B)
		{
			switch (y)
			{
			case 0x8B48D38B:
			{
				int v2 = *(int*)(qword_18002D4F0 + 0x143200000i64);
				if (v2 == 1837819906)
					dword_18002D4F8 = 6;
				if (v2 == 496633225)
					dword_18002D4F8 = 7;
				break;
			}
			case 0xD233FF87:
				dword_18002D4F8 = 4;
				break;
			case 0xD68B4808:
				dword_18002D4F8 = 10;
				break;
			case 0xE870290F:
				dword_18002D4F8 = 9;
				break;
			}
		}
		else
		{
			switch (y)
			{
			case 0x5DE8CC8Bu:
				dword_18002D4F8 = 1;
				break;
			case 0x2A5D3A4u:
				dword_18002D4F8 = 5;
				break;
			case 0x573F72Fu:
				dword_18002D4F8 = 8;
				break;
			case 0x100FFF94u:
				dword_18002D4F8 = 11;
				break;
			case 0x18488948u:
				dword_18002D4F8 = 2;
				break;
			case 0x39CCC328u:
				dword_18002D4F8 = 3;
				break;
			}
		}

		std::vector<uint64_t> qword_18002BB10{
			0,
			0x1457C3B00,
			0x1457CAEE0,
			0x1457D0040,
			0x1457D0170,
			0x1457D1330,
			0x14581D190,
			0x14581E2B0,
			0x145909370,
			0x1459E1040,
			0x145953A90,
			0x14598FDD0,
		};

		assert(dword_18002D4F8 > 0 && dword_18002D4F8 < 12);

		//uint64_t** GlobalPtr = (uint64_t**)(0x14598FDD0 + g_BaseAddress);
		//return GlobalPtr[(Index >> 0x12) & 0x3F] + (Index & 0x3FFFF);
		printf("Global Pointer: 0x%llX.\n", (qword_18002BB10[dword_18002D4F8]) + qword_18002D4F0);

		return (void*)(*((uint64_t*)qword_18002BB10[dword_18002D4F8] + 8i64 * ((Index >> 0x12) & 0x3F) + qword_18002D4F0) + 8i64 * (Index & 0x3FFFF));
	}

	void PrintNativeHandlerAddress(uint64_t hash)
	{
		TRY
		{
			uint64_t addr = (uintptr_t)NativeContext::GetHandler(hash);
			uint64_t off = addr - g_BaseAddress;
			printf("0x%llX handler: RDR2.exe+0x%llX (0x%llX).\n", hash, off, (off + 0x7FF73CAB0000 /*imagebase in ida*/));
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void UnlockSPPreorderBonus()
	{
		constexpr Hash unlock_hash = RAGE_JOAAT("SP_GAME_CONTENT_PRE_ORDER");
		UNLOCK::UNLOCK_SET_UNLOCKED(unlock_hash, true);
	}

	void PlayDinoLadyCutscene()
	{
		// Init anim scene (cutscene)
		AnimScene scene = ANIMSCENE::_CREATE_ANIM_SCENE("cutscene@RCDIN_RSC2", 0, 0, false, true);

		Features::Teleport(208.423f, 1000.71f, 190.036f);

		for (int i = 0; i < 10; i++)
		{
			for (int z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(208.423f, 1000.71f, (float)z);
				Features::YieldThread();
			}

			float groundZ;

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(208.423f, 1000.71f, 1000.f, &groundZ, false))
				break;

			Features::YieldThread();
		}

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
		for (int i = 0; i < 10; i++)
		{
			for (int z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(347.43f, -659.89f, (float)z);
				Features::YieldThread();
			}

			float groundZ;

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(347.43f, -659.89f, 1000.f, &groundZ, false))
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
		ANIMSCENE::SET_ANIM_SCENE_PLAY_LIST(scene, playList, true);
		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_breakout", false, false);

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		ANIMSCENE::SET_ANIM_SCENE_BOOL(scene, "b_playerArthur", b_playerArthur, false);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, (b_playerArthur ? "ARTHUR" : "JOHN"), g_LocalPlayer.m_Entity, 0);

		Ped fishCollector = Features::SpawnPed(CS_FISHCOLLECTOR);
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "CS_FISHCOLLECTOR", fishCollector, 0);
		PED::_RESET_PED_COMPONENTS(fishCollector);

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
		Features::Teleport(2551.05f, -1178.40f, 53.31f);
		for (int i = 0; i < 10; i++)
		{
			for (int z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(2551.05f, -1178.40f, (float)z);
				Features::YieldThread();
			}

			float groundZ;

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(2551.05f, -1178.40f, 1000.f, &groundZ, false))
				break;

			Features::YieldThread();
		}

		// Create characters and objects
		bool b_playerArthur = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == PLAYER_ZERO;
		ANIMSCENE::SET_ANIM_SCENE_ENTITY(scene, "ARTHUR", g_LocalPlayer.m_Entity, 0);

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
	}
}
