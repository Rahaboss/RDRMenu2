#include "pch.h"
#include "Rendering.h"
#include "Rage/natives.h"
#include "Memory/Pointers.h"
#include "Renderer/RGB.h"
#include "Rage/enums.h"
#include "Ped.h"
#include "..\Menu\Rendering.h"

void Script::RenderText(const char* Text, float PosX, float PosY, uint8_t ColR, uint8_t ColG, uint8_t ColB, uint8_t ColA, float Scale)
{
	if (PosX < 0.0f || PosX >= 1.0f)
		LOG_TEXT("%s: Invalid argument PosX: %.2f.", __FUNCTION__, PosX);
	if (PosY < 0.0f || PosY >= 1.0f)
		LOG_TEXT("%s: Invalid argument PosY: %.2f.", __FUNCTION__, PosY);

	TRY
	{
		UIDEBUG::_BG_SET_TEXT_SCALE(Scale, Scale);
		UIDEBUG::_BG_SET_TEXT_COLOR(ColR, ColG, ColB, ColA);
		UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", Text), PosX, PosY);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
