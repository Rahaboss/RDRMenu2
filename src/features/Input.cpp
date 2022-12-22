#include "pch.h"
#include "Input.h"
#include "Features.h"

namespace Features
{
	bool IsKeyClicked(DWORD vKey)
	{
		return GetAsyncKeyState(vKey) & static_cast<SHORT>(1);
	}

	bool IsKeyHeld(DWORD vKey)
	{
		return GetAsyncKeyState(vKey) & static_cast<SHORT>(1 << 15);
	}
}
