#include "math/vector3.h"

#include <math.h>

#include "memory.h"

float3* float3_create(float x, float y, float z)
{
	float3* new_f = allocate(float3);
}

void float3_initialise(float3* f, float x, float y, float z)
{
	f->x = x;
	f->y = y;
	f->z = z;
}

float3 float3_normalise(float3 a)
{
	float m = float3_magnitude(a);
	float3 normal = { .x = a.x / m, .y = a.y / m, .z = a.z / m };
	return normal;
}

float3 float3_cross(float3 a, float3 b)
{
	float3 c = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return c;
}

float float3_dot(float3 a, float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 float3_subtract(float3 a, float3 b)
{
	float3 c = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
	return c;
}

float float3_magnitude(float3 a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}