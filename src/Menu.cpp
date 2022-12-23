#include "pch.h"
#include "Menu.h"
#include "Renderer.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "PlayerInfo.h"
#include "Lists.h"

namespace Menu
{
	void RenderMenu()
	{
		TRY
		{
			ImGui::SetNextWindowSize(ImVec2(700, 550), ImGuiCond_FirstUseEver);
			//ImGui::SetNextWindowSize(ImVec2(700, 550), ImGuiCond_Always);
			if (ImGui::Begin("RDRMenu2", &Renderer::MenuOpen/*, ImGuiWindowFlags_NoResize*/))
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

			if (g_Settings["enable_imgui_demo"].get_ref<bool&>())
				ImGui::ShowDemoWindow(g_Settings["enable_imgui_demo"].get<bool*>());
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RenderPlayerTab()
	{
		if (!ImGui::BeginTabItem("Player"))
			return;

		ImGui::BeginChild("player_child");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Player"))
		{
			RenderPlayerButtons();
			ImGui::Separator();
			RenderPlayerCheckboxes();
			ImGui::Separator();

			static float Money = 10000.0f;
			ImGui::PushItemWidth(200.0f);
			ImGui::InputFloat("Money", &Money, 0, 0, "$%.2f");
			ImGui::SameLine();
			if (ImGui::Button("Set###set_money"))
			{
				QUEUE_JOB()
				{
					Features::SetMoney((int)(Money * 100.f));
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Add###add_money"))
			{
				QUEUE_JOB()
				{
					Features::AddMoney((int)(Money * 100.f));
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove###remove_money"))
			{
				QUEUE_JOB()
				{
					Features::RemoveMoney((int)(Money * 100.f));
				}
				END_JOB()
			}

			ImGui::Separator();
			ImGui::Text("Scale");
			static float PlayerScale = 1.0f;
			ImGui::PushItemWidth(300.0f);
			if (ImGui::SliderFloat("###player_scale", &PlayerScale, 0.01f, 10.0f, "%.2f"))
			{
				QUEUE_JOB()
				{
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Entity, PlayerScale);
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				QUEUE_JOB()
				{
					PlayerScale = 1.0f;
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Entity, PlayerScale);
				}
				END_JOB()
			}
		}
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Mount"))
		{
			const bool MountDisabled = !g_LocalPlayer.m_Mount;
			if (MountDisabled)
				ImGui::BeginDisabled();
			ImGui::BeginGroup();

			if (ImGui::Button("Fill Cores###fill_mount"))
				Features::RestoreHorseCores();

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button("Give Gold Cores###gold_mount"))
				Features::GiveGoldCores(g_LocalPlayer.m_Mount);

			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			if (ImGui::Checkbox("God Mode###mount_god_mode", g_Settings["mount_god_mode"].get<bool*>()) && !g_Settings["mount_god_mode"].get_ref<bool&>())
			{
				QUEUE_JOB()
				{
					ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Mount, FALSE);
				}
				END_JOB()
			}

			ImGui::EndGroup();
			ImGui::Separator();

			ImGui::Text("Scale");
			static float MountScale = 1.0f;
			ImGui::PushItemWidth(300.0f);
			if (ImGui::SliderFloat("###mount_scale", &MountScale, 0.01f, 10.0f, "%.2f"))
			{
				QUEUE_JOB()
				{
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Mount, MountScale);
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset###reset_mount_scale"))
			{
				QUEUE_JOB()
				{
					MountScale = 1.0f;
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Mount, MountScale);
				}
				END_JOB()
			}

			if (MountDisabled)
				ImGui::EndDisabled();
		}
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Teleports"))
			RenderTeleportMenu();
			
		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderWeaponTab()
	{
		if (ImGui::BeginTabItem("Weapon"))
		{
			ImGui::BeginChild("weapon_child");

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

			ImGui::Checkbox("Infinite Ammo", g_Settings["infinite_ammo"].get<bool*>());
			
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
			ImGui::BeginChild("inventory_child");

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
			ImGui::SameLine();
			if (ImGui::Button("Give All Items"))
			{
				Features::GiveAllConsumables();
				Features::GiveAllDocuments();
				Features::GiveAllProvisions();
				Features::GiveAllWeapons();
				Features::GiveAllAmmo();
			}
			ImGui::SameLine();
			if (ImGui::Button("Give All Consumables"))
				Features::GiveAllConsumables();
			
			if (ImGui::Button("Give All Documents"))
				Features::GiveAllDocuments();
			ImGui::SameLine();
			if (ImGui::Button("Give All Provisions"))
				Features::GiveAllProvisions();

			ImGui::Separator();
			
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Give Consumable Items");
			ImGui::SameLine();
			static char ConBuffer[200];
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("###filter_con", ConBuffer, 200);
			ImGui::BeginChild("consumable_menu", ImVec2(0, 200));
			for (const auto& c : g_ConsumableList)
			{
				if (c.first.find(ConBuffer) == std::string::npos)
					continue;
			
				if (ImGui::Selectable(c.first.c_str()))
				{
					QUEUE_JOB(&c)
					{
						Features::GiveInventoryItem(c.second, 99);
					}
					END_JOB()
				}
			}
			ImGui::EndChild();
			ImGui::Separator();
			
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Give Provision Items");
			ImGui::SameLine();
			static char ProBuffer[200];
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("###filter_pro", ProBuffer, 200);
			ImGui::BeginChild("provision_menu");
			for (const auto& p : g_ProvisionList)
			{
				if (p.first.find(ProBuffer) == std::string::npos)
					continue;
			
				if (ImGui::Selectable(p.first.c_str()))
				{
					QUEUE_JOB(&p)
					{
						Features::GiveInventoryItem(p.second, 99);
					}
					END_JOB()
				}
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
			ImGui::BeginChild("world_child");
			
			ImGui::BeginGroup();
			if (ImGui::Button("Set Time To Night", ImVec2(180, 0)))
				Features::SetClockTime(0);
			if (ImGui::Button("Set Time To Morning", ImVec2(180, 0)))
				Features::SetClockTime(6);
			if (ImGui::Button("Set Time To Noon", ImVec2(180, 0)))
				Features::SetClockTime(12);
			if (ImGui::Button("Set Time To Evening", ImVec2(180, 0)))
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

			ImGui::Checkbox("No Snipers", g_Settings["no_snipers"].get<bool*>());

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Checkbox("No Black Borders", g_Settings["no_black_borders"].get<bool*>());

			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			ImGui::Text("Set Weather");
			ImGui::BeginChild("weather_list_menu", ImVec2(200, 0));
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
			ImGui::BeginChild("snow_list_menu", ImVec2(200, 0));
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
			ImGui::BeginChild("spawning_child");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Spawn Ped");
			ImGui::SameLine();
			static char PedBuffer[200];
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("###filter_ped", PedBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
			ImGui::BeginChild("ped_menu", ImVec2(0, 200));
			for (const auto& it : g_PedList)
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

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Spawn Vehicle");
			ImGui::SameLine();
			static char VehBuffer[200];
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("###filter_veh", VehBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
			ImGui::BeginChild("vehicle_menu");
			for (const auto& it : g_VehicleList)
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
			ImGui::BeginChild("debug_child");

			// This native should be fine
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
			if (ImGui::Button("Copy Coords"))
			{
				ImGui::LogToClipboard();
				ImGui::LogText("%.2ff, %.2ff, %.2ff", pos.x, pos.y, pos.z);
				ImGui::LogFinish();
			}
			ImGui::SameLine();
			ImGui::Text("%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);

			ImGui::Separator();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("CPed: 0x%llX", g_LocalPlayer.m_Ped);
			ImGui::SameLine();
			if (ImGui::Button("Copy Address"))
			{
				ImGui::LogToClipboard();
				ImGui::LogText("%llX", g_LocalPlayer.m_Ped);
				ImGui::LogFinish();
			}

			ImGui::AlignTextToFramePadding();
			ImGui::Text("GetEntityAddress: 0x%llX", Pointers::GetEntityAddress(g_LocalPlayer.m_Entity));
			ImGui::SameLine();
			if (ImGui::Button("Copy Address"))
			{
				ImGui::LogToClipboard();
				ImGui::LogText("%llX", Pointers::GetEntityAddress(g_LocalPlayer.m_Entity));
				ImGui::LogFinish();
			}

			uint64_t nhash = 0xB980061DA992779D; // 0xED40380076A31506; // 0xA86D5F069399F44D; // 0x25ACFC650B65C538;
			auto addr = (uintptr_t)NativeContext::GetHandler(nhash);
			auto off = addr - g_BaseAddress;
			ImGui::AlignTextToFramePadding();
			ImGui::Text("0x%llX handler: RDR2.exe+0x%X", nhash, off);

			ImGui::SameLine();
			if (ImGui::Button("Print to console"))
				std::cout << LOG_HEX(nhash) << " handler: RDR2.exe+" << LOG_HEX(off) << " (" <<
				LOG_HEX(0x7FF73CAB0000 /*imagebase in ida*/ + off) << ").\n";

			if (ImGui::Button("Copy IDA Address"))
			{
				ImGui::LogToClipboard();
				ImGui::LogText("%llX", 0x7FF73CAB0000 /*imagebase in ida*/ + off);
				ImGui::LogFinish();
			}

			ImGui::Separator();

			if (ImGui::Button("Get Height"))
			{
				QUEUE_JOB()
				{
					std::cout << "Ped height: " << PED::_GET_PED_HEIGHT(g_LocalPlayer.m_Entity) << "\n";
				}
				END_JOB();
			}
			ImGui::SameLine();

			if (ImGui::Button("Change Player Model"))
			{
				QUEUE_JOB()
				{
					Features::RequestModel(U_F_M_RHDNUDEWOMAN_01);
					PLAYER::SET_PLAYER_MODEL(g_LocalPlayer.m_Index, U_F_M_RHDNUDEWOMAN_01, FALSE);
				}
				END_JOB();
			}

			ImGui::Separator();

			ImGui::Checkbox("Log Ped Spawning", g_Settings["log_ped_spawning"].get<bool*>());
			ImGui::Checkbox("Log Human Spawning", g_Settings["log_human_spawning"].get<bool*>());
			ImGui::Checkbox("Log Vehicle Spawning", g_Settings["log_vehicle_spawning"].get<bool*>());
			ImGui::Checkbox("Log Added Inventory Items", g_Settings["log_added_inventory_items"].get<bool*>());
			ImGui::Checkbox("Enable ImGui Demo Window", g_Settings["enable_imgui_demo"].get<bool*>());

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderLoggerTab()
	{
		if (ImGui::BeginTabItem("Logger"))
		{
			ImGui::BeginChild("logger_child");

			Logger.Draw();

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void RenderMemoryTab()
	{
		if (ImGui::BeginTabItem("Memory"))
		{
			ImGui::BeginChild("memory_child");

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

	void RenderPlayerButtons()
	{
		ImGui::BeginGroup();

		if (ImGui::Button("Clear Wanted", ImVec2(150, 0)))
		{
			QUEUE_JOB()
			{
				Features::ClearWanted();
			}
			END_JOB()
		}

		if (ImGui::Button("Fill Cores", ImVec2(150, 0)))
			Features::RestorePlayerCores();

		if (ImGui::Button("Give Gold Cores", ImVec2(150, 0)))
			Features::GiveGoldCores(g_LocalPlayer.m_Entity);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (ImGui::Button("TP To Waypoint", ImVec2(150, 0)))
		{
			QUEUE_JOB()
			{
				Features::TeleportToWaypoint();
			}
			END_JOB()
		}

		ImGui::PushButtonRepeat(true);
		if (ImGui::Button("TP Through Door", ImVec2(150, 0)))
		{
			QUEUE_JOB()
			{
				Features::TeleportThroughDoor();
			}
			END_JOB()
		}
		ImGui::PopButtonRepeat();

		if (g_LocalPlayer.m_Mount || !g_LocalPlayer.m_LastMount)
		{
			ImGui::BeginDisabled();
			ImGui::Button("TP To Last Mount", ImVec2(150, 0));
			ImGui::EndDisabled();
		}
		else
		{
			if (ImGui::Button("TP To Last Mount", ImVec2(150, 0)))
			{
				QUEUE_JOB()
				{
					PED::SET_PED_ONTO_MOUNT(g_LocalPlayer.m_Entity, g_LocalPlayer.m_LastMount, -1, TRUE);
				}
				END_JOB()
			}
		}

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (ImGui::Button("Spawn Good Honor Enemy", ImVec2(220, 0)))
		{
			QUEUE_JOB()
			{
				Features::SpawnGoodHonorEnemy();
			}
			END_JOB()
		}

		if (ImGui::Button("Spawn Bad Honor Enemy", ImVec2(220, 0)))
		{
			QUEUE_JOB()
			{
				Features::SpawnBadHonorEnemy();
			}
			END_JOB()
		}
		
		if (ImGui::Button("Suicide", ImVec2(220, 0)))
		{
			QUEUE_JOB()
			{
				ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Entity, 0, 0);
			}
			END_JOB()
		}

		ImGui::EndGroup();
	}

	void RenderPlayerCheckboxes()
	{
		ImGui::BeginGroup();
		if (ImGui::Checkbox("God Mode", g_Settings["god_mode"].get<bool*>()) && !g_Settings["god_mode"].get_ref<bool&>())
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

		ImGui::Checkbox("Never Wanted", g_Settings["never_wanted"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Super Jump", g_Settings["super_jump"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("No Sliding", g_Settings["no_sliding"].get<bool*>());
		ImGui::EndGroup();
	}

	void RenderTeleportMenu()
	{
		ImGui::BeginChild("teleport_menu");

		for (const auto& s : g_TeleportList)
		{
			const auto& name = s.first.c_str();
			const auto& pos = s.second;
			if (ImGui::Selectable(name))
				Features::TeleportOnGround(pos.x, pos.y, pos.z);
		}

		ImGui::EndChild();
	}
}
