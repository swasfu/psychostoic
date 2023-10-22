#ifndef VECTOR3_H
#define VECTOR3_H

#include "matrix4x4.h"

typedef struct float3
{
	float x;
	float y;
	float z;
} float3;

float3* float3_create(float x, float y, float z);
void float3_initialise(float3* f, float x, float y, float z);
float3 float3_normalise(float3 a);
float3 float3_cross(float3 a, float3 b);
float float3_dot(float3 a, float3 b);
float3 float3_subtract(float3 a, float3 b);
float float3_magnitude(float3 a);

typedef struct double3
{
	double x;
	double y;
	double z;
} double3;

#endif