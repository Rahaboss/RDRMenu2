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
#include "Script/Notification.h"
#include "Rage/Guid.h"
#include "Script/Network.h"

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
	if (ImGui::Button("Notify"))
	{
		QUEUE_JOB(=)
		{
			[]() {
				// Script::NotifyHelp("Hello from NotifyHelp");
				// Script::NotifyLocation("Hello from NotifyTop");
				// Script::NotifyFeed("Hello from NotifyFeed");
				// Script::NotifyObjective("Hello from NotifyObjective");
				// Script::NotifyToast("Hello from", "NotifyLeft");
				// 
				// Script::NotifyError("Hello from", "NotifyError");
				// Script::NotifyDeath("Hello from NotifyDeath");
				// Script::NotifyWarning("Hello from", "NotifyWarning");

				Script::NotifyHelp("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
				Script::NotifyLocation("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
				Script::NotifyFeed("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
				Script::NotifyObjective("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
				Script::NotifyToast("Hello from RD\xE2\x88\x91""Menu2",
					"\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");

				Script::NotifyHelpFormat("Hello from RD%sMenu2", "\xE2\x88\x91");

				// Script::NotifyError("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9",
				// 	"\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
			}();
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Copy CPed"))
	{
		std::stringstream ss;
		ss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(g_LocalPlayer.m_Ped);
		ImGui::SetClipboardText(ss.str().c_str());
		LOG_TEXT("%llX", reinterpret_cast<uintptr_t>(g_LocalPlayer.m_Ped));
	}
	ImGui::SameLine();
	if (ImGui::Button("Print Hex Coord Debug"))
	{
		QUEUE_JOB(=)
		{
			Vector3 pos = g_LocalPlayer.m_Pos;
			const uint32_t* pos_hex_ptr = reinterpret_cast<uint32_t*>(&pos);
			const uint32_t pos_hex[3]{ pos_hex_ptr[0], pos_hex_ptr[1], pos_hex_ptr[2] };

			for (size_t i = 0; i < 3; i++)
				LOG_TEXT("%u", pos_hex[i]);

			for (size_t i = 0; i < 3; i++)
			{
				const uint8_t* pos_hex_byte_ptr = reinterpret_cast<const uint8_t*>(&pos_hex[i]);
				LOG_TEXT("%x %x %x %x", pos_hex_byte_ptr[0], pos_hex_byte_ptr[1], pos_hex_byte_ptr[2], pos_hex_byte_ptr[3]);
			}
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Print Hair Info"))
	{
		QUEUE_JOB(=)
		{
			LOG_TEXT("playerHeadHair.ePomadeWearOffTimer: %d",			ScriptGlobal(40).At(7748).Get<int>());
			LOG_TEXT("playerHeadHair.headHairLength: %d",				ScriptGlobal(40).At(7748).At(1).Get<int>());
			LOG_TEXT("playerHeadHair.eHeadHairCut: %d",					ScriptGlobal(40).At(7748).At(2).Get<int>());
			LOG_TEXT("playerHeadHair.eHeadHairStyle: %d",				ScriptGlobal(40).At(7748).At(3).Get<int>());
			LOG_TEXT("playerHeadHair.eHeadHairFlags: %d",				ScriptGlobal(40).At(7748).At(4).Get<int>());
			LOG_TEXT("playerHeadHair.headHairNextGrowthTimer: %d",		ScriptGlobal(40).At(7748).At(5).Get<int>());
			LOG_TEXT("playerHeadHair.iNumAcceleratedGrowthCycles: %d",	ScriptGlobal(40).At(7748).At(6).Get<int>());
			LOG_TEXT("Timer: %d", MISC::GET_GAME_TIMER());
		}
		END_JOB()
	}

	RenderInteriorButtons();
}

static void RenderDebugToggles()
{
	ImGui::Checkbox("Render ImGui Demo", g_Settings["render_imgui_demo"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Log AnimScene Functions", g_Settings["log_animscene"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Add Cutscene Info Automatically", g_Settings["add_cutscene_info_automatically"].get<bool*>());
}

static void RenderBenchmarkTimes()
{
	ImGui::Text("Render Thread: %.3fms", Timer::s_RenderThreadTime);
	ImGui::Text("\tESP: %.3fms", Timer::s_ESPTime);
	ImGui::Text("\tMenu: %.3fms", Timer::s_MenuTime);

	ImGui::Text("Script Thread Time: %.3fms", Timer::s_ScriptThreadTime);
	ImGui::Text("\tScript Thread Tick: %.3fms", Timer::s_ScriptThreadTickTime);
	ImGui::Text("\tJob Queue: %.3fms (%llu jobs)", Timer::s_JobQueueTime, JobQueue::GetJobsQueued());
}

static void RenderRGB()
{
	ImGui::ColorButton("RGB Color Button", Renderer::GetImGuiRGBA());
}

static void RenderNativeDebug()
{
	static uint64_t s_NativeInput = 0xEC9A1261BF0CE510;
	ImGui::InputU64("Native Hash", &s_NativeInput, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

	const uintptr_t NativeHandler = TO_IDA(NativeInvoker::GetHandler(s_NativeInput));
	ImGui::Text("IDA Address: 0x%llX", NativeHandler);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##copy_ida_native"))
	{
		std::stringstream NativeAddress;
		NativeAddress << std::hex << std::uppercase << NativeHandler;
		ImGui::SetClipboardText(NativeAddress.str().c_str());
	}
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

		const int int_value = *reinterpret_cast<int*>(mem);

		ImGui::Text("int: %d", int_value);
		ImGui::SameLine();
		if (ImGui::SmallButton((std::string{ "Copy##int_global" } + std::to_string(reinterpret_cast<uintptr_t>(mem))).c_str()))
			ImGui::SetClipboardText(std::to_string(int_value).c_str());

		const float float_value = *reinterpret_cast<float*>(mem);

		ImGui::Text("float: %f", float_value);
		ImGui::SameLine();
		if (ImGui::SmallButton((std::string{ "Copy##float_global" } + std::to_string(reinterpret_cast<uintptr_t>(mem))).c_str()))
			ImGui::SetClipboardText(std::to_string(float_value).c_str());
	}

	int i = 0;
	for (auto it = s_Offsets.begin(); it < s_Offsets.cend(); it++)
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

static void QueueSpeech(const char* speechName, const char* voiceName, Hash speechParamHash, int variation)
{
	QUEUE_JOB(=)
	{
		// const char* speechName;
		// const char* voiceName;
		// alignas(8) int variation;
		// alignas(8) Hash speechParamHash;
		// alignas(8) Ped listenerPed;
		// alignas(8) BOOL syncOverNetwork;
		// alignas(8) int v7;
		// alignas(8) int v8;
		Guid<8> params;

		// params.At<const char*>(1) = "0405_U_M_M_RhdSheriff_01";
		// params.At<const char*>(0) = "RE_PH_RHD_V3_AGGRO";
		// params[3] = RAGE_JOAAT("SPEECH_PARAMS_BEAT_SHOUTED_CLEAR");
		// 
		// params.At<const char*>(1) = "0315_U_M_M_NbxDoctor_01";
		// params.At<const char*>(0) = "CHAT_PEDTYPE_DIALOG";
		// params[3] = RAGE_JOAAT("speech_params_force");
		// 
		// params.At<const char*>(1) = "0315_U_M_M_NbxDoctor_01";
		// params.At<const char*>(0) = "CHAT_PEDTYPE_DIALOG";
		// params[3] = RAGE_JOAAT("speech_params_force");

		params.At<const char*>(1) = voiceName;
		params.At<const char*>(0) = speechName;
		params[3] = speechParamHash;

		params[2] = variation;
		params[4] = 0;
		params[5] = NETWORK::NETWORK_IS_GAME_IN_PROGRESS();
		params[6] = 1;
		params[7] = 1;

		AUDIO::PLAY_PED_AMBIENT_SPEECH_NATIVE(g_LocalPlayer.m_Entity, params.get());
	}
	END_JOB()
}

static void RenderSpeechDebug()
{
	static char s_VoiceName[0x200]{ "ARTHUR" }, s_SpeechName[0x200]{ "RELZ1_AJAA" },
		s_SpeechParam[0x200]{ "SPEECH_PARAMS_FORCE" };

	ImGui::PushItemWidth(300);

	constexpr ImGuiInputTextFlags InputFlags = ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank;
	ImGui::InputText("Voice Name", s_VoiceName, IM_ARRAYSIZE(s_VoiceName), InputFlags);
	ImGui::InputText("Speech Name", s_SpeechName, IM_ARRAYSIZE(s_SpeechName), InputFlags);
	ImGui::InputText("Speech Param", s_SpeechParam, IM_ARRAYSIZE(s_SpeechParam), InputFlags);
	const Hash SpeechParam = rage::joaat(s_SpeechParam);
	ImGui::SameLine();
	ImGui::Text("%u (0x%X)", SpeechParam, SpeechParam);

	static int s_Variation = 1;
	ImGui::SliderInt("Variation", &s_Variation, 1, 20);

	ImGui::PopItemWidth();
	ImGui::Separator();

	if (ImGui::Button("Speak##custom"))
		QueueSpeech(s_SpeechName, s_VoiceName, SpeechParam, s_Variation);
}

static void RenderNotifyDebug()
{
	static char s_Message[64]{};
	ImGui::InputText("Message", s_Message, IM_ARRAYSIZE(s_Message));

	if (ImGui::Button("Help"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyHelp(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Feed"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyFeed(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Objective"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyObjective(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Location"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyLocation(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Death"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyDeath(s_Message);
		}
		END_JOB()
	}
	
	static char s_Description[64]{};
	ImGui::InputText("Description", s_Description, IM_ARRAYSIZE(s_Description));

	if (ImGui::Button("Toast"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyToast(s_Message, s_Description);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Error"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyError(s_Message, s_Description);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Warning"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyWarning(s_Message, s_Description);
		}
		END_JOB()
	}
}

static void RenderScenarioDebug()
{
	ImGui::BeginChild("scenario_left", ImVec2(ImGui::GetContentRegionAvail().x / 2, 200));
	
	static Hash SelectedScenario = 0;

	if (ImGui::Button("Play"))
	{
		QUEUE_JOB(=)
		{
			Script::StartPedScenario(g_LocalPlayer.m_Entity, SelectedScenario);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		QUEUE_JOB(=)
		{
			Script::StopPedScenario(g_LocalPlayer.m_Entity);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop Immediately And Clear Props"))
	{
		QUEUE_JOB(=)
		{
			Script::StopPedScenarioImmediatelyAndClearProps(g_LocalPlayer.m_Entity);
		}
		END_JOB()
	}
	
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("scenario_right", ImVec2(0, 200));
	
	for (const auto& [Name, Scenario] : Lists::ScenarioList)
	{
		if (ImGui::Selectable(Name.c_str(), SelectedScenario == Scenario))
			SelectedScenario = Scenario;
	}

	ImGui::EndChild();

	ImGui::Separator();
	ImGui::Text("CNetworkPlayerMgr: 0x%llX (RDR2.exe+%llX)", (uintptr_t)Pointers::NetworkPlayerMgr,
		(uintptr_t)Pointers::NetworkPlayerMgr - g_BaseAddress);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##copy_CNetworkPlayerMgr"))
	{
		std::stringstream NativeAddress;
		NativeAddress << std::hex << std::uppercase << (uintptr_t)Pointers::NetworkPlayerMgr;
		ImGui::SetClipboardText(NativeAddress.str().c_str());
	}
}

bool func_2556(int iParam0)
{
	if (iParam0 <= -1 || iParam0 > 9)
	{
		return false;
	}
	return true;
}

int func_33()
{
	return ScriptGlobal(1899515).Get<int>();
}

bool func_2557()
{
	return ScriptGlobal(40).At(7748).At(6).Get<int>() > 0;
}

bool func_2241(int iParam0)
{
	return (ScriptGlobal(40).At(7748).At(4).Get<int>() & iParam0) != 0;
}

int func_2787()
{
	if (func_2241(8))
	{
		return 8;
	}
	else if (func_2241(4))
	{
		return 4;
	}
	else if (func_2241(2))
	{
		return 2;
	}
	return 1;
}

int func_2942(float fParam0, float fParam1)
{
	return BUILTIN::FLOOR((((fParam0 / fParam1) - BUILTIN::TO_FLOAT(BUILTIN::FLOOR((fParam0 / fParam1)))) * 24.0f));
}

int func_2943(float fParam0, float fParam1)
{
	return BUILTIN::FLOOR((((fParam0 / fParam1) - BUILTIN::TO_FLOAT(BUILTIN::FLOOR((fParam0 / fParam1)))) * 60.0f));
}

int func_2944(float fParam0, float fParam1)
{
	return BUILTIN::FLOOR((((fParam0 / fParam1) - BUILTIN::TO_FLOAT(BUILTIN::FLOOR((fParam0 / fParam1)))) * 60.0f));
}

void func_2788(int* iParam0, int* iParam1, int* uParam2, int* uParam3, bool bParam4, int iParam5)
{
	int iVar0;
	int iVar1;
	int iVar2;
	float fVar3;

	fVar3 = BUILTIN::TO_FLOAT(iParam5);
	iVar0 = func_2942(BUILTIN::TO_FLOAT(*iParam0), fVar3);
	iVar1 = func_2943(BUILTIN::TO_FLOAT(*iParam1), fVar3);
	iVar2 = func_2944(BUILTIN::TO_FLOAT(*uParam2), fVar3);
	*iParam0 /= iParam5;
	*iParam1 /= iParam5;
	*uParam2 /= iParam5;
	*uParam3 /= iParam5;
	*iParam1 += iVar0;
	*uParam2 += iVar1;
	*uParam3 += iVar2;
	if (bParam4)
	{
		*uParam3 *= -1;
		*uParam2 *= -1;
		*iParam1 *= -1;
		*iParam0 *= -1;
	}
}

void func_2558(int iParam0, int* iParam1, int* iParam2, bool bParam3)
{
	int uVar0;
	int uVar1;

	switch (iParam0)
	{
	case 0:
		*iParam1 = 2;
		*iParam2 = 0;
		break;
	case 1:
		*iParam1 = 2;
		*iParam2 = 0;
		break;
	case 2:
		*iParam1 = 4;
		*iParam2 = 0;
		break;
	case 3:
		*iParam1 = 8;
		*iParam2 = 0;
		break;
	case 4:
		*iParam1 = 12;
		*iParam2 = 0;
		break;
	case 5:
		*iParam1 = 20;
		*iParam2 = 0;
		break;
	case 6:
		*iParam1 = 20;
		*iParam2 = 0;
		break;
	case 7:
		*iParam1 = 32;
		*iParam2 = 0;
		break;
	case 8:
		*iParam1 = 32;
		*iParam2 = 0;
		break;
	case 9:
		*iParam1 = 52;
		*iParam2 = 0;
		break;
	}
	if (bParam3)
	{
		func_2788(iParam1, iParam2, &uVar0, &uVar1, 0, func_2787());
	}
}

int func_715(bool bParam0, int iParam1, int iParam2)
{
	if (bParam0)
	{
		return iParam1;
	}
	return iParam2;
}

int func_326(int iParam0)
{
	return (BUILTIN::SHIFT_RIGHT(iParam0, 26) & 31 * func_715(MISC::IS_BIT_SET(iParam0, 31), -1, 1)) + 1898;
}

int func_327(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 22) & 15;
}

int func_328(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 17) & 31;
}

int func_329(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 12) & 31;
}

int func_330(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 6) & 63;
}

int func_331(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 0) & 63;
}

int func_332(int iParam0, int iParam1)
{
	if (iParam1 < 0)
	{
		iParam1 = 0;
	}
	switch (iParam0)
	{
	case 0:
	case 2:
	case 4:
	case 6:
	case 7:
	case 9:
	case 11:
		return 31;
	case 3:
	case 5:
	case 8:
	case 10:
		return 30;
	case 1:
		if ((iParam1 % 4) == 0)
		{
			if ((iParam1 % 100) != 0)
			{
				return 29;
			}
			else if ((iParam1 % 400) == 0)
			{
				return 29;
			}
		}
		return 28;
	default:
		break;
	}
	return 30;
}

void func_716(int* iParam0, int iParam1)
{
	if (iParam1 < 0)
	{
		return;
	}
	if (iParam1 > 1930 || iParam1 < 1866)
	{
		return;
	}
	*iParam0 -= *iParam0 & 2080374784;
	if (iParam1 < 1898)
	{
		*iParam0 |= BUILTIN::SHIFT_LEFT((1898 - iParam1), 26);
		*iParam0 |= (1 << 31);
	}
	else
	{
		*iParam0 |= BUILTIN::SHIFT_LEFT((iParam1 - 1898), 26);
		*iParam0 -= *iParam0 & (1 << 31);
	}
}

void func_717(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 > 11)
	{
		return;
	}
	*iParam0 -= *iParam0 & 62914560;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 22);
}

void func_718(int* iParam0, int iParam1)
{
	int iVar0;
	int iVar1;

	iVar0 = func_327(*iParam0);
	iVar1 = func_326(*iParam0);
	if (iParam1 < 1 || iParam1 > func_332(iVar0, iVar1))
	{
		return;
	}
	*iParam0 -= *iParam0 & 4063232;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 17);
}

void func_719(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 > 23)
	{
		return;
	}
	*iParam0 -= *iParam0 & 126976;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 12);
}

void func_720(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 >= 60)
	{
		return;
	}
	*iParam0 -= *iParam0 & 4032;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 6);
}

void func_721(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 >= 60)
	{
		return;
	}
	*iParam0 -= *iParam0 & 63;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 0);
}

