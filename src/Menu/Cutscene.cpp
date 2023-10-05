#include "pch.h"
#include "Cutscene.h"
#include "Config/Lists.h"
#include "Script/Cutscene.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Util/String.h"
#include "Config/Settings.h"
#include "Config/Config.h"

static AnimScene s_CutsceneIndex = 0;
static void QueuePlayCutscene(const char* CutsceneName)
{
	if (s_CutsceneIndex)
	{
		LOG_TEXT("%s: Cutscene already running.", __FUNCTION__);
		return;
	}

	s_CutsceneIndex = -1;

	QUEUE_JOB(=)
	{
		[=]() {
			Script::CutsceneHelper Cutscene{ CutsceneName };
			s_CutsceneIndex = Cutscene.GetCutscene();
			Cutscene.PlayAutomatically();
			s_CutsceneIndex = 0;
		}();
	}
	END_JOB()
}

static void QueueSkipCutscene()
{
	QUEUE_JOB(=)
	{
		if (s_CutsceneIndex != -1 && !ANIMSCENE::HAS_ANIM_SCENE_EXITED(s_CutsceneIndex, false))
			ANIMSCENE::TRIGGER_ANIM_SCENE_SKIP(s_CutsceneIndex);
	}
	END_JOB()
}

static Hash StringToHash(const char* String)
{
	char* p;
	Hash h = static_cast<Hash>(strtoul(String, &p, 10));
	if (p != String + strlen(String))
		return 0;
	return h;
}

static bool EntityExistsInJSONArray(const json& Array, const std::string& Name)
{
	if (Array.is_array())
	{
		auto Temp2 = Name;
		Util::StringToLower(Temp2);
		for (const auto& j : Array)
		{
			if (j.contains("name"))
			{
				auto Temp = j["name"].get_ref<const std::string&>();
				Util::StringToLower(Temp);

				if (Temp == Temp2)
					return true;
			}
		}
	}

	return false;
}

static bool IsNewEntityValid(json& SelectedCutscene, const char* Model, const char* Name)
{
	return Util::IsStringValid(Model) || Util::IsStringValid(Name)
		|| (SelectedCutscene.contains("peds") && EntityExistsInJSONArray(SelectedCutscene["peds"], Name))
		|| (SelectedCutscene.contains("objects") && EntityExistsInJSONArray(SelectedCutscene["objects"], Name))
		|| (SelectedCutscene.contains("vehicles") && EntityExistsInJSONArray(SelectedCutscene["vehicles"], Name));
}

static void RenderAddPed(json& SelectedCutscene)
{
	if (!ImGui::BeginPopup("Add Ped##ped_popup"))
		return;

	ImGui::SeparatorText("Add Cutscene Ped");

	static char s_ModelInput[0x100]{};
	ImGui::InputText("Model", s_ModelInput, IM_ARRAYSIZE(s_ModelInput));

	static char s_NameInput[0x100]{};
	ImGui::InputText("Name", s_NameInput, IM_ARRAYSIZE(s_NameInput));

	static char s_MetaPedOutfitInput[0x100]{};
	ImGui::InputText("Meta Ped Outfit", s_MetaPedOutfitInput, IM_ARRAYSIZE(s_MetaPedOutfitInput));

	static char s_MetaPedWearableInput[0x100]{};
	ImGui::InputText("Meta Ped Wearable", s_MetaPedWearableInput, IM_ARRAYSIZE(s_MetaPedWearableInput));

	static bool s_RemoveWeapons = false;
	ImGui::Checkbox("Remove Weapons", &s_RemoveWeapons);

	const bool IsInvalid = IsNewEntityValid(SelectedCutscene, s_ModelInput, s_NameInput);

	if (IsInvalid)
		ImGui::BeginDisabled();
	if (ImGui::Button("Add##add_ped_btn"))
	{
		json j;
		std::string ModelName = s_ModelInput;
		Util::StringToLower(ModelName);
		j["name"] = s_NameInput;

		if (Hash h = StringToHash(ModelName.c_str()))
			j["model"] = h;
		else
			j["model"] = ModelName;

		if (Util::IsStringValid(s_MetaPedOutfitInput))
		{
			if (Hash h = StringToHash(s_MetaPedOutfitInput))
				j["meta_ped_outfit"] = h;
			else
				j["meta_ped_outfit"] = s_MetaPedOutfitInput;
		}

		if (Util::IsStringValid(s_MetaPedWearableInput))
		{
			if (Hash h = StringToHash(s_MetaPedWearableInput))
				j["meta_ped_wearable"] = h;
			else
				j["meta_ped_wearable"] = s_MetaPedWearableInput;
		}

		if (s_RemoveWeapons)
			j["remove_weapons"] = s_RemoveWeapons;

		SelectedCutscene["peds"].push_back(j);

		ImGui::CloseCurrentPopup();
	}
	if (IsInvalid)
		ImGui::EndDisabled();
	
	ImGui::EndPopup();
}

