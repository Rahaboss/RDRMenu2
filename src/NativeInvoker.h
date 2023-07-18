#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Pointers.h"

class NativeContext
{
private:
	scrNativeCallContext m_Context;
	uint64_t m_ArgStack[81];
	uint64_t m_ReturnStack[32];

public:
	NativeContext();

private:
	scrNativeCallContext* GetContext();

public:
	void Reset();

	template <typename T>
	void PushArg(T&& arg)
	{
		static_assert(sizeof(T) <= sizeof(uint64_t));
		*reinterpret_cast<T*>(GetContext()->m_Args + GetContext()->m_ArgCount++) = std::move(arg);
	}

	static scrNativeHandler GetHandler(scrNativeHash hash);

private:
	void FixVectors();

public:
	void PrintNativeStackInfo(scrNativeHash hash, scrNativeHandler Handler);

	void EndCall(scrNativeHash hash);

	template <typename T>
	T GetReturnValue()
	{
		return *reinterpret_cast<T*>(GetContext()->m_ReturnValue);
	}
};

template <typename Ret, typename... Args>
inline Ret invoke(scrNativeHash hash, Args&&... args)
{
	NativeContext Context;
	Context.Reset();
	(Context.PushArg(std::move(args)), ...); // Parameter pack expansion
	Context.EndCall(hash);

	if constexpr (!std::is_same_v<Ret, void>)
		return Context.GetReturnValue<Ret>();
}
