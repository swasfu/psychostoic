#ifndef LINEAR_INTERPOLATION_H
#define LINEAR_INTERPOLATION_H

#include "types.h"

float* linear_interpolation(
	int i0, float d0,
	int i1, float d1,
	uint* length
);

float* concatenate_segments(
	float* a, uint a_length,
	float* b, uint b_length,
	uint* new_length
);

#endif