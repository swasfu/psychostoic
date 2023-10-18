#ifndef VECTOR3_H
#define VECTOR3_H

typedef struct vector3
{
	float x;
	float y;
	float z;
} vector3;

vector3 vector3_instantiate(float x, float y, float z);
vector3 vector3_add(vector3 a, vector3 b);
vector3 vector3_sub(vector3 a, vector3 b);
vector3 vector3_scale(vector3 a, float scalar);
vector3 vector3_normalise(vector3 a);

typedef struct dvector3
{
	double x;
	double y;
	double z;
};

#define VECTOR3(...) vector3_instantiate(__VA_ARGS__)

#endif