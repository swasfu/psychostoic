#ifndef RENDERER_H
#define RENDERER_H

#include "types.h"

#ifdef _WIN32
typedef struct pixel
{
	byte b;
	byte g;
	byte r;
	byte padding;
} pixel;
#endif

typedef struct renderer
{
	uint width;
	uint height;
	void* context;
	void (*context_destructor)(void*);
	pixel* pixels;
} renderer;

typedef void (*render_function)(renderer*, void*);

renderer* renderer_create(uint width, uint height);
void renderer_bind_context(renderer* target, void* context);
void renderer_destroy(renderer* old_renderer);

void render(renderer* target, render_function mode_render);
void clear(renderer* target);

#define RENDERER(...) renderer_create(__VA_ARGS__)
#define DESTROY_RENDERER(renderer) renderer_destroy(renderer)

#endif