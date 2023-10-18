#include "matrix4x4.h"

matrix4x4 matrix4x4_identity()
{
	matrix4x4 identity_matrix = { .x0 = 1, .y1 = 1, .z2 = 1, .w3 = 1 };
	return identity_matrix;
}