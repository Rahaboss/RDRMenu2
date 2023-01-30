#include "pch.h"
#include "Memory.h"
#include "PlayerInfo.h"
#include "Features.h"
#include "Renderer.h"

namespace Menu
{
	void RenderMemoryTab()
	{
		if (!ImGui::BeginTabItem("Memory"))
			return;
		
		ImGui::BeginChild("memory_child");

		uint8_t* MemoryLocation = (uint8_t*)g_LocalPlayer.m_Ped;
		if (!MemoryLocation)
		{
			ImGui::Text("CPed is null");
			ImGui::EndChild();
			ImGui::EndTabItem();
			return;
		}

		if (ImGui::Button("Copy CPed Address"))
		{
			std::stringstream Stream;
			Stream << std::hex << std::uppercase << "0x" << (uint64_t)g_LocalPlayer.m_Ped;
			Features::SetClipboardText(Stream.str());
		}

		ImGui::PushFont(Renderer::DefaultFont);

		uint32_t MemorySize = 0x1000;

		ImGui::Text("%llX", g_LocalPlayer.m_Ped);

		for (uint32_t i = 0; i < MemorySize; i += 16)
		{
			ImGui::Text("%04X ", i);
			for (uint32_t j = 0; j < 16; j++)
			{
				if (j == 8)
				{
					ImGui::SameLine();
					ImGui::Text("");
				}

				ImGui::SameLine();
				uint8_t Byte = MemoryLocation[i + j];
				if (Byte)
					ImGui::Text("%02X", Byte);
				else
					ImGui::TextDisabled("%02X", Byte);
			}

			ImGui::SameLine();
			ImGui::Text("");

			for (uint32_t j = 0; j < 16; j++)
			{
				if (j == 8)
				{
					ImGui::SameLine();
					ImGui::Text("");
				}

				ImGui::SameLine();
				uint8_t Byte = MemoryLocation[i + j];
				if (Byte < 32 || Byte > 126)
					ImGui::TextDisabled(".");
				else
					ImGui::Text("%c", Byte);
			}
		}

		ImGui::PopFont();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
