#include "pch.h"
#include "Cutscene.h"
#include "Config/Lists.h"
#include "Script/Cutscene.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Util/String.h"
#include "Config/Settings.h"
#include "Config/Config.h"
#include "Util/Container.h"

static AnimScene s_CutsceneIndex = 0;
static void QueuePlayCutscene(const char* CutsceneName)
{
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
		auto Temp = Util::StringToLowerCopy(Name);
		for (const auto& j : Array)
		{
			if (j.contains("name"))
			{
				if (Util::StringToLowerCopy(j["name"].get_ref<const std::string&>()) == Temp)
					return true;
			}
		}
	}

	return false;
}

static bool IsNewEntityValid(json& SelectedCutscene, const char* Model, const char* Name)
{
	if (!Util::IsStringValid(Model) || !Util::IsStringValid(Name))
		return false;

	if (SelectedCutscene.contains("peds")
		&& EntityExistsInJSONArray(SelectedCutscene["peds"], Name))
		return false;

	if (SelectedCutscene.contains("objects")
		&& EntityExistsInJSONArray(SelectedCutscene["objects"], Name))
		return false;

	if (SelectedCutscene.contains("vehicles")
		&& EntityExistsInJSONArray(SelectedCutscene["vehicles"], Name))
		return false;

	return true;
}

static char s_PedModelInput[0x100]{};
static char s_PedNameInput[0x100]{};
static char s_PedMetaPedOutfitInput[0x100]{};
static char s_PedMetaPedWearableInput[0x100]{};
static bool s_PedRemoveWeapons = false;
static std::string s_PedSelectedPedName;

static bool RenderPedEditor(json& SelectedCutscene, json& PedObject)
{
	bool Result = false;

	if (!ImGui::BeginPopup("Add Ped##ped_popup"))
		return Result;

	ImGui::SeparatorText("Add Cutscene Ped");

	if (ImGui::InputText("Model", s_PedModelInput, IM_ARRAYSIZE(s_PedModelInput)))
		Util::StringToLower(s_PedModelInput);
	ImGui::InputText("Name", s_PedNameInput, IM_ARRAYSIZE(s_PedNameInput));
	if (ImGui::InputText("Meta Ped Outfit", s_PedMetaPedOutfitInput, IM_ARRAYSIZE(s_PedMetaPedOutfitInput)))
		Util::StringToLower(s_PedMetaPedOutfitInput);
	if (ImGui::InputText("Meta Ped Wearable", s_PedMetaPedWearableInput, IM_ARRAYSIZE(s_PedMetaPedWearableInput)))
		Util::StringToLower(s_PedMetaPedWearableInput);
	ImGui::Checkbox("Remove Weapons", &s_PedRemoveWeapons);

	const bool IsInvalid = !(Util::StringToLowerCopy(s_PedSelectedPedName) == Util::StringToLowerCopy(s_PedNameInput)
		? (Util::IsStringValid(s_PedModelInput) && Util::IsStringValid(s_PedNameInput))
		: IsNewEntityValid(SelectedCutscene, s_PedModelInput, s_PedNameInput));

	if (IsInvalid)
		ImGui::BeginDisabled();
	if (ImGui::Button("Add##add_ped_btn"))
	{
		Result = true;

		json& j = PedObject;
		j.clear();
		std::string ModelName = Util::StringToLowerCopy(s_PedModelInput);
		j["name"] = s_PedNameInput;

		if (Hash h = StringToHash(ModelName.c_str()))
			j["model"] = h;
		else
			j["model"] = ModelName;

		if (Util::IsStringValid(s_PedMetaPedOutfitInput))
		{
			if (Hash h = StringToHash(s_PedMetaPedOutfitInput))
				j["meta_ped_outfit"] = h;
			else
				j["meta_ped_outfit"] = Util::StringToLowerCopy(s_PedMetaPedOutfitInput);
		}

		if (Util::IsStringValid(s_PedMetaPedWearableInput))
		{
			if (Hash h = StringToHash(s_PedMetaPedWearableInput))
				j["meta_ped_wearable"] = h;
			else
				j["meta_ped_wearable"] = Util::StringToLowerCopy(s_PedMetaPedWearableInput);
		}

		if (s_PedRemoveWeapons)
			j["remove_weapons"] = s_PedRemoveWeapons;

		ImGui::CloseCurrentPopup();
	}
	if (IsInvalid)
		ImGui::EndDisabled();

	ImGui::EndPopup();

	return Result;
}

static char s_ObjectModelInput[0x100]{};
static char s_ObjectNameInput[0x100]{};
static std::string s_ObjectSelectedObjectName;

