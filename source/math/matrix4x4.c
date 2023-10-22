#include "math/matrix4x4.h"

float4x4 float4x4_identity()
{
	float4x4 identity_matrix = { .x0 = 1, .y1 = 1, .z2 = 1, .w3 = 1 };
	return identity_matrix;
}

void float4x4_multiply(float4x4* c, float4x4* a, float4x4* b)
{
	c->x0 = a->x0 * b->x0 + a->x1 * b->y0 + a->x2 * b->z0 + a->x3 * b->w0;
	c->x1 = a->x0 * b->x1 + a->x1 * b->y1 + a->x2 * b->z1 + a->x3 * b->w1;
	c->x2 = a->x0 * b->x2 + a->x1 * b->y2 + a->x2 * b->z2 + a->x3 * b->w2;
	c->x3 = a->x0 * b->x3 + a->x1 * b->y3 + a->x2 * b->z3 + a->x3 * b->w3;

	c->y0 = a->y0 * b->x0 + a->y1 * b->y0 + a->y2 * b->z0 + a->y3 * b->w0;
	c->y1 = a->y0 * b->x1 + a->y1 * b->y1 + a->y2 * b->z1 + a->y3 * b->w1;
	c->y2 = a->y0 * b->x2 + a->y1 * b->y2 + a->y2 * b->z2 + a->y3 * b->w2;
	c->y3 = a->y0 * b->x3 + a->y1 * b->y3 + a->y2 * b->z3 + a->y3 * b->w3;

	c->z0 = a->z0 * b->x0 + a->z1 * b->y0 + a->z2 * b->z0 + a->z3 * b->w0;
	c->z1 = a->z0 * b->x1 + a->z1 * b->y1 + a->z2 * b->z1 + a->z3 * b->w1;
	c->z2 = a->z0 * b->x2 + a->z1 * b->y2 + a->z2 * b->z2 + a->z3 * b->w2;
	c->z3 = a->z0 * b->x3 + a->z1 * b->y3 + a->z2 * b->z3 + a->z3 * b->w3;

	c->w0 = a->w0 * b->x0 + a->w1 * b->y0 + a->w2 * b->z0 + a->w3 * b->w0;
	c->w1 = a->w0 * b->x1 + a->w1 * b->y1 + a->w2 * b->z1 + a->w3 * b->w1;
	c->w2 = a->w0 * b->x2 + a->w1 * b->y2 + a->w2 * b->z2 + a->w3 * b->w2;
	c->w3 = a->w0 * b->x3 + a->w1 * b->y3 + a->w2 * b->z3 + a->w3 * b->w3;
}
