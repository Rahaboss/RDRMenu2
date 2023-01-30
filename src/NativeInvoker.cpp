#include "pch.h"
#include "NativeInvoker.h"

NativeContext::NativeContext()
{
	memset(this, 0, sizeof(NativeContext));
	GetContext()->m_ReturnValue = m_ReturnStack;
	GetContext()->m_Args = m_ArgStack;
}

scrNativeCallContext* NativeContext::GetContext()
{
	return &m_Context;
}

void NativeContext::Reset()
{
	GetContext()->m_ArgCount = 0;
	GetContext()->m_DataCount = 0;
	memset(m_ArgStack, 0, sizeof(m_ArgStack));
}

scrNativeHandler NativeContext::GetHandler(uint64_t hash)
{
	return Pointers::GetNativeHandler(hash);
}

void NativeContext::FixVectors()
{
	for (size_t i = 0; i < GetContext()->m_DataCount; i++)
	{
		auto out = GetContext()->m_OutVectors[i];
		auto in = GetContext()->m_InVectors + i;

		out->x = in->x;
		out->y = in->y;
		out->z = in->z;
	}
}

void NativeContext::PrintNativeStackInfo(uint64_t hash, void* Handler)
{
	TRY
	{
		//printf("=== Native Stack Info ===\n");
		LOG_TO_CONSOLE("Caught exception in native 0x%llX!\n", hash);
		LOG_TO_CONSOLE("Handler: 0x%llX\n", (uint64_t)Handler);
		LOG_TO_CONSOLE("Return Value: 0x%llX / %.2f\n", m_Context.GetRet<uint64_t>(), m_Context.GetRet<float>());
		for (uint32_t i = 0; i < m_Context.m_ArgCount; i++)
			LOG_TO_CONSOLE("Arg[%u] = 0x%llX\n", i, m_Context.GetArg<uint64_t>(i));
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void NativeContext::EndCall(uint64_t hash)
{
	if (const auto Handler = GetHandler(hash))
	{
		TRY
		{
			Handler(GetContext());
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
