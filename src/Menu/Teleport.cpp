#include "pch.h"
#include "Teleport.h"
#include "Lists.h"
#include "Features.h"

namespace Menu
{
	void RenderTeleportTab()
	{
		if (!ImGui::BeginTabItem("Teleport"))
			return;

		ImGui::Text("Teleports");
		
		ImGui::BeginChild("teleport_child");

		for (const auto& s : g_TeleportList)
		{
			if (ImGui::Selectable(s.first.c_str()))
				Features::TeleportOnGround(s.second);
		}

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
