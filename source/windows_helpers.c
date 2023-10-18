#include "windows_helpers.h"

#include <stdlib.h>

WCHAR* widen_string(char* string)
{
	const size_t string_size = strlen(string) + 1;
	WCHAR* wide_string = malloc(sizeof(WCHAR) * string_size);
	mbstowcs(wide_string, string, string_size);

	return wide_string;
}