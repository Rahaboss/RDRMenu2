#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void GetLocalPlayerInfo();
	Entity GetMainPlayerEntity();
	void Teleport(const Vector3& pos);
	void TeleportOnGround(Vector3 pos);
	void SetPlayerModel(Hash Model);
	Hash GetDefaultPlayerModel();
	void ResetPlayerModel();
	void AddMoney(int AmountCents);
	void RemoveMoney(int AmountCents);
	int GetMoney();
	void SetMoney(int AmountCents);
	void ProcessPlayerFeatures();
	void SetOnLastMount();
	void ProcessMountFeatures();
	void TeleportToWaypoint();
	void TeleportThroughDoor();
	void SuperJump();
	void Suicide();
	void SetHairStyle(Hash Style, int Length);
}
