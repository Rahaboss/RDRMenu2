#include "pch.h"
#include "Features.h"
#include "Menu.h"
#include "rage/natives.h"

void Features::OnSetup()
{
}

void Features::OnTick()
{
	if (Menu::IsOpen)
		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
}

void Features::OnExit()
{
}
