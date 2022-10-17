#pragma once

#include "pch.h"
#include "joaat.h"

class scrVector
{
public:
	__declspec(align(8)) float x, y, z;

	scrVector() = default;

	scrVector(const float& _x, const float& _y, const float& _z) :
		x(_x), y(_y), z(_z)
	{
	}
}; //Size: 0x0018
static_assert(sizeof(scrVector) == 24);

class __declspec(align(16)) scrVec3N
{
public:
	float x, y, z;
}; //Size: 0x0010
static_assert(sizeof(scrVec3N) == 16);

class scrNativeCallContext
{
public:
	void* m_ReturnValue; //0x0000
	uint32_t m_ArgCount; //0x0008
	uint64_t* m_Args; //0x0010
	uint32_t m_DataCount; //0x0018
	scrVector* m_OutVectors[4]; //0x0020
	scrVec3N m_InVectors[4]; //0x0040
	uint8_t pad[96]; //0x0080

	template<typename T>
	T GetArg(size_t Index)
	{
		return *(T*)(&(m_Args[Index]));
	}

	template<typename T>
	T GetRet()
	{
		return *(T*)(m_ReturnValue);
	}
}; //Size: 0x00E0
static_assert(sizeof(scrNativeCallContext) == 0xE0);

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

typedef scrVector Vector3;

typedef void (*scrNativeHandler)(scrNativeCallContext*);
typedef scrNativeHandler(*GetNativeHandler_t)(uint64_t);

inline std::ostream& operator<<(std::ostream& out, const scrVector& vec)
{
	if (vec.x)
		out << vec.x << 'f';
	else
		out << "0.0f";
	out << ", ";
	if (vec.y)
		out << vec.y << 'f';
	else
		out << "0.0f";
	out << ", ";
	if (vec.z)
		out << vec.z << 'f';
	else
		out << "0.0f";

	return out;
}
