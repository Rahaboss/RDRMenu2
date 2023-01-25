#include "pch.h"
#include "Rendering.h"
#include "Features.h"

// Features for drawing on the screen
// \xE2\x88\x91	= Rockstar
// \xC2\xA6		= Verified
// \xE2\x80\xB9	= Created
// \xE2\x80\xBA	= Blank
// \xCE\xA9		= Padlock
// \xE2\x84\xAE	= Token?
// \xE2\x84\x85	= C/O
// \xE2\x86\x95	= Up/Down Arrow
// \xC6\xB1		= Thick Padlock
// ~n~			= Newline ('\n')
// ~COLOR_RED~\xE2\x88\x91 Admin
// ~COLOR_SOCIAL_CLUB~

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

			Vector3 pos = ENTITY::GET_ENTITY_COORDS(Ent, true, true);
			float x = 0, y = 0;
			if (!WorldToScreen(pos.x, pos.y, pos.z, x, y))
				return;

			//const auto& RGB = GetRGB();
			RenderText(Text, x, y, GET_RGB(), static_cast<uint8_t>(255ui8 * 0.75f), 0.8f);
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
