#include "pch.h"
#include "Player.h"
#include "PlayerInfo.h"
#include "Rage/natives.h"
#include "Entity.h"
#include "World.h"
#include "Thread/Thread.h"
#include "Spawning.h"
#include "Rage/ScriptGlobal.h"
#include "Ped.h"

void Script::GetLocalPlayerInfo()
{
	g_LocalPlayer.m_Index = PLAYER::PLAYER_ID();
	g_LocalPlayer.m_Entity = PLAYER::PLAYER_PED_ID();
	g_LocalPlayer.m_Mount = PED::GET_MOUNT(g_LocalPlayer.m_Entity);
	g_LocalPlayer.m_Vehicle = PED::GET_VEHICLE_PED_IS_IN(g_LocalPlayer.m_Entity, false);
	g_LocalPlayer.m_Model = ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity);
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

void Script::SetPlayerModel(Hash Model)
{
	// Check model validity or if model is already set
	if (!IsModelValid(Model) || g_LocalPlayer.m_Model == Model)
		return;

	// Clean up current model
	ReleaseModel(g_LocalPlayer.m_Model);

	// Request the new model
	RequestModel(Model);

	// Store old CPed structure before change
	rage::CPed* OldPed = g_LocalPlayer.m_Ped;

	// Set the model
	ScriptGlobal(40).At(39).Get<Hash&>() = Model; // medium_update.c: Global_40.f_39
	ScriptGlobal(1935630).At(2).Get<Hash&>() = Model; // medium_update.c: Global_1935630.f_2
	PLAYER::SET_PLAYER_MODEL(g_LocalPlayer.m_Index, Model, false);

	// Get the player info again, because a new player model will be created
	do
	{
		Thread::YieldThread();
		GetLocalPlayerInfo();
	} while (g_LocalPlayer.m_Ped == OldPed);

	// Clean up
	PED::_SET_RANDOM_OUTFIT_VARIATION(g_LocalPlayer.m_Entity, true);
	ReleaseModel(Model);
	Thread::YieldThread();

	// Set up ped info and stats
	PED::SET_PED_RANDOM_COMPONENT_VARIATION(g_LocalPlayer.m_Entity, 0);
	GiveGoldCores(g_LocalPlayer.m_Entity);
}

Hash Script::GetDefaultPlayerModel()
{
	Hash Global_1946054_f_1 = ScriptGlobal(1946054).At(1).Get<Hash>();
	
	// medium_update.c: func_392
	switch (Global_1946054_f_1)
	{
	case RAGE_JOAAT("MPC_PLAYER_TYPE_SP_ARTHUR"):
		return RAGE_JOAAT("PLAYER_ZERO");
	case RAGE_JOAAT("MPC_PLAYER_TYPE_SP_MARSTON"):
		return RAGE_JOAAT("PLAYER_THREE");
	case RAGE_JOAAT("MPC_PLAYER_TYPE_MP_MALE"):
		return RAGE_JOAAT("MP_MALE");
	case RAGE_JOAAT("MPC_PLAYER_TYPE_MP_FEMALE"):
		return RAGE_JOAAT("MP_FEMALE");
	}

	LOG_TEXT("%s: Unknown default player model: %d!\n", __FUNCTION__, Global_1946054_f_1);
	return RAGE_JOAAT("PLAYER_ZERO");
}

void Script::ResetPlayerModel()
{
	SetPlayerModel(GetDefaultPlayerModel());
}
