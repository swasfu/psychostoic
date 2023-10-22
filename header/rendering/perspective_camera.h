#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "math/vector3.h"
#include "math/matrix4x4.h"

typedef struct perspective_camera
{
	float3 position;
	float3 forward;
	float3 up;
	float near_z;
	float far_z;
	float fov;
	float ratio;
} perspective_camera;

perspective_camera* perspective_camera_create(
	float3 position,
	float3 look,
	float3 up,
	float near,
	float far,
	float fov,
	float ratio
);

void look_at(perspective_camera* camera, float3 look);

float4x4 perspective_view_matrix(perspective_camera* camera);
float4x4 perspective_projection_matrix(perspective_camera* camera);

#endif