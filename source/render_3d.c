#include "render_3d.h"

#include "memory.h"
#include "linear_interpolation.h"

render_context_3d* render_context_3d_create(perspective_camera* camera)
{
	render_context_3d* new_context = ALLOC(render_context_3d);
	IFNULL(new_context) return NULL;
	new_context->vertices = NULL;
	new_context->vertex_count = 0;
	new_context->indices = NULL;
	new_context->index_count = 0;
	new_context->camera = camera;
}

void render_context_3d_bind(
	void* context,
	vertex* vertices,
	uint vertex_count,
	uint* indices,
	uint index_count)
{
	render_context_3d* context_3d = (render_context_3d*)context;
	context_3d->vertices = vertices;
	context_3d->vertex_count = vertex_count;
	context_3d->indices = indices;
	context_3d->index_count = index_count;
}

void render_context_3d_destroy(void* old_context)
{
	render_context_3d* old_context_3d = (render_context_3d*)old_context;
	free(old_context_3d->vertices);
	free(old_context_3d->indices);
	free(old_context_3d);
}

void render_3d(renderer* renderer, void* context)
{
	render_context_3d* context_3d = (render_context_3d*)context;
	/*for (int y = 0; y < renderer->height; y++)
	{
		for (int x = 0; x < renderer->width; x++)
		{
			int index = (y * renderer->width + x);
			renderer->pixels[index].b = 255 * ((float)x / (float)renderer->width);
			renderer->pixels[index].r = 255 * ((float)y / (float)renderer->height);
		}
	}*/

	typedef struct screen_space_coord
	{
		float x;
		float y;
	} screen_space_coord;

	// Translate vertices to screen space
	screen_space_coord* screen_space_vertices = ALLOC_ARRAY(screen_space_coord, context_3d->vertex_count);
	IFNULL(screen_space_vertices) return;
	for (int index = 0; index < context_3d->vertex_count; index++)
	{
		screen_space_vertices[index].x = context_3d->vertices[index].position.x;
		screen_space_vertices[index].y = context_3d->vertices[index].position.y;
	}

	for (int index = 0; index < (int)context_3d->index_count - 2; index += 3)
	{
		screen_space_coord* a = &screen_space_vertices[context_3d->indices[index]];
		screen_space_coord* b = &screen_space_vertices[context_3d->indices[index + 1]];
		screen_space_coord* c = &screen_space_vertices[context_3d->indices[index + 2]];

		if (b->y < a->y)
		{
			screen_space_coord* temp = b;
			b = a;
			a = temp;
		}

		if (c->y < a->y)
		{
			screen_space_coord* temp = c;
			c = a;
			a = temp;
		}

		if (c->y < b->y)
		{
			screen_space_coord* temp = c;
			c = b;
			b = temp;
		}

		a->x *= (float)renderer->width;
		a->y *= (float)renderer->height;
		b->x *= (float)renderer->width;
		b->y *= (float)renderer->height;
		c->x *= (float)renderer->width;
		c->y *= (float)renderer->height;

		uint l_ab = 0;
		float* ab = linear_interpolation(a->y, a->x, b->y, b->x, &l_ab);
		uint l_bc = 0;
		float* bc = linear_interpolation(b->y, b->x, c->y, c->x, &l_bc);
		uint l_ac = 0;
		float* ac = linear_interpolation(a->y, a->x, c->y, c->x, &l_ac);
		uint l_abc = 0;
		float* abc = concatenate_segments(ab, l_ab, bc, l_bc, &l_abc);

		float* x_left; float* x_right;
		if (ac[l_ac / 2] > abc[l_ac / 2])
		{
			x_left = abc;
			x_right = ac;
		} else
		{
			x_left = ac;
			x_right = abc;
		}

		int bottom = a->y;
		for (int y = 0; y < l_ac; y++)
		{
			int pixel_y = bottom + y;
			if (pixel_y < 0 || pixel_y > renderer->height - 1) continue;
			for (int x = x_left[y]; x < x_right[y]; x++)
			{
				if (x < 0 || x > renderer->width - 1) continue;
				renderer->pixels[(bottom + y) * renderer->width + x].r = 255;
			}
		}

		free(ac);
		free(abc);
	}

	free(screen_space_vertices);
}