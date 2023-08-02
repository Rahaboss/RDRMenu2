#include "pch.h"
#include "NativeInvoker.h"

NativeContext::NativeContext()
{
	memset(this, 0, sizeof(NativeContext));
	GetContext()->m_ReturnValue = m_ReturnStack;
	GetContext()->m_Args = m_ArgStack;
}

rage::scrNativeCallContext* NativeContext::GetContext()
{
	return &m_Context;
}

void NativeContext::Reset()
{
	GetContext()->m_ArgCount = 0;
	GetContext()->m_DataCount = 0;
	memset(m_ArgStack, 0, sizeof(m_ArgStack));
}

rage::scrNativeHandler NativeContext::GetHandler(rage::scrNativeHash hash)
{
	return Pointers::GetNativeHandler(hash);
}

void NativeContext::FixVectors()
{
	for (uint32_t i = 0; i < GetContext()->m_DataCount; i++)
	{
		auto& out = *(GetContext()->m_OutVectors[i]);
		auto& in = GetContext()->m_InVectors[i];

		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}
}

void NativeContext::PrintNativeStackInfo(rage::scrNativeHash hash, rage::scrNativeHandler Handler)
{
	TRY
	{
		// General native info
		//LOG_TO_CONSOLE("=== Native Stack Info ===\n");
		LOG_TO_CONSOLE("Caught exception in native 0x%llX!\n", hash);
		LOG_TO_CONSOLE("Handler: 0x%llX\n", (uint64_t)Handler);
		
		// Return value info
		if (m_Context.GetRet<uint64_t>() == 0)
			LOG_TO_CONSOLE("Return Value: 0\n");
		else
			LOG_TO_CONSOLE("Return Value: 0x%llX / %.2f\n", m_Context.GetRet<uint64_t>(),
				m_Context.GetRet<float>());

		// Argument info
		for (uint32_t i = 0; i < m_Context.m_ArgCount; i++)
		{
			if (m_Context.GetArg<uint64_t>(i) == 0)
				LOG_TO_CONSOLE("Arg[%u] = 0\n", i);
			else
				LOG_TO_CONSOLE("Arg[%u] = 0x%llX / %.2f\n", i, m_Context.GetArg<uint64_t>(i),
					m_Context.GetArg<float>(i));
		}
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

// extern "C" disables C++ name mangling
extern "C" void CallASM(rage::scrNativeCallContext* Context,
	rage::scrNativeHandler Handler, void* Return);

void NativeContext::EndCall(rage::scrNativeHash hash)
{
	if (const auto Handler = GetHandler(hash))
	{
		TRY
		{
#if 0
			Handler(GetContext());
#else
			CallASM(GetContext(), Handler, Pointers::ReturnAddressSpoof);
#endif

			FixVectors();
		}
		EXCEPT
		{
			//LOG_EXCEPTION();
			PrintNativeStackInfo(hash, Handler);
		}
	}
	else
	{
		LOG_TO_CONSOLE("Failed to find native 0x%llX!\n", hash);
	}
}
