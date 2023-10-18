#ifndef VECTOR4_H
#define VECTOR4_H

typedef struct vector4
{
	float x;
	float y;
	float z;
	float w;
} vector4;

vector4 vector3_instantiate(float x, float y, float z);
vector4 vector3_add(vector3 a, vector3 b);
vector4 vector3_sub(vector3 a, vector3 b);
vector4 vector3_scale(vector3 a, float scalar);
vector4 vector3_normalise(vector3 a);

typedef struct dvector4
{
	double x;
	double y;
	double z;
};

#define VECTOR4(...) vector4_instantiate(__VA_ARGS__)

#endif