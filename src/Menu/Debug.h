#pragma once

#include "pch.h"
#include "Features.h"

namespace Menu
{
	void RenderDebugTab();
	void RenderPedDebug();
	void RenderTextDebug();

	inline bool EnableFreeCam = false;
	inline Cam CamEntity = 0;

	inline bool EnableTextDebug = false;
	inline char TextBuffer[500]{ "~COLOR_RED~\xE2\x88\x91 Admin~n~~COLOR_CASH_GOLD~\xE2\x84\x85 0 Claim Reward~s~\xC2\xB5\x00" };
	inline float TextDebugX = 0.1f, TextDebugY = 0.1f, TextDebugScale = 1.0f;
	inline int TextFlags = 10;

	struct _PedDebug
	{
		Entity ent = 0;
		Hash model = A_C_FISHCHANNELCATFISH_01_XL;
		Vector3 pos{};
		float head{};
		int health{}, max_health{};
		bool freeze = false,
			 invincible = false,
			 visible = true,
			 injured = false;
	} inline PedDebug;
}
