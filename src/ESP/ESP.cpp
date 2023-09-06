#include "pch.h"
#include "ESP.h"
#include "Screen.h"
#include "Renderer/RGB.h"
#include "Rage/enums.h"
#include "Script/Entity.h"
#include "Config/Lists.h"
#include "Menu.h"
#include "Timer.h"
#include "Rage/ScriptGlobal.h"

void ESP::RenderLineArray(const std::vector<ImVec2>& vec, ImU32 Color, float Thickness)
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

bool ESP::RenderPedBoneESP(Ped ped)
{
	const ImU32 Color = Renderer::GetImGuiRGB32();
	constexpr float Thickness = 5.0f;

	ImVec2 Head, Neck, Spine, LShoulder, RShoulder, LElbow, RElbow, LHand,
		RHand, LHip, RHip, LKnee, RKnee, LFoot, RFoot, LToe, RToe;

	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_HEAD, Head)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_NECK0, Neck)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_SPINE_ROOT, Spine)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_L_UPPERARM, LShoulder)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_R_UPPERARM, RShoulder)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, MH_L_ELBOWGRP, LElbow) &&
		!GetPedBoneScreenCoordsScaled(ped, 22711, LElbow)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, MH_R_ELBOWGRP, RElbow) &&
		!GetPedBoneScreenCoordsScaled(ped, 24550, RElbow) &&
		!GetPedBoneScreenCoordsScaled(ped, 2992, RElbow) &&
		!GetPedBoneScreenCoordsScaled(ped, 37346, RElbow)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_L_HAND, LHand)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_R_HAND, RHand)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, RB_L_THIGHROLL, LHip)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, RB_R_THIGHROLL, RHip)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, RB_L_KNEEFRONT, LKnee)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, RB_R_KNEEFRONT, RKnee)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_L_FOOT, LFoot)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_R_FOOT, RFoot)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_L_TOE0, LToe)) return false;
	if (!GetPedBoneScreenCoordsScaled(ped, SKEL_R_TOE0, RToe)) return false;

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

bool ESP::RenderTextOnEntity(Entity ent, const char* Text)
{
	ImVec2 Pos;
	if (ESP::WorldToScreenScaled(Script::GetEntityCoords(ent), Pos.x, Pos.y))
	{
		RenderTextCentered(Text, Pos, Renderer::GetImGuiRGB32());
		
		return true;
	}

	return false;
}

void ESP::RenderText(const char* Text, ImVec2 Pos, ImU32 Color)
{
	ImGui::GetBackgroundDrawList()->AddText(Pos, Color, Text);
}

void ESP::RenderTextCentered(const char* Text, ImVec2 Pos, ImU32 Color)
{
	ImVec2 Size = ImGui::CalcTextSize(Text);
	Pos.x -= (Size.x / 2);
	Pos.y -= (Size.y / 2);

	ImGui::GetBackgroundDrawList()->AddText(Pos, Color, Text);
}

void ESP::RenderPedESP()
{
	const auto peds = Script::GetAllPeds();
	for (size_t i = 0; i < peds.size(); i++)
	{
		for (const auto& p : Lists::PedList)
		{
			if (p.second == Script::GetEntityModel(peds[i]))
			{
				RenderTextOnEntity(peds[i], p.first.c_str());
				break;
			}
		}
	}
}

void ESP::RenderESP()
{
	Timer t;

	TRY
	{
		RenderPedESP();
		ESP::RenderPedBoneESP(ScriptGlobal(35).Get<Ped>());
	}
	EXCEPT{ LOG_EXCEPTION(); }

	Timer::s_ESPTime = t.GetMillis();
}
