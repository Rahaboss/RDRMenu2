#pragma once

#include "pch.h"
#include "rage/natives.h"
#include "rage/enums.h"

struct ExampleAppLog
{
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool                AutoScroll;  // Keep scrolling if already at the bottom.

	ExampleAppLog()
	{
		AutoScroll = true;
		Clear();
	}

	void    Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
	}

	void    Draw()
	{
		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		// Main window
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if (Filter.IsActive())
		{
			// In this example we don't use the clipper when Filter is enabled.
			// This is because we don't have a random access on the result on our filter.
			// A real application processing logs with ten of thousands of entries may want to store the result of
			// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
			for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				if (Filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
			// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
			// within the visible area.
			// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
			// on your side is recommended. Using ImGuiListClipper requires
			// - A) random access into your data
			// - B) items all being the  same height,
			// both of which we can handle since we an array pointing to the beginning of each line of text.
			// When using the filter (in the block of code above) we don't have random access into the data to display
			// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
			// it possible (and would be recommended if you want to search through tens of thousands of entries).
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
	}
};

namespace Menu
{
	// State
	inline bool IsOpen = false;

	// Main menus
	void RenderMenu();
	void RenderPlayerTab();
	void RenderTeleportTab();
	void RenderWeaponTab();
	void RenderInventoryTab();
	void RenderWorldTab();
	void RenderSpawningTab();
	void RenderDebugTab();
	void RenderLoggerTab();
	void RenderMemoryTab();

	// Submenus
	void RenderPlayerButtons();
	void RenderPlayerCheckboxes();
	void RenderPlayerModelChanger();
	void RenderPedDebug();
	void RenderOverlay();
	void RenderOverlayMain();

	inline bool EnableFreeCam = false;
	inline Cam CamEntity = 0;

	struct {
		Entity ent = 0;
		Hash model = A_C_FISHCHANNELCATFISH_01_XL;
		Vector3 pos{};
		float head{};
		int health{}, max_health{};
		bool freeze{}, invincible{}, visible = true;
		bool injured{};
	} inline PedDebug;

	inline int CurCtx = 0;
	inline constexpr char* HUDContextList[]{
		"HUD_CTX_CODE_TOOLS",
		"HUD_CTX_CRAFTING_SEQUENCE",
		"HUD_CTX_FADED_GAMEPLAY",
		"HUD_CTX_FIREFIGHT_CUTSCENE",
		"HUD_CTX_FISHING",
		"HUD_CTX_GOLD_CURRENCY_CHANGE",
		"HUD_CTX_HACK_RADAR_FORCE_HIDE",
		"HUD_CTX_HONOR_SHOW",
		"HUD_CTX_INFINITE_AMMO",
		"HUD_CTX_INFO_CARD",
		"HUD_CTX_INPUT_REVEAL_HUD",
		"HUD_CTX_INSPECT_ITEM",
		"HUD_CTX_IN_CAMP_WARDROBE",
		"HUD_CTX_IN_CAMP_WITH_SUPPLIES",
		"HUD_CTX_IN_CATALOGUE_SHOP_MENU",
		"HUD_CTX_IN_COMBAT_RESTRICTED_SHOP",
		"HUD_CTX_IN_FAST_TRAVEL_MENU",
		"HUD_CTX_IN_GUARMA_AND_BROKE",
		"HUD_CTX_IN_LOBBY",
		"HUD_CTX_IN_MINIGAME",
		"HUD_CTX_IN_MINIGAME_POKER_INTRO",
		"HUD_CTX_IN_MINIGAME_POKER_OUTRO",
		"HUD_CTX_IN_MISSION_CUTSCENE",
		"HUD_CTX_IN_MP_GAME_MODE",
		"HUD_CTX_IN_MP_TUTORIAL_CUTSCENE",
		"HUD_CTX_IN_PLAYER_CAMP",
		"HUD_CTX_IN_QUICK_TIME_EVENT",
		"HUD_CTX_IN_RESPAWN",
		"HUD_CTX_IN_SHOP",
		"HUD_CTX_ITEM_CONSUMPTION_DEADEYE",
		"HUD_CTX_ITEM_CONSUMPTION_DEADEYE_CORE",
		"HUD_CTX_ITEM_CONSUMPTION_HEALTH",
		"HUD_CTX_ITEM_CONSUMPTION_HEALTH_CORE",
		"HUD_CTX_ITEM_CONSUMPTION_HORSE_HEALTH",
		"HUD_CTX_ITEM_CONSUMPTION_HORSE_HEALTH_CORE",
		"HUD_CTX_ITEM_CONSUMPTION_HORSE_STAMINA",
		"HUD_CTX_ITEM_CONSUMPTION_HORSE_STAMINA_CORE",
		"HUD_CTX_ITEM_CONSUMPTION_STAMINA",
		"HUD_CTX_ITEM_CONSUMPTION_STAMINA_CORE",
		"HUD_CTX_LOBBY_TEAM_SELECT",
		"HUD_CTX_MAYOR2_CUTSCENE_OBJECTIVES",
		"HUD_CTX_MINIGAME_SHOOTING",
		"HUD_CTX_MISSION_CONTROLLER",
		"HUD_CTX_MISSION_CONTROLLER_CUTSCENE",
		"HUD_CTX_MISSION_CONTROLLER_INTRO",
		"HUD_CTX_MISSION_CONTROLLER_OUTRO",
		"HUD_CTX_MISSION_CREATOR",
		"HUD_CTX_MISSION_CUTSCENE_WITH_RADAR",
		"HUD_CTX_MP_MATCHMAKING_TRANSITION",
		"HUD_CTX_MP_OUT_OF_AREA_BOUNDS",
		"HUD_CTX_MP_RACES",
		"HUD_CTX_MP_SPECTATING",
		"HUD_CTX_NO_ALIVE_PLAYER_HORSE",
		"HUD_CTX_PLAYER_CAMERA_MODE",
		"HUD_CTX_PROMPT_MONEY",
		"HUD_CTX_RESTING_BY_FIRE",
		"HUD_CTX_ROBBERY_ACTION",
		"HUD_CTX_SCRIPTED_IN_GAME_CUTSCENE",
		"HUD_CTX_SCRIPTED_PLAYER_CONTROL_DISABLED",
		"HUD_CTX_SCRIPT_CME_CUTSCENE",
		"HUD_CTX_SHARP_SHOOTER_EVENT",
		"HUD_CTX_SHOP_OBJECTIVE",
		"HUD_CTX_SKINNING_PROCESS",
		"HUD_CTX_TITHING",
		"HUD_CTX_TITHING_NOGANG_CASH",
		"HUD_CTX_TRANSLATE_OVERLAY",
	};

	inline ExampleAppLog Logger;
}
