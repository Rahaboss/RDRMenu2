#include "pch.h"
#include "Random.h"

static std::seed_seq s_Sequence{ std::chrono::steady_clock::now().time_since_epoch().count() };
static __declspec(thread) std::mt19937_64 s_RandomEngine{ s_Sequence };
static std::uniform_int_distribution<std::mt19937_64::result_type> s_Distribution;

uint64_t Random::U64()
{
	return static_cast<uint64_t>(s_Distribution(s_RandomEngine));
}

uint32_t Random::U32()
{
	return static_cast<uint32_t>(U64());
}

uint16_t Random::U16()
{
	return static_cast<uint16_t>(U64());
}

uint8_t Random::U8()
{
	return static_cast<uint8_t>(U64());
}

int64_t Random::S64()
{
	return static_cast<int64_t>(U64());
}

int32_t Random::S32()
{
	return static_cast<int32_t>(S64());
}

int16_t Random::S16()
{
	return static_cast<int16_t>(S64());
}

int8_t Random::S8()
{
	return static_cast<int8_t>(S64());
}

double Random::F64()
{
	return static_cast<double>(U64()) / static_cast<double>(0xFFFFFFFFFFFFFFFF);
}

float Random::F32()
{
	return static_cast<float>(F64());
}

bool Random::Bool()
{
	return static_cast<bool>(U64() % 2);
}
