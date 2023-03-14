#include "pch.h"
#include "Inventory.h"
#include "Features.h"
#include "JobQueue.h"
#include "PlayerInfo.h"
#include "Lists.h"

namespace Menu
{
	void RenderInventoryTab()
	{
		if (!ImGui::BeginTabItem("Inventory"))
			return;
		
		ImGui::BeginChild("inventory_child");

		if (ImGui::Button("Give All Items"))
		{
			Features::GiveAllConsumables();
			Features::GiveAllDocuments();
			Features::GiveAllProvisions();
			Features::GiveAllWeapons();
			Features::GiveAllAmmo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Give All Item Requests"))
			Features::GiveAllItemRequests();
		ImGui::SameLine();
		if (ImGui::Button("Unlock All Herbs"))
		{
			QUEUE_JOB(=)
			{
				const Vector3& pos = g_LocalPlayer.m_Pos;
				for (const auto& h : g_HerbList)
					COMPENDIUM::COMPENDIUM_HERB_PICKED(h, pos.x, pos.y, pos.z);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Give All Perks"))
		{
			QUEUE_JOB(=)
			{
				for (const auto& [name, hash] : g_ProvisionList)
				{
					if (name.find("Talisman") == std::string::npos && name.find("Trinket") == std::string::npos)
						continue;

					Features::GiveInventoryItem(hash);
				}
			}
			END_JOB()
		}

		ImGui::Separator();
		
		const float ChildHeight = ImGui::GetContentRegionAvail().y / 3;

		ImGui::BeginChild("consumable_menu_outer", ImVec2(0, ChildHeight));
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Give Consumable Items");
		ImGui::SameLine();
		static char ConBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_con", ConBuffer, 200);
		ImGui::SameLine();
		if (ImGui::Button("Give All Consumables"))
			Features::GiveAllConsumables();
		if (ImGui::BeginChild("consumable_menu"))
		{
			std::string s1(ConBuffer);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::toupper);
			for (const auto& c : g_ConsumableList)
			{
				std::string s2(c.first);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::toupper);
				if (s2.find(s1) == std::string::npos)
					continue;
			
				if (ImGui::Selectable(c.first.c_str()))
				{
					QUEUE_JOB(&c)
					{
						Features::GiveInventoryItem(c.second, 99);
					}
					END_JOB()
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndChild();
		ImGui::Separator();
			
		ImGui::BeginChild("provision_menu_outer", ImVec2(0, ChildHeight));
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Give Provision Items");
		ImGui::SameLine();
		static char ProBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_pro", ProBuffer, 200);
		ImGui::SameLine();
		if (ImGui::Button("Give All Provisions"))
			Features::GiveAllProvisions();
		if (ImGui::BeginChild("provision_menu"))
		{
			std::string s1(ProBuffer);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::toupper);
			for (const auto& p : g_ProvisionList)
			{
				std::string s2(p.first);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::toupper);
				if (s2.find(s1) == std::string::npos)
					continue;

				if (ImGui::Selectable(p.first.c_str()))
				{
					QUEUE_JOB(&p)
					{
						Features::GiveInventoryItem(p.second, 99);
					}
					END_JOB()
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndChild();
		ImGui::Separator();

		ImGui::BeginChild("document_menu_outer");
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Give Document Items");
		ImGui::SameLine();
		static char DocBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_doc", DocBuffer, 200);
		ImGui::SameLine();
		if (ImGui::Button("Give All Documents"))
			Features::GiveAllDocuments();
		if (ImGui::BeginChild("document_menu"))
		{
			std::string s1(DocBuffer);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::toupper);
			for (const auto& d : g_DocumentList)
			{
				std::string s2(d.first);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::toupper);
				if (s2.find(s1) == std::string::npos)
					continue;

				if (ImGui::Selectable(d.first.c_str()))
				{
					QUEUE_JOB(&d)
					{
						Features::GiveInventoryItem(d.second, 99);
					}
					END_JOB()
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndChild();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
