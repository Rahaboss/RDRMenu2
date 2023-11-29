#include "pch.h"
#include "Debug.h"
#include "Renderer/Renderer.h"
#include "Renderer/RGB.h"
#include "Config/Settings.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Spawning.h"
#include "Script/Player.h"
#include "Util/Timer.h"
#include "Script/Ped.h"
#include "Script/PlayerInfo.h"
#include "Rage/natives.h"
#include "Thread/Thread.h"
#include "Renderer/ImGuiExtras.h"
#include "Script/Entity.h"
#include "Rage/ScriptGlobal.h"
#include "Script/Interior.h"
#include "Config/Config.h"

static void RenderInteriorButtons()
{
	ImGui::SeparatorText("Art Gallery");
	if (ImGui::Button("Activate##Art Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleArtInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Art Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleArtInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Art Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 2689.29f, -1203.22f, 56.47f });
		}
		END_JOB()
	}

	ImGui::SeparatorText("Dino Barn");
	if (ImGui::Button("Activate##Dino Barn Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleDinoBarnInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Dino Barn Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleDinoBarnInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Dino Barn Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 196.2653f, 985.5404f, 189.1246f });
		}
		END_JOB()
	}

	ImGui::SeparatorText("Saint Denis Hideout");
	if (ImGui::Button("Activate##Saint Denis Hideout Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleSaintDenisHideoutInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Saint Denis Hideout Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleSaintDenisHideoutInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Saint Denis Hideout Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 2759.03f, -1346.07f, 57.75f });
		}
		END_JOB()
	}
	
	ImGui::SeparatorText("Grand Korrigan");
	if (ImGui::Button("Activate##Grand Korrigan Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Grand Korrigan Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Grand Korrigan Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 3288.49f, -1309.71f, 42.8f });
		}
		END_JOB()
	}
	
	ImGui::SeparatorText("Grand Korrigan Docked");
	if (ImGui::Button("Activate##Grand Korrigan Docked Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanDockedInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Grand Korrigan Docked Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanDockedInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Grand Korrigan Docked Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 2868.71f, -1401.51f, 43.45f });
		}
		END_JOB()
	}

	ImGui::SeparatorText("Winter 1");
	if (ImGui::Button("Activate##Winter 1 Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleWinter1Interior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Winter 1 Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleWinter1Interior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Winter 1 Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ -1981.7f, 3254.33f, 641.47f });
		}
		END_JOB()
	}
}

