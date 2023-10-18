#ifndef RENDER_3D_H
#define RENDER_3D_H

#include <stdlib.h>

#include "types.h"
#include "renderer.h"
#include "perspective_camera.h"

typedef struct vertex
{
	vector3 position;
} vertex;

typedef struct render_context_3d
{
	vertex* vertices;
	uint vertex_count;
	uint* indices;
	uint index_count;
	perspective_camera* camera;
} render_context_3d;

render_context_3d* render_context_3d_create(perspective_camera* camera);
void render_context_3d_bind(
	void* context,
	vertex* vertices,
	uint vertex_count,
	uint* indices,
	uint index_count
);
void render_context_3d_bind_vertices(void* context, vertex* vertices, uint vertex_count);
void render_context_3d_bind_indices(void* context, uint* indices, uint index_count);
void render_context_3d_destroy(void* old_context);

void render_3d(renderer* target, void* context);

#define CONTEXT_3D(...) render_context_3d_create(__VA_ARGS__)

#endif