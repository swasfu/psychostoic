#include "rendering/shaders/basic_3d_shader_program.h"

#include "memory.h"
#include "math/linear_interpolation.h"
#include "math/vector4.h"
#include "math/vector3.h"
#include "math/vector2.h"

#include <stdio.h>

basic_3d_context* basic_3d_context_create(
	basic_3d_vertex_shader_input* vertices,
	uint vertex_count,
	uint* indices,
	uint index_count,
	perspective_camera* camera,
	uint render_width,
	uint render_height
)
{
	basic_3d_context* new_context = allocate(basic_3d_context);
	if (!new_context) return NULL;

	new_context->vertices = vertices;
	new_context->vertex_count = vertex_count;
	new_context->indices = indices;
	new_context->index_count = index_count;
	new_context->camera = camera;
	new_context->render_width = render_width;
	new_context->render_height = render_height;

	return new_context;
}

void basic_3d_context_destroy(void* old_context)
{
	basic_3d_context* old_context_3d = (basic_3d_context*)old_context;
	free(old_context_3d->vertices);
	free(old_context_3d->indices);
	free(old_context_3d);
}

void basic_3d_shader_program(pixel* output, void* renderer_context)
{
	basic_3d_context* context = (basic_3d_context*)renderer_context;

	float3* screen_coordinates = allocate_array(float4, context->vertex_count);

	for (uint i = 0; i < context->vertex_count; i++)
	{
		float4 view_coordinates;
		float4_initialise(
			&view_coordinates,
			context->vertices[i].position.x,
			context->vertices[i].position.y,
			context->vertices[i].position.z,
			1
		);

		float4x4 view_matrix = perspective_view_matrix(context->camera);
		float4x4 projection_matrix = perspective_projection_matrix(context->camera);

		float4_transform(
			&view_coordinates,
			&view_matrix
		);

		float4_transform(
			&view_coordinates,
			&projection_matrix
		);

		float3_initialise(
			&screen_coordinates[i],
			((view_coordinates.x / view_coordinates.w) + 1.f) * 0.5f * (float)context->render_width,
			((view_coordinates.y / view_coordinates.w) + 1.f) * 0.5f * (float)context->render_height,
			view_coordinates.z / view_coordinates.w
		);
	}

	typedef struct fragment
	{
		uint x;
		uint y;
		float z;
		float4 colour;
	} fragment;

	typedef struct fragment_layer
	{
		uint fragment_count;
		fragment* fragments;
	} fragment_layer;

	uint triangle_count = context->index_count / 3;

	fragment_layer* layers = allocate_array(fragment_layer, triangle_count);
	for (uint i = 0; i < triangle_count; i++)
	{
		uint a_index = context->indices[i];
		uint b_index = context->indices[i + 1];
		uint c_index = context->indices[i + 2];

		if (screen_coordinates[b_index].y < screen_coordinates[a_index].y)
		{
			uint temp = b_index;
			b_index = a_index;
			a_index = temp;
		}
		if (screen_coordinates[c_index].y < screen_coordinates[a_index].y)
		{
			uint temp = c_index;
			c_index = a_index;
			a_index = temp;
		}
		if (screen_coordinates[c_index].y < screen_coordinates[b_index].y)
		{
			uint temp = c_index;
			c_index = b_index;
			b_index = temp;
		}

		float3 a_pos = screen_coordinates[a_index];
		float3 b_pos = screen_coordinates[b_index];
		float3 c_pos = screen_coordinates[c_index];

		/*printf("Processing triangle %u\na: %f, %f, %f\nb: %f, %f, %f\nc: %f, %f, %f\n",
			i,
			a_pos.x, a_pos.y, a_pos.z,
			b_pos.x, b_pos.y, b_pos.z,
			c_pos.x, c_pos.y, c_pos.z
		);*/

		if (((a_pos.z < 0.f || a_pos.z > 1.f) || ((a_pos.x < 0.f || a_pos.x >= (float)context->render_width) || (a_pos.y < 0.f || a_pos.y >= (float)context->render_height))) &&
			((b_pos.z < 0.f || b_pos.z > 1.f) || ((b_pos.x < 0.f || b_pos.x >= (float)context->render_width) || (b_pos.y < 0.f || b_pos.y >= (float)context->render_height))) &&
			((c_pos.z < 0.f || c_pos.z > 1.f) || ((c_pos.x < 0.f || c_pos.x >= (float)context->render_width) || (c_pos.y < 0.f || c_pos.y >= (float)context->render_height))))
		{
			//printf("Discarded triangle\n");
			layers[i].fragment_count = 0;
			continue;
		}

		float left = a_pos.x, right = a_pos.x;
		if (b_pos.x < left) left = b_pos.x;
		if (b_pos.x > right) right = b_pos.x;
		if (c_pos.x < left) left = c_pos.x;
		if (c_pos.x > right) right = c_pos.x;

		int width = (int)(right - left) + 1;
		int height = (int)(c_pos.y - a_pos.y) + 1;

		//printf("left: %f, right: %f\n", left, right);
		//printf("width: %d, height: %d\n", width, height);

		layers[i].fragments = allocate_array(fragment, width * height);

		int ab_length = (int)(b_pos.y - a_pos.y) + 1;
		float* ab = allocate_array(float, ab_length);
		linear_interpolation(ab, ab_length, a_pos.x, b_pos.x);

		int bc_length = (int)(c_pos.y - b_pos.y) + 1;
		float* bc = allocate_array(float, bc_length);
		linear_interpolation(bc, bc_length, b_pos.x, c_pos.x);

		float* ac = allocate_array(float, height);
		linear_interpolation(ac, height, a_pos.x, c_pos.x);

		float* abc = allocate_array(float, height);
		concatenate_segments(abc, ab, ab_length, bc, bc_length);

		//printf("ac length: %d, abc length: %d\n", height, ab_length + bc_length - 1);

		free(ab);
		free(bc);

		float* left_x = ac;
		float* right_x = abc;
		if (left_x[height / 2] > right_x[height / 2])
		{
			left_x = abc;
			right_x = ac;
		}

		int fragment_count = 0;
		int bottom = (int)(a_pos.y);
		for (int y = 0; y < height; y++)
		{
			int pixel_y = bottom + y;
			if (pixel_y < 0 || pixel_y > context->render_height) continue;
			//printf("x-values: %f - %f\n", left_x[y], right_x[y]);
			int start = (int)left_x[y];
			int end = (int)right_x[y];
			//printf("y-level %d, width %d / %d\n", y, end - start, width);
			for (int x = start; x < end; x++)
			{
				if (x < 0 || x > context->render_width) continue;
				// printf("Fragment %d / %d\n", fragment_count, width * height);
				float2 pp;
				float2_initialise(&pp, (float)x, (float)pixel_y);
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

				float4* a_colour = &context->vertices[a_index].colour;
				float4* b_colour = &context->vertices[b_index].colour;
				float4* c_colour = &context->vertices[c_index].colour;

				layers[i].fragments[fragment_count].x = x;
				layers[i].fragments[fragment_count].y = pixel_y;
				layers[i].fragments[fragment_count].z = 0.5f;
				float4_initialise(
					&layers[i].fragments[fragment_count].colour,
					a_weight * a_colour->x + b_weight * b_colour->x + c_weight * c_colour->x,
					a_weight * a_colour->y + b_weight * b_colour->y + c_weight * c_colour->y,
					a_weight * a_colour->z + b_weight * b_colour->z + c_weight * c_colour->z,
					a_weight * a_colour->w + b_weight * b_colour->w + c_weight * c_colour->w
				);
				fragment_count++;
			}
		}

		layers[i].fragment_count = fragment_count;

		free(ac);
		free(abc);
	}
	free(screen_coordinates);

	for (uint i = 0; i < triangle_count; i++)
	{
		//printf("Drawing triangle %u fragment count: %u\n", i, layers[i].fragment_count);
		for (uint j = 0; j < layers[i].fragment_count; j++)
		{
			fragment* current_fragment = &layers[i].fragments[j];
			pixel* current_pixel = &output[current_fragment->y * context->render_width + current_fragment->x];
			current_pixel->r = current_fragment->colour.x * 255.f;
			current_pixel->g = current_fragment->colour.y * 255.f;
			current_pixel->b = current_fragment->colour.z * 255.f;
		}
		free(layers[i].fragments);
	}

	free(layers);
}

