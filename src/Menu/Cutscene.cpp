#include "pch.h"
#include "Cutscene.h"
#include "Config/Lists.h"
#include "Script/Cutscene.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Util/String.h"
#include "Config/Settings.h"

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

static void RenderCutsceneInfo(json& SelectedCutscene)
{
	ImGui::SeparatorText("Cutscene Controls");

	const bool IsCutscenePlaying = s_CutsceneIndex != 0;

	if (IsCutscenePlaying)
		ImGui::BeginDisabled();
	if (ImGui::Button("Play Cutscene"))
		QueuePlayCutscene(SelectedCutscene["id"].get_ref<const std::string&>().c_str());
	if (IsCutscenePlaying)
		ImGui::EndDisabled();

	ImGui::SameLine();

	if (!IsCutscenePlaying)
		ImGui::BeginDisabled();
	if (ImGui::Button("Skip Cutscene"))
		QueueSkipCutscene();
	if (!IsCutscenePlaying)
		ImGui::EndDisabled();
	
	ImGui::SameLine();

	if (ImGui::Button("Reload List"))
	{
		Lists::InitCutsceneList();
		for (const json& j : Lists::CutsceneList)
		{
			if (j["id"] == SelectedCutscene["id"])
				SelectedCutscene = j;
		}
	}

	ImGui::Checkbox("Disable Black Borders", g_Settings["disable_black_borders"].get<bool*>());

	ImGui::BeginChild("cutscene_info_inner");

	ImGui::SeparatorText("Cutscene Info");
	ImGui::Text("Name: %s", SelectedCutscene["id"].get_ref<const std::string&>().c_str());

	if (SelectedCutscene.contains("playback_id"))
		ImGui::Text("playbackListName: %s", SelectedCutscene["playback_id"].get_ref<const std::string&>().c_str());

	if (SelectedCutscene.contains("player_model"))
		ImGui::Text("Player Model: %s", SelectedCutscene["player_model"].get_ref<const std::string&>().c_str());

	if (SelectedCutscene.contains("peds"))
	{
		ImGui::Text("Peds:");
		for (const json& ped : SelectedCutscene["peds"])
			ImGui::BulletText(ped["name"].get_ref<const std::string&>().c_str());
	}

	if (SelectedCutscene.contains("objects"))
	{
		ImGui::Text("Objects:");
		for (const json& obj : SelectedCutscene["objects"])
			ImGui::BulletText(obj["name"].get_ref<const std::string&>().c_str());
	}

	if (SelectedCutscene.contains("vehicles"))
	{
		ImGui::Text("Vehicles:");
		for (const json& veh : SelectedCutscene["vehicles"])
			ImGui::BulletText(veh["name"].get_ref<const std::string&>().c_str());
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

	inline void Render(json& SelectedCutscene) const
	{
		if (!ImGui::CollapsingHeader(m_Name.data()))
			return;

		for (const json& Cutscene : Lists::CutsceneList)
		{
			const std::string& ID{ Cutscene["id"].get_ref<const std::string&>() };

			if (!MatchesPrefix(ID))
				continue;

			if (ImGui::Selectable(ID.c_str(), ID == SelectedCutscene["id"].get_ref<const std::string&>()))
				SelectedCutscene = Cutscene;
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

static void RenderSortedCutsceneList(json& SelectedCutscene)
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

static void RenderFullCutsceneList(json& SelectedCutscene)
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

	for (const json& Cutscene : Lists::CutsceneList)
	{
		if (Cutscene["id"].get_ref<const std::string&>().find(CutsceneFilterLower) == std::string::npos)
			continue;

		if (g_Settings["disable_default_cutscenes"].get<bool>())
		{
			size_t ElemCount = Cutscene.size() - 1;
			if (Cutscene.contains("player_model")) { ElemCount--; }
			if (ElemCount == 0) { continue; }
		}

		if (ImGui::Selectable(Cutscene["id"].get_ref<const std::string&>().c_str(),
			Cutscene["id"].get_ref<const std::string&>() == SelectedCutscene["id"].get_ref<const std::string&>()))
			SelectedCutscene = Cutscene;
	}

	ImGui::EndChild();
}

static void RenderPresetCutsceneList(json& SelectedCutscene)
{
}

static void RenderCutsceneList(json& SelectedCutscene)
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
		static json s_SelectedCutscene{ *Lists::CutsceneList.begin() };

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
