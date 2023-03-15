#include "pch.h"
#include "Player.h"
#include "JobQueue.h"
#include "Features.h"
#include "PlayerInfo.h"

namespace Menu
{
	void RenderPlayerTab()
	{
		if (!ImGui::BeginTabItem("Player"))
			return;

		ImGui::BeginChild("player_child");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Player"))
		{
			RenderPlayerButtons();
			ImGui::Separator();
			RenderPlayerCheckboxes();
			ImGui::Separator();

			ImGui::Text("Money");
			static float Money = 10000.0f;
			ImGui::PushItemWidth(200.0f);
			ImGui::InputFloat("###money_input", &Money, 0, 0, "$%.2f");
			ImGui::SameLine();
			if (ImGui::Button("Set###set_money"))
			{
				QUEUE_JOB(=)
				{
					Features::SetMoney((int)(Money * 100.f));
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Add###add_money"))
			{
				QUEUE_JOB(=)
				{
					Features::AddMoney((int)(Money * 100.f));
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove###remove_money"))
			{
				QUEUE_JOB(=)
				{
					Features::RemoveMoney((int)(Money * 100.f));
				}
				END_JOB()
			}
		}
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Mount"))
		{
			const bool MountDisabled = !g_LocalPlayer.m_Mount;
			if (MountDisabled)
				ImGui::BeginDisabled();
			ImGui::BeginGroup();

			if (ImGui::Button("Fill Cores###fill_mount"))
				Features::RestoreHorseCores();

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button("Give Gold Cores###gold_mount"))
				Features::GiveGoldCores(g_LocalPlayer.m_Mount);

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button("Clean###clean_mount"))
				Features::CleanPed(g_LocalPlayer.m_Mount);

			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::BeginGroup();

			if (ImGui::Checkbox("God Mode###mount_god_mode", g_Settings["mount_god_mode"].get<bool*>()) && !g_Settings["mount_god_mode"].get<bool>())
			{
				QUEUE_JOB(=)
				{
					ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Mount, false);
				}
				END_JOB()
			}

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::Checkbox("Gold Cores###mount_gold_cores", g_Settings["mount_gold_cores"].get<bool*>());

			ImGui::EndGroup();
			ImGui::Separator();

			ImGui::Text("Scale");
			static float MountScale = 1.0f;
			ImGui::PushItemWidth(300.0f);
			if (ImGui::SliderFloat("###mount_scale", &MountScale, 0.01f, 10.0f, "%.2f"))
			{
				QUEUE_JOB(=)
				{
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Mount, MountScale);
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset###reset_mount_scale"))
			{
				QUEUE_JOB(=)
				{
					MountScale = 1.0f;
					PED::_SET_PED_SCALE(g_LocalPlayer.m_Mount, MountScale);
				}
				END_JOB()
			}

			if (MountDisabled)
				ImGui::EndDisabled();
		}
			
		ImGui::EndChild();
		ImGui::EndTabItem();
	}
	
	void RenderPlayerButtons()
	{
		ImGui::BeginGroup();

		if (ImGui::Button("Clear Wanted", ImVec2(150, 0)))
		{
			QUEUE_JOB(=)
			{
				Features::ClearWanted();
			}
			END_JOB()
		}

		if (ImGui::Button("Fill Cores", ImVec2(150, 0)))
			Features::RestorePlayerCores();

		if (ImGui::Button("Give Gold Cores", ImVec2(150, 0)))
			Features::GiveGoldCores(g_LocalPlayer.m_Entity);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (ImGui::Button("TP To Waypoint", ImVec2(150, 0)))
			Features::TeleportToWaypoint();

		ImGui::PushButtonRepeat(true);
		if (ImGui::Button("TP Through Door", ImVec2(150, 0)))
		{
			QUEUE_JOB(=)
			{
				Features::TeleportThroughDoor();
			}
			END_JOB()
		}
		ImGui::PopButtonRepeat();

		if (g_LocalPlayer.m_Mount || !g_LocalPlayer.m_LastMount)
		{
			ImGui::BeginDisabled();
			ImGui::Button("TP To Last Mount", ImVec2(150, 0));
			ImGui::EndDisabled();
		}
		else
		{
			if (ImGui::Button("TP To Last Mount", ImVec2(150, 0)))
			{
				QUEUE_JOB(=)
				{
					PED::SET_PED_ONTO_MOUNT(g_LocalPlayer.m_Entity, g_LocalPlayer.m_LastMount, -1, true);
				}
				END_JOB()
			}
		}

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (ImGui::Button("Suicide", ImVec2(70, 0)))
		{
			QUEUE_JOB(=)
			{
				ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Entity, 0, 0);
			}
			END_JOB()
		}

		if (ImGui::Button("Clone", ImVec2(70, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::CLONE_PED(g_LocalPlayer.m_Entity, false, false, true);
			}
			END_JOB()
		}

		if (ImGui::Button("Clean", ImVec2(70, 0)))
			Features::CleanPed(g_LocalPlayer.m_Entity);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		
		if (ImGui::Button("Spawn Good Honor Enemy", ImVec2(220, 0)))
		{
			QUEUE_JOB(=)
			{
				Features::SpawnGoodHonorEnemy();
			}
			END_JOB()
		}

		if (ImGui::Button("Spawn Bad Honor Enemy", ImVec2(220, 0)))
		{
			QUEUE_JOB(=)
			{
				Features::SpawnBadHonorEnemy();
			}
			END_JOB()
		}

		ImGui::EndGroup();
	}
	
	void RenderPlayerCheckboxes()
	{
		ImGui::BeginGroup();
		if (ImGui::Checkbox("God Mode", g_Settings["god_mode"].get<bool*>()) && !g_Settings["god_mode"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Features::SetGodmode(false);
			}
			END_JOB()
		}
		ImGui::Checkbox("Clean###clplcb", g_Settings["clean_player"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Never Wanted", g_Settings["never_wanted"].get<bool*>());
		ImGui::Checkbox("Super Run", g_Settings["super_run"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Super Jump", g_Settings["super_jump"].get<bool*>());
		if (ImGui::Checkbox("No Ragdoll", g_Settings["no_ragdoll"].get<bool*>()) && !g_Settings["no_ragdoll"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Features::SetNoRagdoll(false);
			}
			END_JOB()
		}

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("No Sliding", g_Settings["no_sliding"].get<bool*>());
		ImGui::Checkbox("Clothing Menu", g_Settings["render_clothing_menu"].get<bool*>());

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Gold Cores", g_Settings["gold_cores"].get<bool*>());
		ImGui::EndGroup();
	}
}
