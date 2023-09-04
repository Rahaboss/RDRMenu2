#pragma once

#include "pch.h"

namespace rage
{
	class scrVector
	{
	public:
		alignas(8) float x, y, z;

		scrVector() = default;

		constexpr scrVector(float _x, float _y, float _z) :
			x(_x), y(_y), z(_z)
		{
		}

		scrVector operator+(const scrVector& other)
		{
			return scrVector{
				this->x + other.x,
				this->y + other.y,
				this->z + other.z
			};
		}

		scrVector& operator+=(const scrVector& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
		}

		scrVector operator*(float f)
		{
			return scrVector{
				this->x * f,
				this->y * f,
				this->z * f
			};
		}

		scrVector& operator*=(float f)
		{
			this->x *= f;
			this->y *= f;
			this->z *= f;
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
