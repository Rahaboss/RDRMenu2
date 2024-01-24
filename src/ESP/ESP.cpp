#include "pch.h"
#include "ESP.h"
#include "Screen.h"
#include "Renderer/RGB.h"
#include "Rage/enums.h"
#include "Script/Entity.h"
#include "Config/Lists.h"
#include "Menu/Menu.h"
#include "Util/Timer.h"
#include "Rage/ScriptGlobal.h"
#include "Config/Settings.h"
#include "Script/PlayerInfo.h"
#include "Script/Ped.h"
#include "Util/Container.h"

void ESP::RenderLineArray(const std::vector<ImVec2>& vec, ImU32 Color, float Thickness)
{
	if (vec.size() < 2)
		return;

	for (const ImVec2& p : vec)
	{
		if (p.x < 0 || p.y < 0)
			return;
	}

	ImDrawList* l = ImGui::GetBackgroundDrawList();

	for (size_t i = 0; i < vec.size(); i++)
	{
		l->AddLine(vec[i], vec[i + 1], Color, Thickness);

		if (i + 2 == vec.size())
			return;
	}
}

bool ESP::RenderPedBoneESP(Ped ped)
{
	const ImU32 Color = Renderer::GetImGuiRGBA32();
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
		RenderTextCentered(Text, Pos, Renderer::GetImGuiRGBA32());
		
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

static std::vector<Hash> s_UnknownModels;
static void CheckUnknownModel(Hash Model)
{
	if (Lists::HashNameList.empty())
		return;

	if (Util::ContainerContains(s_UnknownModels, Model))
		return;

	s_UnknownModels.push_back(Model);
	LOG_TEXT(__FUNCTION__": Unknown model %u.", Model);
}

void ESP::RenderAnimalESP()
{
	for (Ped ped : Script::GetAllPeds())
	{
		if (ped == g_LocalPlayer.m_Entity)
			continue;

		if (Script::GetPedType(ped) != MPT_ANIMAL)
			continue;

		if (Script::GetEntityHealth(ped) == 0 && g_Settings["esp"]["animal"]["ignore_dead"].get<bool>())
			continue;

		std::string ESPText;
		const Hash Model = Script::GetEntityModel(ped);

		if (g_Settings["esp"]["animal"]["model"].get<bool>())
		{
			std::string ModelName = Lists::GetHashName(Model);
			ESPText.append((ModelName.empty() ? std::to_string(Model) : ModelName));

			if (ModelName.empty())
				CheckUnknownModel(Model);
		}

		if (!ESPText.empty())
			RenderTextOnEntity(ped, ESPText.c_str());
	}
}

void ESP::RenderObjectESP()
{
	for (Object obj : Script::GetAllObjects())
	{
		std::string ESPText;
		const Hash Model = Script::GetEntityModel(obj);

		if (g_Settings["esp"]["object"]["model"].get<bool>())
		{
			std::string ModelName = Lists::GetHashName(Model);
			ESPText.append((ModelName.empty() ? std::to_string(Model) : ModelName));

			if (ModelName.empty())
				CheckUnknownModel(Model);
		}

		if (!ESPText.empty())
			RenderTextOnEntity(obj, ESPText.c_str());
	}
}

void ESP::RenderPedESP()
{
	for (Ped ped : Script::GetAllPeds())
	{
		if (ped == g_LocalPlayer.m_Entity)
			continue;

		if (Script::GetPedType(ped) == MPT_ANIMAL)
			continue;

		if (Script::GetEntityHealth(ped) == 0 && g_Settings["esp"]["ped"]["ignore_dead"].get<bool>())
			continue;

		std::string ESPText;
		const Hash Model = Script::GetEntityModel(ped);

		if (g_Settings["esp"]["ped"]["model"].get<bool>())
		{
			std::string ModelName = Lists::GetHashName(Model);
			ESPText.append((ModelName.empty() ? std::to_string(Model) : ModelName));

			if (ModelName.empty())
				CheckUnknownModel(Model);
		}

		if (g_Settings["esp"]["ped"]["bone"].get<bool>())
			RenderPedBoneESP(ped);

		if (!ESPText.empty())
			RenderTextOnEntity(ped, ESPText.c_str());
	}
}

void ESP::RenderPickupESP()
{
	for (Object obj : Script::GetAllPickups())
	{
		std::string ESPText;
		const Hash Model = Script::GetEntityModel(obj);

		if (g_Settings["esp"]["pickup"]["model"].get<bool>())
		{
			std::string ModelName = Lists::GetHashName(Model);
			ESPText.append((ModelName.empty() ? std::to_string(Model) : ModelName));

			if (ModelName.empty())
				CheckUnknownModel(Model);
		}

		if (!ESPText.empty())
			RenderTextOnEntity(obj, ESPText.c_str());
	}
}

void ESP::RenderLocalPlayerESP()
{
	if (g_Settings["esp"]["player"]["bone"].get<bool>())
		RenderPedBoneESP(g_LocalPlayer.m_Entity);

	if (g_Settings["esp"]["player"]["model"].get<bool>())
	{
		std::string ModelName = Lists::GetHashName(g_LocalPlayer.m_Model);
		RenderTextOnEntity(g_LocalPlayer.m_Entity, ModelName.c_str());
	}
}

void ESP::RenderVehicleESP()
{
	for (Vehicle veh : Script::GetAllVehicles())
	{
		std::string ESPText;
		const Hash Model = Script::GetEntityModel(veh);

		if (g_Settings["esp"]["vehicle"]["model"].get<bool>())
		{
			std::string ModelName = Lists::GetHashName(Model);
			ESPText.append((ModelName.empty() ? std::to_string(Model) : ModelName));

			if (ModelName.empty())
				CheckUnknownModel(Model);
		}

		if (!ESPText.empty())
			RenderTextOnEntity(veh, ESPText.c_str());
	}
}

void ESP::RenderESP()
{
	TRY
	{
		Timer ESPTimer;

		if (g_Settings["esp"]["animal"]["enable"].get<bool>())
			RenderAnimalESP();

		if (g_Settings["esp"]["object"]["enable"].get<bool>())
			RenderObjectESP();
		
		if (g_Settings["esp"]["ped"]["enable"].get<bool>())
			RenderPedESP();

		if (g_Settings["esp"]["pickup"]["enable"].get<bool>())
			RenderPickupESP();

		if (g_Settings["esp"]["player"]["enable"].get<bool>())
			RenderLocalPlayerESP();

		if (g_Settings["esp"]["vehicle"]["enable"].get<bool>())
			RenderVehicleESP();

		Timer::s_ESPTime = ESPTimer.GetMillis();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
