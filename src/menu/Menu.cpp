#include "pch.h"
#include "Menu.h"
#include "Features.h"

namespace Menu
{
	static bool MenuOpen = true, SelectClicked = false;
	static float Scale = 0.5f;
	static int CurrentRenderedItem = 0, SelectedItem = 0, MaxItems = 1;
	bool RenderButton(const char* Text)
	{
		TRY
		{ 
			const bool Selected = CurrentRenderedItem == SelectedItem;
			constexpr float VInterval = 0.04f;
			float XPos = 0.025f, YPos = VInterval * CurrentRenderedItem + 0.025f;
			constexpr float ShadowOffset = 0.0015f;

			// Render dropshadow
			Features::RenderText(Text, XPos + ShadowOffset, YPos + ShadowOffset, 0, 0, 0, 255, Scale);
		
			// Render label text
			Features::RenderText(Text, XPos, YPos, (Selected ? 0 : 255), 255, 255, 255, Scale);

			// Update menu state
			CurrentRenderedItem++;
			MaxItems++;

			return Selected && SelectClicked;
		}
		EXCEPT{ LOG_EXCEPTION(); }
		return false;
	}

	bool RenderToggle(const char* Text, bool& Toggle, const char* EnabledText, const char* DisabledText)
	{
		const bool Selected = CurrentRenderedItem == SelectedItem;
		std::string Label(Text);
		Label += ": ";
		if (Toggle)
			Label += EnabledText;
		else
			Label += DisabledText;

		bool Ret = RenderButton(Label.c_str());
		if (Ret)
			Toggle = !Toggle;

		return Ret;
	}

	bool UpdateMenuState()
	{
		TRY
		{
			if (Features::IsKeyClicked(VK_INSERT))
				MenuOpen = !MenuOpen;
			
			if (Features::IsKeyClicked(VK_ESCAPE) || Features::IsKeyClicked(VK_NUMPAD0) || Features::IsKeyClicked(VK_BACK))
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

			if (RenderButton("Spawn Turret"))
				Features::SpawnVehicle(GATLING_GUN);

			if (RenderButton("Give $100,000"))
				Features::SetMoney(10000000);

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

			//RenderToggle("Enable No Snipers", Features::EnableNoSnipers);
			//
			//RenderToggle("Enable No Reload", Features::EnableNoReload);
			//
			//RenderToggle("Enable No Sliding", Features::EnableNoSliding);
			//
			//RenderToggle("Enable AddInventoryItem Logging", Features::EnableAddInventoryItemLogging);

			if (RenderButton("Eject Menu"))
				g_Running = false;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
