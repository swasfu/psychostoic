#include "perspective_camera.h"
#include "memory.h"

perspective_camera* perspective_camera_create(
	vector3 position,
	vector3 look,
	vector3 up,
	float near_z,
	float far_z
)
{
	perspective_camera* new_camera = ALLOC(perspective_camera);
	IFNULL(new_camera) return NULL;

	new_camera->position = position;
	new_camera->look = look;
	new_camera->up = up;
	new_camera->near_z = near_z;
	new_camera->far_z = far_z;

	return new_camera;
}