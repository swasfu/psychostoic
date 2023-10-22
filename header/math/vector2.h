#ifndef VECTOR2_H
#define VECTOR2_H

#include "matrix4x4.h"

typedef struct float2
{
	float x;
	float y;
} float2;

float2* float2_create(float x, float y, float z);
void float2_initialise(float2* f, float x, float y);
float2 float2_normalise(float2 a);
float float2_cross(float2 a, float2 b);
float float2_dot(float2 a, float2 b);
float2 float2_subtract(float2 a, float2 b);
float float2_magnitude(float2 a);

typedef struct double2
{
	double x;
	double y;
} double2;

#endif