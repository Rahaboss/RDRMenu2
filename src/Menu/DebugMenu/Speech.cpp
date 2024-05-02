#include "pch.h"
#include "Speech.h"
#include "Rage/natives.h"
#include "Thread/JobQueue.h"
#include "Rage/Guid.h"
#include "Script/PlayerInfo.h"

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

void DebugMenu::RenderSpeechTab()
{
	if (!ImGui::BeginTabItem("Speech##debug"))
		return;

	TRY
	{
		ImGui::SeparatorText("Speech Debug");
		RenderSpeechDebug();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndTabItem();
}
