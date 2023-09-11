#include "pch.h"
#include "Cutscene.h"
#include "Config/Lists.h"
#include "Script/Cutscene.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Util/String.h"
#include "Config/Settings.h"

static AnimScene s_CutsceneIndex = 0;
static void QueuePlayCutscene(const char* CutsceneName)
{
	if (s_CutsceneIndex)
	{
		LOG_TEXT("%s: Cutscene already running.\n", __FUNCTION__);
		return;
	}

	s_CutsceneIndex = -1;

	QUEUE_JOB(=)
	{
		[=]() {
			Script::CutsceneHelper Cutscene{ CutsceneName };
			s_CutsceneIndex = Cutscene.GetCutscene();
			Cutscene.PlayAutomatically();
			s_CutsceneIndex = 0;
		}();
	}
	END_JOB()
}

static void QueueSkipCutscene()
{
	QUEUE_JOB(=)
	{
		if (s_CutsceneIndex != -1 && !ANIMSCENE::HAS_ANIM_SCENE_EXITED(s_CutsceneIndex, false))
			ANIMSCENE::TRIGGER_ANIM_SCENE_SKIP(s_CutsceneIndex);
	}
	END_JOB()
}

static void RenderCutsceneInfo(json& SelectedCutscene)
{
	ImGui::SeparatorText("Cutscene Controls");

	const bool IsCutscenePlaying = s_CutsceneIndex != 0;

	if (IsCutscenePlaying)
		ImGui::BeginDisabled();
	if (ImGui::Button("Play Cutscene"))
		QueuePlayCutscene(SelectedCutscene["id"].get_ref<const std::string&>().c_str());
	if (IsCutscenePlaying)
		ImGui::EndDisabled();

	ImGui::SameLine();

	if (!IsCutscenePlaying)
		ImGui::BeginDisabled();
	if (ImGui::Button("Skip Cutscene"))
		QueueSkipCutscene();
	if (!IsCutscenePlaying)
		ImGui::EndDisabled();

	ImGui::Checkbox("Disable Black Borders", g_Settings["disable_black_borders"].get<bool*>());

	ImGui::BeginChild("cutscene_info_inner");

	ImGui::SeparatorText("Cutscene Info");

	TRY
	{
		ImGui::Text("Cutscene Name: %s", SelectedCutscene["id"].get_ref<const std::string&>().c_str());

		if (SelectedCutscene.contains("playback_id"))
			ImGui::Text("Cutscene playbackListName: %s", SelectedCutscene["playback_id"].get_ref<const std::string&>().c_str());

		if (SelectedCutscene.contains("player_model"))
			ImGui::Text("Player Model: %s", SelectedCutscene["player_model"].get_ref<const std::string&>().c_str());

		if (SelectedCutscene.contains("peds"))
		{
			ImGui::Text("Cutscene Peds:");
			for (const json& ped : SelectedCutscene["peds"])
				ImGui::BulletText(ped["name"].get_ref<const std::string&>().c_str());
		}

		if (SelectedCutscene.contains("objects"))
		{
			ImGui::Text("Cutscene Objects:");
			for (const json& obj : SelectedCutscene["objects"])
				ImGui::BulletText(obj["name"].get_ref<const std::string&>().c_str());
		}

		if (SelectedCutscene.contains("vehicles"))
	{
		ImGui::Text("Cutscene Vehicles:");
		for (const json& veh : SelectedCutscene["vehicles"])
			ImGui::BulletText(veh["name"].get_ref<const std::string&>().c_str());
	}
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild();
}

static void RenderCutsceneList(json& SelectedCutscene)
{
	ImGui::SeparatorText("Filter Cutscenes");
	static char s_CutsceneFilter[200]{};
	ImGui::InputText("##cutscene_filter", s_CutsceneFilter, IM_ARRAYSIZE(s_CutsceneFilter));
	ImGui::SameLine();
	if (ImGui::Button("Clear##clear_cutscene_filter"))
		*s_CutsceneFilter = '\0';

	ImGui::SeparatorText("Cutscenes");
	ImGui::BeginChild("cutscene_list_inner");

	std::string CutsceneFilterLower{ s_CutsceneFilter };
	Util::StringToLower(CutsceneFilterLower);

	for (const json& Cutscene : Lists::CutsceneList)
	{
		if (Cutscene["id"].get_ref<const std::string&>().find(CutsceneFilterLower) == std::string::npos)
			continue;

		if (ImGui::Selectable(Cutscene["id"].get_ref<const std::string&>().c_str(),
			Cutscene["id"].get_ref<const std::string&>() == SelectedCutscene["id"].get_ref<const std::string&>()))
			SelectedCutscene = Cutscene;
	}

	ImGui::EndChild();
}

void Menu::RenderCutsceneTab()
{
	if (!ImGui::BeginTabItem("Cutscene"))
		return;

	ImGui::BeginChild("cutscene_child");

	static json s_SelectedCutscene{ *Lists::CutsceneList.begin() };

	ImGui::BeginChild("cutscene_info", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
	RenderCutsceneInfo(s_SelectedCutscene);
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("cutscene_list");
	RenderCutsceneList(s_SelectedCutscene);
	ImGui::EndChild();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
