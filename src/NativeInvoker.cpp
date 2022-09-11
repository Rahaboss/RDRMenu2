#include "pch.h"
#include "NativeInvoker.h"
#include "Pointers.h"

scrNativeHandler NativeContext::GetHandler(const uint64_t& hash)
{
	return Pointers::GetNativeHandler(hash);
}
