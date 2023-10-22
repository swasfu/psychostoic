#ifndef MATRIX4X4_H
#define MATRIX4X4_H

typedef struct float4x4
{
	float x0, x1, x2, x3;
	float y0, y1, y2, y3;
	float z0, z1, z2, z3;
	float w0, w1, w2, w3;
} float4x4;

float4x4 float4x4_identity();

void float4x4_multiply(float4x4* c, float4x4* a, float4x4* b);

#endif