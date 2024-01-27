#ifndef MENIAL
#define MENIAL

#include <stddef.h>

/* FUNCTION DECLARATIONS */

void print_prompt ();
void free_string_arr (char** arr);
char** copy_string_arr (char** arr, size_t size);
char* string_from_arr (char** arr);

#endif