/*
void basic_3d_shader_program(pixel* output, void* context)
{
	basic_3d_context* program_context = (basic_3d_context*)context;

	basic_3d_vertex_shader_uniforms vertex_shader_uniforms;
	vertex_shader_uniforms.camera = program_context->camera;
	basic_3d_vertex_shader_output* vertex_shader_output = allocate_array(
		basic_3d_vertex_shader_output,
		program_context->vertex_count
	);
	if (!vertex_shader_output) return;

	for (uint i = 0; i < program_context->vertex_count; i++)
	{
		basic_3d_vertex_shader(
			&vertex_shader_uniforms,
			&program_context->vertices[i],
			&vertex_shader_output[i]
		);
	}

	uint triangle_count = program_context->index_count / 3;
	basic_3d_rasterizer_uniforms rasterizer_uniforms;
	rasterizer_uniforms.width = program_context->render_width;
	rasterizer_uniforms.height = program_context->render_height;
	basic_3d_rasterizer_output* rasterizer_output = allocate_array(
		basic_3d_rasterizer_output,
		triangle_count
	);
	if (!rasterizer_output) return;
	for (uint i = 0; i < triangle_count; i += 1)
	{
		basic_3d_rasterizer_input rasterizer_input;
		rasterizer_input.a = &vertex_shader_output[program_context->indices[i * 3]];
		rasterizer_input.b = &vertex_shader_output[program_context->indices[i * 3 + 1]];
		rasterizer_input.c = &vertex_shader_output[program_context->indices[i * 3 + 2]];

		basic_3d_rasterizer(
			&rasterizer_uniforms,
			&rasterizer_input,
			&rasterizer_output[i]
		);
	}
	free(vertex_shader_output);

	for (uint i = 0; i < triangle_count; i += 1)
	{
		basic_3d_rasterizer_output triangle = rasterizer_output[i];

		for (uint p = 0; p < triangle.pixel_count; p++)
		{
			basic_3d_rasterizer_pixel rpixel = triangle.pixels[p];
			pixel* screen_pixel = &output[rpixel.y * program_context->render_width + rpixel.x];
			screen_pixel->r = rpixel.colour.x * 255;
			screen_pixel->g = rpixel.colour.y * 255;
			screen_pixel->b = rpixel.colour.z * 255;
		}

		if (triangle.pixels) free(triangle.pixels);
	}
	free(rasterizer_output);
}
*/