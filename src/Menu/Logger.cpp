#include "pch.h"
#include "Logger.h"

namespace Menu
{
	void RenderLoggerTab()
	{
		if (!ImGui::BeginTabItem("Logger"))
			return;

		ImGui::BeginChild("logger_child");

		Logger.Draw();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
