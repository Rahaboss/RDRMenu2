#include "pch.h"
#include "Screen.h"
#include "Memory/Pointers.h"
#include "Script/Ped.h"
#include "Script/Rendering.h"

bool ESP::WorldToScreen(const Vector3& WorldPos, float& ScreenX, float& ScreenY)
{
	TRY
	{
		return Pointers::WorldToScreen(WorldPos, ScreenX, ScreenY);
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return false;
}

const ImVec2& ESP::GetScreenSize()
{
	return ImGui::GetMainViewport()->WorkSize;
}

bool ESP::WorldToScreenScaled(const Vector3& WorldPos, float& ScreenX, float& ScreenY)
{
	TRY
	{
		bool Result = Pointers::WorldToScreen(WorldPos, ScreenX, ScreenY);
		if (Result)
		{
			ScreenX *= GetScreenSize().x;
			ScreenY *= GetScreenSize().y;
		}

		return Result;
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return false;
}

bool ESP::GetPedBoneScreenCoords(Ped ped, int boneId, ImVec2& OutCoords)
{
	TRY
	{
		if (!Script::DoesPedBoneExist(ped, boneId))
			return false;

		if (!WorldToScreen(Script::GetPedBoneCoords(ped, boneId), OutCoords.x, OutCoords.y))
			return false;
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return true;
}

bool ESP::GetPedBoneScreenCoordsScaled(Ped ped, int boneId, ImVec2& OutCoords)
{
	TRY
	{
		if (!Script::DoesPedBoneExist(ped, boneId))
			return false;

		if (!WorldToScreenScaled(Script::GetPedBoneCoords(ped, boneId), OutCoords.x, OutCoords.y))
			return false;

		return true;
	}
	EXCEPT{ LOG_EXCEPTION(); }
	
	return false;
}
