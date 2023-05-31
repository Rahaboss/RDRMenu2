#include "pch.h"
#include "World.h"
#include "Features.h"
#include "Lists.h"
#include "JobQueue.h"

namespace Menu
{
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

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (ImGui::Button("Set Time To Noon", ImVec2(180, 0)))
			Features::SetClockTime(12);
		if (ImGui::Button("Set Time To Evening", ImVec2(180, 0)))
			Features::SetClockTime(18);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		
		if (ImGui::Button("Reveal Map"))
			Features::RevealMap();
		if (ImGui::Button("Noon And Sunny"))
		{
			Features::SetClockTime(12);
			Features::SetWeatherType(SUNNY);
		}

		ImGui::EndGroup();
		ImGui::Separator();

		ImGui::SetNextItemWidth(200.0f);
		static float s_TimeScale = 1.0f;
		if (ImGui::SliderFloat("Time Scale", &s_TimeScale, 0.0f, 1.0f, "%.2f"))
		{
			QUEUE_JOB(=)
			{
				MISC::SET_TIME_SCALE(s_TimeScale);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset###rs_timescale"))
		{
			s_TimeScale = 1.0f;
			QUEUE_JOB(=)
			{
				MISC::SET_TIME_SCALE(s_TimeScale);
			}
			END_JOB()
		}
		ImGui::Separator();

		ImGui::BeginGroup();
		ImGui::Checkbox("No Snipers", g_Settings["no_snipers"].get<bool*>());
		ImGui::SameLine();
		ImGui::Checkbox("No Black Borders", g_Settings["no_black_borders"].get<bool*>());
		ImGui::SameLine();
		ImGui::Checkbox("Disable (Most) West Elizabeth Pinkertons", g_Settings["disable_west_elizabeth_pinkertons"].get<bool*>());

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::Text("Set Weather");
		ImGui::BeginChild("weather_list_menu", ImVec2(200, 0));
		for (const auto& w : g_WeatherTypeList)
		{
			if (ImGui::Selectable(w.first.c_str()))
				Features::SetWeatherType(w.second);
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
}
