#include "pch.h"
#include "Model.h"
#include "Util/String.h"
#include "Config/Lists.h"
#include "Script/PlayerInfo.h"
#include "Thread/JobQueue.h"
#include "Script/Player.h"
#include "Rage/natives.h"
#include "Script/Ped.h"
#include "Rage/ScriptGlobal.h"

static bool func_2556(int iParam0)
{
	if (iParam0 <= -1 || iParam0 > 9)
	{
		return false;
	}
	return true;
}

static int func_33()
{
	return ScriptGlobal(1899515).Get<int>();
}

static bool func_2557()
{
	return ScriptGlobal(40).At(7748).At(6).Get<int>() > 0;
}

static bool func_2241(int iParam0)
{
	return (ScriptGlobal(40).At(7748).At(4).Get<int>() & iParam0) != 0;
}

static int func_2787()
{
	if (func_2241(8))
	{
		return 8;
	}
	else if (func_2241(4))
	{
		return 4;
	}
	else if (func_2241(2))
	{
		return 2;
	}
	return 1;
}

static int func_2942(float fParam0, float fParam1)
{
	return BUILTIN::FLOOR((((fParam0 / fParam1) - BUILTIN::TO_FLOAT(BUILTIN::FLOOR((fParam0 / fParam1)))) * 24.0f));
}

static int func_2943(float fParam0, float fParam1)
{
	return BUILTIN::FLOOR((((fParam0 / fParam1) - BUILTIN::TO_FLOAT(BUILTIN::FLOOR((fParam0 / fParam1)))) * 60.0f));
}

static int func_2944(float fParam0, float fParam1)
{
	return BUILTIN::FLOOR((((fParam0 / fParam1) - BUILTIN::TO_FLOAT(BUILTIN::FLOOR((fParam0 / fParam1)))) * 60.0f));
}

static void func_2788(int* iParam0, int* iParam1, int* uParam2, int* uParam3, bool bParam4, int iParam5)
{
	int iVar0;
	int iVar1;
	int iVar2;
	float fVar3;

	fVar3 = BUILTIN::TO_FLOAT(iParam5);
	iVar0 = func_2942(BUILTIN::TO_FLOAT(*iParam0), fVar3);
	iVar1 = func_2943(BUILTIN::TO_FLOAT(*iParam1), fVar3);
	iVar2 = func_2944(BUILTIN::TO_FLOAT(*uParam2), fVar3);
	*iParam0 /= iParam5;
	*iParam1 /= iParam5;
	*uParam2 /= iParam5;
	*uParam3 /= iParam5;
	*iParam1 += iVar0;
	*uParam2 += iVar1;
	*uParam3 += iVar2;
	if (bParam4)
	{
		*uParam3 *= -1;
		*uParam2 *= -1;
		*iParam1 *= -1;
		*iParam0 *= -1;
	}
}

static void func_2558(int iParam0, int* iParam1, int* iParam2, bool bParam3)
{
	int uVar0;
	int uVar1;

	switch (iParam0)
	{
	case 0:
		*iParam1 = 2;
		*iParam2 = 0;
		break;
	case 1:
		*iParam1 = 2;
		*iParam2 = 0;
		break;
	case 2:
		*iParam1 = 4;
		*iParam2 = 0;
		break;
	case 3:
		*iParam1 = 8;
		*iParam2 = 0;
		break;
	case 4:
		*iParam1 = 12;
		*iParam2 = 0;
		break;
	case 5:
		*iParam1 = 20;
		*iParam2 = 0;
		break;
	case 6:
		*iParam1 = 20;
		*iParam2 = 0;
		break;
	case 7:
		*iParam1 = 32;
		*iParam2 = 0;
		break;
	case 8:
		*iParam1 = 32;
		*iParam2 = 0;
		break;
	case 9:
		*iParam1 = 52;
		*iParam2 = 0;
		break;
	}
	if (bParam3)
	{
		func_2788(iParam1, iParam2, &uVar0, &uVar1, 0, func_2787());
	}
}

static int func_715(bool bParam0, int iParam1, int iParam2)
{
	if (bParam0)
	{
		return iParam1;
	}
	return iParam2;
}

static int func_326(int iParam0)
{
	return (BUILTIN::SHIFT_RIGHT(iParam0, 26) & 31 * func_715(MISC::IS_BIT_SET(iParam0, 31), -1, 1)) + 1898;
}

static int func_327(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 22) & 15;
}

