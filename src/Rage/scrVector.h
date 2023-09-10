#pragma once

#include "pch.h"

namespace rage
{
	class scrVector
	{
	public:
		alignas(8) float x, y, z;

		inline constexpr scrVector() :
			x(0.0f), y(0.0f), z(0.0f)
		{
		}

		inline constexpr scrVector(float _x, float _y, float _z) :
			x(_x), y(_y), z(_z)
		{
		}

		inline constexpr scrVector operator+(const scrVector& Other)
		{
			return scrVector{
				x + Other.x,
				y + Other.y,
				z + Other.z
			};
		}

		inline scrVector& operator+=(const scrVector& Other)
		{
			x += Other.x;
			y += Other.y;
			z += Other.z;
		}

		inline constexpr scrVector operator*(float f)
		{
			return scrVector{
				x * f,
				y * f,
				z * f
			};
		}

		inline scrVector& operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
		}
	}; //Size: 0x0018
	static_assert(sizeof(scrVector) == 24);

	class alignas(16) scrVec3N
	{
	public:
		float x, y, z;
	}; //Size: 0x0010
	static_assert(sizeof(scrVec3N) == 16);
}
