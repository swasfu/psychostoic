#include "vector3.h"

vector3 vector3_instantiate(float x, float y, float z)
{
	vector3 new_vector = { x, y, z };
	return new_vector;
}