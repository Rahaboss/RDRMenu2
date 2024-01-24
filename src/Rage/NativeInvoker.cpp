#include "pch.h"
#include "NativeInvoker.h"

NativeInvoker::NativeInvoker()
{
	memset(this, 0, sizeof(NativeInvoker));
	GetContext()->m_ReturnValue = m_ReturnStack;
	GetContext()->m_Args = m_ArgStack;
}

rage::scrNativeCallContext* NativeInvoker::GetContext()
{
	return &m_Context;
}

void NativeInvoker::Reset()
{
	GetContext()->m_ArgCount = 0;
	GetContext()->m_DataCount = 0;
	memset(m_ArgStack, 0, sizeof(m_ArgStack));
}

rage::scrNativeHandler NativeInvoker::GetHandler(rage::scrNativeHash hash)
{
	return Pointers::GetNativeHandler(hash);
}

void NativeInvoker::FixVectors()
{
	for (uint32_t i = 0; i < GetContext()->m_DataCount; i++)
	{
		rage::scrVector& out = *(GetContext()->m_OutVectors[i]);
		const rage::scrVec3N& in = GetContext()->m_InVectors[i];

		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}
}

// extern "C" disables C++ name mangling
extern "C" void CallASM(rage::scrNativeCallContext* Context, rage::scrNativeHandler Handler, void* Return);

void NativeInvoker::EndCall(rage::scrNativeHash hash)
{
	if (const rage::scrNativeHandler Handler = GetHandler(hash))
	{
		TRY
		{
#if ENABLE_NATIVE_RETURN_SPOOFING
			CallASM(GetContext(), Handler, Pointers::ReturnAddressSpoof);
#else
			Handler(GetContext());
#endif
			
			FixVectors();
		}
		EXCEPT { LOG_EXCEPTION(); }
	}
	else
	{
		LOG_TEXT("Failed to find native 0x%llX.", hash);
	}
}
