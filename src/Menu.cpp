#include "pch.h"
#include "Menu.h"
#include "Renderer.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "PlayerInfo.h"
#include "Lists.h"
#include "Signature.h"
#include "ScriptGlobal.h"

namespace Menu
{
	void RenderMenu()
	{
		TRY
		{
			if (IsOpen)
			{
				ImGui::SetNextWindowSize(ImVec2(700, 550), ImGuiCond_FirstUseEver);
				if (ImGui::Begin("RDRMenu2", &IsOpen))
				{
					ImGui::BeginTabBar("tab_bar");
					RenderPlayerTab();
					RenderTeleportTab();
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

				if (g_Settings["enable_imgui_demo"].get<bool>())
					ImGui::ShowDemoWindow(g_Settings["enable_imgui_demo"].get<bool*>());

				if (g_Settings["render_clothing_menu"].get<bool>())
					RenderClothingMenu();
			}

			if (g_Settings["enable_overlay"].get<bool>())
				RenderOverlay();
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
				QUEUE_JOB(=)
				{
					Features::SetMoney((int)(Money * 100.f));
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Add###add_money"))
			{
				QUEUE_JOB(=)
				{
					Features::AddMoney((int)(Money * 100.f));
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove###remove_money"))
			{
				QUEUE_JOB(=)
				{
					Features::RemoveMoney((int)(Money * 100.f));
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
			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button("Clean###clean_mount"))
				Features::CleanPed(g_LocalPlayer.m_Mount);

			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			if (ImGui::Checkbox("God Mode###mount_god_mode", g_Settings["mount_god_mode"].get<bool*>()) && !g_Settings["mount_god_mode"].get<bool>())
			{
				QUEUE_JOB(=)
				{
					ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Mount, false);
				}
				END_JOB()
			}

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Checkbox("Gold Cores###mount_gold_cores", g_Settings["mount_gold_cores"].get<bool*>());

			ImGui::EndGroup();
			ImGui::Separator();

			ImGui::Text("Scale");
			static float MountScale = 1.0f;
			ImGui::PushItemWidth(300.0f);
			if (ImGui::SliderFloat("###mount_scale", &MountScale, 0.01f, 10.0f, "%.2f"))
			{
				QUEUE_JOB(=)
				{
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Mount, MountScale);
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset###reset_mount_scale"))
			{
				QUEUE_JOB(=)
				{
					MountScale = 1.0f;
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Mount, MountScale);
				}
				END_JOB()
			}

			if (MountDisabled)
				ImGui::EndDisabled();
		}
			
		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderTeleportTab()
	{
		if (!ImGui::BeginTabItem("Teleport"))
			return;

		ImGui::Text("Teleports");
		ImGui::Separator();
		ImGui::BeginChild("teleport_child");

		for (const auto& s : g_TeleportList)
		{
			const auto& name = s.first.c_str();
			const auto& pos = s.second;
			if (ImGui::Selectable(name))
				Features::TeleportOnGround(pos.x, pos.y, pos.z);
		}

		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderWeaponTab()
	{
		if (!ImGui::BeginTabItem("Weapon"))
			return;

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
			QUEUE_JOB(=)
			{
				Vehicle veh = Features::SpawnVehicle(GATLING_GUN);
				Features::EndSpawnVehicle(GATLING_GUN, veh);
			}
			END_JOB()
		}

		if (ImGui::Button("Drop Current Weapon"))
			Features::DropCurrentWeapon();

		if (ImGui::Button("Remove All Weapons"))
		{
			QUEUE_JOB(=)
			{
				WEAPON::REMOVE_ALL_PED_WEAPONS(g_LocalPlayer.m_Entity, true, true);
			}
			END_JOB()
		}

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::Checkbox("Infinite Ammo", g_Settings["infinite_ammo"].get<bool*>());
			
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("RGB Electric Lantern", g_Settings["rgb_elec_lantern"].get<bool*>());

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::Text("Weapon List");
		ImGui::BeginChild("weapon_list_menu", ImVec2(270, 0));

		static Hash CurrentWeapon = WEAPON_REVOLVER_DOUBLEACTION_EXOTIC;
		for (const auto& w : g_WeaponList)
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
			Features::GiveWeapon(CurrentWeapon);
		ImGui::SameLine();
		if (ImGui::Button("Remove Weapon"))
			Features::RemoveWeapon(CurrentWeapon);

		ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::EndTabItem();
	}

	void RenderInventoryTab()
	{
		if (!ImGui::BeginTabItem("Inventory"))
			return;
		
		ImGui::BeginChild("inventory_child");

		if (ImGui::Button("Give All Items"))
		{
			Features::GiveAllConsumables();
			Features::GiveAllDocuments();
			Features::GiveAllProvisions();
			Features::GiveAllWeapons();
			Features::GiveAllAmmo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Give All Item Requests"))
			Features::GiveAllItemRequests();
		ImGui::SameLine();
		if (ImGui::Button("Unlock All Herbs"))
		{
			QUEUE_JOB(=)
			{
				const Vector3& pos = g_LocalPlayer.m_Pos;
				for (const auto& h : g_HerbList)
					COMPENDIUM::COMPENDIUM_HERB_PICKED(h, pos.x, pos.y, pos.z);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Give All Perks"))
		{
			QUEUE_JOB(=)
			{
				for (const auto& [name, hash] : g_ProvisionList)
				{
					if (name.find("Talisman") == std::string::npos && name.find("Trinket") == std::string::npos)
						continue;

					Features::GiveInventoryItem(hash);
				}
			}
			END_JOB()
		}

		ImGui::Separator();
			
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Give Consumable Items");
		ImGui::SameLine();
		static char ConBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_con", ConBuffer, 200);
		ImGui::SameLine();
		if (ImGui::Button("Give All Consumables"))
			Features::GiveAllConsumables();
		ImGui::BeginChild("consumable_menu", ImVec2(0, 200));
		{
			std::string s1(ConBuffer);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::toupper);
			for (const auto& c : g_ConsumableList)
			{
				std::string s2(c.first);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::toupper);
				if (s2.find(s1) == std::string::npos)
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
		}
		ImGui::EndChild();
		ImGui::Separator();
			
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Give Provision Items");
		ImGui::SameLine();
		static char ProBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_pro", ProBuffer, 200);
		ImGui::SameLine();
		if (ImGui::Button("Give All Provisions"))
			Features::GiveAllProvisions();
		ImGui::BeginChild("provision_menu", ImVec2(0, 200));
		{
			std::string s1(ProBuffer);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::toupper);
			for (const auto& p : g_ProvisionList)
			{
				std::string s2(p.first);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::toupper);
				if (s2.find(s1) == std::string::npos)
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
		}
		ImGui::EndChild();
		ImGui::Separator();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Give Document Items");
		ImGui::SameLine();
		static char DocBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_doc", DocBuffer, 200);
		ImGui::SameLine();
		if (ImGui::Button("Give All Documents"))
			Features::GiveAllDocuments();
		ImGui::BeginChild("document_menu", ImVec2(0, 200));
		{
			std::string s1(DocBuffer);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::toupper);
			for (const auto& d : g_DocumentList)
			{
				std::string s2(d.first);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::toupper);
				if (s2.find(s1) == std::string::npos)
					continue;

				if (ImGui::Selectable(d.first.c_str()))
				{
					QUEUE_JOB(&d)
					{
						Features::GiveInventoryItem(d.second, 99);
					}
					END_JOB()
				}
			}
		}
		ImGui::EndChild();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderWorldTab()
	{
		if (!ImGui::BeginTabItem("World"))
			return;
		
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
			Features::RevealMap();
		
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
				QUEUE_JOB(=)
				{
					MISC::SET_WEATHER_TYPE(hash, true, true, false, 0.0f, false);
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
				Features::SetSnowType(s.second);
		}
		ImGui::EndChild();
		ImGui::EndGroup();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderSpawningTab()
	{
		if (!ImGui::BeginTabItem("Spawning"))
			return;
		
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
					Features::EndSpawnPed(ped);
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

	void RenderDebugTab()
	{
		if (!ImGui::BeginTabItem("Debug"))
			return;
		
		ImGui::BeginChild("debug_child");

		Vector3 DebugPos = g_LocalPlayer.m_Pos;
		bool DebugTeleport = false;
		constexpr float ArrowButtonSpeed = 50.0f;
		ImGui::PushItemWidth(200.0f);
		ImGui::PushButtonRepeat(true);
		
		if (ImGui::ArrowButton("###debug_x-", ImGuiDir_Left))
		{
			DebugTeleport = true;
			DebugPos.x -= ArrowButtonSpeed;
		}
		ImGui::SameLine();
		DebugTeleport |= ImGui::DragFloat("X###debug_x", &DebugPos.x);
		ImGui::SameLine();
		if (ImGui::ArrowButton("###debug_x+", ImGuiDir_Right))
		{
			DebugTeleport = true;
			DebugPos.x += ArrowButtonSpeed;
		}

		if (ImGui::ArrowButton("###debug_y-", ImGuiDir_Left))
		{
			DebugTeleport = true;
			DebugPos.y -= ArrowButtonSpeed;
		}
		ImGui::SameLine();
		DebugTeleport |= ImGui::DragFloat("Y###debug_y", &DebugPos.y);
		ImGui::SameLine();
		if (ImGui::ArrowButton("###debug_y+", ImGuiDir_Right))
		{
			DebugTeleport = true;
			DebugPos.y += ArrowButtonSpeed;
		}

		if (ImGui::ArrowButton("###debug_z-", ImGuiDir_Left))
		{
			DebugTeleport = true;
			DebugPos.z -= ArrowButtonSpeed;
		}
		ImGui::SameLine();
		DebugTeleport |= ImGui::DragFloat("Z###debug_z", &DebugPos.z);
		ImGui::SameLine();
		if (ImGui::ArrowButton("###debug_z+", ImGuiDir_Right))
		{
			DebugTeleport = true;
			DebugPos.z += ArrowButtonSpeed;
		}

		ImGui::PopButtonRepeat();
		ImGui::PopItemWidth();
		if (DebugTeleport)
		{
			QUEUE_JOB(=)
			{
				ENTITY::SET_ENTITY_COORDS(g_LocalPlayer.m_Entity, DebugPos.x, DebugPos.y, DebugPos.z - 1, false, false, false, false);
			}
			END_JOB()
		}
		if (ImGui::Checkbox("Freeze Position", g_Settings["freeze_player"].get<bool*>()) && !g_Settings["freeze_player"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Entity, false);
			}
			END_JOB()
		}

		if (ImGui::Button("Copy Coords"))
		{
			//ImGui::SetClipboardText("%.2ff, %.2ff, %.2ff");
			ImGui::LogToClipboard();
			ImGui::LogText("%.2ff, %.2ff, %.2ff", g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);
			ImGui::LogFinish();
		}
		ImGui::SameLine();
		ImGui::Text("%.2f, %.2f, %.2f", g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);

		ImGui::SameLine();
		ImGui::ColorButton("RGB Color", Features::GetImGuiRGB(),ImGuiColorEditFlags_Uint8);
		ImGui::Separator();

		ImGui::BeginGroup();
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

		uint64_t nhash = 0xBD5DD5EAE2B6CE14; // 0xB980061DA992779D; // 0xED40380076A31506; // 0xA86D5F069399F44D; // 0x25ACFC650B65C538;
		auto addr = (uintptr_t)NativeContext::GetHandler(nhash);
		auto off = addr - g_BaseAddress;
		ImGui::AlignTextToFramePadding();
		ImGui::Text("0x%llX handler: RDR2.exe+0x%X", nhash, off);

		ImGui::SameLine();
		if (ImGui::Button("Print to console"))
			printf("0x%llX handler: RDR2.exe+0x%llX (0x%llX).\n", nhash, off, (off + 0x7FF73CAB0000 /*imagebase in ida*/));

		if (ImGui::Button("Copy IDA Address"))
		{
			ImGui::LogToClipboard();
			ImGui::LogText("%llX", off + 0x7FF73CAB0000 /*imagebase in ida*/);
			ImGui::LogFinish();
		}
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		auto Global_35 = ScriptGlobal(35).Get<Ped*>();
		ImGui::Text("Global_35: 0x%llX", Global_35);
		if (Global_35)
			ImGui::Text("Global_35 = %u", *Global_35);
		ImGui::Text("g_LocalPlayer.m_Entity = %u", g_LocalPlayer.m_Entity);
		auto Global_1946054_f_1 = ScriptGlobal(1946054).At(1).Get<Hash*>();
		ImGui::Text("Global_1946054.f_1: 0x%llX", Global_1946054_f_1);
		if (Global_1946054_f_1)
		{
			ImGui::Text("Global_1946054.f_1 = %u", *Global_1946054_f_1);
			ImGui::Text("Global_1946054.f_1 = %s", Features::GetPedModelName(*Global_1946054_f_1));
		}

		ImGui::EndGroup();
		ImGui::Separator();

		ImGui::Text("\xE2\x84\xAE \xE2\x84\x85 \xE2\x88\x91 \xE2\x86\x95 \xC6\xB1");
		ImGui::Text("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9\n");
		ImGui::Separator();

		if (ImGui::Button("Get Height"))
		{
			QUEUE_JOB(=)
			{
				printf("Ped height: %.2f.\n", PED::_GET_PED_HEIGHT(g_LocalPlayer.m_Entity));
			}
			END_JOB();
		}
		ImGui::SameLine();
		if (ImGui::Button("A Quiet Time Cutscene"))
		{
			QUEUE_JOB(=)
			{
				const char* animDict = "cutscene@SAL1_INT";
				auto iLocal_31 = ANIMSCENE::_CREATE_ANIM_SCENE(animDict, 0, 0, false, true);

				//Ped arthur = Features::SpawnPed(PLAYER_ZERO);
				Ped arthur = Features::SpawnPed(PLAYER_THREE);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "ARTHUR", arthur, 0);

				Ped dutch = Features::SpawnPed(CS_DUTCH);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "DUTCH", dutch, 0);

				Ped molly = Features::SpawnPed(CS_MOLLYOSHEA);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "MollyOshea", molly, 0);

				Ped lenny = Features::SpawnPed(CS_LENNY);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "Lenny", lenny, 0);

				for (int i = 0; i < 20 && !ANIMSCENE::IS_ANIM_SCENE_LOADED(iLocal_31, true, false); i++)
				{
					ANIMSCENE::LOAD_ANIM_SCENE(iLocal_31);
					Features::YieldThread();
				}
				ANIMSCENE::START_ANIM_SCENE(iLocal_31);

				while (!ANIMSCENE::IS_ANIM_SCENE_FINISHED(iLocal_31, false))
				{
					Features::YieldThread();
				}

				PED::DELETE_PED(&arthur);
				PED::DELETE_PED(&dutch);
				PED::DELETE_PED(&molly);
				PED::DELETE_PED(&lenny);
			}
			END_JOB()
		}

		if (ImGui::Button("Corrupt scrNativeCallContext::SetVectorResults"))
			*(Signature("8B 41 18 4C 8B C1 85 C0").Get<uint8_t*>()) = 0xC3;

		if (ImGui::Button("Change Player Model"))
			Features::SetPlayerModel();
		ImGui::SameLine();
		if (ImGui::Button("Reset Player Model"))
			Features::ResetPlayerModel();

		if (ImGui::Button("Knock Off Ped Prop"))
		{
			QUEUE_JOB(=)
			{
				PED::KNOCK_OFF_PED_PROP(g_LocalPlayer.m_Entity, false, true, false, true);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Reload Lists"))
		{
			QUEUE_JOB(=)
			{
				Lists::Reload();
			}
			END_JOB()
		}
		
		if (ImGui::Button("Get Component Categories"))
		{
			QUEUE_JOB(=)
			{
				printf("=== BEGIN METAPED COMPONENT CATEGORIES ===\n");
				const int num = PED::_GET_NUM_COMPONENT_CATEGORIES_IN_PED(g_LocalPlayer.m_Entity);
				for (int i = 0; i < num; i++)
				{
					Hash h = PED::_GET_PED_COMPONENT_CATEGORY_BY_INDEX(g_LocalPlayer.m_Entity, i);
					printf("%u", h);
					const char* name = HUD::GET_STRING_FROM_HASH_KEY(h);
					if (name && name[0])
						printf(" %s", name);
					printf("\n");
				}
				printf("=== END METAPED COMPONENT CATEGORIES ===\n");
			}
			END_JOB()
		}

		ImGui::Separator();
		ImGui::Text("Beta Cutscenes");

		if (ImGui::Button("A Test of Faith II (Arthur)"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayDinoLadyCutscene();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("A Fisher of Fish II (Arthur)"))
		{
			QUEUE_JOB(=)
			{
				for (int i = 0; i <= 2; i++)
					Features::PlayFishCollectorCutscene(i);
			}
			END_JOB()
		}

		if (ImGui::Button("The Gilded Cage"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayIndustryCutscene();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Annesburg Jail Breakout With Charles"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayAnnesburgBreakoutCutscene();
			}
			END_JOB()
		}
		
		if (ImGui::Button("Jack Cutscene (John)"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayJackCutscene();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Charles Leaving To Natives (Chapter 4)"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayCharlesLeavingCutscene();
			}
			END_JOB()
		}
		//ImGui::SameLine();
		//if (ImGui::Button("Spawn Charles Horse"))
		//{
		//	QUEUE_JOB(=)
		//	{
		//		Ped ped = Features::SpawnPed(A_C_HORSE_GANG_CHARLES);
		//		int id = PED::_REQUEST_METAPED_OUTFIT(A_C_HORSE_GANG_CHARLES, 0x4B96E611);
		//		while (!PED::_HAS_METAPED_OUTFIT_LOADED(id))
		//			Features::YieldThread();
		//		PED::_APPLY_PED_METAPED_OUTFIT(id, ped, true, false);
		//		PED::_RELEASE_METAPED_OUTFIT_REQUEST(id);
		//	}
		//	END_JOB()
		//}

		ImGui::Separator();

		if (ImGui::BeginCombo("HUD Context", HUDContextList[CurCtx]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(HUDContextList); i++)
			{
				if (ImGui::Selectable(HUDContextList[i], i == CurCtx))
					CurCtx = i;
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Enable"))
		{
			QUEUE_JOB(=)
			{
				HUD::_ENABLE_HUD_CONTEXT(joaat(HUDContextList[CurCtx]));
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Disable"))
		{
			QUEUE_JOB(=)
			{
				HUD::_DISABLE_HUD_CONTEXT(joaat(HUDContextList[CurCtx]));
			}
			END_JOB()
		}

		ImGui::Separator();
		ImGui::BeginGroup();
		ImGui::Checkbox("Log Ped Spawning", g_Settings["log_ped_spawning"].get<bool*>());
		ImGui::Checkbox("Log Human Spawning", g_Settings["log_human_spawning"].get<bool*>());
		ImGui::Checkbox("Log Vehicle Spawning", g_Settings["log_vehicle_spawning"].get<bool*>());
		ImGui::Checkbox("Log Added Inventory Items", g_Settings["log_added_inventory_items"].get<bool*>());
		if (ImGui::Checkbox("Enable Freecam", &EnableFreeCam) && !EnableFreeCam)
		{
			QUEUE_JOB(=)
			{
				CAM::SET_CAM_ACTIVE(CamEntity, false);
				CAM::RENDER_SCRIPT_CAMS(false, true, 500, true, true, 0);
				CAM::DESTROY_CAM(CamEntity, false);
				STREAMING::SET_FOCUS_ENTITY(g_LocalPlayer.m_Entity);

				ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Vehicle, false);

				CamEntity = 0;
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Log Created Cutscenes", g_Settings["log_created_cutscenes"].get<bool*>());
		ImGui::Checkbox("Log Set Decor", g_Settings["log_set_decor"].get<bool*>());
		ImGui::Checkbox("Enable ImGui Demo Window", g_Settings["enable_imgui_demo"].get<bool*>());
		ImGui::Checkbox("Enable Overlay", g_Settings["enable_overlay"].get<bool*>());
		ImGui::EndGroup();
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Ped Debug"))
			RenderPedDebug();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderLoggerTab()
	{
		if (!ImGui::BeginTabItem("Logger"))
			return;
		
		ImGui::BeginChild("logger_child");

		Logger.Draw();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderMemoryTab()
	{
		if (!ImGui::BeginTabItem("Memory"))
			return;
		
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

	void RenderPlayerButtons()
	{
		ImGui::BeginGroup();

		if (ImGui::Button("Clear Wanted", ImVec2(150, 0)))
		{
			QUEUE_JOB(=)
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
			Features::TeleportToWaypoint();

		ImGui::PushButtonRepeat(true);
		if (ImGui::Button("TP Through Door", ImVec2(150, 0)))
		{
			QUEUE_JOB(=)
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
				QUEUE_JOB(=)
				{
					PED::SET_PED_ONTO_MOUNT(g_LocalPlayer.m_Entity, g_LocalPlayer.m_LastMount, -1, true);
				}
				END_JOB()
			}
		}

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (ImGui::Button("Suicide", ImVec2(70, 0)))
		{
			QUEUE_JOB(=)
			{
				ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Entity, 0, 0);
			}
			END_JOB()
		}

		if (ImGui::Button("Clone", ImVec2(70, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::CLONE_PED(g_LocalPlayer.m_Entity, false, false, true);
			}
			END_JOB()
		}

		if (ImGui::Button("Clean", ImVec2(70, 0)))
			Features::CleanPed(g_LocalPlayer.m_Entity);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		
		if (ImGui::Button("Spawn Good Honor Enemy", ImVec2(220, 0)))
		{
			QUEUE_JOB(=)
			{
				Features::SpawnGoodHonorEnemy();
			}
			END_JOB()
		}

		if (ImGui::Button("Spawn Bad Honor Enemy", ImVec2(220, 0)))
		{
			QUEUE_JOB(=)
			{
				Features::SpawnBadHonorEnemy();
			}
			END_JOB()
		}

		ImGui::EndGroup();
	}

	void RenderPlayerCheckboxes()
	{
		ImGui::BeginGroup();
		if (ImGui::Checkbox("God Mode", g_Settings["god_mode"].get<bool*>()) && !g_Settings["god_mode"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Features::SetGodmode(false);
			}
			END_JOB()
		}
		ImGui::Checkbox("Clean###clplcb", g_Settings["clean_player"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Never Wanted", g_Settings["never_wanted"].get<bool*>());
		ImGui::Checkbox("Super Run", g_Settings["super_run"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Super Jump", g_Settings["super_jump"].get<bool*>());
		if (ImGui::Checkbox("No Ragdoll", g_Settings["no_ragdoll"].get<bool*>()) && !g_Settings["no_ragdoll"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Features::SetNoRagdoll(false);
			}
			END_JOB()
		}

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("No Sliding", g_Settings["no_sliding"].get<bool*>());
		ImGui::Checkbox("Clothing Menu", g_Settings["render_clothing_menu"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Gold Cores", g_Settings["gold_cores"].get<bool*>());
		ImGui::EndGroup();
	}

	void RenderPlayerModelChanger()
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Set Player Model");
		ImGui::SameLine();
		static char ModBuffer[200];
		ImGui::SetNextItemWidth(250.0f);
		ImGui::InputText("###filter_ped", ModBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
		ImGui::SameLine();
		if (ImGui::Button("Set###set_model_manually"))
			Features::SetPlayerModel(joaat(ModBuffer));

		const float width = ImGui::GetContentRegionAvail().x / 2;
		ImGui::BeginChild("player_model_child", ImVec2(width, 200));
		for (const auto& p : g_PedList)
		{
			if (p.first.find(ModBuffer) == std::string::npos)
				continue;

			if (ImGui::Selectable(p.first.c_str()))
				Features::SetPlayerModel(p.second);
		}
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("player_model_preset_child", ImVec2(0, 200));
		if (ImGui::Selectable("Arthur"))
			Features::SetPlayerModel(PLAYER_ZERO);
		if (ImGui::Selectable("John"))
			Features::SetPlayerModel(PLAYER_THREE);
		if (ImGui::Selectable("Dutch"))
			Features::SetPlayerModel(CS_DUTCH);
		if (ImGui::Selectable("Robot"))
			Features::SetPlayerModel(CS_CRACKPOTROBOT);
		if (ImGui::Selectable("Naked Woman"))
			Features::SetPlayerModel(U_F_M_RHDNUDEWOMAN_01);
		if (ImGui::Selectable("Naked Man"))
			Features::SetPlayerModel(RE_NAKEDSWIMMER_MALES_01);
		if (ImGui::Selectable("Clan Member (1)"))
			Features::SetPlayerModel(RE_RALLY_MALES_01);
		if (ImGui::Selectable("Clan Member (2)"))
			Features::SetPlayerModel(RE_RALLYDISPUTE_MALES_01);
		if (ImGui::Selectable("Clan Member (3)"))
			Features::SetPlayerModel(RE_RALLYSETUP_MALES_01);
		if (ImGui::Selectable("2 Headed Skeleton (1)"))
			Features::SetPlayerModel(U_M_M_CIRCUSWAGON_01);
		if (ImGui::Selectable("2 Headed Skeleton (2)"))
			Features::SetPlayerModel(U_F_M_CIRCUSWAGON_01);
		if (ImGui::Selectable("Magnifico"))
			Features::SetPlayerModel(CS_MAGNIFICO);
		if (ImGui::Selectable("Bertram"))
			Features::SetPlayerModel(CS_ODDFELLOWSPINHEAD);
		if (ImGui::Selectable("Strange Man"))
			Features::SetPlayerModel(CS_MYSTERIOUSSTRANGER);
		if (ImGui::Selectable("Vampire"))
			Features::SetPlayerModel(CS_VAMPIRE);
		if (ImGui::Selectable("Swamp Freak"))
			Features::SetPlayerModel(CS_SWAMPFREAK);
		if (ImGui::Selectable("Jack"))
			Features::SetPlayerModel(CS_JACKMARSTON);
		if (ImGui::Selectable("Jack (Teen)"))
			Features::SetPlayerModel(CS_JACKMARSTON_TEEN);
		if (ImGui::Selectable("Gavin"))
			Features::SetPlayerModel(CS_GAVIN);
		ImGui::EndChild();
	}

	void RenderPedDebug()
	{
		const bool PedSpawned = PedDebug.ent;

		ImGui::PushItemWidth(350.0f);
		ImGui::BeginGroup();

		// Ped controls
		if (ImGui::Button("Spawn"))
		{
			QUEUE_JOB(=)
			{
				PedDebug.ent = Features::SpawnPed(PedDebug.model);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(PedDebug.model);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (!PedSpawned)
			ImGui::BeginDisabled();
		if (ImGui::Button("Despawn"))
		{
			QUEUE_JOB(=)
			{
				Features::DeletePed(PedDebug.ent);
				Features::EndSpawnPed(PedDebug.ent);
				PedDebug.ent = 0;
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Resurrect"))
		{
			QUEUE_JOB(=)
			{
				PED::RESURRECT_PED(PedDebug.ent);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Revive"))
		{
			QUEUE_JOB(=)
			{
				PED::REVIVE_INJURED_PED(PedDebug.ent);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Random Component Variations"))
		{
			QUEUE_JOB(=)
			{
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(PedDebug.ent, 0);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Random Outfit Variations"))
		{
			QUEUE_JOB(=)
			{
				PED::_SET_RANDOM_OUTFIT_VARIATION(PedDebug.ent, true);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Good Honor"))
		{
			QUEUE_JOB(=)
			{
				DECORATOR::DECOR_SET_INT(PedDebug.ent, "honor_override", -9999);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Set Bad Honor"))
		{
			QUEUE_JOB(=)
			{
				DECORATOR::DECOR_SET_INT(PedDebug.ent, "honor_override", 9999);
			}
			END_JOB()
		}

		if (ImGui::Button("TP Player To Ped"))
			Features::TeleportOnGround(PedDebug.pos);
		ImGui::SameLine();
		if (ImGui::Button("TP Ped To Player"))
		{
			QUEUE_JOB(=)
			{
				const Vector3& pos = g_LocalPlayer.m_Pos;
				ENTITY::SET_ENTITY_COORDS(PedDebug.ent, pos.x, pos.y, pos.z, false, false, false, false);
			}
			END_JOB()
		}
		if (!PedSpawned)
			ImGui::EndDisabled();

		ImGui::EndGroup();
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Freeze", &PedDebug.freeze);
		ImGui::Checkbox("Invincible", &PedDebug.invincible);
		ImGui::Checkbox("Visible", &PedDebug.visible);

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		// Ped Status
		if (!PedSpawned)
			ImGui::BeginDisabled();

		ImGui::Text("Entity Index: %u (0x%X)", PedDebug.ent, PedDebug.ent);
		ImGui::Text("Coords: %.2f, %.2f, %.2f", PedDebug.pos.x, PedDebug.pos.y, PedDebug.pos.z);
		ImGui::Text("Heading: %.2f", PedDebug.head);
		ImGui::Text("Health: %d/%d", PedDebug.health, PedDebug.max_health);
		ImGui::Text("Injured: %s", (PedDebug.injured ? "true" : "false"));

		if (!PedSpawned)
			ImGui::EndDisabled();

		ImGui::EndGroup();
	}

	void RenderOverlay()
	{
		TRY
		{
			bool* p_open = g_Settings["enable_overlay"].get<bool*>();
			static int location = 0;
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			if (location >= 0)
			{
				const float PAD = 10.0f;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
				ImVec2 work_size = viewport->WorkSize;
				ImVec2 window_pos, window_pos_pivot;
				window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				window_flags |= ImGuiWindowFlags_NoMove;
			}
			else if (location == -2)
			{
				// Center window
				ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
				window_flags |= ImGuiWindowFlags_NoMove;
			}
			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			if (ImGui::Begin("###overlay", p_open, window_flags))
			{
				RenderOverlayMain();
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
					if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
					if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
					if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
					if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
					if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
					if (p_open && ImGui::MenuItem("Close")) *p_open = false;
					ImGui::EndPopup();
				}
			}
			ImGui::End();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RenderOverlayMain()
	{
		TRY
		{
			ImGuiIO& io = ImGui::GetIO();
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.0f, %.0f)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse Position: <invalid>");
			ImGui::Text("Framerate: %.1f (%.3f ms)", io.Framerate, 1000.0f / io.Framerate);
			ImGui::Text("%.2f, %.2f, %.2f", g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RenderClothingMenu()
	{
		if (!ImGui::Begin("Clothing", g_Settings["render_clothing_menu"].get<bool*>()))
			ImGui::End();
		
		ImGui::Text("Scale");
		static float PlayerScale = 1.0f;
		ImGui::PushItemWidth(300.0f);
		if (ImGui::SliderFloat("###player_scale", &PlayerScale, 0.01f, 10.0f, "%.2f"))
		{
			QUEUE_JOB(=)
			{
				PED::_SET_PED_SCALE(g_LocalPlayer.m_Entity, PlayerScale);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			QUEUE_JOB(=)
			{
				PlayerScale = 1.0f;
				PED::_SET_PED_SCALE(g_LocalPlayer.m_Entity, PlayerScale);
			}
			END_JOB()
		}
		ImGui::Separator();
		
		ImGui::BeginGroup();
		if (ImGui::Button("Remove Gunbelt", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("GUNBELTS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Knife Holster", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("HOLSTERS_KNIFE");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Right Holster", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("HOLSTERS_RIGHT");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Satchel Strap", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("SATCHEL_STRAPS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Hat", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("HATS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Left Holster", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("HOLSTERS_LEFT");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Satchel", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("SATCHELS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Knock Off Hat", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::KNOCK_OFF_PED_PROP(g_LocalPlayer.m_Entity, false, true, false, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Accessories", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("ACCESSORIES");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Belt", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("BELTS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Eyewear", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("EYEWEAR");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Gloves", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("GLOVES");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		
		ImGui::BeginGroup();
		if (ImGui::Button("Remove Hair", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("HAIR");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Crossdraw Holster", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("HOLSTERS_CROSSDRAW");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Loadouts", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("LOADOUTS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Large Mask", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("MASKS_LARGE");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Neckwear", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("NECKWEAR");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Mask", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = joaat("WEARABLE_MASKS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Everything", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				for (int i = 0; i < PED::_GET_NUM_COMPONENT_CATEGORIES_IN_PED(g_LocalPlayer.m_Entity); i++)
				{
					Hash category = PED::_GET_PED_COMPONENT_CATEGORY_BY_INDEX(g_LocalPlayer.m_Entity, i);
					if (PED::_IS_METAPED_USING_COMPONENT(g_LocalPlayer.m_Entity, category))
						PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, category, 1);
				}
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		
		if (ImGui::Button("Set Legend Of The East Outfit"))
		{
			QUEUE_JOB(=)
			{
				Hash model = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity);
				if (model == PLAYER_ZERO)
					PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, 13, false);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Set Naked Outfit"))
		{
			QUEUE_JOB(=)
			{
				Hash model = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity);
				if (model == PLAYER_ZERO)
					PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, 14, false);
				else if (model == PLAYER_THREE)
					PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, 28, false);
			}
			END_JOB()
		}
		ImGui::Separator();

		static std::vector<std::string> labels;
		labels.clear();
		for (int i = 0; i < PED::_GET_NUM_COMPONENT_CATEGORIES_IN_PED(g_LocalPlayer.m_Entity); i++)
		{
			Hash category = PED::_GET_PED_COMPONENT_CATEGORY_BY_INDEX(g_LocalPlayer.m_Entity, i);
			if (PED::_IS_METAPED_USING_COMPONENT(g_LocalPlayer.m_Entity, category))
			{
				std::string label("Remove ");
				label += std::to_string(category);
				labels.push_back(label);
				if (ImGui::Button(labels[labels.size() - 1].c_str()))
				{
					QUEUE_JOB(=)
					{
						PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, category, 1);
						PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
					}
					END_JOB()
				}
			}
		}

		ImGui::Separator();
		ImGui::Text("Set Outfit");
		static int Outfit = 0;
		static bool KeepAcc = false;
		ImGui::PushButtonRepeat(true);
		if (ImGui::ArrowButton("###lof", ImGuiDir_Left))
		{
			QUEUE_JOB(=)
			{
				if (Outfit == 0)
					++Outfit;
				PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, --Outfit, KeepAcc);
			}
			END_JOB()
		}
		ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("%d", Outfit);
		ImGui::SameLine();
		if (ImGui::ArrowButton("###rof", ImGuiDir_Right))
		{
			QUEUE_JOB(=)
			{
				if (Outfit == PED::GET_NUM_META_PED_OUTFITS(g_LocalPlayer.m_Entity) - 1)
					--Outfit;
				PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, ++Outfit, KeepAcc);
			}
			END_JOB()
		}
		ImGui::PopButtonRepeat();
		ImGui::SameLine();
		ImGui::Checkbox("p2 (Keep Accessories)", &KeepAcc);
		ImGui::Separator();

		RenderPlayerModelChanger();

		ImGui::End();
	}
}
