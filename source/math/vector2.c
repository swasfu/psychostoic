#include "math/vector2.h"

#include <math.h>

#include "memory.h"

float2* float2_create(float x, float y)
{
	float2* new_f = allocate(float2);
}

void float2_initialise(float2* f, float x, float y)
{
	f->x = x;
	f->y = y;
}

float2 float2_normalise(float2 a)
{
	float m = float2_magnitude(a);
	float2 normal = { .x = a.x / m, .y = a.y / m };
	return normal;
}

float float2_cross(float2 a, float2 b)
{
	return a.x * b.y - a.y * b.x;
}

float float2_dot(float2 a, float2 b)
{
	return a.x * b.x + a.y * b.y;
}

float2 float2_subtract(float2 a, float2 b)
{
	float2 c = {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
	return c;
}

float float2_magnitude(float2 a)
{
	return sqrtf(a.x * a.x + a.y * a.y);
}