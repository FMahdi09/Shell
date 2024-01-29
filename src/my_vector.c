#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "my_vector.h"
#include "menial.h"

/*
allocates and initializes a my_vector with default values
*/
my_vector* create_vector ()
{
        my_vector* new_vector = malloc (sizeof (my_vector));
        new_vector->argv = calloc (VECTOR_DEFAULT_SIZE, sizeof (char*));
        new_vector->cur_size = 0;
        new_vector->max_size = VECTOR_DEFAULT_SIZE;

        return new_vector;
}

/*
frees allocated memory of a my_vector struct
*/
void free_vector (my_vector* v)
{
        free_string_arr_s (v->argv, v->max_size);
        free (v);
}

/*
adds a null terminated string to the stored array
*/
void add_string (my_vector* v, char* str)
{
        if (v->cur_size >= v->max_size)
        {
                v->max_size = v->max_size * 2;
                v->argv = realloc (v->argv, v->max_size);
        }

        v->argv[v->cur_size++] = strdup (str);
}