#include "pch.h"
#include "Menu.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Renderer.h"
#include "JobQueue.h"
#include "rage/lists.h"
#include "PlayerInfo.h"

namespace Menu
{
	void RenderMenu()
	{
		TRY
		{
			ImGui::SetNextWindowSize(ImVec2(700, 550), ImGuiCond_Always);
			if (ImGui::Begin("RDRMenu2", &Renderer::MenuOpen, ImGuiWindowFlags_NoResize))
			{
				ImGui::BeginTabBar("tab_bar");
				RenderPlayerTab();
				RenderWeaponTab();
				RenderInventoryTab();
				RenderWorldTab();
				RenderSpawningTab();
				RenderDebugTab();
				RenderLoggerTab();
				RenderMemoryTab();
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
		EXCEPT{ LOG_EXCEPTION(); }
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

			if (ImGui::Button("TP To Last Mount"))
			{
				QUEUE_JOB()
				{
					if (g_LocalPlayer.m_Mount)
						return;

					if (Ped mount = PED::_GET_LAST_MOUNT(g_LocalPlayer.m_Entity))
						PED::SET_PED_ONTO_MOUNT(g_LocalPlayer.m_Entity, mount, -1, TRUE);
				}
				END_JOB()
			}

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button("Spawn Good Honor Enemy"))
			{
				QUEUE_JOB()
				{
					Features::SpawnGoodHonorEnemy();
				}
				END_JOB()
			}

			if (ImGui::Button("Spawn Bad Honor Enemy"))
			{
				QUEUE_JOB()
				{
					Features::SpawnBadHonorEnemy();
				}
				END_JOB()
			}

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
			ImGui::Checkbox("Super Jump", &Features::EnableSuperJump);

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

			ImGui::BeginGroup();
			if (ImGui::Button("Give Weapons"))
				Features::GiveAllWeapons();

			if (ImGui::Button("Give Dual Wield Weapons"))
				Features::GiveAllDualWieldWeapons();

			if (ImGui::Button("Give Ammo"))
				Features::GiveAllAmmo();
			
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button("Spawn Turret"))
			{
				QUEUE_JOB()
				{
					Vehicle veh = Features::SpawnVehicle(GATLING_GUN);
					Features::EndSpawnVehicle(GATLING_GUN, veh);
				}
				END_JOB()
			}

			if (ImGui::Button("Drop Current Weapon"))
				Features::DropCurrentWeapon();

			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			ImGui::Checkbox("Infinite Ammo", &Features::EnableInfiniteAmmo);
			
			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			ImGui::Text("Weapon List");
			ImGui::BeginChild("weapon_list_menu", ImVec2(270, 0));

			static Hash CurrentWeapon = WEAPON_REVOLVER_DOUBLEACTION_EXOTIC;
			for (const auto& w : g_WeaponMenuList)
			{
				if (ImGui::Selectable(w.first.c_str(), CurrentWeapon == w.second))
					CurrentWeapon = w.second;
			}
			
			ImGui::EndChild();
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Text("Weapon Name: %s", HUD::GET_STRING_FROM_HASH_KEY(CurrentWeapon));
			ImGui::Text("Weapon Hash: %X", CurrentWeapon);

			ImGui::Separator();
			if (ImGui::Button("Give Weapon"))
			{
				switch (CurrentWeapon)
				{
				case AMMO_MOLOTOV_VOLATILE:
				case AMMO_DYNAMITE_VOLATILE:
				case AMMO_THROWING_KNIVES_IMPROVED:
				case AMMO_THROWING_KNIVES_POISON:
				case AMMO_TOMAHAWK_IMPROVED:
				case AMMO_TOMAHAWK_HOMING:
					Features::GiveAmmo(CurrentWeapon);
					break;
				default:
					Features::GiveWeapon(CurrentWeapon);
					break;
				}
			}

			ImGui::EndGroup();
			ImGui::EndChild();

			ImGui::EndTabItem();
		}
	}

	void RenderInventoryTab()
	{
		if (ImGui::BeginTabItem("Inventory"))
		{
			ImGui::BeginChild("inventory_child", ImVec2(0, 0));

			if (ImGui::Button("Unlock All Herbs"))
			{
				QUEUE_JOB()
				{
					Vector3 pos = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
					for (const auto& h : g_HerbList)
						COMPENDIUM::COMPENDIUM_HERB_PICKED(h, pos.x, pos.y, pos.z);
				}
				END_JOB()
			}

			ImGui::Separator();

			ImGui::Text("Give Inventory Items");
			ImGui::BeginChild("item_menu", ImVec2(0, 200));
			if (ImGui::Selectable("Aged Pirate Rum"))
			{
				QUEUE_JOB()
				{
					Features::GiveAgedPirateRum();
				}
				END_JOB()
			}
			if (ImGui::Selectable("Ginseng Elixir"))
			{
				QUEUE_JOB()
				{
					Features::GiveGinsengElixir();
				}
				END_JOB()
			}
			if (ImGui::Selectable("Valerian Root"))
			{
				QUEUE_JOB()
				{
					Features::GiveValerianRoot();
				}
				END_JOB()
			}
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
			
			ImGui::BeginGroup();
			if (ImGui::Button("Set Time To Night"))
				Features::SetClockTime(0);
			if (ImGui::Button("Set Time To Morning"))
				Features::SetClockTime(6);
			if (ImGui::Button("Set Time To Noon"))
				Features::SetClockTime(12);
			if (ImGui::Button("Set Time To Evening"))
				Features::SetClockTime(18);
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
			
			if (ImGui::Button("Reveal Map"))
			{
				QUEUE_JOB()
				{
					Features::RevealMap();
				}
				END_JOB()
			}
			
			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			ImGui::Checkbox("No Snipers", &Features::EnableNoSnipers);

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Checkbox("No Black Borders", &Features::EnableNoBlackBorders);

			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			ImGui::Text("Set Weather");
			ImGui::BeginChild("weather_list_menu", ImVec2(200, 200));
			for (const auto& w : g_WeatherTypeList)
			{
				if (ImGui::Selectable(w.first.c_str()))
				{
					Hash hash = w.second;
					QUEUE_JOB(hash)
					{
						MISC::SET_WEATHER_TYPE(hash, TRUE, TRUE, FALSE, 0.0f, FALSE);
					}
					END_JOB()
				}
			}
			ImGui::EndChild();
			ImGui::EndGroup();
			ImGui::SameLine();

			ImGui::BeginGroup();
			ImGui::Text("Set Snow Type");
			ImGui::BeginChild("snow_list_menu", ImVec2(200, 200));
			for (const auto& s : g_SnowTypeList)
			{
				if (ImGui::Selectable(s.first.c_str()))
				{
					int hash = s.second;
					QUEUE_JOB(hash)
					{
						Features::SetSnowType(hash);
					}
					END_JOB()
				}
			}
			ImGui::EndChild();
			ImGui::EndGroup();

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderSpawningTab()
	{
		if (ImGui::BeginTabItem("Spawning"))
		{
			ImGui::BeginChild("spawning_child", ImVec2(0, 0));

#if ENABLE_LARGE_STACK_ITEMS
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Spawn Ped");
			ImGui::SameLine();
			static char PedBuffer[200];
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("###filter_ped", PedBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
			ImGui::BeginChild("ped_menu", ImVec2(0, 200));
			for (const auto& it : g_PedMenuList)
			{
				if (it.first.find(PedBuffer) == std::string::npos)
					continue;

				if (ImGui::Selectable(it.first.c_str()))
				{
					QUEUE_JOB(&it)
					{
						Ped ped = Features::SpawnPed(it.second);
						Features::EndSpawnPed(it.second, ped);
					}
					END_JOB()
				}
			}
			ImGui::EndChild();
			ImGui::Separator();
#endif

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Spawn Vehicle");
			ImGui::SameLine();
			static char VehBuffer[200];
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("###filter_veh", VehBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
			ImGui::BeginChild("vehicle_menu", ImVec2(0, 200));
			for (const auto& it : g_PedVehicleList)
			{
				if (it.first.find(VehBuffer) == std::string::npos)
					continue;

				if (ImGui::Selectable(it.first.c_str()))
				{
					QUEUE_JOB(&it)
					{
						Vehicle veh = Features::SpawnVehicle(it.second);
						Features::EndSpawnVehicle(it.second, veh);
					}
					END_JOB()
				}
			}
			ImGui::EndChild();

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
			ImGui::SameLine();

			// This native should be fine
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
			ImGui::Text("%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);

			ImGui::Separator();

			ImGui::Text("Rockstar: \xE2\x88\x91");
			ImGui::Text("Rockstar Verified: \xC2\xA6");
			ImGui::Text("Rockstar Created: \xE2\x80\xB9");
			ImGui::Text("Rockstar Blank: \xE2\x80\xBA");
			ImGui::Text("Padlock: \xCE\xA9");
			
			ImGui::Separator();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("CPed: 0x%llX", g_LocalPlayer.m_Ped);
			ImGui::SameLine();
			if (ImGui::Button("Print to console"))
				std::cout << "CPed: " << LOG_HEX(g_LocalPlayer.m_Ped) << "\n";

			ImGui::AlignTextToFramePadding();
			ImGui::Text("GetEntityAddress: 0x%llX", Pointers::GetEntityAddress(g_LocalPlayer.m_Entity));
			ImGui::SameLine();
			if (ImGui::Button("Print to console"))
				std::cout << "GetEntityAddress: " << LOG_HEX(Pointers::GetEntityAddress(g_LocalPlayer.m_Entity)) << "\n";

			uint64_t nhash = 0xA86D5F069399F44D;
			auto addr = (uintptr_t)g_NativeContext.GetHandler(nhash);
			auto off = addr - g_BaseAddress;
			ImGui::AlignTextToFramePadding();
			ImGui::Text("0x%llX handler: RDR2.exe+0x%X", nhash, off);

			ImGui::SameLine();
			if (ImGui::Button("Print to console"))
				std::cout << LOG_HEX(nhash) << " handler: RDR2.exe+" << LOG_HEX(off) << " (" <<
				LOG_HEX(0x7FF73CAB0000 /*imagebase in ida*/ + off) << ").\n";
				

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

	void RenderLoggerTab()
	{
		if (ImGui::BeginTabItem("Logger"))
		{
			ImGui::BeginChild("logger_child", ImVec2(0, 0));

			Logger.Draw();

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderMemoryTab()
	{
		if (ImGui::BeginTabItem("Memory"))
		{
			ImGui::BeginChild("memory_child", ImVec2(0, 0));

			uint32_t MemorySize = 0x1000;
			if (uint8_t* MemoryLocation = (uint8_t*)g_LocalPlayer.m_Ped)
			{
				ImGui::PushFont(Renderer::DefaultFont);

				for (uint32_t i = 0; i < MemorySize; i += 16)
				{
					ImGui::Text("%04X ", i);
					for (uint32_t j = 0; j < 16; j++)
					{
						if (j == 8)
						{
							ImGui::SameLine();
							ImGui::Text("");
						}

						ImGui::SameLine();
						uint8_t Byte = MemoryLocation[i + j];
						if (Byte)
							ImGui::Text("%02X", Byte);
						else
							ImGui::TextDisabled("%02X", Byte);
					}

					ImGui::SameLine();
					ImGui::Text("");

					for (uint32_t j = 0; j < 16; j++)
					{
						if (j == 8)
						{
							ImGui::SameLine();
							ImGui::Text("");
						}

						ImGui::SameLine();
						uint8_t Byte = MemoryLocation[i + j];
						if (Byte < 32 || Byte > 126)
							ImGui::TextDisabled(".");
						else
							ImGui::Text("%c", Byte);
					}
				}

				ImGui::PopFont();
			}

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderTeleportMenu()
	{
		ImGui::Text("Teleports");

		ImGui::BeginChild("teleport_menu", ImVec2(0, 0));

		for (const auto& s : g_TeleportList)
		{
			if (ImGui::Selectable(s.first.c_str()))
				Features::TeleportOnGround(s.second.x, s.second.y, s.second.z);
		}

		ImGui::EndChild();
	}
}
