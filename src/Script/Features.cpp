#include "pch.h"
#include "Features.h"
#include "Menu/Menu.h"
#include "Rage/natives.h"
#include "Config/Settings.h"
#include "Script/World.h"
#include "Thread/JobQueue.h"
#include "Script/Player.h"
#include "Script/Entity.h"
#include "PlayerInfo.h"
#include "Script/Rendering.h"
#include "Renderer/RGB.h"
#include "Script/Ped.h"
#include "Rage/enums.h"
#include "ESP/Screen.h"
#include "Config/Lists.h"
#include "Util/Timer.h"
#include "Script/Weapon.h"
#include "Script/Freecam.h"
#include "Script/Notification.h"

void Features::OnSetup()
{
	TRY
	{
		LOG_TEXT("Running script setup.");
		Lists::Create();
		Script::NotifyHelp("Hello from RD" "\xE2\x88\x91" "Menu2");
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Features::OnTick()
{
	TRY
	{
		Timer ScriptThreadTickTimer;

		if (Menu::IsOpen)
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

		Script::ProcessPlayerFeatures();

		Script::ProcessMountFeatures();

		if (g_Settings["weapon"]["rapid_fire"].get<bool>())
			Script::RapidFire();

		if (g_Settings["weapon"]["rgb_electric_lantern"].get<bool>())
			Script::RGBElectricLantern();

		if (g_Settings["disable_pinkerton_patrols"].get<bool>())
			Script::DisablePinkertonPatrols();

		if (g_Settings["never_wanted"].get<bool>())
			Script::ClearWanted();

		if (g_Settings["disable_black_borders"].get<bool>())
			Script::DisableBlackBorders();

		if (Script::FreecamEnabled)
			Script::TickFreecam();

		if (g_Settings["disable_hud"].get<bool>())
			Script::DisableHUD(true);

		Timer::s_ScriptThreadTickTime = ScriptThreadTickTimer.GetMillis();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Features::OnExit()
{
	LOG_TEXT("Running script cleanup.");
	
	Script::DestroyFreecam();
}

void Features::StartExit()
{
	QUEUE_JOB(=)
	{
		OnExit();
		g_Running = false;
	}
	END_JOB()
	
	const auto EndTime = std::chrono::system_clock::now() + 500ms;
	while (g_Running && std::chrono::system_clock::now() < EndTime)
		std::this_thread::sleep_for(10ms);
	
	g_Running = false;
}
