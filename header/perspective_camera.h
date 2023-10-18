#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "vector3.h"

typedef struct perspective_camera
{
	vector3 position;
	vector3 look;
	vector3 up;
	float near_z;
	float far_z;
} perspective_camera;

perspective_camera* perspective_camera_create(
	vector3 position,
	vector3 look,
	vector3 up,
	float near,
	float far
);

#define PERSPECTIVE_CAMERA(...) perspective_camera_create(__VA_ARGS__)

#endif