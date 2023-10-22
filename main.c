#include <stdio.h>
#include "window.h"
#include "rendering/renderer.h"
#include "rendering/shaders/basic_3d_shader_program.h"
#include "memory.h"
#include "math/vector3.h"

#include <time.h>

int main()
{
	window* game_window = window_create("Psychostoic", 1280, 960);
	if (!game_window)
	{
		printf("Failed to create game window");
		return -1;
	}

	renderer* game_renderer = renderer_create(1280, 960);
	if (!game_renderer)
	{
		printf("Failed to create renderer");
		return -1;
	}

	float3 camera_position;
	float3_initialise(&camera_position, 0.f, 0.f, -3.f);
	float3 camera_forward;
	float3_initialise(&camera_forward, 0.f, 0.f, 1.f);
	float3 camera_up;
	float3_initialise(&camera_up, 0.f, 1.f, 0.f);

	perspective_camera* camera = perspective_camera_create(
		camera_position,
		camera_forward,
		camera_up,
		0.1f,
		100.f,
		70.f,
		(float)game_renderer->width / (float)game_renderer->height
	);
	if (!camera)
	{
		printf("Failed to create perspective camera");
		return -1;
	}

	basic_3d_vertex_shader_input* vertices = allocate_array(
		basic_3d_vertex_shader_input,
		4
	);
	if (!vertices)
	{
		printf("Failed to create vertex array");
		return -1;
	}
	float3_initialise(&vertices[0].position, -0.5f, -0.5f, 0.0f);
	float4_initialise(&vertices[0].colour, 1.f, 1.f, 0.f, 1.f);
	float3_initialise(&vertices[1].position, 0.5f, -0.5f, 0.0f);
	float4_initialise(&vertices[1].colour, 0.f, 1.f, 0.f, 1.f);
	float3_initialise(&vertices[2].position, 0.0f, 0.5f, 0.0f);
	float4_initialise(&vertices[2].colour, 0.f, 0.f, 1.f, 1.f);
	float3_initialise(&vertices[3].position, 0.3f, 0.25f, 0.5f);
	float4_initialise(&vertices[3].colour, 1.f, 0.f, 1.f, 1.f);

	uint* indices = allocate_array(uint, 6);
	if (!indices)
	{
		printf("Failed to create index array");
		return -1;
	}
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	void* render_context = basic_3d_context_create(
		vertices,
		4,
		indices,
		6,
		camera,
		game_renderer->width,
		game_renderer->height
	);
	if (!render_context)
	{
		printf("Failed to create render context");
		return -1;
	}

	renderer_bind_context(game_renderer, render_context, basic_3d_context_destroy);
	renderer_bind_shader_program(game_renderer, basic_3d_shader_program);

	time_t start = time(NULL);
	uint frame_count = 0;
	float dx = 0.008f;
	float dz = 0.008f;
	/*float3 origin;
	float3_initialise(&origin, 0.f, 0.f, 0.f);*/
	while (window_process(game_window))
	{
		if (camera->position.x > 3.f || camera->position.x < -3.f) dx = -dx;
		if (camera->position.z > 3.f || camera->position.z < -3.f) dz = -dz;
		camera->position.x += dx;
		camera->position.z += dz;
		//look_at(camera, origin);
		/*camera->forward.x = -camera->position.x;
		camera->forward.y = -camera->position.y;
		camera->forward.z = -camera->position.z;*/
		renderer_clear(game_renderer, 0);
		renderer_render(game_renderer);
		window_draw(game_window, game_renderer);
		frame_count++;
	}

	time_t end = time(NULL);

	printf("Average FPS: %.2f", ((float)frame_count / (float)(end - start)));

	renderer_destroy(game_renderer);

	window_destroy(game_window);

	return 0;
}