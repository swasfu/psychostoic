#ifndef MATRIX4X4_H
#define MATRIX4X4_H

typedef struct matrix4x4
{
	float x0, x1, x2, x3;
	float y0, y1, y2, y3;
	float z0, z1, z2, z3;
	float w0, w1, w2, w3;
} matrix4x4;

matrix4x4 matrix4x4_identity();

#endif