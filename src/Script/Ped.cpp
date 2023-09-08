#include "pch.h"
#include "Ped.h"
#include "Rage/natives.h"

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
		if (void* temp = Pointers::sub_7FF669910FEC(ped))
			return Pointers::GetPedBoneIndex(temp, boneId) != -1;
	}
	EXCEPT{ LOG_EXCEPTION(); }
	
	return false;
}

Vector3 Script::GetPedBoneCoords(Ped ped, int boneId)
{
	Vector3 Result{};
	Pointers::GetPedBoneCoords(Result, ped, boneId, Vector3{});
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