static int func_328(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 17) & 31;
}

static int func_329(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 12) & 31;
}

static int func_330(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 6) & 63;
}

static int func_331(int iParam0)
{
	return BUILTIN::SHIFT_RIGHT(iParam0, 0) & 63;
}

static int func_332(int iParam0, int iParam1)
{
	if (iParam1 < 0)
	{
		iParam1 = 0;
	}
	switch (iParam0)
	{
	case 0:
	case 2:
	case 4:
	case 6:
	case 7:
	case 9:
	case 11:
		return 31;
	case 3:
	case 5:
	case 8:
	case 10:
		return 30;
	case 1:
		if ((iParam1 % 4) == 0)
		{
			if ((iParam1 % 100) != 0)
			{
				return 29;
			}
			else if ((iParam1 % 400) == 0)
			{
				return 29;
			}
		}
		return 28;
	default:
		break;
	}
	return 30;
}

static void func_716(int* iParam0, int iParam1)
{
	if (iParam1 < 0)
	{
		return;
	}
	if (iParam1 > 1930 || iParam1 < 1866)
	{
		return;
	}
	*iParam0 -= *iParam0 & 2080374784;
	if (iParam1 < 1898)
	{
		*iParam0 |= BUILTIN::SHIFT_LEFT((1898 - iParam1), 26);
		*iParam0 |= (1 << 31);
	}
	else
	{
		*iParam0 |= BUILTIN::SHIFT_LEFT((iParam1 - 1898), 26);
		*iParam0 -= *iParam0 & (1 << 31);
	}
}

static void func_717(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 > 11)
	{
		return;
	}
	*iParam0 -= *iParam0 & 62914560;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 22);
}

static void func_718(int* iParam0, int iParam1)
{
	int iVar0;
	int iVar1;

	iVar0 = func_327(*iParam0);
	iVar1 = func_326(*iParam0);
	if (iParam1 < 1 || iParam1 > func_332(iVar0, iVar1))
	{
		return;
	}
	*iParam0 -= *iParam0 & 4063232;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 17);
}

static void func_719(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 > 23)
	{
		return;
	}
	*iParam0 -= *iParam0 & 126976;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 12);
}

static void func_720(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 >= 60)
	{
		return;
	}
	*iParam0 -= *iParam0 & 4032;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 6);
}

static void func_721(int* iParam0, int iParam1)
{
	if (iParam1 < 0 || iParam1 >= 60)
	{
		return;
	}
	*iParam0 -= *iParam0 & 63;
	*iParam0 |= BUILTIN::SHIFT_LEFT(iParam1, 0);
}

static void func_333(int* iParam0, int iParam1, int iParam2, int iParam3, int iParam4, int iParam5, int iParam6)
{
	func_716(iParam0, iParam6);
	func_717(iParam0, iParam5);
	func_718(iParam0, iParam4);
	func_719(iParam0, iParam3);
	func_720(iParam0, iParam2);
	func_721(iParam0, iParam1);
}

static void func_1025(int* iParam0, int iParam1, int iParam2, int iParam3, int iParam4, int iParam5, int iParam6, bool bParam7)
{
	int iVar0;
	int iVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	int iVar5;
	int iVar6;

	iVar0 = func_326(*iParam0);
	iVar1 = func_327(*iParam0);
	iVar2 = func_328(*iParam0);
	iVar3 = func_329(*iParam0);
	iVar4 = func_330(*iParam0);
	iVar5 = func_331(*iParam0);
	if (((((iParam6 == 0 && iParam5 == 0) && iParam4 == 0) && iParam3 == 0) && iParam2 == 0) && iParam1 == 0)
	{
		return;
	}
	if (iParam1 < 0)
	{
		return;
	}
	if (iParam2 < 0)
	{
		return;
	}
	if (iParam3 < 0)
	{
		return;
	}
	if (iParam4 < 0)
	{
		return;
	}
	if (iParam5 < 0)
	{
		return;
	}
	if (iParam6 < 0)
	{
		return;
	}
	iVar5 += iParam1;
	while (iVar5 >= 60)
	{
		iParam2++;
		iVar5 -= 60;
	}
	iVar4 += iParam2;
	while (iVar4 >= 60)
	{
		iParam3++;
		iVar4 -= 60;
	}
	iVar3 += iParam3;
	while (iVar3 >= 24)
	{
		iParam4++;
		iVar3 -= 24;
	}
	iVar2 += iParam4;
	iVar6 = func_332(iVar1, iVar0);
	while (iVar2 > iVar6)
	{
		iParam5++;
		iVar2 -= iVar6;
		if (iVar1 > 11)
		{
			iParam6++;
			iVar1 -= 12;
		}
		iVar6 = func_332(iVar1, iVar0);
	}
	iVar1 += iParam5;
	while (iVar1 > 11)
	{
		iParam6++;
		iVar1 -= 12;
	}
	if (!bParam7)
	{
		iVar0 += iParam6;
	}
	func_333(iParam0, iVar5, iVar4, iVar3, iVar2, iVar1, iVar0);
}

