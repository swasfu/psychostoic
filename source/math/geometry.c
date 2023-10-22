#include "math/geometry.h"

float radiansf(float degrees)
{
	return degrees * PI_F / 180.f;
}

float degreesf(float radians)
{
	return radians * 180.f / PI_F;
}