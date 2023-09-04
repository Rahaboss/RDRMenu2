#include "pch.h"
#include "Player.h"
#include "PlayerInfo.h"
#include "Rage/natives.h"
#include "Entity.h"
#include "World.h"
#include "Thread/Thread.h"

void Script::GetLocalPlayerInfo()
{
	g_LocalPlayer.m_Index = PLAYER::PLAYER_ID();
	g_LocalPlayer.m_Entity = PLAYER::PLAYER_PED_ID();
	g_LocalPlayer.m_Mount = PED::GET_MOUNT(g_LocalPlayer.m_Entity);
	g_LocalPlayer.m_Vehicle = PED::GET_VEHICLE_PED_IS_IN(g_LocalPlayer.m_Entity, false);
	g_LocalPlayer.m_Ped = rage::CPedFactory::GetLocalPed();
}

Entity Script::GetMainPlayerEntity()
{
	if (g_LocalPlayer.m_Vehicle) { return g_LocalPlayer.m_Vehicle; }
	if (g_LocalPlayer.m_Mount) { return g_LocalPlayer.m_Mount; }
	return g_LocalPlayer.m_Entity;
}

void Script::Teleport(const Vector3& pos)
{
	SetEntityCoords(GetMainPlayerEntity(), pos);
}

void Script::TeleportOnGround(const Vector3& pos)
{
	LoadGround(pos);
	Teleport(pos);
	Thread::YieldThread();
	PlaceEntityOnGround(GetMainPlayerEntity());
}
