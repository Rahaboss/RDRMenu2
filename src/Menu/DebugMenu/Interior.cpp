#include "pch.h"
#include "Interior.h"
#include "Thread/JobQueue.h"
#include "Script/Interior.h"
#include "Script/Player.h"

static void RenderInteriorButtons()
{
	ImGui::SeparatorText("Art Gallery");
	if (ImGui::Button("Activate##Art Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleArtInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Art Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleArtInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Art Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 2689.29f, -1203.22f, 56.47f });
		}
		END_JOB()
	}

	ImGui::SeparatorText("Dino Barn");
	if (ImGui::Button("Activate##Dino Barn Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleDinoBarnInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Dino Barn Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleDinoBarnInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Dino Barn Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 196.2653f, 985.5404f, 189.1246f });
		}
		END_JOB()
	}

	ImGui::SeparatorText("Saint Denis Hideout");
	if (ImGui::Button("Activate##Saint Denis Hideout Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleSaintDenisHideoutInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Saint Denis Hideout Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleSaintDenisHideoutInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Saint Denis Hideout Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 2759.03f, -1346.07f, 57.75f });
		}
		END_JOB()
	}
	
	ImGui::SeparatorText("Grand Korrigan");
	if (ImGui::Button("Activate##Grand Korrigan Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Grand Korrigan Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Grand Korrigan Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 3288.49f, -1309.71f, 42.8f });
		}
		END_JOB()
	}
	
	ImGui::SeparatorText("Grand Korrigan Docked");
	if (ImGui::Button("Activate##Grand Korrigan Docked Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanDockedInterior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Grand Korrigan Docked Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleGrandKorringanDockedInterior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Grand Korrigan Docked Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ 2868.71f, -1401.51f, 43.45f });
		}
		END_JOB()
	}

	ImGui::SeparatorText("Winter 1");
	if (ImGui::Button("Activate##Winter 1 Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleWinter1Interior(true);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Deactivate##Winter 1 Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::ToggleWinter1Interior(false);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Teleport##Winter 1 Interior"))
	{
		QUEUE_JOB(=)
		{
			Script::Teleport(Vector3{ -1981.7f, 3254.33f, 641.47f });
		}
		END_JOB()
	}
}

void DebugMenu::RenderInteriorMenu()
{
	if (!ImGui::BeginTabItem("Interior"))
		return;

	TRY
	{
		RenderInteriorButtons();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndTabItem();
}
