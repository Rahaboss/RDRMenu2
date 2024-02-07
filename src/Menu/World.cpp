#include "pch.h"
#include "World.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/World.h"
#include "Config/Lists.h"
#include "Script/Freecam.h"
#include "Util/Random.h"

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
	for (size_t i = 0; i < Lists::SnowList.size(); i++)
	{
		if (ImGui::Selectable(Lists::SnowList[i].c_str()))
		{
			QUEUE_JOB(=)
			{
				Script::SetSnow(static_cast<int>(i));
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
	if (ImGui::Button("Reveal Map"))
		JobQueue::Add(Script::RevealMap);
	ImGui::SameLine();
	if (ImGui::Button("Noon And Sunny"))
		JobQueue::Add(Script::NoonAndSunny);

	ImGui::SeparatorText("Toggles");
	ImGui::Checkbox("Disable West Elizabeth Pinkerton Patrols", g_Settings["disable_pinkerton_patrols"].get<bool*>());
	ImGui::Checkbox("Disable Invisible Snipers", g_Settings["disable_invisible_snipers"].get<bool*>());
	ImGui::Checkbox("Enable All DLCs", g_Settings["enable_dlcs"].get<bool*>());
	ImGui::SameLine();
	if (ImGui::Checkbox("Freecam", &Script::FreecamEnabled))
	{
		if (!Script::FreecamEnabled)
			JobQueue::Add(Script::DestroyFreecam);
	}

	ImGui::SeparatorText("Sliders");
	ImGui::TextUnformatted("Time Scale");
	static float s_TimeScale = 1.0f;
	ImGui::SetNextItemWidth(250.0f);
	if (ImGui::SliderFloat("##timescale", &s_TimeScale, 0.0f, 1.0f, "%.2f"))
	{
		s_TimeScale = std::clamp(s_TimeScale, 0.0f, 1.0f);
		QUEUE_JOB(=)
		{
			Script::SetTimeScale(s_TimeScale);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset##reset_timescale"))
	{
		s_TimeScale = 1.0f;
		QUEUE_JOB(=)
		{
			Script::SetTimeScale(s_TimeScale);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Random##random_timescale"))
	{
		s_TimeScale = Random::F32();
		QUEUE_JOB(=)
		{
			Script::SetTimeScale(s_TimeScale);
		}
		END_JOB()
	}

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
