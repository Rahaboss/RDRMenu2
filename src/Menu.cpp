#include "pch.h"
#include "Menu.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Renderer.h"
#include "JobQueue.h"
#include "rage/lists.h"

namespace Menu
{
	void RenderMenu()
	{
		ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Always);
		if (ImGui::Begin("RDRMenu2", &Renderer::MenuOpen, ImGuiWindowFlags_NoResize))
		{
			ImGui::BeginTabBar("tab_bar");
			TRY
			{
				RenderPlayerTab();
				RenderWeaponTab();
				RenderWorldTab();
				RenderDebugTab();
			}
			EXCEPT{ LOG_EXCEPTION(); }
			if (ImGui::BeginTabItem("Exit"))
			{
				g_Running = false;
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();

		if (EnableDemoWindow)
			ImGui::ShowDemoWindow(&EnableDemoWindow);
	}

	void RenderPlayerTab()
	{
		if (ImGui::BeginTabItem("Player"))
		{
			ImGui::BeginChild("player_child", ImVec2(0, 0));
			ImGui::BeginGroup();

			if (ImGui::Button("Clear Wanted"))
			{
				QUEUE_JOB()
				{
					Features::ClearWanted();
				}
				END_JOB()
			}

			if (ImGui::Button("Give $100,000"))
			{
				QUEUE_JOB()
				{
					Features::SetMoney(10000000);
				}
				END_JOB()
			}

			if (ImGui::Button("Restore Cores"))
			{
				QUEUE_JOB()
				{
					Features::RestorePlayerCores();
					Features::RestoreHorseCores();
				}
				END_JOB()
			}

			if (ImGui::Button("Give Core XP Items"))
			{
				QUEUE_JOB()
				{
					Features::GiveAgedPirateRum();
					Features::GiveGinsengElixir();
					Features::GiveValerianRoot();
				}
				END_JOB()
			}

			if (ImGui::Button("Spawn Good Honor Enemy"))
			{
				QUEUE_JOB()
				{
					Features::SpawnGoodHonorEnemy();
				}
				END_JOB()
			}

			if (ImGui::Button("Spawn Charles"))
			{
				QUEUE_JOB()
				{
					constexpr Hash model = CS_CHARLESSMITH;
					Ped ped = Features::SpawnPed(model);
					Features::EndSpawnPed(model, ped);
				}
				END_JOB()
			}

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button("TP To Waypoint"))
			{
				QUEUE_JOB()
				{
					Features::TeleportToWaypoint();
				}
				END_JOB()
			}

			ImGui::PushButtonRepeat(true);
			if (ImGui::Button("TP Through Door"))
			{
				QUEUE_JOB()
				{
					Features::TeleportThroughDoor();
				}
				END_JOB()
			}
			ImGui::PopButtonRepeat();

			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			if (ImGui::Checkbox("God Mode", &Features::EnableGodMode) && !Features::EnableGodMode)
			{
				QUEUE_JOB()
				{
					Features::SetGodmode(false);
				}
				END_JOB()
			}

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Checkbox("No Sliding", &Features::EnableNoSliding);
			
			ImGui::EndGroup();
			ImGui::Separator();

			RenderTeleportMenu();
			
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderWeaponTab()
	{
		if (ImGui::BeginTabItem("Weapon"))
		{
			ImGui::BeginChild("weapon_child", ImVec2(0, 0));

			if (ImGui::Button("Give Weapons"))
			{
				QUEUE_JOB()
				{
					Features::GiveAllWeapons();
				}
				END_JOB()
			}

			if (ImGui::Button("Give Dual Wield Weapons"))
			{
				QUEUE_JOB()
				{
					Features::GiveAllDualWieldWeapons();
				}
				END_JOB()
			}

			if (ImGui::Button("Give Ammo"))
			{
				QUEUE_JOB()
				{
					Features::GiveAllAmmo();
				}
				END_JOB()
			}

			if (ImGui::Button("Spawn Turret"))
			{
				QUEUE_JOB()
				{
					Features::SpawnVehicle(GATLING_GUN);
				}
				END_JOB()
			}

			if (ImGui::Button("Drop Current Weapon"))
			{
				QUEUE_JOB()
				{
					Features::DropCurrentWeapon();
				}
				END_JOB()
			}

			ImGui::Separator();

			ImGui::Checkbox("Infinite Ammo", &Features::EnableInfiniteAmmo);

			ImGui::Separator();

			ImGui::BeginChild("weapon_list", ImVec2(0, 200));

			ImGui::BeginChild("weapon_list_menu", ImVec2(200, 0));
			static Hash CurrentWeapon = WEAPON_REVOLVER_DOUBLEACTION_EXOTIC;
			for (auto it = g_WeaponMenuList.begin(); it != g_WeaponMenuList.end(); it++)
			{
				if (ImGui::Selectable(it->first.c_str(), CurrentWeapon == it->second))
					CurrentWeapon = it->second;
			}
			ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginChild("weapon_menu", ImVec2(0, 0));

			ImGui::Text("Weapon Name: %s", HUD::GET_STRING_FROM_HASH_KEY(CurrentWeapon));
			ImGui::Text("Weapon Hash: %X", CurrentWeapon);
			
			ImGui::Separator();

			if (ImGui::Button("Give Weapon"))
			{
				QUEUE_JOB(=)
				{
					Features::GiveWeapon(CurrentWeapon);
				}
				END_JOB()
			}

			ImGui::EndChild();
			ImGui::EndChild();

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderWorldTab()
	{
		if (ImGui::BeginTabItem("World"))
		{
			ImGui::BeginChild("world_child", ImVec2(0, 0));

			if (ImGui::Button("Reveal Map"))
			{
				QUEUE_JOB()
				{
					Features::RevealMap();
				}
				END_JOB()
			}

			ImGui::Separator();

			ImGui::Checkbox("No Snipers", &Features::EnableNoSnipers);

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderDebugTab()
	{
		if (ImGui::BeginTabItem("Debug"))
		{
			ImGui::BeginChild("debug_child", ImVec2(0, 0));

			if (ImGui::Button("Print Coords"))
			{
				QUEUE_JOB()
				{
					std::cout << ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE) << "\n";
				}
				END_JOB()
			}

			ImGui::Separator();

			ImGui::Checkbox("Log Ped Spawning", &Features::EnablePedSpawnLogging);
			ImGui::Checkbox("Log Human Spawning", &Features::EnableHumanSpawnLogging);
			ImGui::Checkbox("Log Vehicle Spawning", &Features::EnableVehicleSpawnLogging);
			ImGui::Checkbox("Log Added Inventory Items", &Features::EnableAddInventoryItemLogging);
			ImGui::Checkbox("Enable ImGui Demo Window", &EnableDemoWindow);

			ImGui::Separator();

			ImGui::Text("Logger");
			Logger.Draw();

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderTeleportMenu()
	{
		ImGui::Text("Teleports");

		ImGui::BeginChild("teleport_menu", ImVec2(0, 0));

		if (ImGui::Selectable("Annesburg"))
			Features::TeleportOnGround(2921.04f, 1294.39f, 44.639f);
		if (ImGui::Selectable("Armadillo"))
			Features::TeleportOnGround(-3627.98f, -2591.47f, -13.9677f);
		if (ImGui::Selectable("Beaver Hollow (Chapter 6)"))
			Features::TeleportOnGround(2377.05f, 1309.92f, 108.714f);
		if (ImGui::Selectable("Beecher's Hope (Epilogue Part 2)"))
			Features::TeleportOnGround(-1648.99f, -1416.1f, 83.4074f);
		if (ImGui::Selectable("Benedict Point"))
			Features::TeleportOnGround(-5264.47f, -3416.71f, -21.8179f);
		if (ImGui::Selectable("Blackwater"))
			Features::TeleportOnGround(-799.675f, -1261.2f, 43.8076f);
		if (ImGui::Selectable("Braithwaite Manor"))
			Features::TeleportOnGround(1013.0f, -1657.04f, 47.2939f);
		if (ImGui::Selectable("Butcher Creek"))
			Features::TeleportOnGround(2508.95f, 807.195f, 72.7694f);
		if (ImGui::Selectable("Caliga Hall"))
			Features::TeleportOnGround(1693.79f, -1390.27f, 44.2264f);
		if (ImGui::Selectable("Clemens Point (Chapter 3)"))
			Features::TeleportOnGround(685.991f, -1188.57f, 46.4375f);
		if (ImGui::Selectable("Colter (Chapter 1)"))
			Features::TeleportOnGround(-1360.84f, 2394.43f, 307.056f);
		if (ImGui::Selectable("Cornwall Kerosene & Tar"))
			Features::TeleportOnGround(426.267f, 696.999f, 116.203f);
		if (ImGui::Selectable("Emerald Ranch"))
			Features::TeleportOnGround(1363.91f, 306.949f, 87.6744f);
		if (ImGui::Selectable("Guarma (Chapter 5)"))
			Features::TeleportOnGround(1424.31f, -7325.1f, 81.4575f);
		if (ImGui::Selectable("Horseshoe Overlook (Chapter 2)"))
			Features::TeleportOnGround(-79.4716f, -4.14132f, 94.9357f);
		if (ImGui::Selectable("Lagras"))
			Features::TeleportOnGround(2112.28f, -650.864f, 42.4284f);
		if (ImGui::Selectable("Lakay (Chapter 5)"))
			Features::TeleportOnGround(2302.03f, -751.615f, 41.9054f);
		if (ImGui::Selectable("MacFarlane's Ranch"))
			Features::TeleportOnGround(-2302.04f, -2448.72f, 62.3551f);
		if (ImGui::Selectable("Manzanita Trading Post"))
			Features::TeleportOnGround(-1964.7f, -1583.59f, 116.383f);
		if (ImGui::Selectable("Pronghorn Ranch (Epilogue Part 1)"))
			Features::TeleportOnGround(-2567.1f, 459.28f, 145.776f);
		if (ImGui::Selectable("Rhodes"))
			Features::TeleportOnGround(1305.83f, -1298.29f, 76.5653f);
		if (ImGui::Selectable("Saint Denis"))
			Features::TeleportOnGround(2376.26f, -1139.36f, 46.8324f);
		if (ImGui::Selectable("Shady Belle (Chapter 4)"))
			Features::TeleportOnGround(1840.26f, -1821.53f, 44.408f);
		if (ImGui::Selectable("Strawberry"))
			Features::TeleportOnGround(-1738.43f, -409.492f, 155.592f);
		if (ImGui::Selectable("Thieves Landing"))
			Features::TeleportOnGround(-1465.19f, -2336.58f, 43.2596f);
		if (ImGui::Selectable("Tumbleweed"))
			Features::TeleportOnGround(-5415.37f, -2938.96f, 1.3023f);
		if (ImGui::Selectable("Valentine"))
			Features::TeleportOnGround(-332.713f, 785.198f, 116.385f);
		if (ImGui::Selectable("Van Horn Trading Post"))
			Features::TeleportOnGround(2961.2f, 498.593f, 46.1982f);
		if (ImGui::Selectable("Wapiti Indian Reservation"))
			Features::TeleportOnGround(475.954f, 2183.3f, 244.565f);

		ImGui::EndChild();
	}
}
