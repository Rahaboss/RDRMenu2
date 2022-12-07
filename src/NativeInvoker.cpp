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

void NativeContext::PrintNativeStackInfo(uint64_t Hash, void* Handler)
{
	TRY
	{
		//std::cout << "=== Native Stack Info ===\n";
		std::cout << "Caught exception in native " << LOG_HEX(Hash) << "!\n";
		std::cout << "Handler: " << LOG_HEX(Handler) << "\n";
		std::cout << "Return Value: " << LOG_HEX(m_Context.GetRet<uint64_t>()) << " / " << m_Context.GetRet<float>() << "\n";
		for (uint32_t i = 0; i < m_Context.m_ArgCount; i++)
		{
			std::cout << "Arg[" << i << "] = " << LOG_HEX(m_Context.GetArg<uint64_t>(i)) << "\n";
		}
	}
	EXCEPT{ LOG_EXCEPTION(); }
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
		EXCEPT
		{
			//LOG_EXCEPTION();
			PrintNativeStackInfo(hash, Handler);
		}
	}
	else
	{
		std::cout << "Failed to find native " << LOG_HEX(hash) << "!\n";
	}
}
