#include "pch.h"
#include "Math.h"

namespace Features
{
	float DegreeToRadian(float f)
	{
		return (3.14159265359f / 180.0f) * f;
	}
	
	Vector3 RotationToDirection(const Vector3& v)
	{
		float x = DegreeToRadian(v.x);
		float z = DegreeToRadian(v.z);

		float num = abs(cos(x));

		return Vector3{ -sin(z) * num, cos(z) * num, sin(x) };
	}
}
