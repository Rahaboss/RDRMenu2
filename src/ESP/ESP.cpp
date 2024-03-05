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

	RenderText(Text, Pos, Color);
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
		// Skip local player
		if (ped == g_LocalPlayer.m_Entity)
			continue;

		// Skip animals
		if (Script::GetPedType(ped) == MPT_ANIMAL)
			continue;

		// Skip dead peds
		if (g_Settings["esp"]["ped"]["ignore_dead"].get<bool>() && Script::GetEntityHealth(ped) == 0)
			continue;

		// Render ped ESP
		RenderPedESP(ped, Script::GetEntityModel(ped),
			g_Settings["esp"]["ped"]["bone"].get<bool>(),
			g_Settings["esp"]["ped"]["box"].get<bool>(),
			g_Settings["esp"]["ped"]["model"].get<bool>()
		);
	}
}

bool ESP::RenderPedESP(Ped ped, Hash Model, bool BoneESP, bool BoxESP, bool ModelESP)
{
	// Get ped model box on screen
	BoxCoords Box{};
	if (!GetPedBoxCoords(ped, Box))
		return false;

	// Render bone ESP
	if (BoneESP)
	{
		if (!RenderPedBoneESP(ped))
			return false;
	}

	// Render bone ESP
	if (BoxESP)
		RenderBoxCoords(Box);

	// Get ESP text
	std::string ESPText;
	if (ModelESP)
	{
		const std::string ModelName = Lists::GetHashName(Model);
		ESPText.append((ModelName.empty() ? std::to_string(Model) : ModelName));

		if (ModelName.empty())
			CheckUnknownModel(Model);
	}

	// Render ESP text
	if (!ESPText.empty())
	{
		const ImVec2 TextSize = ImGui::CalcTextSize(ESPText.c_str());
		const ImVec2 TextPos{ ((Box.BottomRight.x + Box.TopLeft.x) / 2) - (TextSize.x / 2), Box.BottomRight.y };
		RenderText(ESPText.c_str(), TextPos, Renderer::GetImGuiRGBA32());
	}

	return true;
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
	RenderPedESP(g_LocalPlayer.m_Entity, g_LocalPlayer.m_Model,
		g_Settings["esp"]["player"]["bone"].get<bool>(),
		g_Settings["esp"]["player"]["box"].get<bool>(),
		g_Settings["esp"]["player"]["model"].get<bool>()
	);

#if !DIST
	if (g_Settings["esp"]["player"]["bone_debug"].get<bool>())
		RenderPedBoneDebugESP(g_LocalPlayer.m_Entity);
#endif // !DIST
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

bool ESP::GetPedBoxCoords(Ped ped, BoxCoords& BoxCoords)
{
	const Hash Bones[]{
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

	bool FirstValue = true;
	for (size_t i = 0; i < ARRAYSIZE(Bones); i++)
	{
		Hash Bone = Bones[i];
		
		ImVec2 Coords;
		if (GetPedBoneScreenCoordsScaled(ped, Bone, Coords))
		{
			if (FirstValue)
			{
				BoxCoords.TopLeft = BoxCoords.BottomRight = Coords;
				FirstValue = false;
			}
			else
			{
				BoxCoords.TopLeft.x = std::min(BoxCoords.TopLeft.x, Coords.x);
				BoxCoords.TopLeft.y = std::min(BoxCoords.TopLeft.y, Coords.y);
				BoxCoords.BottomRight.x = std::max(BoxCoords.BottomRight.x, Coords.x);
				BoxCoords.BottomRight.y = std::max(BoxCoords.BottomRight.y, Coords.y);
			}
		}
		else
			return false;
	}

	const Hash OptionalBones[]{
		MH_L_ELBOWGRP,
		22711,
		MH_R_ELBOWGRP,
		24550,
		2992,
		37346,

		SPR_HAIR_01,
		SPR_HAIR_010,
		SPR_HAIR_011,
		SPR_HAIR_012,
		SPR_HAIR_013,
		SPR_HAIR_02,
		SPR_HAIR_03,
		SPR_HAIR_04,
		SPR_HAIR_05,
		SPR_HAIR_06,
		SPR_HAIR_07,
		SPR_HAIR_08,
		SPR_HAIR_09,

		SKEL_L_FINGER00,
		SKEL_L_FINGER01,
		SKEL_L_FINGER02,
		SKEL_L_FINGER10,
		SKEL_L_FINGER11,
		SKEL_L_FINGER12,
		SKEL_L_FINGER13,
		SKEL_L_FINGER20,
		SKEL_L_FINGER21,
		SKEL_L_FINGER22,
		SKEL_L_FINGER23,
		SKEL_L_FINGER30,
		SKEL_L_FINGER31,
		SKEL_L_FINGER32,
		SKEL_L_FINGER33,
		SKEL_L_FINGER40,
		SKEL_L_FINGER41,
		SKEL_L_FINGER42,
		SKEL_L_FINGER43,

		SKEL_R_FINGER00,
		SKEL_R_FINGER01,
		SKEL_R_FINGER02,
		SKEL_R_FINGER10,
		SKEL_R_FINGER11,
		SKEL_R_FINGER12,
		SKEL_R_FINGER13,
		SKEL_R_FINGER20,
		SKEL_R_FINGER21,
		SKEL_R_FINGER22,
		SKEL_R_FINGER23,
		SKEL_R_FINGER30,
		SKEL_R_FINGER31,
		SKEL_R_FINGER32,
		SKEL_R_FINGER33,
		SKEL_R_FINGER40,
		SKEL_R_FINGER41,
		SKEL_R_FINGER42,
		SKEL_R_FINGER43,

		SKEL_L_TOE0,
		SKEL_L_TOE10,
		SKEL_L_TOE20,

		SKEL_R_TOE0,
		SKEL_R_TOE10,
		SKEL_R_TOE20,

		31176,
		31177,
		31178,
		31179,
		31180,
		31181,
		26982,
		59847,
	};

	for (size_t i = 0; i < ARRAYSIZE(OptionalBones); i++)
	{
		Hash Bone = OptionalBones[i];

		ImVec2 Coords;
		if (GetPedBoneScreenCoordsScaled(ped, Bone, Coords))
		{
			BoxCoords.TopLeft.x = std::min(BoxCoords.TopLeft.x, Coords.x);
			BoxCoords.TopLeft.y = std::min(BoxCoords.TopLeft.y, Coords.y);
			BoxCoords.BottomRight.x = std::max(BoxCoords.BottomRight.x, Coords.x);
			BoxCoords.BottomRight.y = std::max(BoxCoords.BottomRight.y, Coords.y);
		}
	}

	return true;
}

void ESP::RenderBoxCoords(const BoxCoords& BoxCoords)
{
	ImVec2 TopRight{ BoxCoords.BottomRight.x, BoxCoords.TopLeft.y };
	ImVec2 BottomLeft{ BoxCoords.TopLeft.x, BoxCoords.BottomRight.y };

	ImGui::GetBackgroundDrawList()->AddRect(BoxCoords.TopLeft, BoxCoords.BottomRight, Renderer::GetImGuiRGBA32(), 0, 0, 3);

#if 0
	int x = (int)BoxCoords.TopLeft.x - 5;
	int y = (int)BoxCoords.TopLeft.y - 1;
	int y2 = (int)BottomLeft.y + 1;

	size_t cycle = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count() % 10;

	int y3 = ((y2 - y) / 10) * (1 + (int)cycle) + y;

	//std::vector<ImVec2> Health{ ImVec2{ (int)BoxCoords.TopLeft.x - 2, (int)BoxCoords.TopLeft.y - 1 }, ImVec2{ (int)BottomLeft.x - 5, (int)BottomLeft.y + 1 } };
	std::vector<ImVec2> Health{ {(float)x, (float)y}, {(float)x, (float)y2} };
	std::vector<ImVec2> Health2{ {(float)x, (float)y}, {(float)x, (float)y3} };

	//ImGui::GetBackgroundDrawList()->AddRectFilled(Health[0], Health[1], IM_COL32(0, 0, 0, 255), 0, 0);
	//ImGui::GetBackgroundDrawList()->AddRectFilled(Health[0], Health[1], IM_COL32(0, 255, 0, 255), 0, 0);

	RenderLineArray(Health, IM_COL32(0, 0, 0, 255), 4);
	RenderLineArray(Health2, IM_COL32(0, 255, 0, 255), 2);
#endif
}

void ESP::RenderPedBoneDebugESP(Ped ped)
{
	ImVec2 ScreenPos;
	for (int i = 0; i < 0x10000; i++)
	{
		if (GetPedBoneScreenCoordsScaled(ped, i, ScreenPos))
		{
			const std::string Text = std::to_string(i);
			RenderText(Text.c_str(), ScreenPos);
		}
	}
}
