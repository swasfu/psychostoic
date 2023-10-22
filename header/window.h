#ifndef WINDOW_H
#define WINDOW_H

#include "types.h"
#include "rendering/renderer.h"

#ifdef _WIN32
#include <windows.h>
typedef struct windows_window_data
{
	HINSTANCE instance_handle;
	HWND window_handle;
	WCHAR* window_class_name;
	HDC device_context_handle;
} windows_window_data;

#endif

typedef struct window
{
	void* window_data;
	uint width;
	uint height;
} window;

window* window_create(char* name, uint width, uint height);
void window_destroy(window* old_window);
bool window_process(window* receiver);
void window_draw(window* canvas_window, renderer* target);

#endif