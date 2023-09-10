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
		auto& out = *(GetContext()->m_OutVectors[i]);
		auto& in = GetContext()->m_InVectors[i];

		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}
}

void NativeInvoker::EndCall(rage::scrNativeHash hash)
{
	if (const auto Handler = GetHandler(hash))
	{
		TRY
		{
			Handler(GetContext());

			FixVectors();
		}
		EXCEPT { LOG_EXCEPTION(); }
	}
	else
	{
		LOG_TEXT("Failed to find native 0x%llX.", hash);
	}
}