void func_333(int* iParam0, int iParam1, int iParam2, int iParam3, int iParam4, int iParam5, int iParam6)
{
	func_716(iParam0, iParam6);
	func_717(iParam0, iParam5);
	func_718(iParam0, iParam4);
	func_719(iParam0, iParam3);
	func_720(iParam0, iParam2);
	func_721(iParam0, iParam1);
}

void func_1025(int* iParam0, int iParam1, int iParam2, int iParam3, int iParam4, int iParam5, int iParam6, bool bParam7)
{
	int iVar0;
	int iVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	int iVar5;
	int iVar6;

	iVar0 = func_326(*iParam0);
	iVar1 = func_327(*iParam0);
	iVar2 = func_328(*iParam0);
	iVar3 = func_329(*iParam0);
	iVar4 = func_330(*iParam0);
	iVar5 = func_331(*iParam0);
	if (((((iParam6 == 0 && iParam5 == 0) && iParam4 == 0) && iParam3 == 0) && iParam2 == 0) && iParam1 == 0)
	{
		return;
	}
	if (iParam1 < 0)
	{
		return;
	}
	if (iParam2 < 0)
	{
		return;
	}
	if (iParam3 < 0)
	{
		return;
	}
	if (iParam4 < 0)
	{
		return;
	}
	if (iParam5 < 0)
	{
		return;
	}
	if (iParam6 < 0)
	{
		return;
	}
	iVar5 += iParam1;
	while (iVar5 >= 60)
	{
		iParam2++;
		iVar5 -= 60;
	}
	iVar4 += iParam2;
	while (iVar4 >= 60)
	{
		iParam3++;
		iVar4 -= 60;
	}
	iVar3 += iParam3;
	while (iVar3 >= 24)
	{
		iParam4++;
		iVar3 -= 24;
	}
	iVar2 += iParam4;
	iVar6 = func_332(iVar1, iVar0);
	while (iVar2 > iVar6)
	{
		iParam5++;
		iVar2 -= iVar6;
		if (iVar1 > 11)
		{
			iParam6++;
			iVar1 -= 12;
		}
		iVar6 = func_332(iVar1, iVar0);
	}
	iVar1 += iParam5;
	while (iVar1 > 11)
	{
		iParam6++;
		iVar1 -= 12;
	}
	if (!bParam7)
	{
		iVar0 += iParam6;
	}
	func_333(iParam0, iVar5, iVar4, iVar3, iVar2, iVar1, iVar0);
}

