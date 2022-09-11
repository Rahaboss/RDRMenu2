#pragma once

#include "pch.h"
#include "rage.h"

typedef int Any;
typedef int Blip;
typedef int Cam;
typedef int Entity;
typedef int FireId;
typedef joaat_t Hash;
typedef int Interior;
typedef int ItemSet;
typedef Entity Object;
typedef Entity Ped;
typedef Object Pickup;
typedef int Player;
typedef Entity ScrHandle;
typedef Entity Vehicle;

typedef int AnimScene;
typedef int PersChar;
typedef int PopZone;
typedef int Prompt;
typedef int PropSet;
typedef int Volume;

// Forward declarations
class scrNativeCallContext;

typedef void (*scrNativeHandler)(scrNativeCallContext*);
//typedef scrNativeHandler(*get_native_handler_t)(uint64_t);
//typedef void (*fix_vectors_t)(scrNativeCallContext*);

class scrVector
{
public:
	__declspec(align(8)) float x, y, z;
};
static_assert(sizeof(scrVector) == 24);
typedef scrVector Vector3;

class __declspec(align(16)) scrVec3N
{
public:
	 float x, y, z;
};
static_assert(sizeof(scrVec3N) == 16);

class scrNativeCallContext
{
public:
	void* m_ReturnValue; //0x0000
	uint32_t m_ArgCount; //0x0008
	uint64_t* m_Args; //0x0010
	uint32_t m_DataCount; //0x0018
	scrVector* m_OutVectors[4]; //
	scrVec3N m_InVectors[4]; //
	uint8_t pad[96]; //
};
static_assert(sizeof(scrNativeCallContext) == 0xE0);

class NativeContext : public scrNativeCallContext
{
private:
	uint64_t m_ArgStack[81];
	uint64_t m_ReturnStack[32];

public:
	NativeContext()
	{
		memset(this, 0, sizeof(NativeContext));
		m_ReturnValue = m_ReturnStack;
		m_Args = m_ArgStack;
	}

	void Reset()
	{
		m_ArgCount = 0;
		m_DataCount = 0;
		memset(m_ArgStack, 0, sizeof(m_ArgStack));
	}

	void FixVectors()
	{
		for (size_t i = 0; i < m_DataCount; i++)
		{
			auto out = m_OutVectors[i];
			auto in = m_InVectors + i;

			out->x = in->x;
			out->y = in->y;
			out->z = in->z;
		}
	}

	scrNativeHandler GetHandler(const uint64_t& hash);

	template <typename T>
	void PushArg(T&& arg)
	{
		static_assert(sizeof(T) <= sizeof(uint64_t));
		*reinterpret_cast<T*>(m_Args + m_ArgCount++) = std::move(arg);
	}

	scrNativeCallContext* GetContext()
	{
		return this;
	}

	void EndCall(const uint64_t& hash)
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

	template <typename T>
	T GetReturnValue()
	{
		return *reinterpret_cast<T*>(m_ReturnValue);
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
