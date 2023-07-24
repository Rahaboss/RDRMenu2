#include "pch.h"
#include "Cutscene.h"
#include "Features.h"
#include "Lists.h"
#include "JobQueue.h"

namespace Menu
{
	void RenderCutsceneTab()
	{
		if (!ImGui::BeginTabItem("Cutscene"))
			return;

		ImGui::BeginChild("cutscene_child");

		/*
		static int s_CurrentCutscene = 0;
		static char CutFilter[200]{ "cutscene@" };
		ImGui::BeginGroup();

		ImGui::Text("Select Cutscene:");
		if (ImGui::BeginChild("###cutscene_list", ImVec2(350, 250)))
		{
			std::string s1(CutFilter);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::tolower);

			for (int i = 0; i < g_Cutscenes.size(); i++)
			{
				if (g_Settings["disable_default_cutscenes"].get<bool>())
				{
					int cs_size = static_cast<int>(g_Cutscenes[i].size());
					if (g_Cutscenes[i].contains("player_model"))
						cs_size--;
					if (g_Cutscenes[i].contains("player_outfit_preset"))
						cs_size--;

					if (cs_size <= 1)
						continue;
				}

				const std::string& Name = g_Cutscenes[i]["id"].get_ref<const std::string&>();

				std::string s2(Name);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::tolower);
				if (s2.find(s1) == std::string::npos)
					continue;

				if (ImGui::Selectable(Name.c_str(), i == s_CurrentCutscene))
					s_CurrentCutscene = i;
			}
		}
		ImGui::EndChild();

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Text("Filter Cutscenes:");
		if (ImGui::BeginChild("###cutscene_info", ImVec2(0, 250)) && !g_Cutscenes.empty())
		{
			ImGui::PushItemWidth(300);
			ImGui::InputText("###filter_cut", CutFilter, 200);
			ImGui::PopItemWidth();

			ImGui::Checkbox("Disable Default Cutscenes", g_Settings["disable_default_cutscenes"].get<bool*>());
			ImGui::Checkbox("Disable Cinematic Black Borders", g_Settings["no_black_borders"].get<bool*>());
			ImGui::Separator();

			const nlohmann::json& CurrentJson = g_Cutscenes[s_CurrentCutscene];
			if (ImGui::Button("Play"))
			{
				QUEUE_JOB(&)
				{
					Features::PlayCutsceneFromJson(CurrentJson);
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Reload List"))
			{
				QUEUE_JOB(= )
				{
					Lists::ReloadCutscenesList();
				}
				END_JOB()
			}

			ImGui::Text("Cutscene Info:");
			ImGui::Text("animDict: %s", CurrentJson["id"].get_ref<const std::string&>().c_str());
			if (CurrentJson.size() != 1)
			{
				if (CurrentJson.contains("playback_id"))
				{
					ImGui::Text("playbackListName: %s", CurrentJson["playback_id"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("player_model"))
				{
					ImGui::Text("Player Model: %s", CurrentJson["player_model"].get_ref<const std::string&>().c_str());
				}

				if (CurrentJson.contains("peds"))
				{
					ImGui::Text("Cutscene Peds:");
					for (const auto& cs_ped : CurrentJson["peds"])
						ImGui::BulletText(cs_ped["name"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("objects"))
				{
					ImGui::Text("Cutscene Objects:");
					for (const auto& cs_ped : CurrentJson["objects"])
						ImGui::BulletText(cs_ped["name"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("vehicles"))
				{
					ImGui::Text("Cutscene Vehicles:");
					for (const auto& cs_ped : CurrentJson["vehicles"])
						ImGui::BulletText(cs_ped["name"].get_ref<const std::string&>().c_str());
				}
			}
		}
		ImGui::EndChild();

		ImGui::EndGroup();
		ImGui::Separator();
		*/
		
		// CollapsHeader -> BeginChild(0,0) (rest of frame size) -> EndChild -> Separator -> CollapsHeader -> Beta Cutscenes

		if (ImGui::BeginChild("###cutscene_list_organized", ImVec2(370, 0)))
		{
			for (const auto& Cat : Categories)
			{
				Cat.Render();
				ImGui::Separator();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();

		if (ImGui::BeginChild("###cutscene_info") && !g_Cutscenes.empty())
		{
			ImGui::Checkbox("Disable Default Cutscenes", g_Settings["disable_default_cutscenes"].get<bool*>());
			ImGui::Checkbox("Disable Cinematic Black Borders", g_Settings["no_black_borders"].get<bool*>());
			ImGui::Separator();

			const nlohmann::json& CurrentJson = g_Cutscenes[SelectedCutscene];
			if (ImGui::Button("Play"))
			{
				QUEUE_JOB(&)
				{
					Features::PlayCutsceneFromJson(CurrentJson);
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Reload List"))
			{
				QUEUE_JOB(=)
				{
					Lists::ReloadCutscenesList();
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Noon And Sunny"))
				Features::SetNoonAndSunny();
			ImGui::Separator();

#if 0
			int FilteredCutsceneCount = 0;
			for (const auto& Cat : Categories)
			{
				for (int i = 0; i < g_Cutscenes.size(); i++)
				{
					auto ID = g_Cutscenes[i]["id"].get_ref<const std::string&>();
					
					for (const auto& Prefix : Cat.m_Prefixes)
					{
						if (ID.find(Prefix) == 0)
						{
							FilteredCutsceneCount++;
							break;
						}
					}
				}
			}
			ImGui::Text("g_Cutscenes.size() = %u", g_Cutscenes.size());
			ImGui::Text("FilteredCutsceneCount = %u", FilteredCutsceneCount);

			if (g_Cutscenes.size() != FilteredCutsceneCount)
			{
				for (int i = 0; i < g_Cutscenes.size(); i++)
				{
					bool FoundMissingCutscene = true;

					for (const auto& Cat : Categories)
					{
						auto ID = g_Cutscenes[i]["id"].get_ref<const std::string&>();

						for (const auto& Prefix : Cat.m_Prefixes)
						{
							if (ID.find(Prefix) == 0)
							{
								FoundMissingCutscene = false;
								goto asdasd123;
							}
						}

					}
					
					asdasd123:
					if (FoundMissingCutscene)
					{
						ImGui::Text(g_Cutscenes[i]["id"].get_ref<const std::string&>().c_str());
					}
				}
			}
			ImGui::Separator();
#endif

			ImGui::Text("Cutscene Info:");
			ImGui::Text("animDict: %s", CurrentJson["id"].get_ref<const std::string&>().c_str());
			if (CurrentJson.size() != 1)
			{
				if (CurrentJson.contains("playback_id"))
				{
					ImGui::Text("playbackListName: %s", CurrentJson["playback_id"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("player_model"))
				{
					ImGui::Text("Player Model: %s", CurrentJson["player_model"].get_ref<const std::string&>().c_str());
				}

				if (CurrentJson.contains("peds"))
				{
					ImGui::Text("Cutscene Peds:");
					for (const auto& cs_ped : CurrentJson["peds"])
						ImGui::BulletText(cs_ped["name"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("objects"))
				{
					ImGui::Text("Cutscene Objects:");
					for (const auto& cs_obj : CurrentJson["objects"])
						ImGui::BulletText(cs_obj["name"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("vehicles"))
				{
					ImGui::Text("Cutscene Vehicles:");
					for (const auto& cs_veh : CurrentJson["vehicles"])
						ImGui::BulletText(cs_veh["name"].get_ref<const std::string&>().c_str());
				}
			}
		}
		ImGui::EndChild();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
