#include "window.h"

#define DEFAULT_WINDOW_POS_X  100
#define DEFAULT_WINDOW_POS_Y  100
#define DEFAULT_WINDOW_WIDTH  640
#define DEFAULT_WINDOW_HEIGHT 480

#ifdef _WIN32
#include "windows_helpers.h"

LRESULT CALLBACK windows_window_procedure(HWND window_handle, UINT message, WPARAM word_param, LPARAM long_param)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(window_handle, message, word_param, long_param);
}

window* window_create(char* name)
{
	window* new_window = malloc(sizeof(window));
	IFNULL(new_window) return NULL;

	windows_window_data* window_data = malloc(sizeof(windows_window_data));
	IFNULL(window_data) return NULL;
	window_data->instance_handle = GetModuleHandle(NULL);
	window_data->window_class_name = widen_string(name);

	WNDCLASS window_class = { 0 };
	window_class.lpszClassName = window_data->window_class_name;
	window_class.hInstance = window_data->instance_handle;
	window_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.lpfnWndProc = windows_window_procedure;

	RegisterClass(&window_class);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	RECT rect;
	rect.left = DEFAULT_WINDOW_POS_X;
	rect.top = DEFAULT_WINDOW_POS_Y;
	rect.right = DEFAULT_WINDOW_POS_X + DEFAULT_WINDOW_WIDTH;
	rect.bottom = DEFAULT_WINDOW_POS_Y + DEFAULT_WINDOW_HEIGHT;
	AdjustWindowRect(&rect, style, FALSE);

	window_data->window_handle = CreateWindowEx(
		0,
		window_data->window_class_name,
		window_data->window_class_name,
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		window_data->instance_handle,
		NULL
	);

	window_data->device_context_handle = GetDC(window_data->window_handle);

	new_window->window_data = window_data;

	ShowWindow(window_data->window_handle, SW_SHOW);

	return new_window;
}

void window_destroy(window* old_window)
{
	windows_window_data* window_data = (windows_window_data*)old_window->window_data;
	UnregisterClass(window_data->window_class_name, window_data->instance_handle);
	free(window_data->window_class_name);
	free(window_data);
	free(old_window);
}

bool window_process(window* receiver)
{
	MSG message;

	while (PeekMessage(&message, NULL, 0u, 0u, PM_REMOVE))
	{
		if (message.message == WM_QUIT) return FALSE;
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return TRUE;
}

void window_draw(window* canvas_window, renderer* target)
{
	//LPVOID pixel_memory = VirtualAlloc(0, width * height * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//IFNULL(pixel_memory) return;

	BITMAPINFO bitmap_info;
	bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth = target->width;
	bitmap_info.bmiHeader.biHeight = target->height;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;

	windows_window_data* window_data = (windows_window_data*)canvas_window->window_data;
	StretchDIBits(
		window_data->device_context_handle,
		0,
		0,
		DEFAULT_WINDOW_WIDTH,
		DEFAULT_WINDOW_HEIGHT,
		0,
		0,
		target->width,
		target->height,
		target->pixels,
		&bitmap_info,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	//VirtualFree(pixel_memory, 0, MEM_RELEASE);
}

#endif