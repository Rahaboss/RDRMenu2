#include "pch.h"
#include "General.h"
#include "Thread/JobQueue.h"
#include "Config/Lists.h"
#include "Config/Settings.h"
#include "Util/Timer.h"
#include "Renderer/RGB.h"
#include "Script/Notification.h"
#include "Script/PlayerInfo.h"
#include "Rage/ScriptGlobal.h"
#include "Rage/natives.h"

static void RenderDebugButtons()
{
	if (ImGui::Button("Reload Lists"))
	{
		QUEUE_JOB(=)
		{
			Lists::Destroy();
			Lists::Create();
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Notify"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyHelp("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
			Script::NotifyLocation("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
			Script::NotifyFeed("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
			Script::NotifyObjective("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");
			Script::NotifyToast("Hello from RD\xE2\x88\x91""Menu2",
				"\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9");

			Script::NotifyHelpFormat("Hello from RD%sMenu2", "\xE2\x88\x91");
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Copy CPed"))
	{
		std::stringstream ss;
		ss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(g_LocalPlayer.m_Ped);
		ImGui::SetClipboardText(ss.str().c_str());
		LOG_TEXT("%llX", reinterpret_cast<uintptr_t>(g_LocalPlayer.m_Ped));
	}
	ImGui::SameLine();
	if (ImGui::Button("Print Hex Coord Debug"))
	{
		QUEUE_JOB(=)
		{
			Vector3 pos = g_LocalPlayer.m_Pos;
			const uint32_t* pos_hex_ptr = reinterpret_cast<uint32_t*>(&pos);
			const uint32_t pos_hex[3]{ pos_hex_ptr[0], pos_hex_ptr[1], pos_hex_ptr[2] };

			for (size_t i = 0; i < 3; i++)
				LOG_TEXT("%u", pos_hex[i]);

			for (size_t i = 0; i < 3; i++)
			{
				const uint8_t* pos_hex_byte_ptr = reinterpret_cast<const uint8_t*>(&pos_hex[i]);
				LOG_TEXT("%x %x %x %x", pos_hex_byte_ptr[0], pos_hex_byte_ptr[1], pos_hex_byte_ptr[2], pos_hex_byte_ptr[3]);
			}
		}
		END_JOB()
	}
}

static void RenderDebugToggles()
{
	ImGui::Checkbox("Render ImGui Demo", g_Settings["render_imgui_demo"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Log AnimScene Functions", g_Settings["log_animscene"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Add Cutscene Info Automatically", g_Settings["add_cutscene_info_automatically"].get<bool*>());
}

static void RenderBenchmarkTimes()
{
	ImGui::Text("Render Thread: %.3fms", Timer::s_RenderThreadTime);
	ImGui::Text("\tESP: %.3fms", Timer::s_ESPTime);
	ImGui::Text("\tMenu: %.3fms", Timer::s_MenuTime);

	ImGui::Text("Script Thread Time: %.3fms", Timer::s_ScriptThreadTime);
	ImGui::Text("\tScript Thread Tick: %.3fms", Timer::s_ScriptThreadTickTime);
	ImGui::Text("\tJob Queue: %.3fms (%llu jobs)", Timer::s_JobQueueTime, JobQueue::GetJobsQueued());
}

static void RenderRGB()
{
	ImGui::ColorButton("RGB Color Button", Renderer::GetImGuiRGBA());
}

void DebugMenu::RenderGeneralTab()
{
	if (!ImGui::BeginTabItem("General"))
		return;

	TRY
	{
		ImGui::SeparatorText("Buttons");
		RenderDebugButtons();

		ImGui::SeparatorText("Toggles");
		RenderDebugToggles();

		ImGui::SeparatorText("Benchmark Times");
		RenderBenchmarkTimes();

		ImGui::SeparatorText("RGB");
		RenderRGB();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndTabItem();
}