static void func_2239(int iParam0, bool bParam1)
{
	int iVar0;
	int iVar1;
	bool bVar2;

	if (!func_2556(iParam0))
	{
		return;
	}
	if (ScriptGlobal(40).At(7748).At(1).Get<int>() == iParam0)
	{
		return;
	}
	ScriptGlobal(40).At(7748).At(1).Get<int&>() = iParam0;
	if (!bParam1)
	{
		ScriptGlobal(40).At(7748).At(5).Get<int&>() = func_33();
		bVar2 = (ScriptGlobal(40).At(7748).At(1).Get<int>() < 9 && func_2557());
		func_2558(ScriptGlobal(40).At(7748).At(1).Get<int>(), &iVar0, &iVar1, bVar2);
		func_1025(ScriptGlobal(40).At(7748).At(5).Get<int*>(), 0, 0, iVar1, iVar0, 0, 0, 0);
	}
}

static int func_765()
{
	return ScriptGlobal(40).At(7748).At(1).Get<int>();
}

static void func_2555()
{
	int iVar0;

	if (func_765() <= 3)
	{
		iVar0 = func_33();
		func_1025(&iVar0, 0, 0, 0, 2, 0, 0, 0);
		DECORATOR::DECOR_SET_INT(ScriptGlobal(35).Get<int>(), "hairShort", iVar0);
		if (DECORATOR::DECOR_EXIST_ON(ScriptGlobal(35).Get<int>(), "hairLong"))
		{
			DECORATOR::DECOR_REMOVE(ScriptGlobal(35).Get<int>(), "hairLong");
		}
	}
	else if (DECORATOR::DECOR_EXIST_ON(ScriptGlobal(35).Get<int>(), "hairShort"))
	{
		DECORATOR::DECOR_REMOVE(ScriptGlobal(35).Get<int>(), "hairShort");
	}
}

static void RenderHairChanger()
{
	if (ScriptGlobal(40).At(7748).Get<int*>())
	{
		ImGui::Text("0) playerHeadHair.ePomadeWearOffTimer: %d", ScriptGlobal(40).At(7748).Get<int>());
		ImGui::Text("1) playerHeadHair.headHairLength: %d", ScriptGlobal(40).At(7748).At(1).Get<int>());
		ImGui::Text("2) playerHeadHair.eHeadHairCut: %d", ScriptGlobal(40).At(7748).At(2).Get<int>());
		ImGui::Text("3) playerHeadHair.eHeadHairStyle: %d", ScriptGlobal(40).At(7748).At(3).Get<int>());
		ImGui::Text("4) playerHeadHair.eHeadHairFlags: %d", ScriptGlobal(40).At(7748).At(4).Get<int>());
		ImGui::Text("5) playerHeadHair.headHairNextGrowthTimer: %d", ScriptGlobal(40).At(7748).At(5).Get<int>());
		ImGui::Text("6) playerHeadHair.iNumAcceleratedGrowthCycles: %d", ScriptGlobal(40).At(7748).At(6).Get<int>());

		ImGui::Separator();
		int x = ScriptGlobal(40).At(7748).At(1).Get<int>();
		if (ImGui::SliderInt("Hair Length", &x, 0, 9))
		{
			func_2239(x, 0);
			ScriptGlobal(40).At(7748).At(5).Get<int&>() = MISC::GET_GAME_TIMER() + 1000;
			func_2555();
		}
	}
}

