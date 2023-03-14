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

	void RenderBoneESP(Ped ped)
	{
		const ImU32 Color = GetImGuiRGB32();
		constexpr float Thickness = 5.0f;
		
		ImVec2 Head, Neck, Spine, LShoulder, RShoulder, LElbow, RElbow, LHand,
			RHand, LHip, RHip, LKnee, RKnee, LFoot, RFoot, LToe, RToe;

		if (!GetPedBoneScreenCoords(ped, SKEL_HEAD, Head)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_NECK0, Neck)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_SPINE_ROOT, Spine)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_UPPERARM, LShoulder)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_UPPERARM, RShoulder)) return;
		if (!GetPedBoneScreenCoords(ped, MH_L_ELBOWGRP, LElbow) && !GetPedBoneScreenCoords(ped, 22711, LElbow)) return;
		if (!GetPedBoneScreenCoords(ped, MH_R_ELBOWGRP, RElbow) && !GetPedBoneScreenCoords(ped, 24550, RElbow)
			&& !GetPedBoneScreenCoords(ped, 2992, RElbow) && !GetPedBoneScreenCoords(ped, 37346, RElbow)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_HAND, LHand)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_HAND, RHand)) return;
		if (!GetPedBoneScreenCoords(ped, RB_L_THIGHROLL, LHip)) return;
		if (!GetPedBoneScreenCoords(ped, RB_R_THIGHROLL, RHip)) return;
		if (!GetPedBoneScreenCoords(ped, RB_L_KNEEFRONT, LKnee)) return;
		if (!GetPedBoneScreenCoords(ped, RB_R_KNEEFRONT, RKnee)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_FOOT, LFoot)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_FOOT, RFoot)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_L_TOE0, LToe)) return;
		if (!GetPedBoneScreenCoords(ped, SKEL_R_TOE0, RToe)) return;

		// Head + Body
		RenderLineArray({ Head, Neck, Spine }, Color, Thickness);

		// Arms
		RenderLineArray({ Neck, LShoulder, LElbow, LHand }, Color, Thickness);
		RenderLineArray({ Neck, RShoulder, RElbow, RHand }, Color, Thickness);

		// Legs
		RenderLineArray({ Spine, LHip, LKnee, LFoot, LToe }, Color, Thickness);
		RenderLineArray({ Spine, RHip, RKnee, RFoot, RToe }, Color, Thickness);
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
				RenderBoneESP(g_LocalPlayer.m_Entity);
				//RenderBoneDebug(g_LocalPlayer.m_Entity);
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
			if (ENTITY::DOES_ENTITY_EXIST(p))
				NewPeds.push_back(p);
		}
		g_AddedPeds = NewPeds;

		for (const auto& p : g_AddedPeds)
			RenderBoneESP(p);
	}
}
