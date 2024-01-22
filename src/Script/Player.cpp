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
#include "Rage/enums.h"
#include "Config/Settings.h"
#include "Rage/ScriptGlobal.h"

void Script::GetLocalPlayerInfo()
{
	g_LocalPlayer.m_Index = PLAYER::PLAYER_ID();
	g_LocalPlayer.m_Entity = ScriptGlobal(35).Get<Ped>(); // PLAYER::PLAYER_PED_ID()
	g_LocalPlayer.m_Mount = ScriptGlobal(1935630).At(40).Get<Ped>(); // PED::GET_MOUNT(g_LocalPlayer.m_Entity);
	g_LocalPlayer.m_LastMount = PLAYER::_GET_ACTIVE_HORSE_FOR_PLAYER(g_LocalPlayer.m_Index);
	g_LocalPlayer.m_Vehicle = PED::GET_VEHICLE_PED_IS_IN(g_LocalPlayer.m_Entity, false);
	g_LocalPlayer.m_Model = GetEntityModel(g_LocalPlayer.m_Entity); // ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity)
	g_LocalPlayer.m_Ped = rage::CPedFactory::GetLocalPed();
	g_LocalPlayer.m_Pos = ScriptGlobal(36).Get<const Vector3&>(); // GetEntityCoords(g_LocalPlayer.m_Entity);
	g_LocalPlayer.m_Heading = ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity);
	g_LocalPlayer.m_Speed = ENTITY::GET_ENTITY_SPEED(g_LocalPlayer.m_Entity);
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

void Script::TeleportOnGround(Vector3 pos)
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
	if (Hash* Global_1946054_f_1 = ScriptGlobal(1946054).At(1).Get<Hash*>())
	{
		// medium_update.c: func_392
		switch (*Global_1946054_f_1)
		{
		case RAGE_JOAAT("MPC_PLAYER_TYPE_SP_ARTHUR"):
		case RAGE_JOAAT("MPC_PLAYER_TYPE_SP_ARTHUR_SICK"):
			return RAGE_JOAAT("PLAYER_ZERO");
		case RAGE_JOAAT("MPC_PLAYER_TYPE_SP_MARSTON"):
			return RAGE_JOAAT("PLAYER_THREE");
		case RAGE_JOAAT("MPC_PLAYER_TYPE_MP_MALE"):
			return RAGE_JOAAT("MP_MALE");
		case RAGE_JOAAT("MPC_PLAYER_TYPE_MP_FEMALE"):
			return RAGE_JOAAT("MP_FEMALE");
		}
	}

	return RAGE_JOAAT("PLAYER_ZERO");
}

void Script::ResetPlayerModel()
{
	Hash Model = GetDefaultPlayerModel();
	SetPlayerModel(Model);

	switch (Model)
	{
	case RAGE_JOAAT("PLAYER_ZERO"):
		SetMetaPedOutfit(g_LocalPlayer.m_Entity, RAGE_JOAAT("META_OUTFIT_COOL_WEATHER"));
		break;
	case RAGE_JOAAT("PLAYER_THREE"):
		SetMetaPedOutfit(g_LocalPlayer.m_Entity, RAGE_JOAAT("META_OUTFIT_GUNSLINGER"));
		break;
	}
}

void Script::AddMoney(int AmountCents)
{
	MONEY::_MONEY_INCREMENT_CASH_BALANCE(AmountCents, ADD_REASON_DEFAULT);
}

void Script::RemoveMoney(int AmountCents)
{
	MONEY::_MONEY_DECREMENT_CASH_BALANCE(AmountCents);
}

int Script::GetMoney()
{
	return MONEY::_MONEY_GET_CASH_BALANCE();
}

void Script::SetMoney(int AmountCents)
{
	const int Amount = AmountCents - GetMoney();
	if (Amount > 0)
		AddMoney(Amount);
	else if (Amount < 0)
		RemoveMoney(-Amount);
}

