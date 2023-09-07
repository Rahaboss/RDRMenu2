#include "pch.h"
#include "Math.h"
#include "Rage/natives.h"

float Script::DegreeToRadian(float f)
{
	return f * static_cast<float>(M_PI / 180.0);
}

Vector3 Script::RotationToDirection(const Vector3& v)
{
	float x = DegreeToRadian(v.x);
	float z = DegreeToRadian(v.z);

	float num = abs(cos(x));

	return Vector3{ -sin(z) * num, cos(z) * num, sin(x) };
}