void func_2239(int iParam0, bool bParam1)
{
	int iVar0;
	int iVar1;
	bool bVar2;

	if (!func_2556(iParam0))
	{
		return;
	}
	if (ScriptGlobal(40).At(7748).At(1).Get<int>() == iParam0)
	{
		return;
	}
	ScriptGlobal(40).At(7748).At(1).Get<int&>() = iParam0;
	if (!bParam1)
	{
		ScriptGlobal(40).At(7748).At(5).Get<int&>() = func_33();
		bVar2 = (ScriptGlobal(40).At(7748).At(1).Get<int>() < 9 && func_2557());
		func_2558(ScriptGlobal(40).At(7748).At(1).Get<int>(), &iVar0, &iVar1, bVar2);
		func_1025(ScriptGlobal(40).At(7748).At(5).Get<int*>(), 0, 0, iVar1, iVar0, 0, 0, 0);
	}
}

int func_765()
{
	return ScriptGlobal(40).At(7748).At(1).Get<int>();
}

void func_2555()
{
	int iVar0;

	if (func_765() <= 3)
	{
		iVar0 = func_33();
		func_1025(&iVar0, 0, 0, 0, 2, 0, 0, 0);
		DECORATOR::DECOR_SET_INT(ScriptGlobal(35).Get<int>(), "hairShort", iVar0);
		if (DECORATOR::DECOR_EXIST_ON(ScriptGlobal(35).Get<int>(), "hairLong"))
		{
			DECORATOR::DECOR_REMOVE(ScriptGlobal(35).Get<int>(), "hairLong");
		}
	}
	else if (DECORATOR::DECOR_EXIST_ON(ScriptGlobal(35).Get<int>(), "hairShort"))
	{
		DECORATOR::DECOR_REMOVE(ScriptGlobal(35).Get<int>(), "hairShort");
	}
}

