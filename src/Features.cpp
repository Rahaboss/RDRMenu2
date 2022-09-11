#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "Fiber.h"
#include "JobQueue.h"
#include "rage/natives.h"
#include "PlayerInfo.h"

namespace Features
{
	void ExecuteAsThread(joaat_t script_hash, void(*function)())
	{
		TRY
		{
			if (!function || !Pointers::ThreadCollection || !Pointers::ActiveThread)
				return;

			for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
			{
				auto thread = Pointers::ThreadCollection->m_pData[i];
				if (!thread || !thread->m_ThreadID || thread->m_ScriptHash != script_hash)
					continue;
				
				// backup and change thread
				auto og_thread = *Pointers::ActiveThread;
				*Pointers::ActiveThread = thread;

				// execute
				function();

				// restore thread 
				*Pointers::ActiveThread = og_thread;
				return;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		std::cout << "Couldn't execute as thread!\n";
	}

	void Setup()
	{
		//std::cout << "Thread Collection: " << LOG_HEX(Pointers::ThreadCollection) << '\n';
		//
		//std::cout << "Hash of current script: " << SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() << '\n';
		//std::cout << "RAGE_JOAAT(\"main\"): " << RAGE_JOAAT("main") << '\n';
		//std::cout << "SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME handler: " << Pointers::GetNativeHandler(0xBC2C927F5C264960) << '\n';
		//std::cout << "Player name: " << PLAYER::GET_PLAYER_NAME(0) << '\n';
		//
		//[]() {
		//	QUEUE_JOB()
		//	{
		//		std::cout << "Hash of current script in job queue: " << SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() << '\n';
		//	}
		//	END_JOB
		//}();
	}

	void OnTick()
	{
		TRY
		{
			GetLocalPlayerInfo();
			
			if (GetAsyncKeyState(VK_PRIOR) & 1)
				TeleportToWaypoint();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RunJobQueue()
	{
		g_JobQueue.Run();
	}

	int FpsTick()
	{
		int result = 0;

		static auto fc_begin = *Pointers::FrameCount;
		static auto time_begin = GetTickCount64();

		auto time_new = GetTickCount64();
		if (time_begin + 1000 < time_new)
		{
			auto fc_new = *Pointers::FrameCount;
			result = fc_new - fc_begin;

			fc_begin = *Pointers::FrameCount;
			time_begin = time_new;
		}

		return result;
	}

	void YieldThread()
	{
		Fiber::GetCurrent()->YieldThread();
	}

	void GetLocalPlayerInfo()
	{
		TRY
		{
			g_LocalPlayer.m_Index = PLAYER::PLAYER_ID();
			g_LocalPlayer.m_Entity = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_LocalPlayer.m_Index);
			g_LocalPlayer.m_Mount = PED::GET_MOUNT(g_LocalPlayer.m_Entity);
			g_LocalPlayer.m_Vehicle = PED::GET_VEHICLE_PED_IS_IN(g_LocalPlayer.m_Entity, FALSE);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Entity GetMountOrVehicle()
	{
		if (g_LocalPlayer.m_Mount)
			return g_LocalPlayer.m_Mount;
		return g_LocalPlayer.m_Vehicle;
	}

	Entity GetMainEntity()
	{
		if (auto e = GetMountOrVehicle())
			return e;
		return g_LocalPlayer.m_Entity;
	}

	void Teleport(const float& x, const float& y, const float& z)
	{
		ENTITY::SET_ENTITY_COORDS(GetMainEntity(), x, y, z, FALSE, FALSE, FALSE, FALSE);
	}

	void Teleport(const Vector3& pos)
	{
		Teleport(pos.x, pos.y, pos.z);
	}

	void TeleportToWaypoint()
	{
		TRY
		{
			Vector3 coords{};
			bool found = false, playerBlip = false;
			if (MAP::IS_WAYPOINT_ACTIVE())
			{
				coords = MAP::_GET_WAYPOINT_COORDS();
				found = true;
				playerBlip = true;
			}
#if 0
			else
			{
				//for (int i = 0; i < 3; i++)
				{
					Blip blip = 0;// MAP::GET_FIRST_BLIP_INFO_ID(i);
					if (MAP::DOES_BLIP_EXIST(blip))
					{
						coords = MAP::GET_BLIP_COORDS(blip);
						found = true;
						//break;
					}
				}
			}
#endif

			if (!found)
			{
				std::cout << "Waypoint not active!\n";
				return;
			}
			
			if (playerBlip)
			{
				float groundZ;
				bool useGroundZ;
				for (int i = 0; i < 100; i++)
				{
					float testZ = (i * 10.f) - 100.f;

					Teleport(coords.x, coords.y, testZ);
					if (i % 5 == 0)
						YieldThread();

					useGroundZ = MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, testZ, &groundZ, false);
					if (useGroundZ)
						break;
				}

				coords.z = (useGroundZ ? groundZ : ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE).z);
			}

			Teleport(coords);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
