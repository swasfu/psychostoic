#include "math/vector4.h"

void float4_initialise(float4* f, float x, float y, float z, float w)
{
	f->x = x;
	f->y = y;
	f->z = z;
	f->w = w;
}

void float4_transform(float4* v, float4x4* m)
{
	float4 new_v;
	new_v.x = v->x * m->x0 + v->y * m->y0 + v->z * m->z0 + v->w * m->w0;
	new_v.y = v->x * m->x1 + v->y * m->y1 + v->z * m->z1 + v->w * m->w1;
	new_v.z = v->x * m->x2 + v->y * m->y2 + v->z * m->z2 + v->w * m->w2;
	new_v.w = v->x * m->x3 + v->y * m->y3 + v->z * m->z3 + v->w * m->w3;
	*v = new_v;
}