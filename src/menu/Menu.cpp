#include "pch.h"
#include "Menu.h"
#include "Features.h"
#include "PlayerInfo.h"

namespace Menu
{
	static bool MenuOpen = true, SelectClicked = false;
	static float Scale = 0.5f;
	static int CurrentRenderedItem = 0, SelectedItem = 0, MaxItems = 1;
	static uint8_t SelectedColor[3]{ 255, 0, 0 };
	bool RenderButton(const char* Text, bool Disabled)
	{
		TRY
		{ 
			const bool Selected = CurrentRenderedItem == SelectedItem;
			constexpr float VInterval = 0.04f;
			float XPos = 0.025f, YPos = VInterval * CurrentRenderedItem + 0.025f;
			constexpr float ShadowOffset = 0.0015f;

			// Render dropshadow
			Features::RenderText(Text, XPos + ShadowOffset, YPos + ShadowOffset, 0, 0, 0, 255, Scale);
		
			uint8_t TextColor[4];
			TextColor[3] = 255;
			if (Selected)
			{
				TextColor[0] = SelectedColor[0];
				TextColor[1] = SelectedColor[1];
				TextColor[2] = SelectedColor[2];
			}
			else
			{
				TextColor[0] = TextColor[1] = TextColor[2] = 255;
			}

			if (Disabled)
			{
				TextColor[0] -= 100;
				if (!Selected)
				{
					TextColor[1] -= 100;
					TextColor[2] -= 100;
				}
			}

			// Render label text
			Features::RenderText(Text, XPos, YPos, TextColor[0], TextColor[1], TextColor[2], TextColor[3], Scale);

			// Update menu state
			CurrentRenderedItem++;
			MaxItems++;

			if (Disabled)
				return false;

			if (Selected && SelectClicked)
			{
				std::cout << __FUNCTION__ << "(\"" << Text << "\")\n";
				return true;
			}
			return false;
			//return Selected && SelectClicked;
		}
		EXCEPT{ LOG_EXCEPTION(); }
		return false;
	}

	bool RenderToggle(const char* Text, bool& Toggle, bool Disabled, const char* EnabledText, const char* DisabledText)
	{
		const bool Selected = CurrentRenderedItem == SelectedItem;
		std::string Label(Text);
		Label += ": ";
		if (Toggle)
			Label += EnabledText;
		else
			Label += DisabledText;

		bool Ret = RenderButton(Label.c_str(), Disabled);
		if (Ret)
			Toggle = !Toggle;

		return Ret;
	}

	bool UpdateMenuState()
	{
		TRY
		{
			if (!IsGameWindowActive())
			{
				SelectClicked = false;
				CurrentRenderedItem = 0;
				MaxItems = 0;
				return MenuOpen;
			}

			if (Features::IsKeyClicked(VK_INSERT))
				MenuOpen = !MenuOpen;
			
			if (/*Features::IsKeyClicked(VK_ESCAPE) || */Features::IsKeyClicked(VK_NUMPAD0) || Features::IsKeyClicked(VK_BACK))
				MenuOpen = false;

			if (!MenuOpen)
				return false;

			if (Features::IsKeyClicked(VK_NUMPAD8) || Features::IsKeyClicked(VK_UP))
			{
				SelectedItem--;
				if (SelectedItem == -1)
					SelectedItem = MaxItems - 1;
			}

			if (Features::IsKeyClicked(VK_NUMPAD2) || Features::IsKeyClicked(VK_DOWN))
			{
				SelectedItem++;
				if (SelectedItem == MaxItems)
					SelectedItem = 0;
			}

			SelectClicked = Features::IsKeyClicked(VK_NUMPAD5) || Features::IsKeyClicked(VK_RETURN);
			CurrentRenderedItem = 0;
			MaxItems = 0;

			return true;
		}
		EXCEPT{ LOG_EXCEPTION(); }
		return false;
	}

	bool IsGameWindowActive()
	{
		return GetForegroundWindow() == FindWindow(L"sgaWindow", NULL);
	}

	void RenderMenu()
	{
		TRY
		{
			if (!UpdateMenuState())
				return;
			
			if (RenderButton("Reveal Map"))
				Features::RevealMap();

			if (RenderButton("Give Core XP Items"))
			{
				Features::GiveAgedPirateRum();
				Features::GiveGinsengElixir();
				Features::GiveValerianRoot();
			}

			if (RenderButton("Spawn Good Honor Enemy"))
				Features::SpawnGoodHonorEnemy();

			if (RenderButton("Give Weapons"))
			{
				Features::GiveAllWeapons();
				Features::GiveAllDualWieldWeapons();
				Features::GiveAllAmmo();
			}

			if (RenderButton("Clear Wanted"))
				Features::ClearWanted();

			if (RenderButton("Restore Cores"))
			{
				Features::RestorePlayerCores();
				Features::RestoreHorseCores();
			}

			//if (RenderButton("Spawn Turret"))
			//	Features::SpawnVehicle(GATLING_GUN);
			//
			//if (RenderButton("Give $100,000"))
			//	Features::SetMoney(10000000);

			if (RenderButton("Give Cleaver"))
				Features::GiveWeapon(WEAPON_MELEE_CLEAVER);

			if (RenderButton("Spawn Charles"))
			{
				constexpr Hash model = CS_CHARLESSMITH;
				Ped ped = Features::SpawnPed(model);
				Features::EndSpawnPed(model, ped);
			}

			if (RenderButton("Drop Current Weapon"))
				Features::DropCurrentWeapon();

			if (RenderButton("TP To Waypoint"))
				Features::TeleportToWaypoint();

			if (RenderButton("TP Through Door"))
				Features::TeleportThroughDoor();

			if (RenderButton("TP To Guarma"))
				Features::Teleport(1424.31f, -7325.1f, 81.4575f);

			if (RenderToggle("Enable Godmode", Features::EnableGodMode) && !Features::EnableGodMode)
				Features::SetGodmode(false);

			RenderToggle("Enable Infinite Ammo", Features::EnableNoReload);
			
			//RenderToggle("Enable No Snipers", Features::EnableNoSnipers);
			//
			//RenderToggle("Enable No Sliding", Features::EnableNoSliding);
			//
			//RenderToggle("Enable AddInventoryItem Logging", Features::EnableAddInventoryItemLogging);

			if (RenderButton("Print Mount Attributes", g_LocalPlayer.m_Mount == 0))
				Features::PrintPedAttributes(g_LocalPlayer.m_Mount);

			if (RenderButton("Eject Menu"))
				g_Running = false;

			//if (RenderButton("Exit Game"))
			//	exit(0);
			//
			//if (RenderButton("Print Ped Attributes"))
			//	Features::PrintPedAttributes(g_LocalPlayer.m_Entity);
			//
			//if (RenderButton("Fix And Print Ped Attributes"))
			//{
			//	Features::FixAttributes(g_LocalPlayer.m_Entity);
			//	Features::PrintPedAttributes(g_LocalPlayer.m_Entity);
			//}
			//
			//if (g_LocalPlayer.m_Mount)
			//{
			//	if (RenderButton("Print Mount Attributes"))
			//		Features::PrintPedAttributes(g_LocalPlayer.m_Mount);
			//
			//	if (RenderButton("Fix And Print Mount Attributes"))
			//	{
			//		Features::FixHorseAttributes(g_LocalPlayer.m_Mount);
			//		Features::PrintPedAttributes(g_LocalPlayer.m_Mount);
			//	}
			//}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
