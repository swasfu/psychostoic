#include "rendering/shaders/basic_3d_rasterizer.h"

#include "math/vector3.h"
#include "memory.h"
#include "math/linear_interpolation.h"
#include "math/vector2.h"

#include <math.h>

void basic_3d_rasterizer(
	basic_3d_rasterizer_uniforms* uniforms,
	basic_3d_rasterizer_input* input,
	basic_3d_rasterizer_output* output
)
{
	basic_3d_vertex_shader_output* a = input->a;
	basic_3d_vertex_shader_output* b = input->b;
	basic_3d_vertex_shader_output* c = input->c;

	// Order the vertices from bottom to top
	if (b->position.y < a->position.y)
	{
		basic_3d_vertex_shader_output* temp = b;
		b = a;
		a = temp;
	}

	if (c->position.y < a->position.y)
	{
		basic_3d_vertex_shader_output* temp = c;
		c = a;
		a = temp;
	}

	if (c->position.y < b->position.y)
	{
		basic_3d_vertex_shader_output* temp = c;
		c = b;
		b = temp;
	}

	float3 a_pos;
	float3_initialise(
		&a_pos,
		a->position.x / a->position.w,
		a->position.y / a->position.w,
		a->position.z / a->position.w
	);
	float3 b_pos;
	float3_initialise(
		&b_pos,
		b->position.x / b->position.w,
		b->position.y / b->position.w,
		b->position.z / b->position.w
	);
	float3 c_pos;
	float3_initialise(
		&c_pos,
		c->position.x / c->position.w,
		c->position.y / c->position.w,
		c->position.z / c->position.w
	);

	a_pos.x = (a_pos.x + 1.f) * 0.5f * (float)uniforms->width;
	a_pos.y = (a_pos.y + 1.f) * 0.5f * (float)uniforms->height;
	b_pos.x = (b_pos.x + 1.f) * 0.5f * (float)uniforms->width;
	b_pos.y = (b_pos.y + 1.f) * 0.5f * (float)uniforms->height;
	c_pos.x = (c_pos.x + 1.f) * 0.5f * (float)uniforms->width;
	c_pos.y = (c_pos.y + 1.f) * 0.5f * (float)uniforms->height;

	uint l_ab = 0;
	float* ab = linear_interpolation((int)a_pos.y, a_pos.x, (int)b_pos.y, b_pos.x, &l_ab);
	uint l_bc = 0;
	float* bc = linear_interpolation((int)b_pos.y, b_pos.x, (int)c_pos.y, c_pos.x, &l_bc);
	uint l_ac = 0;
	float* ac = linear_interpolation((int)a_pos.y, a_pos.x, (int)c_pos.y, c_pos.x, &l_ac);
	uint l_abc = 0;
	float* abc = concatenate_segments(ab, l_ab, bc, l_bc, &l_abc);

	float* x_left; float* x_right;
	if (ac[l_ac / 2] > abc[l_ac / 2])
	{
		x_left = abc;
		x_right = ac;
	} else
	{
		x_left = ac;
		x_right = abc;
	}

	int bottom = (int)a_pos.y;

	float left = a_pos.x;
	if (left > b_pos.x) left = b_pos.x;
	if (left > c_pos.x) left = c_pos.x;

	float right = a_pos.x;
	if (right < b_pos.x) right = b_pos.x;
	if (right < c_pos.x) right = c_pos.x;

	float width = right - left;

	output->pixels = allocate_array(basic_3d_rasterizer_pixel, ((uint)width + 1) * l_ac);
	if (!output->pixels) return;

	uint pixel_count = 0;

	for (uint y = 0; y < l_ac; y++)
	{
		int pixel_y = bottom + y;
		if (pixel_y < 0 || pixel_y > uniforms->height - 1) continue;
		int left = x_left[y];
		for (uint x = 0; x < (uint)(x_right[y] - x_left[y]); x++)
		{
			int pixel_x = left + x;
			if (pixel_x < 0 || pixel_x > uniforms->width - 1) continue;

			float2 pp;
			float2_initialise(&pp, (float)pixel_x, (float)pixel_y);
			float2 ap;
			float2_initialise(&ap, a_pos.x, a_pos.y);
			float2 bp;
			float2_initialise(&bp, b_pos.x, b_pos.y);
			float2 cp;
			float2_initialise(&cp, c_pos.x, c_pos.y);

			float a_weight = float2_cross(float2_subtract(cp, pp), float2_subtract(bp, pp)) * 0.5f;
			if (a_weight < 0) a_weight = -a_weight;
			float b_weight = float2_cross(float2_subtract(ap, pp), float2_subtract(cp, pp)) * 0.5f;
			if (b_weight < 0) b_weight = -b_weight;
			float c_weight = float2_cross(float2_subtract(ap, pp), float2_subtract(bp, pp)) * 0.5f;
			if (c_weight < 0) c_weight = -c_weight;

			float area = float2_cross(float2_subtract(cp, ap), float2_subtract(bp, ap)) * 0.5f;
			if (area < 0) area = -area;
			float inverse_area = 1.f / area;
			a_weight *= inverse_area;
			b_weight *= inverse_area;
			c_weight *= inverse_area;
			if (a_weight > 1.f) a_weight = 1.f;
			if (b_weight > 1.f) b_weight = 1.f;
			if (c_weight > 1.f) c_weight = 1.f;

			output->pixels[pixel_count].x = pixel_x;
			output->pixels[pixel_count].y = pixel_y;
			float4_initialise(
				&output->pixels[pixel_count].colour,
				a_weight * a->colour.x + b_weight * b->colour.x + c_weight * c->colour.x,
				a_weight * a->colour.y + b_weight * b->colour.y + c_weight * c->colour.y,
				a_weight * a->colour.z + b_weight * b->colour.z + c_weight * c->colour.z,
				a_weight * a->colour.w + b_weight * b->colour.w + c_weight * c->colour.w
			);
			pixel_count++;
		}
	}

	output->pixel_count = pixel_count;

	free(ac);
	free(abc);
}