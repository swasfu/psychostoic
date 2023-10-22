#include "rendering/renderer.h"

#include "memory.h"

renderer* renderer_create(uint width, uint height)
{
	renderer* new_renderer = allocate(renderer);
	if (new_renderer == NULL) return NULL;
	new_renderer->width = width;
	new_renderer->height = height;
	new_renderer->context = NULL;
	new_renderer->context_destructor = NULL;
	new_renderer->pixels = allocate_array(pixel, width * height);
	if (new_renderer->pixels == NULL)
	{
		renderer_destroy(new_renderer);
		return NULL;
	}
	return new_renderer;
}

void renderer_bind_shader_program(renderer* target, shader_program program)
{
	target->program = program;
}

void renderer_bind_context(renderer* target, void* context, destructor context_destructor)
{
	if (context && context_destructor)
	{
		target->context = context;
		target->context_destructor = context_destructor;
	}
}

void renderer_destroy(renderer* old_renderer)
{
	if (old_renderer->pixels) free(old_renderer->pixels);
	if (old_renderer->context) old_renderer->context_destructor(old_renderer->context);
	free(old_renderer);
}

void renderer_render(renderer* target)
{
	if (target->program && target->context && target->pixels)
	{
		target->program(target->pixels, target->context);
	}
}

void renderer_clear(renderer* target, byte value)
{
	if (target->pixels) set_memory(target->pixels, target->width * target->height * sizeof(pixel), value);
}