static void RenderDebugButtons()
{
	if (ImGui::Button("Spawn Ped"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnPed(RAGE_JOAAT("CS_DUTCH"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Spawn Vehicle"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnVehicle(RAGE_JOAAT("WAGON02X"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Spawn Object"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnObject(RAGE_JOAAT("P_STOOL01X"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Spawn Pickup"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnPickup(RAGE_JOAAT("PICKUP_WEAPON_PISTOL_M1899"));
		}
		END_JOB()
	}

	if (ImGui::Button("Change Player Model"))
	{
		QUEUE_JOB(=)
		{
			Script::SetPlayerModel(RAGE_JOAAT("CS_DUTCH"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset Player Model"))
		JobQueue::Add(Script::ResetPlayerModel);

	if (ImGui::Button("Reload Settings"))
		JobQueue::Add(Settings::Create);
	ImGui::SameLine();
	if (ImGui::Button("Reload Lists"))
	{
		QUEUE_JOB(=)
		{
			Lists::Destroy();
			Lists::Create();
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Test"))
	{
		QUEUE_JOB(=)
		{
			Script::SetPlayerModel(RAGE_JOAAT("MSP_SALOON1_FEMALES_01"));
			Script::SetMetaPedOutfit(g_LocalPlayer.m_Entity, 1467286073);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Test2"))
	{
		QUEUE_JOB(=)
		{
			Script::SetPlayerModel(RAGE_JOAAT("MSP_SALOON1_MALES_01"));
			Script::SetMetaPedOutfit(g_LocalPlayer.m_Entity, 1467286073);
		}
		END_JOB()
	}
	
	if (ImGui::Button("Start Music"))
	{
		QUEUE_JOB(=)
		{
			AUDIO::PREPARE_MUSIC_EVENT("SAL1_START");
			AUDIO::TRIGGER_MUSIC_EVENT("SAL1_START");
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("End Music"))
	{
		QUEUE_JOB(=)
		{
			AUDIO::PREPARE_MUSIC_EVENT("SAL1_STOP");
			AUDIO::TRIGGER_MUSIC_EVENT("SAL1_STOP");
		}
		END_JOB()
	}

	if (ImGui::Button("Hair Test 1"))
	{
		QUEUE_JOB(=)
		{
			//PED::_APPLY_SHOP_ITEM_TO_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("CLOTHING_ITEM_HAIR_SWEPT_BACK"), true, false, false);
			Script::SetHairStyle(RAGE_JOAAT("CLOTHING_ITEM_HAIR_SWEPT_BACK"), 7);
			PED::_0xAAB86462966168CE(g_LocalPlayer.m_Entity, false);
			PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Hair Test 2"))
	{
		QUEUE_JOB(=)
		{
			//PED::_APPLY_SHOP_ITEM_TO_PED(g_LocalPlayer.m_Entity, 933586678, true, false, false);
			Script::SetHairStyle(933586678, 9);
			PED::_0xAAB86462966168CE(g_LocalPlayer.m_Entity, false);
			PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Hair Test 3"))
	{
		QUEUE_JOB(=)
		{
			//PED::_APPLY_SHOP_ITEM_TO_PED(g_LocalPlayer.m_Entity, 1156231582, true, false, false);
			Script::SetHairStyle(1156231582, 2);
			PED::_0xAAB86462966168CE(g_LocalPlayer.m_Entity, false);
			PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, false);
		}
		END_JOB()
	}

	if (ImGui::Button("Inv"))
	{
		QUEUE_JOB(=)
		{
			[]() {
				json j;

				for (const auto& [Name, Hash] : Lists::ConsumableList)
				{
					if (!ITEMDATABASE::_ITEMDATABASE_IS_KEY_VALID(Hash, 0))
						continue;
					j["consumables"].push_back(Lists::GetHashNameOrUint(Hash));
				}

				for (const auto& [Name, Hash] : Lists::DocumentList)
				{
					if (!ITEMDATABASE::_ITEMDATABASE_IS_KEY_VALID(Hash, 0))
						continue;
					j["documents"].push_back(Lists::GetHashNameOrUint(Hash));
				}

				for (const auto& [Name, Hash] : Lists::ProvisionList)
				{
					if (!ITEMDATABASE::_ITEMDATABASE_IS_KEY_VALID(Hash, 0))
						continue;
					j["provisions"].push_back(Lists::GetHashNameOrUint(Hash));
				}

				std::ofstream f{ Config::GetConfigPath().append("inv.json") };
				f << j.dump(1, '\t');
				f << '\n';
			}();
		}
		END_JOB()
	}

	RenderInteriorButtons();
}

static void RenderGlobalDebug()
{
	//Hash* Hair = ScriptGlobal(1946054).At(1497).At(1).At(1, 3).Get<Hash*>();
	Hash* Hair = ScriptGlobal(40).At(7748).At(2).Get<Hash*>();
	
	ImGui::SetNextItemWidth(200);
	ImGui::InputU32("Hair Type", Hair);
	
	Hash* Length = ScriptGlobal(40).At(7748).At(1).Get<Hash*>();

	ImGui::SetNextItemWidth(200);
	ImGui::InputU32("Hair Length", Length);

	ImGui::Separator();

	static std::vector<int> s_Offsets{ 1946054, 1497, 1, 1, 3 };

	std::string GlobalString = "Global";
	GlobalString += '_';
	GlobalString += std::to_string(s_Offsets[0]);

	for (size_t i = 1; i < s_Offsets.size(); i++)
	{
		GlobalString += ".f_";
		GlobalString += std::to_string(s_Offsets[i]);
	}

	int offset = 0;
	for (size_t i = 0; i < s_Offsets.size(); i++)
		offset += s_Offsets[i];
	
	if (void* mem = ScriptGlobal(offset).Get<void*>())
	{
		ImGui::TextUnformatted(GlobalString.c_str());

		ImGui::Text("int: %d", *(int*)mem);
		ImGui::SameLine();
		if (ImGui::SmallButton((std::string{ "Copy##int_global" } + std::to_string((uintptr_t)mem)).c_str()))
			ImGui::SetClipboardText(std::to_string(*(int*)mem).c_str());

		ImGui::Text("float: %f", *(float*)mem);
		ImGui::SameLine();
		if (ImGui::SmallButton((std::string{ "Copy##float_global" } + std::to_string((uintptr_t)mem)).c_str()))
			ImGui::SetClipboardText(std::to_string(*(float*)mem).c_str());
	}

	int i = 0;
	for (auto it = s_Offsets.begin(); it < s_Offsets.end(); it++)
	{
		ImGui::SetNextItemWidth(300);
		ImGui::InputInt((std::string{ "##input_global" } + std::to_string(i)).c_str(), &(*it));
		if (s_Offsets.size() != 1)
		{
			ImGui::SameLine();
			if (ImGui::Button((std::string{ "Remove##global" } + std::to_string(i)).c_str()))
				it = s_Offsets.erase(it);
		}

		i++;
	}

	if (ImGui::Button("Add Offset"))
		s_Offsets.push_back(0);
}

void Menu::RenderDebugTab()
{
	if (!ImGui::BeginTabItem("Debug"))
		return;

	ImGui::BeginChild("debug_child");

	if (ImGui::CollapsingHeader("Settings JSON"))
	{
		ImGui::PushFont(Renderer::DefaultFont);

		ImGui::Text(g_Settings.dump(2).c_str());

		ImGui::PopFont();
	}

	ImGui::SeparatorText("Buttons");
	RenderDebugButtons();
	
	ImGui::SeparatorText("Toggles");
	ImGui::Checkbox("Render ImGui Demo", g_Settings["render_imgui_demo"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Log AnimScene Functions", g_Settings["log_animscene"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Add Cutscene Info Automatically", g_Settings["add_cutscene_info_automatically"].get<bool*>());

	ImGui::SeparatorText("Benchmark Times");
	ImGui::Text("Render Thread: %.3fms", Timer::s_RenderThreadTime);
	ImGui::Text("\tESP: %.3fms", Timer::s_ESPTime);
	ImGui::Text("\tMenu: %.3fms", Timer::s_MenuTime);

	ImGui::Text("Script Thread Time: %.3fms", Timer::s_ScriptThreadTime);
	ImGui::Text("\tScript Thread Tick: %.3fms", Timer::s_ScriptThreadTickTime);
	ImGui::Text("\tJob Queue: %.3fms (%llu jobs)", Timer::s_JobQueueTime, JobQueue::GetJobsQueued());

	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("RGB");
	ImGui::SameLine();
	ImGui::ColorButton("##RGB", Renderer::GetImGuiRGBA());

	ImGui::SeparatorText("Native Debug");
	static uint64_t s_NativeInput = 0xEC9A1261BF0CE510;
	ImGui::InputU64("Native Hash", &s_NativeInput, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
	
	const auto NativeHandler = TO_IDA(NativeInvoker::GetHandler(s_NativeInput));
	ImGui::Text("IDA Address: 0x%llX", NativeHandler);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##copy_ida_native"))
	{
		std::stringstream NativeAddress;
		NativeAddress << std::hex << std::uppercase << NativeHandler;
		ImGui::SetClipboardText(NativeAddress.str().c_str());
	}

	ImGui::SeparatorText("Global Debug");
	RenderGlobalDebug();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
