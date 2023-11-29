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

static void RenderConsumableList()
{
	TRY
	{
		ImGui::SeparatorText("Consumables");

		if (Lists::ConsumableList.empty())
			return;

		ImGui::BeginChild("consumable_child_inner");
		
		for (const auto& [Name, Item] : Lists::ConsumableList)
		{
			if (ImGui::Selectable(Name.c_str()))
				QueueAddItem(Item);
		}

		ImGui::EndChild(); // consumable_child_inner
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

static void RenderDocumentList()
{
	TRY
	{
		ImGui::SeparatorText("Documents");

		if (Lists::DocumentList.empty())
			return;
	
		ImGui::BeginChild("document_child_inner");

		for (const auto& [Name, Item] : Lists::DocumentList)
		{
			if (ImGui::Selectable(Name.c_str()))
				QueueAddItem(Item);
		}

		ImGui::EndChild(); // document_child_inner
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

static void RenderProvisionList()
{
	TRY
	{
		ImGui::SeparatorText("Provisions");

		if (Lists::ProvisionList.empty())
			return;

		ImGui::BeginChild("provision_child_inner");

		for (const auto& [Name, Item] : Lists::ProvisionList)
		{
			if (ImGui::Selectable(Name.c_str()))
				QueueAddItem(Item);
		}

		ImGui::EndChild(); // provision_child_inner
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Menu::RenderInventoryTab()
{
	if (!ImGui::BeginTabItem("Inventory"))
		return;

	ImGui::BeginChild("inventory_child");

#if 1

	static char s_InventoryFilter[0x100]{};
	ImGui::SetNextItemWidth(300);
	ImGui::InputText("Inventory Filter", s_InventoryFilter, IM_ARRAYSIZE(s_InventoryFilter));
	ImGui::Separator();

	ImGui::BeginChild("inventory_child_inner");

	const std::string InventoryFilterLower = Util::StringToLowerCopy(s_InventoryFilter);
	for (const auto& [Name, Hash] : Lists::InventoryList)
	{
		const std::string NameLower = Util::StringToLowerCopy(Name);
		if (NameLower.find(InventoryFilterLower) == std::string::npos)
			continue;

		if (ImGui::Selectable(Name.c_str()))
			QueueAddItem(Hash);
	}

	ImGui::EndChild(); // inventory_child_inner

#else
	const ImVec2 PanelSize{ 0, ImGui::GetContentRegionAvail().y / 3.0f };

	ImGui::BeginChild("consumable_child", PanelSize);
	TRY
	{
		RenderConsumableList();
	}
	EXCEPT{ LOG_EXCEPTION(); }
	ImGui::EndChild(); // consumable_child

	ImGui::BeginChild("document_child", PanelSize);
	TRY
	{
		RenderDocumentList();
	}
	EXCEPT{ LOG_EXCEPTION(); }
	ImGui::EndChild(); // document_child

	ImGui::BeginChild("provision_child"); // No PanelSize because it is not accurate for the last one
	TRY
	{
		RenderProvisionList();
	}
	EXCEPT{ LOG_EXCEPTION(); }
	ImGui::EndChild(); // provision_child
#endif

	ImGui::EndChild(); // inventory_child
	ImGui::EndTabItem();
}
