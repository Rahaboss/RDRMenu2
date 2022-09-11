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

scrNativeHandler NativeContext::GetHandler(const uint64_t& hash)
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

void NativeContext::EndCall(const uint64_t& hash)
{
	if (const auto Handler = GetHandler(hash))
	{
		TRY
		{
			Handler(GetContext());
			FixVectors();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	else
	{
		printf("Failed to find native 0x%llX!\n", hash);
	}
}