static void RenderAddObject(json& SelectedCutscene)
{
	if (!ImGui::BeginPopup("Add Object##obj_popup"))
		return;

	ImGui::SeparatorText("Add Cutscene Object");

	static char s_ModelInput[0x100]{};
	ImGui::InputText("Model", s_ModelInput, IM_ARRAYSIZE(s_ModelInput));

	static char s_NameInput[0x100]{};
	ImGui::InputText("Name", s_NameInput, IM_ARRAYSIZE(s_NameInput));

	const bool IsInvalid = IsNewEntityValid(SelectedCutscene, s_ModelInput, s_NameInput);

	if (IsInvalid)
		ImGui::BeginDisabled();
	if (ImGui::Button("Add##add_obj_btn"))
	{
		json j;
		std::string ModelName = s_ModelInput;
		Util::StringToLower(ModelName);
		j["name"] = s_NameInput;

		if (Hash h = StringToHash(ModelName.c_str()))
			j["model"] = h;
		else
			j["model"] = ModelName;

		SelectedCutscene["objects"].push_back(j);

		ImGui::CloseCurrentPopup();
	}
	if (IsInvalid)
		ImGui::EndDisabled();

	ImGui::EndPopup();
}

static void RenderAddVehicle(json& SelectedCutscene)
{
	if (!ImGui::BeginPopup("Add Vehicle##veh_popup"))
		return;

	ImGui::SeparatorText("Add Cutscene Vehicle");

	static char s_ModelInput[0x100]{};
	ImGui::InputText("Model", s_ModelInput, IM_ARRAYSIZE(s_ModelInput));

	static char s_NameInput[0x100]{};
	ImGui::InputText("Name", s_NameInput, IM_ARRAYSIZE(s_NameInput));

	ImGui::TextUnformatted("Extras");
	static std::vector<int> s_Extras;
	for (auto it = s_Extras.begin(); it < s_Extras.end(); it++)
	{
		ImGui::BulletText("%d", *it);
		ImGui::SameLine();
		std::string DelBtnStr{ "X##" };
		DelBtnStr.append(std::to_string(*it));
		if (ImGui::SmallButton(DelBtnStr.c_str()))
			it = s_Extras.erase(it);
	}

	if (s_Extras.size() < 16)
	{
		if (ImGui::BeginCombo("Add Extra", "Add Extra"))
		{
			for (int i = 1; i <= 16; i++)
			{
				if (std::find(s_Extras.begin(), s_Extras.end(), i) == s_Extras.end())
				{
					if (ImGui::Selectable(std::to_string(i).c_str()))
						s_Extras.push_back(i);
				}
			}
			ImGui::EndCombo();
		}
	}

	const bool IsInvalid = IsNewEntityValid(SelectedCutscene, s_ModelInput, s_NameInput);

	if (IsInvalid)
		ImGui::BeginDisabled();
	if (ImGui::Button("Add##add_veh_btn"))
	{
		json j;
		std::string ModelName = s_ModelInput;
		Util::StringToLower(ModelName);
		j["name"] = s_NameInput;

		if (Hash h = StringToHash(ModelName.c_str()))
			j["model"] = h;
		else
			j["model"] = ModelName;

		if (!s_Extras.empty())
		{
			auto Temp = s_Extras;
			std::sort(Temp.begin(), Temp.end());
			j["extras"] = Temp;
		}

		SelectedCutscene["vehicles"].push_back(j);

		ImGui::CloseCurrentPopup();
	}
	if (IsInvalid)
		ImGui::EndDisabled();

	ImGui::EndPopup();
}

