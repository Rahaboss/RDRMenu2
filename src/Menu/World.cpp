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
		if (ImGui::Button("Set Time To Noon", ImVec2(180, 0)))
			Features::SetClockTime(12);
		if (ImGui::Button("Set Time To Evening", ImVec2(180, 0)))
			Features::SetClockTime(18);
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
			
		if (ImGui::Button("Reveal Map"))
			Features::RevealMap();
		
		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::Checkbox("No Snipers", g_Settings["no_snipers"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("No Black Borders", g_Settings["no_black_borders"].get<bool*>());

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::Text("Set Weather");
		ImGui::BeginChild("weather_list_menu", ImVec2(200, 0));
		for (const auto& w : g_WeatherTypeList)
		{
			if (ImGui::Selectable(w.first.c_str()))
			{
				Hash hash = w.second;
				QUEUE_JOB(=)
				{
					MISC::SET_WEATHER_TYPE(hash, true, true, false, 0.0f, false);
				}
				END_JOB()
			}
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
