#include "pch.h"
#include "Rendering.h"
#include "Features.h"
#include "PlayerInfo.h"

// Features for drawing on the screen
// \xE2\x88\x91	= Rockstar
// \xC2\xA6		= Verified
// \xE2\x80\xB9	= Created
// \xE2\x80\xBA	= Blank
// \xCE\xA9		= Padlock
// \xE2\x84\xAE	= Token?
// \xE2\x84\x85	= C/O
// \xE2\x86\x95	= Up/Down Arrow
// \xC6\xB1		= Thick Padlock
// ~n~			= Newline ('\n')
// ~COLOR_RED~\xE2\x88\x91 Admin
// ~COLOR_SOCIAL_CLUB~

namespace Features
{
	void RenderText(const char* Text, float PosX, float Posy, uint8_t ColR, uint8_t ColG, uint8_t ColB, uint8_t ColA, float Scale)
	{
		TRY
		{
			UIDEBUG::_BG_SET_TEXT_SCALE(Scale, Scale);
			UIDEBUG::_BG_SET_TEXT_COLOR(ColR, ColG, ColB, ColA);
			UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", Text), PosX, Posy);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RenderTextOnEntity(Entity Ent, const char* Text)
	{
		TRY
		{ 
			if (!Ent)
				return;

			Vector3 pos = ENTITY::GET_ENTITY_COORDS(Ent, true, true);
			float x = 0, y = 0;
			if (!WorldToScreen(pos.x, pos.y, pos.z, x, y))
				return;

			//const auto& RGB = GetRGB();
			RenderText(Text, x, y, GET_RGB(), static_cast<uint8_t>(255ui8 * 0.75f), 0.8f);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	bool WorldToScreen(float X, float Y, float Z, float& OutX, float& OutY)
	{
		TRY
		{
			if (HUD::GET_HUD_SCREEN_POSITION_FROM_WORLD_POSITION(X, Y, Z, &OutX, &OutY))
				return false;
			return true;
		}
		EXCEPT{ LOG_EXCEPTION(); }
		
		return false;
	}

	bool WorldToScreen(const Vector3& WorldPos, float& ScreenX, float& ScreenY)
	{
		return Pointers::WorldToScreen(&WorldPos, &ScreenX, &ScreenY);
	}

	bool GetPedBoneScreenCoords(Ped ped, int boneId, ImVec2& OutCoords)
	{
		if (PED::GET_PED_BONE_INDEX(ped, boneId) == -1)
			return false;
		if (!WorldToScreen(PED::GET_PED_BONE_COORDS(ped, boneId, 0, 0, 0), OutCoords.x, OutCoords.y))
			return false;
		OutCoords.x *= ImGui::GetMainViewport()->WorkSize.x;
		OutCoords.y *= ImGui::GetMainViewport()->WorkSize.y;
		return true;
	}

	bool RenderBoneESP(Ped ped)
	{
		const ImU32 Color = GetImGuiRGB32();
		constexpr float Thickness = 5.0f;
		
		ImVec2 Head, Neck, Spine, LShoulder, RShoulder, LElbow, RElbow, LHand,
			RHand, LHip, RHip, LKnee, RKnee, LFoot, RFoot, LToe, RToe;

		if (!GetPedBoneScreenCoords(ped, SKEL_HEAD, Head)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_NECK0, Neck)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_SPINE_ROOT, Spine)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_UPPERARM, LShoulder)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_UPPERARM, RShoulder)) return false;
		if (!GetPedBoneScreenCoords(ped, MH_L_ELBOWGRP, LElbow) && !GetPedBoneScreenCoords(ped, 22711, LElbow)) return false;
		if (!GetPedBoneScreenCoords(ped, MH_R_ELBOWGRP, RElbow) && !GetPedBoneScreenCoords(ped, 24550, RElbow)
			&& !GetPedBoneScreenCoords(ped, 2992, RElbow) && !GetPedBoneScreenCoords(ped, 37346, RElbow)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_HAND, LHand)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_HAND, RHand)) return false;
		if (!GetPedBoneScreenCoords(ped, RB_L_THIGHROLL, LHip)) return false;
		if (!GetPedBoneScreenCoords(ped, RB_R_THIGHROLL, RHip)) return false;
		if (!GetPedBoneScreenCoords(ped, RB_L_KNEEFRONT, LKnee)) return false;
		if (!GetPedBoneScreenCoords(ped, RB_R_KNEEFRONT, RKnee)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_FOOT, LFoot)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_FOOT, RFoot)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_TOE0, LToe)) return false;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_TOE0, RToe)) return false;

		// Head + Body
		RenderLineArray({ Head, Neck, Spine }, Color, Thickness);

		// Arms
		RenderLineArray({ Neck, LShoulder, LElbow, LHand }, Color, Thickness);
		RenderLineArray({ Neck, RShoulder, RElbow, RHand }, Color, Thickness);

		// Legs
		RenderLineArray({ Spine, LHip, LKnee, LFoot, LToe }, Color, Thickness);
		RenderLineArray({ Spine, RHip, RKnee, RFoot, RToe }, Color, Thickness);

		return true;
	}

	void RenderLineArray(const std::vector<ImVec2>& vec, ImU32 Color, float Thickness)
	{
		TRY
		{
			if (vec.size() < 2)
			return;

			for (const auto& p : vec)
			{
				if (p.x < 0 || p.y < 0)
					return;
			}

			auto l = ImGui::GetBackgroundDrawList();

			for (size_t i = 0; i < vec.size(); i++)
			{
				l->AddLine(vec[i], vec[i + 1], Color, Thickness);

				if (i + 2 == vec.size())
					return;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	void RenderBoneDebug(Ped ped)
	{
		auto l = ImGui::GetBackgroundDrawList();

		for (int i = 0; i <= 0xFFFF; i++)
		//for (int i = 20000; i <= 30000; i++)
		{
			ImVec2 pos;
			if (!GetPedBoneScreenCoords(ped, i, pos))
				continue;
			std::string text = std::to_string(i);
			l->AddText(pos, IM_COL32_WHITE, text.c_str());
		}
	}

	void RenderImGuiESP()
	{
		TRY
		{
			if (g_LocalPlayer.m_Entity)
			{
				if (g_Settings["esp"]["player_bone"].get<bool>())
					RenderBoneESP(g_LocalPlayer.m_Entity);
				if (g_Settings["esp"]["player_bone_debug"].get<bool>())
					RenderBoneDebug(g_LocalPlayer.m_Entity);
				if (g_Settings["esp"]["player_box"].get<bool>())
					RenderBoxESP(g_LocalPlayer.m_Entity);
				RenderTextESP(g_LocalPlayer.m_Entity, g_Settings["esp"]["player_model"].get<bool>(), true);
			}
			
			RenderAddedPedESP();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	void RenderAddedPedESP()
	{
		std::vector<Ped> NewPeds;
		for (const auto& p : g_AddedPeds)
		{
			if (ENTITY::DOES_ENTITY_EXIST(p) && ENTITY::IS_ENTITY_A_PED(p) && PED::IS_PED_HUMAN(p))
				NewPeds.push_back(p);
		}
		g_AddedPeds = NewPeds;

		for (const auto& p : g_AddedPeds)
		{
			if (g_Settings["esp"]["added_ped_bone"].get<bool>())
				RenderBoneESP(p);
			if (g_Settings["esp"]["added_ped_box"].get<bool>())
				RenderBoxESP(p);
			RenderTextESP(p, g_Settings["esp"]["added_ped_model"].get<bool>(), true);
		}
	}
	
	bool RenderTextESP(Ped ped, bool ModelESP, bool HealthESP)
	{
		ImVec2 TopLeft, BottomRight;
		if (!GetBoxCoords(ped, TopLeft, BottomRight))
			return false;

		std::string Text;

		if (ModelESP)
			Text += GetPedModelName(ENTITY::GET_ENTITY_MODEL(ped)) + "\n";

		if (HealthESP)
			Text += std::string("Health: ") + std::to_string(ENTITY::GET_ENTITY_HEALTH(ped)) + std::string(" / ")
				+ std::to_string(ENTITY::GET_ENTITY_MAX_HEALTH(ped, FALSE)) + std::string("\n");

		if (Text.empty())
			return false;

		ImVec2 TextSize = ImGui::CalcTextSize(Text.c_str());
		ImVec2 TextPos = ImVec2(((TopLeft.x + BottomRight.x) / 2) - (TextSize.x / 2), BottomRight.y);

		auto l = ImGui::GetBackgroundDrawList();
		l->AddText(TextPos, GetImGuiRGB32(), Text.c_str());

		return true;
	}
	
	bool RenderBoxESP(Ped ped)
	{
		ImVec2 TopLeft, BottomRight;
		if (!GetBoxCoords(ped, TopLeft, BottomRight))
			return false;

		auto l = ImGui::GetBackgroundDrawList();
		l->AddRect(TopLeft, BottomRight, GetImGuiRGB32(), 0, 0, 2);

		return true;
	}
	
	bool GetBoxCoords(Ped ped, ImVec2& TopLeft, ImVec2& BottomRight)
	{
		constexpr uint32_t Bones[]{
			SKEL_HEAD,
			SKEL_NECK0,
			SKEL_SPINE_ROOT,
			SKEL_L_UPPERARM,
			SKEL_R_UPPERARM,
			SKEL_L_HAND,
			SKEL_R_HAND,
			RB_L_THIGHROLL,
			RB_R_THIGHROLL,
			RB_L_KNEEFRONT,
			RB_R_KNEEFRONT,
			SKEL_L_FOOT,
			SKEL_R_FOOT,
			SKEL_L_TOE0,
			SKEL_R_TOE0,
		};

		ImVec2 temp;
		if (!GetPedBoneScreenCoords(ped, SKEL_HEAD, temp))
			return false;

		TopLeft = temp;
		BottomRight = temp;

		for (const auto& b : Bones)
		{
			if (!GetPedBoneScreenCoords(ped, b, temp))
				return false;

			if (temp.x < TopLeft.x)
				TopLeft.x = temp.x;
			if (temp.x > BottomRight.x)
				BottomRight.x = temp.x;

			if (temp.y < TopLeft.y)
				TopLeft.y = temp.y;
			if (temp.y > BottomRight.y)
				BottomRight.y = temp.y;
		}

		constexpr uint32_t OptionalBones[]{
			MH_L_ELBOWGRP,
			MH_R_ELBOWGRP,
			22711,
			24550,
			2992,
			37346,
			SPR_HAIR_01,
			SPR_HAIR_02,
			SPR_HAIR_03,
			SPR_HAIR_04,
			SPR_HAIR_05,
			SPR_HAIR_06,
			SPR_HAIR_07,
			SPR_HAIR_08,
			SPR_HAIR_09,
			SPR_HAIR_010,
			SPR_HAIR_011,
			SPR_HAIR_012,
			SPR_HAIR_013,
			SKEL_L_TOE10,
			SKEL_L_TOE20,
			SKEL_R_TOE10,
			SKEL_R_TOE20,
			HAT_LOC,
			57278,
			37762,
		};

		for (const auto& b : OptionalBones)
		{
			if (!GetPedBoneScreenCoords(ped, b, temp))
				continue;

			if (temp.x < TopLeft.x)
				TopLeft.x = temp.x;
			if (temp.x > BottomRight.x)
				BottomRight.x = temp.x;

			if (temp.y < TopLeft.y)
				TopLeft.y = temp.y;
			if (temp.y > BottomRight.y)
				BottomRight.y = temp.y;
		}

		return true;
	}
}
