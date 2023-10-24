#include "math/linear_interpolation.h"
#include "memory.h"

void linear_interpolation(
	float* values,
	int length,
	float d0, float d1
)
{
	float slope = (d1 - d0) / (float)length;

	for (int i = 0; i < length; i++)
	{
		values[i] = d0;
		d0 += slope;
	}
}

void concatenate_segments(
	float* new_segment,
	float* a, int a_length,
	float* b, int b_length
)
{
	if (b_length == 0) memcpy(new_segment, a, a_length * sizeof(float));
	else
	{
		memcpy(new_segment, a, (a_length - 1) * sizeof(float));
		memcpy(new_segment + (a_length - 1), b, b_length * sizeof(float));
	}
}