static bool RenderObjectEditor(json& SelectedCutscene, json& ObjectObject)
{
	bool Result = false;

	if (!ImGui::BeginPopup("Add Object##obj_popup"))
		return Result;

	ImGui::SeparatorText("Add Cutscene Object");

	if (ImGui::InputText("Model", s_ObjectModelInput, IM_ARRAYSIZE(s_ObjectModelInput)))
		Util::StringToLower(s_ObjectModelInput);
	ImGui::InputText("Name", s_ObjectNameInput, IM_ARRAYSIZE(s_ObjectNameInput));

	const bool IsInvalid = !(Util::StringToLowerCopy(s_ObjectSelectedObjectName) == Util::StringToLowerCopy(s_ObjectNameInput)
		? (Util::IsStringValid(s_ObjectModelInput) && Util::IsStringValid(s_ObjectNameInput))
		: IsNewEntityValid(SelectedCutscene, s_ObjectModelInput, s_ObjectNameInput));

	if (IsInvalid)
		ImGui::BeginDisabled();
	if (ImGui::Button("Add##add_obj_btn"))
	{
		Result = true;

		json& j = ObjectObject;
		std::string ModelName = Util::StringToLowerCopy(s_ObjectModelInput);
		j["name"] = s_ObjectNameInput;

		if (Hash h = StringToHash(ModelName.c_str()))
			j["model"] = h;
		else
			j["model"] = ModelName;

		ImGui::CloseCurrentPopup();
	}
	if (IsInvalid)
		ImGui::EndDisabled();

	ImGui::EndPopup();

	return Result;
}

static char s_VehicleModelInput[0x100]{};
static char s_VehicleNameInput[0x100]{};
static std::vector<int> s_VehicleExtras;
static std::string s_VehicleSelectedVehicleName;

static bool RenderVehicleEditor(json& SelectedCutscene, json& VehicleObject)
{
	bool Result = false;

	if (!ImGui::BeginPopup("Add Vehicle##veh_popup"))
		return Result;

	ImGui::SeparatorText("Add Cutscene Vehicle");

	if (ImGui::InputText("Model", s_VehicleModelInput, IM_ARRAYSIZE(s_VehicleModelInput)))
		Util::StringToLower(s_VehicleModelInput);
	ImGui::InputText("Name", s_VehicleNameInput, IM_ARRAYSIZE(s_VehicleNameInput));

	ImGui::TextUnformatted("Extras");
	for (auto it = s_VehicleExtras.begin(); it < s_VehicleExtras.end(); it++)
	{
		ImGui::BulletText("%d", *it);
		ImGui::SameLine();
		std::string DelBtnStr{ "X##" };
		DelBtnStr.append(std::to_string(*it));
		if (ImGui::SmallButton(DelBtnStr.c_str()))
			it = s_VehicleExtras.erase(it);
	}

	if (s_VehicleExtras.size() < 16)
	{
		if (ImGui::BeginCombo("Add Extra", "Add Extra"))
		{
			for (int i = 1; i <= 16; i++)
			{
				if (Util::ContainerContains(s_VehicleExtras, i))
				{
					if (ImGui::Selectable(std::to_string(i).c_str()))
						s_VehicleExtras.push_back(i);
				}
			}
			ImGui::EndCombo();
		}
	}

	const bool IsInvalid = !(Util::StringToLowerCopy(s_VehicleSelectedVehicleName) == Util::StringToLowerCopy(s_VehicleNameInput)
		? (Util::IsStringValid(s_VehicleModelInput) && Util::IsStringValid(s_VehicleNameInput))
		: IsNewEntityValid(SelectedCutscene, s_VehicleModelInput, s_VehicleNameInput));

	if (IsInvalid)
		ImGui::BeginDisabled();
	if (ImGui::Button("Add##add_veh_btn"))
	{
		Result = true;

		json& j = VehicleObject;
		std::string ModelName = Util::StringToLowerCopy(s_VehicleModelInput);
		j["name"] = s_VehicleNameInput;

		if (Hash h = StringToHash(ModelName.c_str()))
			j["model"] = h;
		else
			j["model"] = ModelName;

		if (!s_VehicleExtras.empty())
		{
			auto Temp = s_VehicleExtras;
			Util::ContainerSort(Temp);
			j["extras"] = Temp;
		}

		ImGui::CloseCurrentPopup();
	}
	if (IsInvalid)
		ImGui::EndDisabled();

	ImGui::EndPopup();
	
	return Result;
}

static void RenderCutsceneControls(json::iterator& SelectedCutscene)
{
	const bool IsCutscenePlaying = s_CutsceneIndex != 0;

	if (IsCutscenePlaying)
		ImGui::BeginDisabled();
	if (ImGui::Button("Play Cutscene"))
		QueuePlayCutscene((*SelectedCutscene)["id"].get_ref<const std::string&>().c_str());
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
		const std::string CutsceneName = (*SelectedCutscene)["id"].get_ref<const std::string&>();
		Lists::InitCutsceneList();
		SelectedCutscene = Lists::GetCutscene(CutsceneName);
		assert(SelectedCutscene != Lists::CutsceneList.end());
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
}

