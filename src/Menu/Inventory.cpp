#include "pch.h"
#include "Inventory.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Inventory.h"
#include "Util/String.h"

static void QueueAddItem(Hash ItemHash)
{
	QUEUE_JOB(=)
	{
		Script::GiveInventoryItem(ItemHash);
	}
	END_JOB()
}

void Menu::RenderInventoryTab()
{
	if (!ImGui::BeginTabItem("Inventory"))
		return;

	ImGui::BeginChild("inventory_child");

	static char s_InventoryFilter[0x100]{};
	ImGui::SetNextItemWidth(300);
	ImGui::InputText("Inventory Filter", s_InventoryFilter, IM_ARRAYSIZE(s_InventoryFilter));
	ImGui::Separator();

	ImGui::BeginChild("inventory_child_inner");

	const std::string InventoryFilterLower = Util::StringToLowerCopy(s_InventoryFilter);
	for (const auto& [Name, Model] : Lists::InventoryList)
	{
		const std::string NameLower = Util::StringToLowerCopy(Name);
		if (NameLower.find(InventoryFilterLower) == std::string::npos)
			continue;

		if (ImGui::Selectable(Name.c_str()))
			QueueAddItem(Model);
	}

	ImGui::EndChild(); // inventory_child_inner

	ImGui::EndChild(); // inventory_child
	ImGui::EndTabItem();
}
