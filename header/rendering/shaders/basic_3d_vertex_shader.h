#ifndef BASIC_3D_VERTEX_SHADER
#define BASIC_3D_VERTEX_SHADER

#include "rendering/shaders/shader.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "rendering/perspective_camera.h"

typedef struct basic_3d_vertex_shader_uniforms
{
	perspective_camera* camera;
} basic_3d_vertex_shader_uniforms;

typedef struct basic_3d_vertex_shader_input
{
	float3 position;
	float4 colour;
} basic_3d_vertex_shader_input;

typedef struct basic_3d_vertex_shader_output
{
	float4 position;
	float4 colour;
} basic_3d_vertex_shader_output;

void basic_3d_vertex_shader(
	basic_3d_vertex_shader_uniforms* uniforms,
	basic_3d_vertex_shader_input* input,
	basic_3d_vertex_shader_output* output
);

#endif