#include "pch.h"
#include "Notify.h"
#include "Thread/JobQueue.h"
#include "Script/Notification.h"

static void RenderNotifyDebug()
{
	static char s_Message[64]{};
	ImGui::InputText("Message", s_Message, IM_ARRAYSIZE(s_Message));

	if (ImGui::Button("Help"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyHelp(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Feed"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyFeed(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Objective"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyObjective(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Location"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyLocation(s_Message);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Death"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyDeath(s_Message);
		}
		END_JOB()
	}
	
	static char s_Description[64]{};
	ImGui::InputText("Description", s_Description, IM_ARRAYSIZE(s_Description));

	if (ImGui::Button("Toast"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyToast(s_Message, s_Description);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Error"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyError(s_Message, s_Description);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Warning"))
	{
		QUEUE_JOB(=)
		{
			Script::NotifyWarning(s_Message, s_Description);
		}
		END_JOB()
	}
}

void DebugMenu::RenderNotifyTab()
{
	if (!ImGui::BeginTabItem("Notify##debug"))
		return;

	TRY
	{
		ImGui::SeparatorText("Notify Debug");
		RenderNotifyDebug();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndTabItem();
}
