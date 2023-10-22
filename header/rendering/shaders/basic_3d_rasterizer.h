#ifndef BASIC_3D_RASTERIZER
#define BASIC_3D_RASTERIZER

#include "math/vector4.h"
#include "types.h"

#include "rendering/shaders/basic_3d_vertex_shader.h"

typedef struct basic_3d_rasterizer_uniforms
{
	uint width;
	uint height;
} basic_3d_rasterizer_uniforms;

typedef struct basic_3d_rasterizer_input
{
	basic_3d_vertex_shader_output* a;
	basic_3d_vertex_shader_output* b;
	basic_3d_vertex_shader_output* c;
} basic_3d_rasterizer_input;

typedef struct basic_3d_rasterizer_pixel
{
	uint x;
	uint y;
	float4 colour;
} basic_3d_rasterizer_pixel;

typedef struct basic_3d_rasterizer_output
{
	uint pixel_count;
	basic_3d_rasterizer_pixel* pixels;
} basic_3d_rasterizer_output;

void basic_3d_rasterizer(
	basic_3d_rasterizer_uniforms* uniforms,
	basic_3d_rasterizer_input* input,
	basic_3d_rasterizer_output* output
);

#endif