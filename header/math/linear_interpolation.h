#ifndef LINEAR_INTERPOLATION_H
#define LINEAR_INTERPOLATION_H

#include "types.h"

void linear_interpolation(
	float* values,
	int length,
	float d0, float d1
);

void concatenate_segments(
	float* new_segment,
	float* a, int a_length,
	float* b, int b_length
);

#endif