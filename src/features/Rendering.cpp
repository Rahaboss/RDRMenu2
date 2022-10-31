#include "pch.h"
#include "Features.h"

// Features for drawing on the screen

namespace Features
{
	void RenderText(const char* Text, float PosX, float Posy, uint8_t ColR, uint8_t ColG, uint8_t ColB, uint8_t ColA, float Scale)
	{
		TRY
		{
			UIDEBUG::_BG_SET_TEXT_SCALE(Scale, Scale);
			UIDEBUG::_BG_SET_TEXT_COLOR(ColR, ColG, ColB, ColA);
			UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", Text), PosX, Posy);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RenderTextOnEntity(Entity Ent, const char* Text)
	{
		TRY
		{ 
			if (!Ent)
				return;

			Vector3 pos = ENTITY::GET_ENTITY_COORDS(Ent, TRUE, TRUE);
			float x = 0, y = 0;
			if (!WorldToScreen(pos.x, pos.y, pos.z, x, y))
				return;

			RenderText(Text, x, y, g_rgb[0], g_rgb[1], g_rgb[2], (uint8_t)(255 * 0.75f), 0.8f);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	bool WorldToScreen(float X, float Y, float Z, float& OutX, float& OutY)
	{
		TRY
		{
			if (HUD::GET_HUD_SCREEN_POSITION_FROM_WORLD_POSITION(X, Y, Z, &OutX, &OutY))
				return false;
			return true;
		}
		EXCEPT{ LOG_EXCEPTION(); }
		
		return false;
	}
}
