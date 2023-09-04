#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void SetEntityCoords(Entity ent, const Vector3& pos);
	void PlaceEntityOnGround(Entity ent);
}
