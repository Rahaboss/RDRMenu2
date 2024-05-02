#include "pch.h"
#include "Proof.h"
#include "Rage/natives.h"
#include "Script/PlayerInfo.h"
#include "Script/Entity.h"

static void RenderProofDebug()
{
	static bool b[9]{};
	const char* Labels[ARRAYSIZE(b)]{
		"Bullet",
		"Fire",
		"Explosion",
		"Collision",
		"Melee",
		"Steam",
		"Smoke",
		"Headshot",
		"Special",
	};

	for (int i = 0; i < 9; i++)
		ImGui::Checkbox(Labels[i], b + i);

	ImGui::Text("Proofs: %d", ENTITY::_GET_ENTITY_PROOFS(g_LocalPlayer.m_Entity));

	Script::SetEntityProofs(g_LocalPlayer.m_Entity, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8]);
}

void DebugMenu::RenderProofTab()
{
	if (!ImGui::BeginTabItem("Proof##debug"))
		return;

	TRY
	{
		ImGui::SeparatorText("Proof Debug");
		RenderProofDebug();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndTabItem();
}
