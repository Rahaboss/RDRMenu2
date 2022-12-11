#include "pch.h"
#include "Menu.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Renderer.h"
#include "JobQueue.h"

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

			if (ImGui::Button("TP To Guarma"))
			{
				Features::TeleportOnGround(1424.31f, -7325.1f, 81.4575f);
			}

			if (ImGui::Button("TP To Mainland"))
			{
				Features::TeleportOnGround(74.4952f, 40.1398f, 101.401f);
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

			ImGui::Separator();

			if (ImGui::Checkbox("God Mode", &Features::EnableGodMode) && !Features::EnableGodMode)
			{
				QUEUE_JOB()
				{
					Features::SetGodmode(false);
				}
				END_JOB()
			}
			ImGui::Checkbox("No Sliding", &Features::EnableNoSliding);
			
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
					Features::GiveAllDualWieldWeapons();
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

			if (ImGui::Button("Give Cleaver"))
			{
				QUEUE_JOB()
				{
					Features::GiveWeapon(WEAPON_MELEE_CLEAVER);
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
					std::cout << ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
				}
				END_JOB()
			}

			ImGui::Separator();

			ImGui::Checkbox("Log Ped Spawning", &Features::EnablePedSpawnLogging);
			ImGui::Checkbox("Log Human Spawning", &Features::EnableHumanSpawnLogging);
			ImGui::Checkbox("Log Vehicle Spawning", &Features::EnableVehicleSpawnLogging);
			ImGui::Checkbox("Log Added Inventory Items", &Features::EnableAddInventoryItemLogging);
			ImGui::Checkbox("Enable ImGui Demo Window", &EnableDemoWindow);

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}
}