static void RenderCutsceneGeneralInfo(json& Cutscene)
{
	TRY
	{
		ImGui::Text("Name: %s", Cutscene["id"].get_ref<const std::string&>().c_str());

		if (Cutscene.contains("playback_id"))
			ImGui::Text("playbackListName: %s", Cutscene["playback_id"].get_ref<const std::string&>().c_str());
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

static void RenderCutsceneEntities(json& Cutscene)
{
	if (Cutscene.contains("player_model"))
		ImGui::Text("Player Model: %s", Cutscene["player_model"].get_ref<const std::string&>().c_str());

	if (Cutscene.contains("player_meta_ped_outfit"))
	{
		if (Cutscene["player_meta_ped_outfit"].is_number())
			ImGui::Text("Player Meta Ped Outfit: %u", Cutscene["player_meta_ped_outfit"].get<Hash>());
		else
			ImGui::Text("Player Meta Ped Outfit: %s", Cutscene["player_meta_ped_outfit"].get_ref<const std::string&>().c_str());
	}
	
	{
		ImGui::Text("Peds:");
		ImGui::SameLine();
		if (ImGui::SmallButton("Add Ped"))
		{
			s_PedSelectedPedName.clear();
			ImGui::OpenPopup("Add Ped##ped_popup");
		}

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
					continue;
				}
				ImGui::SameLine();

				std::string EditBtnStr{ "Edit##" };
				EditBtnStr.append(ped["name"].get_ref<const std::string&>());
				if (ImGui::SmallButton(EditBtnStr.c_str()))
				{
					strcpy_s(s_PedModelInput, ped["model"].get_ref<const std::string&>().c_str());
					strcpy_s(s_PedNameInput, ped["name"].get_ref<const std::string&>().c_str());
					s_PedRemoveWeapons = ped.contains("remove_weapons") && ped["remove_weapons"].get<bool>();
					if (ped.contains("meta_ped_outfit"))
					{
						if (ped["meta_ped_outfit"].is_number())
						{
							std::string Outfit = std::to_string(ped["meta_ped_outfit"].get<Hash>());
							strcpy_s(s_PedMetaPedOutfitInput, Outfit.c_str());
						}
						else
						{
							strcpy_s(s_PedMetaPedOutfitInput, ped["meta_ped_outfit"].get_ref<const std::string&>().c_str());
						}
					}
					else
					{
						s_PedMetaPedOutfitInput[0] = '\0';
					}
					if (ped.contains("meta_ped_wearable"))
					{
						if (ped["meta_ped_wearable"].is_number())
						{
							std::string Wearable = std::to_string(ped["meta_ped_wearable"].get<Hash>());
							strcpy_s(s_PedMetaPedWearableInput, Wearable.c_str());
						}
						else
						{
							strcpy_s(s_PedMetaPedWearableInput, ped["meta_ped_wearable"].get_ref<const std::string&>().c_str());
						}
					}
					else
					{
						s_PedMetaPedWearableInput[0] = '\0';
					}
					s_PedSelectedPedName = ped["name"].get_ref<const std::string&>();

					ImGui::OpenPopup("Add Ped##ped_popup");
				}
			}
		}

		static json EditingPed;

		if (RenderPedEditor(Cutscene, EditingPed))
		{
			bool Changed = false;

			if (!s_PedSelectedPedName.empty() && Cutscene.contains("peds"))
			{
				for (json::iterator it = Cutscene["peds"].begin(); it < Cutscene["peds"].end(); it++)
				{
					if ((*it)["name"] == s_PedSelectedPedName)
					{
						*it = EditingPed;
						Changed = true;
						break;
					}
				}
			}

			if (!Changed)
				Cutscene["peds"].push_back(EditingPed);
		}
	}

	{
		ImGui::Text("Objects:");
		ImGui::SameLine();
		if (ImGui::SmallButton("Add Object"))
		{
			s_ObjectSelectedObjectName.clear();
			ImGui::OpenPopup("Add Object##obj_popup");
		}

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
					continue;
				}
				ImGui::SameLine();

				std::string EditBtnStr{ "Edit##" };
				EditBtnStr.append(obj["name"].get_ref<const std::string&>());
				if (ImGui::SmallButton(EditBtnStr.c_str()))
				{
					strcpy_s(s_ObjectModelInput, obj["model"].get_ref<const std::string&>().c_str());
					strcpy_s(s_ObjectNameInput, obj["name"].get_ref<const std::string&>().c_str());
					s_ObjectSelectedObjectName = obj["name"].get_ref<const std::string&>();

					ImGui::OpenPopup("Add Object##obj_popup");
				}
			}
		}

		static json EditingObject;

		if (RenderObjectEditor(Cutscene, EditingObject))
		{
			bool Changed = false;

			if (!s_ObjectSelectedObjectName.empty() && Cutscene.contains("objects"))
			{
				for (json::iterator it = Cutscene["objects"].begin(); it < Cutscene["objects"].end(); it++)
				{
					if ((*it)["objects"] == s_ObjectSelectedObjectName)
					{
						*it = EditingObject;
						Changed = true;
						break;
					}
				}
			}

			if (!Changed)
				Cutscene["objects"].push_back(EditingObject);
		}
	}
	
	{
		ImGui::Text("Vehicles:");
		ImGui::SameLine();
		if (ImGui::SmallButton("Add Vehicle"))
		{
			s_VehicleSelectedVehicleName.clear();
			ImGui::OpenPopup("Add Vehicle##veh_popup");
		}

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
				ImGui::SameLine();

				std::string EditBtnStr{ "Edit##" };
				EditBtnStr.append(veh["name"].get_ref<const std::string&>());
				if (ImGui::SmallButton(EditBtnStr.c_str()))
				{
					strcpy_s(s_VehicleModelInput, veh["model"].get_ref<const std::string&>().c_str());
					strcpy_s(s_VehicleNameInput, veh["name"].get_ref<const std::string&>().c_str());
					if (veh.contains("extras"))
						s_VehicleExtras = veh["extras"].get<std::vector<int>>();
					else
						s_VehicleExtras.clear();
					s_VehicleSelectedVehicleName = veh["name"].get_ref<const std::string&>();
					
					ImGui::OpenPopup("Add Vehicle##veh_popup");
				}
			}
		}

		static json EditingVehicle;

		if (RenderVehicleEditor(Cutscene, EditingVehicle))
		{
			bool Changed = false;

			if (!s_VehicleSelectedVehicleName.empty() && Cutscene.contains("vehicles"))
			{
				for (json::iterator it = Cutscene["vehicles"].begin(); it < Cutscene["vehicles"].end(); it++)
				{
					if ((*it)["vehicles"] == s_VehicleSelectedVehicleName)
					{
						*it = EditingVehicle;
						Changed = true;
						break;
					}
				}
			}

			if (!Changed)
				Cutscene["vehicles"].push_back(EditingVehicle);
		}
	}
}

