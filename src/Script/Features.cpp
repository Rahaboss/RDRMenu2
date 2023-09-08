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

static bool s_ScriptsSetup = false;
void Features::OnSetup()
{
	TRY
	{
		LOG_TEXT("Running script setup.\n");
		Lists::Create();

		s_ScriptsSetup = true;
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Features::OnTick()
{
	TRY
	{
		Timer t;
		
		Script::ProcessPlayerFeatures();

		if (Menu::IsOpen)
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

		if (g_Settings["rapid_fire"].get<bool>())
			Script::RapidFire();

		if (g_Settings["disable_pinkerton_patrols"].get<bool>())
			Script::DisablePinkertonPatrols();

		Timer::s_ScriptThreadTickTime = t.GetMillis();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Features::OnExit()
{
	LOG_TEXT("Running script cleanup.\n");
}

void Features::StartExit()
{
	QUEUE_JOB(=)
	{
		OnExit();
		g_Running = false;
	}
	END_JOB()
	
	const auto EndTime = std::chrono::system_clock::now() + 1s;
	while (g_Running && std::chrono::system_clock::now() < EndTime)
		std::this_thread::sleep_for(10ms);
	
	g_Running = false;
}