static int s_OutfitPreset = 0;
static char s_ModelFilter[200]{};
static void RenderModelChanger()
{
	ImGui::SetNextItemWidth(230.0f);
	ImGui::InputText("Filter Models", s_ModelFilter, IM_ARRAYSIZE(s_ModelFilter));
	ImGui::SameLine();
	if (ImGui::Button("Clear##clear_model"))
		s_ModelFilter[0] = '\0';
	ImGui::Separator();

	ImGui::BeginChild("player_model_list");
	const std::string FilterUpper = Util::StringToUpperCopy(s_ModelFilter);
	for (const auto& [Name, Model] : Lists::PedList)
	{
		if (!Util::StringContains(Name, FilterUpper))
			continue;

		if (ImGui::Selectable(Name.c_str(), g_LocalPlayer.m_Model == Model))
		{
			QUEUE_JOB(=)
			{
				Script::SetPlayerModel(Model);
				s_OutfitPreset = 0;
			}
			END_JOB()
		}
	}
	ImGui::EndChild(); // player_model_list
}

static void QueueSetOutfitPreset()
{
	const int MaxOutfits = PED::GET_NUM_META_PED_OUTFITS(g_LocalPlayer.m_Entity) - 1;
	s_OutfitPreset = std::clamp(s_OutfitPreset, 0, MaxOutfits);
	Script::SetPedOutfitPreset(g_LocalPlayer.m_Entity, s_OutfitPreset);
}

static void RenderOutfitPresetList()
{
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted("Set Outfit Preset:");
	ImGui::SameLine();

	ImGui::PushButtonRepeat(true);

	if (ImGui::ArrowButton("##outfit_left", ImGuiDir_Left))
	{
		s_OutfitPreset--;
		JobQueue::Add(QueueSetOutfitPreset);
	}
	ImGui::SameLine();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("%d", s_OutfitPreset);
	ImGui::SameLine();

	if (ImGui::ArrowButton("##outfit_right", ImGuiDir_Right))
	{
		s_OutfitPreset++;
		JobQueue::Add(QueueSetOutfitPreset);
	}

	ImGui::PopButtonRepeat();
	ImGui::SameLine();

	if (ImGui::Button("0##reset_outfit_preset"))
	{
		s_OutfitPreset = 0;
		JobQueue::Add(QueueSetOutfitPreset);
	}
}

static Hash s_SelectedModel = 0;
static std::string s_SelectedModelName;
static void RenderMetaPedOutfitList()
{
	RenderOutfitPresetList();
	ImGui::Separator();

	if (s_SelectedModel != g_LocalPlayer.m_Model)
	{
		s_SelectedModel = g_LocalPlayer.m_Model;
		s_SelectedModelName = Util::StringToUpperCopy(Lists::GetHashName(s_SelectedModel));
	}

	const json& Outfits = Lists::MetaPedOutfits[s_SelectedModelName];
	if (!Outfits.is_array())
		return;

	ImGui::BeginChild("player_outfit_list");

	for (size_t i = 0; i < Outfits.size(); i++)
	{
		std::string Name;
		Hash Model = 0;

		if (Outfits[i].is_string())
		{
			Name = Outfits[i].get_ref<const std::string&>();
			Model = rage::joaat(Name);
		}
		else if (Outfits[i].is_number())
		{
			Model = Outfits[i].get<uint32_t>();
			Name = std::to_string(Model);
		}
		
		if (Name.empty())
			continue;

		if (ImGui::Selectable(Name.c_str(), PED::_IS_META_PED_OUTFIT_EQUIPPED(g_LocalPlayer.m_Entity, Model)))
		{
			QUEUE_JOB(=)
			{
				Script::SetMetaPedOutfit(g_LocalPlayer.m_Entity, Model);
			}
			END_JOB()
		}
	}

	ImGui::EndChild(); // player_model_list
}

void Menu::RenderModelTab()
{
	if (!ImGui::BeginTabItem("Model"))
		return;

	ImGui::BeginChild("model_child");

	const ImVec2 Region = ImGui::GetContentRegionAvail();

	ImGui::BeginChild("model_left", ImVec2(Region.x * 0.6f, 0));

	TRY
	{
		ImGui::SeparatorText("Set Hair");
		RenderHairChanger();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild(); // model_left
	ImGui::SameLine();
	ImGui::BeginChild("model_right");

	TRY
	{
		ImGui::BeginChild("model_top_right", ImVec2(0, Region.y / 2));
		ImGui::SeparatorText("Set Model");
		RenderModelChanger();
		ImGui::EndChild(); // model_top_right

		ImGui::BeginChild("model_bottom_right");
		ImGui::SeparatorText("Set Outfit");
		RenderMetaPedOutfitList();
		ImGui::EndChild(); // model_bottom_right
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild(); // model_right

	ImGui::EndChild(); // model_child
	ImGui::EndTabItem();
}
