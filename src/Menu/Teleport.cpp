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
		ImGui::Separator();
		ImGui::BeginChild("teleport_child");

		for (const auto& s : g_TeleportList)
		{
			const auto& name = s.first.c_str();
			const auto& pos = s.second;
			if (ImGui::Selectable(name))
				Features::TeleportOnGround(pos.x, pos.y, pos.z);
		}

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
