#include "pch.h"
#include "Entity.h"
#include "Rage/natives.h"

void Script::SetEntityCoords(Entity ent, const Vector3& pos)
{
	ENTITY::SET_ENTITY_COORDS(ent, pos.x, pos.y, pos.z, false, false, false, false);
}

void Script::PlaceEntityOnGround(Entity ent)
{
	ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ent, true);
}