void Script::ProcessPlayerFeatures()
{
	TRY
	{
		GetLocalPlayerInfo();

		if (g_Settings["player"]["godmode"].get<bool>())
			SetEntityInvincible(g_LocalPlayer.m_Entity, true);

		if (g_Settings["player"]["gold_cores"].get<bool>())
			GiveGoldCores(g_LocalPlayer.m_Entity);

		if (g_Settings["player"]["no_ragdoll"].get<bool>())
			SetPedNoRagdoll(g_LocalPlayer.m_Entity, true);

		if (g_Settings["player"]["clean"].get<bool>())
			CleanPed(g_LocalPlayer.m_Entity);

		if (g_Settings["player"]["super_jump"].get<bool>())
			SuperJump();

		if (g_Settings["player"]["no_sliding"].get<bool>())
			NoSliding(g_LocalPlayer.m_Entity);

		if (g_Settings["player"]["super_run"].get<bool>())
			SuperRun(g_LocalPlayer.m_Entity);

		if (g_Settings["player"]["invisible"].get<bool>())
			SetEntityInvisible(g_LocalPlayer.m_Entity, true);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::SetOnLastMount()
{
	SetPedOntoMount(g_LocalPlayer.m_Entity, g_LocalPlayer.m_LastMount);
}

void Script::ProcessMountFeatures()
{
	TRY
	{
		if (g_Settings["mount"]["godmode"].get<bool>())
			SetEntityInvincible(g_LocalPlayer.m_Mount, true);

		if (g_Settings["mount"]["gold_cores"].get<bool>())
			GiveGoldCores(g_LocalPlayer.m_Mount);

		if (g_Settings["mount"]["clean"].get<bool>())
			CleanPed(g_LocalPlayer.m_Mount);

		if (g_Settings["mount"]["no_sliding"].get<bool>())
			NoSliding(g_LocalPlayer.m_Mount);

		if (g_Settings["mount"]["no_ragdoll"].get<bool>())
			SetPedNoRagdoll(g_LocalPlayer.m_Mount, true);

		if (g_Settings["mount"]["super_run"].get<bool>())
			SuperRun(g_LocalPlayer.m_Mount);

		if (g_Settings["mount"]["invisible"].get<bool>())
			SetEntityInvisible(g_LocalPlayer.m_Mount, true);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::TeleportToWaypoint()
{
	TRY
	{
		if (!MAP::IS_WAYPOINT_ACTIVE())
		{
			LOG_TEXT("Waypoint not active.");
			return;
		}
		
		Vector3 Coords{ MAP::_GET_WAYPOINT_COORDS() };
		float GroundZ;
		bool UseGroundZ;
		
		for (int i = 0; i < 100; i++)
		{
			const float TestZ = (i * 10.0f) - 100.0f;

			Teleport(Vector3{ Coords.x, Coords.y, TestZ });
			if (i % 5 == 0)
				Thread::YieldThread();

			UseGroundZ = MISC::GET_GROUND_Z_FOR_3D_COORD(Coords.x, Coords.y, TestZ, &GroundZ, false);
			if (UseGroundZ)
				break;
		}

		Coords.z = (UseGroundZ ? GroundZ : GetEntityCoords(GetMainPlayerEntity()).z);
		Teleport(Coords);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::TeleportThroughDoor()
{
	Teleport(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(GetMainPlayerEntity(), 0.0, 3.0, -0.3));
}

void Script::SuperJump()
{
	MISC::SET_SUPER_JUMP_THIS_FRAME(g_LocalPlayer.m_Index);
}

void Script::Suicide()
{
	SetEntityInvincible(g_LocalPlayer.m_Entity, false);
	ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Entity, 0, 0);
}

void Script::SetHairStyle(Hash Style, int Length)
{
	if (Hash* pStyle = ScriptGlobal(1946054).At(1497).At(1).At(1, 3).Get<Hash*>())
		if (Hash* pStyle2 = ScriptGlobal(40).At(7748).At(2).Get<Hash*>())
			*pStyle = *pStyle2 = Style;
	if (int* pLength = ScriptGlobal(1946054).At(2656).Get<int*>())
		if (int* pLength2 = ScriptGlobal(40).At(7748).At(1).Get<int*>())
			*pLength = *pLength2 = Length;
}
