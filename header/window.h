#ifndef WINDOW_H
#define WINDOW_H

#include "types.h"
#include "renderer.h"

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
} window;

window* window_create(char* name);
bool window_process(window* receiver);
void window_draw(window* canvas_window, renderer* target);

#define WINDOW(...) window_create(__VA_ARGS__)

#endif