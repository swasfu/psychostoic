#ifndef BASIC_3D_PIXEL_SHADER
#define BASIC_3D_PIXEL_SHADER

#include "math/vector4.h"

typedef struct basic_3d_pixel_shader_input
{
	float4 position;
	float4 colour;
} basic_3d_pixel_shader_input;

typedef struct basic_3d_pixel_shader_output
{
	float4 colour;
} basic_3d_pixel_shader_output;

#endif