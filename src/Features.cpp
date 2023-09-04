#include "pch.h"
#include "Features.h"
#include "Menu.h"
#include "Rage/natives.h"
#include "Config/Settings.h"
#include "Script/World.h"
#include "Thread/JobQueue.h"
#include "Script/Player.h"

void Features::OnSetup()
{
	LOG_TEXT("Running script setup.\n");
}

void Features::OnTick()
{
	Script::GetLocalPlayerInfo();

	if (Menu::IsOpen)
		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

	if (g_Settings["disable_pinkerton_patrols"].get<bool>())
		Script::DisablePinkertonPatrols();
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
