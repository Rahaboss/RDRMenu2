#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void GetLocalPlayerInfo();
	Entity GetMainPlayerEntity();
	void Teleport(const Vector3& pos);
	void TeleportOnGround(const Vector3& pos);
	void SetPlayerModel(Hash Model);
	Hash GetDefaultPlayerModel();
	void ResetPlayerModel();
}
