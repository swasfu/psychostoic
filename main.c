#include <stdio.h>
#include "window.h"
#include "renderer.h"
#include "render_3d.h"
#include "memory.h"

int main()
{
	window* game_window = WINDOW("Psychostoic");
	IFNULL(game_window)
	{
		printf("Failed to create game window");
		return -1;
	}

	renderer* renderer = RENDERER(640, 480);
	IFNULL(renderer)
	{
		printf("Failed to create renderer");
		return -1;
	}

	perspective_camera* camera = PERSPECTIVE_CAMERA(
		VECTOR3(0.f, 0.f, 0.f),
		VECTOR3(0.f, 0.f, 1.f),
		VECTOR3(0.f, 1.f, 0.f),
		0.1f,
		100.f);
	IFNULL(camera)
	{
		printf("Failed to create perspective camera");
		return -1;
	}

	void* render_context = CONTEXT_3D(camera);
	IFNULL(render_context)
	{
		printf("Failed to create render context");
		return -1;
	}

	vertex* vertices = ALLOC_ARRAY(vertex, 3);
	vertices[0].position = VECTOR3(0.1f, 0.2f, 0.0f);
	vertices[1].position = VECTOR3(0.4f, 0.9f, 0.0f);
	vertices[2].position = VECTOR3(1.1f, 0.5f, 0.0f);

	uint* indices = ALLOC_ARRAY(uint, 3);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	render_context_3d_bind(render_context, vertices, 3, indices, 3);

	renderer_bind_context(renderer, render_context, render_context_3d_destroy);

	while (window_process(game_window))
	{
		clear(renderer);
		render(renderer, render_3d);
		window_draw(game_window, renderer);
	}

	renderer_destroy(renderer);

	return 0;
}