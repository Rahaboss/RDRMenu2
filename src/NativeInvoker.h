#pragma once

#include "pch.h"
#include "rage/scrNativeCallContext.h"
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

private:
	scrNativeHandler GetHandler(const uint64_t& hash);

	void FixVectors();

public:
	void EndCall(const uint64_t& hash);

	template <typename T>
	T GetReturnValue()
	{
		return *reinterpret_cast<T*>(GetContext()->m_ReturnValue);
	}
} inline g_NativeContext;

template <typename Ret, typename... Args>
Ret invoke(const uint64_t& hash, Args&&... args)
{
	g_NativeContext.Reset();
	(g_NativeContext.PushArg(std::move(args)), ...); // Parameter pack expansion
	g_NativeContext.EndCall(hash);

	if constexpr (!std::is_same_v<Ret, void>)
		return g_NativeContext.GetReturnValue<Ret>();
}
