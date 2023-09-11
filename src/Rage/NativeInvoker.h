#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Memory/Pointers.h"

class NativeInvoker
{
private:
	rage::scrNativeCallContext m_Context;
	uint64_t m_ArgStack[81];
	uint64_t m_ReturnStack[32];

public:
	NativeInvoker();

	rage::scrNativeCallContext* GetContext();

public:
	void Reset();

	template<typename T>
	inline void PushArg(T&& arg)
	{
		static_assert(sizeof(T) <= sizeof(uint64_t));
		*reinterpret_cast<T*>(GetContext()->m_Args + GetContext()->m_ArgCount++) = std::move(arg);
	}

	static rage::scrNativeHandler GetHandler(rage::scrNativeHash hash);

private:
	void FixVectors();

public:
	void EndCall(rage::scrNativeHash hash);

	template<typename T>
	inline T GetReturnValue()
	{
		return *reinterpret_cast<T*>(GetContext()->m_ReturnValue);
	}
};

template<typename Ret, typename... Args>
inline Ret invoke(rage::scrNativeHash hash, Args&&... args)
{
	NativeInvoker Context;
	Context.Reset();
	(Context.PushArg(std::move(args)), ...); // Parameter pack expansion
	Context.EndCall(hash);

	if constexpr (!std::is_same_v<Ret, void>)
		return Context.GetContext()->GetRet<Ret>();
}
