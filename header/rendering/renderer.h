#ifndef RENDERER_H
#define RENDERER_H

#include "types.h"
#include "rendering/shaders/shader.h"
#include "rendering/pixel.h"

typedef struct renderer
{
	uint width;
	uint height;
	shader_program program;
	void* context;
	void (*context_destructor)(void*);
	pixel* pixels;
} renderer;

typedef void (*destructor)(void*);

renderer* renderer_create(uint width, uint height);
void renderer_bind_shader_program(renderer* target, shader_program program);
void renderer_bind_context(renderer* target, void* context, destructor context_destructor);
void renderer_destroy(renderer* old_renderer);

void renderer_render(renderer* target);
void renderer_clear(renderer* target, byte value);

#endif