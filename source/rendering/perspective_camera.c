#include "rendering/perspective_camera.h"
#include "memory.h"

#include <math.h>

#include "math/geometry.h"

perspective_camera* perspective_camera_create(
	float3 position,
	float3 forward,
	float3 up,
	float near_z,
	float far_z,
	float fov,
	float ratio
)
{
	perspective_camera* new_camera = allocate(perspective_camera);
	if (!new_camera) return NULL;

	new_camera->position = position;
	new_camera->forward = forward;
	new_camera->up = up;
	new_camera->near_z = near_z;
	new_camera->far_z = far_z;
	new_camera->fov = fov;
	new_camera->ratio = ratio;

	return new_camera;
}

void look_at(perspective_camera* camera, float3 look)
{
	camera->forward = float3_subtract(look, camera->position);
}

float4x4 perspective_view_matrix(perspective_camera* camera)
{
	float3 z_axis = float3_normalise(camera->forward);
	float3 x_axis = float3_normalise(float3_cross(camera->up, z_axis));
	float3 y_axis = float3_cross(z_axis, x_axis);

	float4x4 view_matrix = {
		.x0 = x_axis.x, .x1 = y_axis.x, .x2 = z_axis.x,
		.y0 = x_axis.y, .y1 = y_axis.y, .y2 = z_axis.y,
		.z0 = x_axis.z, .z1 = y_axis.z, .z2 = z_axis.z,
		.w0 = -float3_dot(x_axis, camera->position),
		.w1 = -float3_dot(y_axis, camera->position),
		.w2 = -float3_dot(z_axis, camera->position),
		.w3 = 1
	};

	return view_matrix;
}

float4x4 perspective_projection_matrix(perspective_camera* camera)
{
	float focal = tanf(radiansf(camera->fov) * .5f);
	float4x4 projection_matrix = {
		.x0 = 1.f / (camera->ratio * focal),
		.y1 = 1.f / focal,
		.z2 = camera->far_z / (camera->far_z - camera->near_z),
		.z3 = 1,
		.w2 = -camera->near_z * camera->far_z / (camera->far_z - camera->near_z)
	};

	return projection_matrix;
}