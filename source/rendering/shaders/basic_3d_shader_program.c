#include "rendering/shaders/basic_3d_shader_program.h"

#include "memory.h"
#include "math/linear_interpolation.h"
#include "math/vector4.h"

#include "rendering/shaders/basic_3d_rasterizer.h"

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
		free(triangle.pixels);
	}
	free(rasterizer_output);
}