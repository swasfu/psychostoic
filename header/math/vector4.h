#ifndef VECTOR4_H
#define VECTOR4_H

#include "matrix4x4.h"

typedef struct float4
{
	float x;
	float y;
	float z;
	float w;
} float4;

float4* float4_create(float x, float y, float z, float w);
void float4_initialise(float4* f, float x, float y, float z, float w);
float4 float4_normalise(float4 a);
float4 float4_cross(float4 a, float4 b);
float4 float4_subtract(float4 a, float4 b);
float float4_magnitude(float4 a);

void float4_transform(float4* v, float4x4* m);

typedef struct double4
{
	double x;
	double y;
	double z;
	double w;
} double4;

#endif