static void RenderHairDebug()
{
	if (ScriptGlobal(40).At(7748).Get<int*>())
	{
		ImGui::Text("0) playerHeadHair.ePomadeWearOffTimer: %d", ScriptGlobal(40).At(7748).Get<int>());
		ImGui::Text("1) playerHeadHair.headHairLength: %d", ScriptGlobal(40).At(7748).At(1).Get<int>());
		ImGui::Text("2) playerHeadHair.eHeadHairCut: %d", ScriptGlobal(40).At(7748).At(2).Get<int>());
		ImGui::Text("3) playerHeadHair.eHeadHairStyle: %d", ScriptGlobal(40).At(7748).At(3).Get<int>());
		ImGui::Text("4) playerHeadHair.eHeadHairFlags: %d", ScriptGlobal(40).At(7748).At(4).Get<int>());
		ImGui::Text("5) playerHeadHair.headHairNextGrowthTimer: %d", ScriptGlobal(40).At(7748).At(5).Get<int>());
		ImGui::Text("6) playerHeadHair.iNumAcceleratedGrowthCycles: %d", ScriptGlobal(40).At(7748).At(6).Get<int>());

		ImGui::Separator();
		int x = ScriptGlobal(40).At(7748).At(1).Get<int>();
		if (ImGui::SliderInt("Hair Length", &x, 0, 9))
		{
			func_2239(x, 0);
			ScriptGlobal(40).At(7748).At(5).Get<int&>() = MISC::GET_GAME_TIMER() + 1000;
			func_2555();
		}
	}
}

