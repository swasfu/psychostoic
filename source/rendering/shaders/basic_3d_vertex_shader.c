#include "rendering/shaders/basic_3d_vertex_shader.h"
#include "math/vector4.h"

void basic_3d_vertex_shader(
	basic_3d_vertex_shader_uniforms* uniforms,
	basic_3d_vertex_shader_input* input,
	basic_3d_vertex_shader_output* output)
{
	float4_initialise(
		&output->position,
		input->position.x,
		input->position.y,
		input->position.z,
		1
	);

	float4x4 view_matrix = perspective_view_matrix(uniforms->camera);
	float4x4 projection_matrix = perspective_projection_matrix(uniforms->camera);

	float4_transform(
		&output->position,
		&view_matrix
	);

	float4_transform(
		&output->position,
		&projection_matrix
	);

	output->colour = input->colour;
}
