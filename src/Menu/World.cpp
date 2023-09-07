#include "pch.h"
#include "World.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/World.h"
#include "Config/Lists.h"

static void RenderWeatherChanger()
{
	for (const auto& [Name, Type] : Lists::WeatherList)
	{
		if (ImGui::Selectable(Name.c_str()))
		{
			Hash Weather = Type;
			QUEUE_JOB(=)
			{
				Script::SetWeather(Weather);
			}
			END_JOB()
		}
	}
}

static void RenderSnowChanger()
{
	for (int i = 0; i < (int)Lists::SnowList.size(); i++)
	{
		if (ImGui::Selectable(Lists::SnowList[i].c_str()))
		{
			QUEUE_JOB(=)
			{
				Script::SetSnow(i);
			}
			END_JOB()
		}
	}
}

static void RenderTimeChanger()
{
	int Hour = -1;
	if (ImGui::Selectable("Midnight")) { Hour = 0; }
	if (ImGui::Selectable("Morning")) { Hour = 7; }
	if (ImGui::Selectable("Noon")) { Hour = 12; }
	if (ImGui::Selectable("Evening")) { Hour = 18; }

	if (Hour != -1)
	{
		QUEUE_JOB(=)
		{
			Script::SetClock(Hour);
		}
		END_JOB()
	}
}

void Menu::RenderWorldTab()
{
	if (!ImGui::BeginTabItem("World"))
		return;

	ImGui::BeginChild("world_child");

	ImGui::BeginChild("left_half", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));

	// Left Pane
	ImGui::SeparatorText("Buttons");
	if (ImGui::Button("Noon + Sunny"))
		JobQueue::Add(Script::NoonAndSunny);

	ImGui::SeparatorText("Toggles");
	ImGui::Checkbox("Disable West Elizabeth Pinkerton Patrols", g_Settings["disable_pinkerton_patrols"].get<bool*>());
	ImGui::Checkbox("Enable All DLCs", g_Settings["enable_dlcs"].get<bool*>());

	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("right_half");

	// Weather Changer
	ImGui::BeginChild("right_upper", ImVec2(0, ImGui::GetContentRegionAvail().y / 2));
	ImGui::SeparatorText("Change Weather");
	ImGui::BeginChild("right_upper_inner");
	RenderWeatherChanger();
	ImGui::EndChild();
	ImGui::EndChild();

	// Snow Changer
	ImGui::BeginChild("right_middle", ImVec2(0, ImGui::GetContentRegionAvail().y / 2));
	ImGui::SeparatorText("Change Snow");
	ImGui::BeginChild("right_middle_inner");
	RenderSnowChanger();
	ImGui::EndChild();
	ImGui::EndChild();

	// Snow Changer
	ImGui::BeginChild("right_lower");
	ImGui::SeparatorText("Set Clock Time");
	ImGui::BeginChild("right_lower_inner");
	RenderTimeChanger();
	ImGui::EndChild();
	ImGui::EndChild();

	ImGui::EndChild();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
