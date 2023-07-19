#pragma once

#include "pch.h"
#include "CutsceneHelper.h"

namespace Menu
{
	void RenderCutsceneTab();

	inline int SelectedCutscene = 0;
	class CutsceneCategory
	{
	public:
		CutsceneCategory() = delete;
		CutsceneCategory(std::string_view Name, const std::vector<std::string_view>& Prefixes):
			m_Name(Name),
			m_Prefixes(Prefixes)
		{
		}

		void Render() const
		{
			if (!ImGui::CollapsingHeader(m_Name.data()) || g_Cutscenes.empty())
				return;

			for (int i = 0; i < g_Cutscenes.size(); i++)
			{
				auto ID = g_Cutscenes[i]["id"].get_ref<const std::string&>();
				bool MatchesPrefix = false;

				for (const auto& Prefix : m_Prefixes)
				{
					if (ID.find(Prefix) == 0)
					{
						MatchesPrefix = true;
						break;
					}
				}

				if (!MatchesPrefix)
					continue;

				if (ImGui::Selectable(ID.c_str(), SelectedCutscene == i))
					SelectedCutscene = i;
			}
		}

	//private:
		std::string_view m_Name;
		std::vector<std::string_view> m_Prefixes;
	};

	const CutsceneCategory Categories[]
	{
		{
			"Abigail",
			{
				"cutscene@rabi",
			}
		},
		{
			"Beecher's Hope",
			{
				"cutscene@rbch",
			}
		},
		{
			"Bounties",
			{
				"cutscene@bou",
				"cutscene@rbdul",
				"cutscene@rbsos",
			}
		},
		{
			"Callaway (Gunslingers)",
			{
				"cutscene@rcal",
				"cutscene@rgun",
			}
		},
		{
			"Calderon",
			{
				"cutscene@rcld",
			}
		},
		{
			"Camp Activity",
			{
				"cutscene@ca",
				"cutscene@prhm6",
				"cutscene@rhmrb",
				"cutscene@prhmls",
			}
		},
		{
			"Caravan Absence (Pronghorn Ranch)",
			{
				"cutscene@ptl",
			}
		},
		{
			"Chance Encounters (Strangers & Freaks)",
			{
				"cutscene@rbwcg",
				"cutscene@rwarv",
				"cutscene@robt",
				"cutscene@roddf",
				"cutscene@rehc",
				"cutscene@reth",
				"cutscene@rckpt",
				"cutscene@rcrle",
				"cutscene@lcmp",
				"cutscene@rfma",
				"cutscene@rktty",
				"cutscene@rhntn",
				"cutscene@rmllr",
				"cutscene@rsklr",
				"cutscene@rslvc",
				"cutscene@rrtl",
				"cutscene@prhmr",
				"cutscene@rcmp",
				"cutscene@rmasn",
				"cutscene@rmayr",
				"cutscene@prhma",
			}
		},
		{
			"Chance Encounters (Collectibles)",
			{
				"cutscene@rccig",
				"cutscene@rcdin",
				"cutscene@rcexo",
				"cutscene@rcfsh",
				"cutscene@rcrkf",
				"cutscene@rctxd",
			}
		},
		{
			"Cornwall",
			{
				"cutscene@crn",
			}
		},
		{
			"Doctor's Opinion",
			{
				"cutscene@rdopn",
			}
		},
		{
			"Downes Family",
			{
				"cutscene@rdown",
				"cutscene@redw",
			}
		},
		{
			"Dutch",
			{
				"cutscene@rdtc",
			}
		},
		{
			"Epilogue Reunions",
			{
				"cutscene@ccmrb",
				"cutscene@cctll",
				"cutscene@rrfa",
			}
		},
		{
			"Ex-Confederal (Lemoyne Raiders)",
			{
				"cutscene@rxcf",
			}
		},
		{
			"Finale 1 (Red Dead Redemption)",
			{
				"cutscene@fin1",
			}
		},
		{
			"Finale 2 (American Venom)",
			{
				"cutscene@fin2",
			}
		},
		{
			"Gang",
			{
				"cutscene@rgng",
				"cutscene@gng",
			}
		},
		{
			"Grays & Braithwaites",
			{
				"cutscene@brt",
				"cutscene@gry",
				"cutscene@fud",
				"cutscene@rfud",
				"cutscene@rbnp",
			}
		},
		{
			"Guama (Guarma)",
			{
				"cutscene@gua",
				"cutscene@fus",
				"cutscene@smg",
			}
		},
		{
			"Hosea",
			{
				"cutscene@rchnt",
				"cutscene@rhmr0",
			}
		},
		{
			"Industry",
			{
				"cutscene@ind",
			}
		},
		{
			"Jack",
			{
				"cutscene@rjck",
			}
		},
		{
			"Jail Breakout",
			{
				"cutscene@jbo",
			}
		},
		{
			"Laramie",
			{
				"cutscene@lar",
			}
		},
		{
			"Loanshark (Strauss Debt Collection)",
			{
				"cutscene@lsemr",
				"cutscene@lshnt",
				"cutscene@lshss",
				"cutscene@lsmin",
				"cutscene@lssol",
			}
		},
		{
			"Marston",
			{
				"cutscene@mar",
				"cutscene@rhlp",
			}
		},
		{
			"Mary",
			{
				"cutscene@rmry",
				"cutscene@mry",
			}
		},
		{
			"Mob (Angelo Bronte)",
			{
				"cutscene@mob",
			}
		},
		{
			"Monroe (Captain Monroe)",
			{
				"cutscene@rmnr",
			}
		},
		{
			"Mudtown (Valentine)",
			{
				"cutscene@mud",
				"cutscene@rmud",
			}
		},
		{
			"Natives (Wapiti Reservation)",
			{
				"cutscene@ntv",
				"cutscene@rnatv",
			}
		},
		{
			"Native Son (Eagle Flies)",
			{
				"cutscene@nts",
			}
		},
		{
			"New Bordeaux (Saint Denis)",
			{
				"cutscene@nbd",
			}
		},
		{
			"O'Driscolls (Dusters)",
			{
				"cutscene@odr",
				"cutscene@dst",
				"cutscene@rdst",
			}
		},
		{
			"Pearson",
			{
				"cutscene@rprsn",
			}
		},
		{
			"Reverend (Reverend Swanson)",
			{
				"cutscene@rev",
				"cutscene@rrvrd",
			}
		},
		{
			"Sadie",
			{
				"cutscene@rsad",
				"cutscene@sad",
			}
		},
		{
			"Saloon",
			{
				"cutscene@sal",
			}
		},
		{
			"Sean",
			{
				"cutscene@sen",
			}
		},
		{
			"Strauss",
			{
				"cutscene@rstr",
			}
		},
		{
			"Susan",
			{
				"cutscene@sus",
			}
		},
		{
			"Train Robbery",
			{
				"cutscene@trn",
			}
		},
		{
			"Trelawny",
			{
				"cutscene@tre",
				"cutscene@rchrb",
			}
		},
		{
			"Utopia (Micah)",
			{
				"cutscene@utp",
			}
		},
		{
			"Winter",
			{
				"cutscene@wnt",
			}
		},
	};
}
