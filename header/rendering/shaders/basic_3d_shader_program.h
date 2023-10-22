#ifndef BASIC_3D_SHADER_PROGRAM
#define BASIC_3D_SHADER_PROGRAM

#include <stdlib.h>

#include "types.h"
#include "rendering/renderer.h"
#include "rendering/perspective_camera.h"
#include "rendering/shaders/basic_3d_vertex_shader.h"

typedef struct basic_3d_context
{
	basic_3d_vertex_shader_input* vertices;
	uint vertex_count;
	uint* indices;
	uint index_count;
	perspective_camera* camera;
	uint render_width;
	uint render_height;
} basic_3d_context;

basic_3d_context* basic_3d_context_create(
	basic_3d_vertex_shader_input* vertices,
	uint vertex_count,
	uint* indices,
	uint index_count,
	perspective_camera* camera,
	uint render_width,
	uint render_height
);
void basic_3d_context_destroy(void* old_context);

void basic_3d_shader_program(pixel* output, void* context);

#endif