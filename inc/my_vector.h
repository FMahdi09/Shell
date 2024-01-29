#ifndef VECTOR
#define VECTOR

#include <sys/types.h>

/* STRUCTS */

typedef struct my_vector
{
        char** argv;
        size_t max_size;
        size_t cur_size;
} my_vector;

/* MACROS */

#define VECTOR_DEFAULT_SIZE 4

/* FUNCTION DECLARATIONS */

my_vector* create_vector ();
void free_vector (my_vector* v);
void add_string (my_vector* v, char* str);

#endif