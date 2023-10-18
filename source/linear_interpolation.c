#include "linear_interpolation.h"
#include "memory.h"

float* linear_interpolation(
	int i0, float d0,
	int i1, float d1,
	uint* length
)
{
	*length = (i1 - i0) + 1;
	float* values = ALLOC_ARRAY(float, *length);
	IFNULL(values) return NULL;
	float slope = (d1 - d0) / *length;
	for (uint i = 0; i < *length; i++)
	{
		values[i] = d0;
		d0 += slope;
	}
	return values;
}

float* concatenate_segments(
	float* a, uint a_length,
	float* b, uint b_length,
	uint* new_length
)
{
	*new_length = a_length + b_length - 1;
	float* new_segment = ALLOC_ARRAY(float, *new_length * sizeof(float));
	memcpy(new_segment, a, (a_length - 1) * sizeof(float));
	memcpy(new_segment + (a_length - 1), b, b_length * sizeof(float));
	free(a);
	free(b);
	return new_segment;
}