static void RenderCutsceneInfo(json::iterator& SelectedCutscene)
{
	json& Cutscene = *SelectedCutscene;
	ImGui::SeparatorText("Cutscene Controls");

	const bool IsCutscenePlaying = s_CutsceneIndex != 0;

	if (IsCutscenePlaying)
		ImGui::BeginDisabled();
	if (ImGui::Button("Play Cutscene"))
		QueuePlayCutscene(Cutscene["id"].get_ref<const std::string&>().c_str());
	if (IsCutscenePlaying)
		ImGui::EndDisabled();

	ImGui::SameLine();

	if (!IsCutscenePlaying)
		ImGui::BeginDisabled();
	if (ImGui::Button("Skip Cutscene"))
		QueueSkipCutscene();
	if (!IsCutscenePlaying)
		ImGui::EndDisabled();
	
	if (ImGui::Button("Reload List"))
	{
		Lists::InitCutsceneList();
		for (json::iterator it = Lists::CutsceneList.begin(); it < Lists::CutsceneList.end(); it++)
		{
			if ((*it)["id"] == Cutscene["id"])
				SelectedCutscene = it;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Save List"))
	{
		std::filesystem::path Path{ Config::GetConfigPath().append("Cutscenes.json") };
		std::ofstream File{ Path };

		if (File)
			File << Lists::CutsceneList.dump(1, '\t') << '\n';
		else
			LOG_TEXT("Can't open file: %s.", Path.string().c_str());
	}

	ImGui::Checkbox("Disable Black Borders", g_Settings["disable_black_borders"].get<bool*>());

	ImGui::BeginChild("cutscene_info_inner");

	ImGui::SeparatorText("Cutscene Info");
	ImGui::Text("Name: %s", Cutscene["id"].get_ref<const std::string&>().c_str());

	if (Cutscene.contains("playback_id"))
		ImGui::Text("playbackListName: %s", Cutscene["playback_id"].get_ref<const std::string&>().c_str());

	if (Cutscene.contains("player_model"))
		ImGui::Text("Player Model: %s", Cutscene["player_model"].get_ref<const std::string&>().c_str());

	ImGui::Text("Peds:");
	ImGui::SameLine();
	if (ImGui::SmallButton("Add Ped"))
		ImGui::OpenPopup("Add Ped##ped_popup");

	RenderAddPed(Cutscene);

	if (Cutscene.contains("peds"))
	{
		for (json::iterator it = Cutscene["peds"].begin(); it < Cutscene["peds"].end(); it++)
		{
			const json& ped = *it;
			ImGui::BulletText(ped["name"].get_ref<const std::string&>().c_str());
			ImGui::SameLine();
			std::string DelBtnStr{ "X##" };
			DelBtnStr.append(ped["name"].get_ref<const std::string&>());
			if (ImGui::SmallButton(DelBtnStr.c_str()))
			{
				it = Cutscene["peds"].erase(it);
				if (Cutscene["peds"].empty())
				{
					Cutscene.erase("peds");
					break;
				}
			}
		}
	}

	ImGui::Text("Objects:");
	ImGui::SameLine();
	if (ImGui::SmallButton("Add Object"))
		ImGui::OpenPopup("Add Object##obj_popup");

	RenderAddObject(Cutscene);

	if (Cutscene.contains("objects"))
	{
		for (json::iterator it = Cutscene["objects"].begin(); it < Cutscene["objects"].end(); it++)
		{
			const json& obj = *it;
			ImGui::BulletText(obj["name"].get_ref<const std::string&>().c_str());
			ImGui::SameLine();
			std::string DelBtnStr{ "X##" };
			DelBtnStr.append(obj["name"].get_ref<const std::string&>());
			if (ImGui::SmallButton(DelBtnStr.c_str()))
			{
				it = Cutscene["objects"].erase(it);
				if (Cutscene["objects"].empty())
				{
					Cutscene.erase("objects");
					break;
				}
			}
		}
	}

	ImGui::Text("Vehicles:");
	ImGui::SameLine();
	if (ImGui::SmallButton("Add Vehicle"))
		ImGui::OpenPopup("Add Vehicle##veh_popup");

	RenderAddVehicle(Cutscene);

	if (Cutscene.contains("vehicles"))
	{
		for (json::iterator it = Cutscene["vehicles"].begin(); it < Cutscene["vehicles"].end(); it++)
		{
			const json& veh = *it;
			ImGui::BulletText(veh["name"].get_ref<const std::string&>().c_str());
			ImGui::SameLine();
			std::string DelBtnStr{ "X##" };
			DelBtnStr.append(veh["name"].get_ref<const std::string&>());
			if (ImGui::SmallButton(DelBtnStr.c_str()))
			{
				it = Cutscene["vehicles"].erase(it);
				if (Cutscene["vehicles"].empty())
				{
					Cutscene.erase("vehicles");
					break;
				}
			}
		}
	}
	
	ImGui::EndChild();
}

class CutsceneCategory
{
public:
	CutsceneCategory() = delete;

	CutsceneCategory(std::string_view Name, const std::vector<std::string_view>& Filters) :
		m_Name(Name), m_Filters(Filters)
	{
	}

	inline bool MatchesPrefix(const std::string& ID) const
	{
		for (const std::string_view& Filter : m_Filters)
		{
			if (ID.find(Filter) != std::string::npos)
				return true;
		}

		return false;
	}

	inline void Render(json::iterator& SelectedCutscene) const
	{
		if (!ImGui::CollapsingHeader(m_Name.data()))
			return;

		for (json::iterator it = Lists::CutsceneList.begin(); it < Lists::CutsceneList.end(); it++)
		{
			const json& Cutscene = *it;
			const std::string& ID{ Cutscene["id"].get_ref<const std::string&>() };

			if (!MatchesPrefix(ID))
				continue;

			if (ImGui::Selectable(ID.c_str(), ID == (*SelectedCutscene)["id"].get_ref<const std::string&>()))
				SelectedCutscene = it;
		}
	}

//private:
	std::string_view m_Name;
	std::vector<std::string_view> m_Filters;
};

inline const std::vector<CutsceneCategory> s_Categories{
	{
		"Abigail",
		{
			"rabi",
		}
	},
	{
		"Beecher's Hope",
		{
			"rbch",
		}
	},
	{
		"Bounties",
		{
			"bou",
			"rbdul",
			"rbsos",
		}
	},
	{
		"Callaway (Gunslingers)",
		{
			"rcal",
			"rgun",
		}
	},
	{
		"Calderon",
		{
			"rcld",
		}
	},
	{
		"Camp Activity",
		{
			"@ca",
			"prhm6",
			"prhmls",
			"rhmrb",
		}
	},
	{
		"Caravan Absence (Pronghorn Ranch)",
		{
			"@ptl",
		}
	},
	{
		"Chance Encounters (Strangers & Freaks)",
		{
			"lcmp",
			"prhma",
			"prhmr",
			"rbwcg",
			"rckpt",
			"rcmp",
			"rcrle",
			"rehc",
			"reth",
			"rfma",
			"rhntn",
			"rktty",
			"rmasn",
			"rmayr",
			"rmllr",
			"robt",
			"roddf",
			"rrtl",
			"rsklr",
			"rslvc",
			"rwarv",
		}
	},
	{
		"Chance Encounters (Collectibles)",
		{
			"rccig",
			"rcdin",
			"rcexo",
			"rcfsh",
			"rcrkf",
			"rctxd",
		}
	},
	{
		"Cornwall",
		{
			"crn",
		}
	},
	{
		"Doctor's Opinion",
		{
			"rdopn",
		}
	},
	{
		"Downes Family",
		{
			"rdown",
			"redw",
		}
	},
	{
		"Dutch",
		{
			"rdtc",
		}
	},
	{
		"Epilogue Reunions",
		{
			"ccmrb",
			"cctll",
			"rrfa",
		}
	},
	{
		"Ex-Confederal (Lemoyne Raiders)",
		{
			"rxcf",
		}
	},
	{
		"Finale 1 (Red Dead Redemption)",
		{
			"fin1",
		}
	},
	{
		"Finale 2 (American Venom)",
		{
			"fin2",
		}
	},
	{
		"Gang",
		{
			"gng",
		}
	},
	{
		"Grays & Braithwaites",
		{
			"brt",
			"gry",
			"fud",
			"rfud",
			"rbnp",
		}
	},
	{
		"Guama (Guarma)",
		{
			"gua",
			"fus",
			"smg",
		}
	},
	{
		"Hosea",
		{
			"rchnt",
			"rhmr0",
		}
	},
	{
		"Industry",
		{
			"ind",
		}
	},
	{
		"Jack",
		{
			"rjck",
		}
	},
	{
		"Jail Breakout",
		{
			"jbo",
		}
	},
	{
		"Laramie",
		{
			"lar",
		}
	},
	{
		"Loanshark (Strauss Debt Collection)",
		{
			"lsemr",
			"lshnt",
			"lshss",
			"lsmin",
			"lssol",
		}
	},
	{
		"Marston",
		{
			"mar",
			"rhlp",
		}
	},
	{
		"Mary",
		{
			"rmry",
			"mry",
		}
	},
	{
		"Mob (Angelo Bronte)",
		{
			"mob",
		}
	},
	{
		"Monroe (Captain Monroe)",
		{
			"rmnr",
		}
	},
	{
		"Mudtown (Valentine)",
		{
			"mud",
			"rmud",
		}
	},
	{
		"Natives (Wapiti Reservation)",
		{
			"ntv",
			"rnatv",
		}
	},
	{
		"Native Son (Eagle Flies)",
		{
			"nts",
		}
	},
	{
		"New Bordeaux (Saint Denis)",
		{
			"nbd",
		}
	},
	{
		"O'Driscolls (Dusters)",
		{
			"odr",
			"dst",
			"rdst",
		}
	},
	{
		"Pearson",
		{
			"rprsn",
		}
	},
	{
		"Reverend (Reverend Swanson)",
		{
			"rev",
			"rrvrd",
		}
	},
	{
		"Sadie",
		{
			"rsad",
			"sad",
		}
	},
	{
		"Saloon",
		{
			"sal",
		}
	},
	{
		"Sean",
		{
			"sen1",
		}
	},
	{
		"Strauss",
		{
			"rstr",
		}
	},
	{
		"Susan",
		{
			"sus",
		}
	},
	{
		"Train Robbery",
		{
			"trn",
		}
	},
	{
		"Trelawny",
		{
			"tre",
			"rchrb",
		}
	},
	{
		"Utopia (Micah)",
		{
			"utp",
		}
	},
	{
		"Winter",
		{
			"wnt",
		}
	},
};

static void RenderSortedCutsceneList(json::iterator& SelectedCutscene)
{
	ImGui::SeparatorText("Cutscenes");
	ImGui::BeginChild("cutscene_list_inner");

	for (const CutsceneCategory& Category : s_Categories)
	{
		Category.Render(SelectedCutscene);
		ImGui::Separator();
	}

	ImGui::EndChild();
}

static void RenderFullCutsceneList(json::iterator& SelectedCutscene)
{
	ImGui::SeparatorText("Filter Cutscenes");
	static char s_CutsceneFilter[200]{};
	ImGui::InputText("##cutscene_filter", s_CutsceneFilter, IM_ARRAYSIZE(s_CutsceneFilter));
	ImGui::SameLine();
	if (ImGui::Button("Clear##clear_cutscene_filter"))
		*s_CutsceneFilter = '\0';
	
	ImGui::Separator();
	ImGui::Checkbox("Disable Default Cutscenes", g_Settings["disable_default_cutscenes"].get<bool*>());

	ImGui::SeparatorText("Cutscenes");
	ImGui::BeginChild("cutscene_list_inner");

	std::string CutsceneFilterLower{ s_CutsceneFilter };
	Util::StringToLower(CutsceneFilterLower);

	for (json::iterator it = Lists::CutsceneList.begin(); it < Lists::CutsceneList.end(); it++)
	{
		const json& Cutscene = *it;
		if (Cutscene["id"].get_ref<const std::string&>().find(CutsceneFilterLower) == std::string::npos)
			continue;

		if (g_Settings["disable_default_cutscenes"].get<bool>())
		{
			size_t ElemCount = Cutscene.size() - 1;
			if (Cutscene.contains("player_model")) { ElemCount--; }
			if (ElemCount == 0) { continue; }
		}

		if (ImGui::Selectable(Cutscene["id"].get_ref<const std::string&>().c_str(),
			Cutscene["id"].get_ref<const std::string&>() == (*SelectedCutscene)["id"].get_ref<const std::string&>()))
			SelectedCutscene = it;
	}

	ImGui::EndChild();
}

static void RenderPresetCutsceneList(json::iterator& SelectedCutscene)
{
}

static void RenderCutsceneList(json::iterator& SelectedCutscene)
{
	ImGui::SeparatorText("Select List");
	static int s_SelectedList = 0;
	ImGui::RadioButton("Sorted", &s_SelectedList, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Full", &s_SelectedList, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Preset", &s_SelectedList, 2);
	
	switch (s_SelectedList)
	{
	case 0:
		RenderSortedCutsceneList(SelectedCutscene);
		break;
	case 1:
		RenderFullCutsceneList(SelectedCutscene);
		break;
	case 2:
		RenderPresetCutsceneList(SelectedCutscene);
		break;
	}
}

void Menu::RenderCutsceneTab()
{
	if (!ImGui::BeginTabItem("Cutscene"))
		return;

	ImGui::BeginChild("cutscene_child");

	if (Lists::CutsceneList.empty())
	{
		ImGui::TextUnformatted("Cutscene list is not loaded");
	}
	else
	{
		static json::iterator s_SelectedCutscene{ Lists::CutsceneList.begin() };

		ImGui::BeginChild("cutscene_info", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
		RenderCutsceneInfo(s_SelectedCutscene);
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("cutscene_list");
		RenderCutsceneList(s_SelectedCutscene);
		ImGui::EndChild();
	}

	ImGui::EndChild();
	ImGui::EndTabItem();
}
