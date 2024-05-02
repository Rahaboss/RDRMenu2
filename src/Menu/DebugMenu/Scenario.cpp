#include "pch.h"
#include "Scenario.h"
#include "Thread/JobQueue.h"
#include "Script/PlayerInfo.h"
#include "Rage/natives.h"
#include "Script/Ped.h"
#include "Config/Lists.h"

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
	ImGui::BeginChild("scenario_right");
	
	for (const auto& [Name, Scenario] : Lists::ScenarioList)
	{
		if (ImGui::Selectable(Name.c_str(), SelectedScenario == Scenario))
			SelectedScenario = Scenario;
	}

	ImGui::EndChild();
}

void DebugMenu::RenderScenarioTab()
{
	if (!ImGui::BeginTabItem("Scenario##debug"))
		return;

	TRY
	{
		ImGui::SeparatorText("Scenario Debug");
		RenderScenarioDebug();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndTabItem();
}
