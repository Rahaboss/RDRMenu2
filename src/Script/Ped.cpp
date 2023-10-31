#include "pch.h"
#include "Ped.h"
#include "Rage/natives.h"
#include "Thread/Thread.h"
#include "Rage/enums.h"
#include "Entity.h"

void Script::GiveGoldCores(Ped ped)
{
	for (int i = 0; i < 3; i++)
	{
		constexpr float Duration = 10000.0f; // seconds
		constexpr int CoreValue = 100; // 0 - 100
		constexpr BOOL Sound = false;

		ATTRIBUTE::DISABLE_ATTRIBUTE_OVERPOWER(ped, i);
		ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(ped, i, CoreValue);
		ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(ped, i, Duration, Sound);
		ATTRIBUTE::ENABLE_ATTRIBUTE_OVERPOWER(ped, i, Duration, Sound);
	}
}

void Script::FillCores(Ped ped)
{
	for (int i = 0; i < 3; i++)
		ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(ped, i, 100);
}

bool Script::DoesPedBoneExist(Ped ped, int boneId)
{
	TRY
	{
		if (rage::CPed* PedAddress = Pointers::GetPedAddress(ped))
			return Pointers::GetPedBoneIndex(PedAddress, boneId) != -1;
	}
	EXCEPT{ LOG_EXCEPTION(); }
	
	return false;
}

Vector3 Script::GetPedBoneCoords(Ped ped, int boneId)
{
	Vector3 Result{};
	TRY
	{
		Pointers::GetPedBoneCoords(Result, ped, boneId, Vector3{});
	}
	EXCEPT{ LOG_EXCEPTION(); }
	return Result;
}

void Script::SetPedOntoMount(Ped ped, Ped Mount, int Seat)
{
	if (ped && Mount)
		PED::SET_PED_ONTO_MOUNT(ped, Mount, -1, true);
}

void Script::SetPedNoRagdoll(Ped ped, bool Toggle)
{
	if (Toggle)
		PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, -1);
	else
		PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, -1);
}

void Script::SetPedOutfitPreset(Ped ped, int Preset, bool KeepAccessories)
{
	PED::_EQUIP_META_PED_OUTFIT_PRESET(ped, Preset, KeepAccessories);
}

void Script::SetMetaPedOutfit(Ped ped, Hash Outfit)
{
	const int requestId = PED::_REQUEST_META_PED_OUTFIT(Script::GetEntityModel(ped), Outfit);
	while (!PED::_HAS_META_PED_OUTFIT_LOADED(requestId))
		Thread::YieldThread();
	PED::_APPLY_PED_META_PED_OUTFIT(requestId, ped, true, false);
	PED::_RELEASE_META_PED_OUTFIT_REQUEST(requestId);
}

void Script::CleanPed(Ped ped)
{
	PED::CLEAR_PED_BLOOD_DAMAGE(ped);
	PED::CLEAR_PED_WETNESS(ped);
	PED::CLEAR_PED_ENV_DIRT(ped);
	PED::CLEAR_PED_DECORATIONS(ped);
	PED::_SET_PED_DIRT_CLEANED(ped, 0.0f, -1, true, true);
	for (int i = 0; i < 10; i++)
	{
		PED::_CLEAR_PED_BLOOD_DAMAGE_FACIAL(ped, i);
		PED::CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(ped, i);
		PED::CLEAR_PED_DAMAGE_DECAL_BY_ZONE(ped, i, "ALL");
	}
}

void Script::NoSliding(Ped ped)
{
	// PCF_0x435F091E = set ped can run into steep slope
	PED::SET_PED_RESET_FLAG(ped, PCF_0x435F091E, true);
}

void Script::SuperRun(Ped ped)
{
	if (PED::IS_PED_RAGDOLL(ped))
		return;

	if (TASK::IS_PED_RUNNING(ped) || TASK::IS_PED_SPRINTING(ped))
		ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1, true, true, true, false, true);
}

eMetaPedType Script::GetPedType(Ped ped)
{
	return Pointers::GetMetaPedType(ped);
}
