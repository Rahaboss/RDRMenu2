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
}

void Menu::RenderDebugTab()
{
	if (!ImGui::BeginTabItem("Debug"))
		return;

	ImGui::BeginChild("debug_child");

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
	
	const uintptr_t NativeHandler = TO_IDA(NativeInvoker::GetHandler(s_NativeInput));
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

	ImGui::SeparatorText("Speech Debug");
	RenderSpeechDebug();

	ImGui::SeparatorText("Notify Debug");
	RenderNotifyDebug();

	ImGui::SeparatorText("Scenario Debug");
	RenderScenarioDebug();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
