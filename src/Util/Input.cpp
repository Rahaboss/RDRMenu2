#include "pch.h"
#include "Input.h"

bool Input::IsKeyClicked(DWORD vKey)
{
	return GetAsyncKeyState(vKey) & static_cast<SHORT>(1);
}

bool Input::IsKeyHeld(DWORD vKey)
{
	return GetAsyncKeyState(vKey) & static_cast<SHORT>(1 << 15);
}