static void RenderNetworkDebug()
{
	if (const rage::netPlayerData* NetPlayerData = Script::GetNetPlayerData(g_LocalPlayer.m_Index))
	{
		ImGui::Text("Name: %s", NetPlayerData->m_Name);

		ImGui::Text("Rockstar ID: %llu", NetPlayerData->m_RockstarID);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Rockstar ID"))
		{
			std::stringstream ss;
			ss << NetPlayerData->m_RockstarID;
			ImGui::SetClipboardText(ss.str().c_str());
		}

		ImGui::Text("Internal IP: %u.%u.%u.%u", NetPlayerData->m_InternalIP.m_Field1, NetPlayerData->m_InternalIP.m_Field2,
			NetPlayerData->m_InternalIP.m_Field3, NetPlayerData->m_InternalIP.m_Field4);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Internal IP"))
			Script::CopyIP(NetPlayerData->m_InternalIP);

		ImGui::Text("External IP: %u.%u.%u.%u", NetPlayerData->m_ExternalIP.m_Field1, NetPlayerData->m_ExternalIP.m_Field2,
			NetPlayerData->m_ExternalIP.m_Field3, NetPlayerData->m_ExternalIP.m_Field4);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##External IP"))
			Script::CopyIP(NetPlayerData->m_ExternalIP);
	}
}

static void RenderDebugMain()
{
	ImGui::SeparatorText("Buttons");
	RenderDebugButtons();

	ImGui::SeparatorText("Toggles");
	RenderDebugToggles();

	ImGui::SeparatorText("Benchmark Times");
	RenderBenchmarkTimes();

	ImGui::SeparatorText("RGB");
	RenderRGB();

	ImGui::SeparatorText("Native Debug");
	TRY
	{
		RenderNativeDebug();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::SeparatorText("Global Debug");
	TRY
	{
		RenderGlobalDebug();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::SeparatorText("Speech Debug");
	RenderSpeechDebug();

	ImGui::SeparatorText("Notify Debug");
	RenderNotifyDebug();

	ImGui::SeparatorText("Scenario Debug");
	RenderScenarioDebug();

	ImGui::SeparatorText("Hair Debug");
	RenderHairDebug();

	ImGui::SeparatorText("Network Debug");
	RenderNetworkDebug();
}

void Menu::RenderDebugTab()
{
	if (!ImGui::BeginTabItem("Debug"))
		return;

	ImGui::BeginChild("debug_child");

	TRY
	{
		RenderDebugMain();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild(); // debug_child
	ImGui::EndTabItem();
}