static void RenderCutsceneInfo(json::iterator& SelectedCutscene)
{
	ImGui::SeparatorText("Cutscene Controls");

	TRY
	{
		RenderCutsceneControls(SelectedCutscene);
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::SeparatorText("Cutscene Info");

	if (ImGui::BeginChild("cutscene_info_inner"))
	{
		json& Cutscene = *SelectedCutscene;

		TRY
		{
			RenderCutsceneGeneralInfo(Cutscene);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		TRY
		{
			RenderCutsceneEntities(Cutscene);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	ImGui::EndChild(); // cutscene_info_inner
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
			"reth",
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
		"Grays & Braithwaites (Feud)",
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

	std::string CutsceneFilterLower{ Util::StringToLowerCopy(s_CutsceneFilter) };

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

static std::map<std::string, std::string> s_CutsceneList{
	{ "A Fisher of Fish II", "cutscene@rcfsh_rsc2" },
	{ "A New Future Imagined", "cutscene@rabi3_rsc2" },
	{ "A Test of Faith II", "cutscene@rcdin_rsc2" },
	{ "Annesburg Jail Breakout With Charles", "cutscene@jbo6_ext" },
	{ "Bare Knuckle Friendships", "cutscene@mar6_rsc2" },
	{ "Beecher's Hope", "cutscene@rbch5_rsc4" },
	{ "Charles Leaving To Help Natives", "cutscene@ntv0_int" },
	{ "Ending Strawberry", "cutscene@fin2_ext_p7" },
	{ "Jack Cutscene", "cutscene@rjck1_rsc5" },
	{ "Money Lending and Other Sins", "cutscene@rdown_rsc_2" },
	{ "The Fine Art of Conversation", "cutscene@ntv2_ext" },
	{ "The Gilded Cage", "cutscene@ind1_mcs_1" },
};
static void RenderPresetCutsceneList(json::iterator& SelectedCutscene)
{
	ImGui::SeparatorText("Cutscenes");
	ImGui::BeginChild("cutscene_list_inner");

	for (const auto& [CutsceneLabel, CutsceneName] : s_CutsceneList)
	{
		if (const auto it = Lists::GetCutscene(CutsceneName); it != Lists::CutsceneList.end())
		{
			if (ImGui::Selectable(CutsceneLabel.c_str(), (*SelectedCutscene)["id"].get_ref<const std::string&>() == CutsceneName))
				SelectedCutscene = it;
		}
	}

	ImGui::EndChild();
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
