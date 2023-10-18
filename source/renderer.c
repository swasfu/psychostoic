#include "renderer.h"
#include "render_3d.h"

#include "memory.h"

renderer* renderer_create(uint width, uint height)
{
	renderer* new_renderer = ALLOC(renderer);
	IFNULL(new_renderer) return NULL;
	new_renderer->width = width;
	new_renderer->height = height;
	new_renderer->context = NULL;
	new_renderer->context_destructor = NULL;
	new_renderer->pixels = ALLOC_ARRAY(pixel, width * height);
	IFNULL(new_renderer->pixels)
	{
		DESTROY_RENDERER(new_renderer);
		return NULL;
	}
	return new_renderer;
}

void renderer_bind_context(renderer* target, void* context, void (*context_destructor)(void*))
{
	target->context = context;
	target->context_destructor = context_destructor;
}

void renderer_destroy(renderer* old_renderer)
{
	free(old_renderer->pixels);
	old_renderer->context_destructor(old_renderer->context);
	free(old_renderer);
}

void render(renderer* target, render_function mode_render)
{
	IFNULL(target->context) return NULL;
	mode_render(target, target->context);
}

void clear(renderer* target)
{
	ZERO_MEMORY(target->pixels, target->width * target->height * sizeof(pixel));
}
