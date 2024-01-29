#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "globals.h"
#include "job.h"

/* 
prints the shell prompt to standard output
format: <user>@<hostmachine>:<working_directory>€
*/
void print_prompt ()
{
        printf("%s@%s:%s€ ", user, hostname, cwd);
}

/*
deallocates memory of a null terminated string array
*/
void free_string_arr (char** arr)
{
        char** tmp = arr;

        while (*tmp)
        {
                free (*tmp);
                ++tmp;
        }

        free (arr);
}

/*
deallocates memory of a string array with given size
*/
void free_string_arr_s (char** arr, size_t size)
{
        for (int i = 0; i < size; ++i)
        {
                free (arr[i]);
        }

        free (arr);
}

/*
returns a copy of a string array and null terminates it
*/
char** copy_string_arr (char** arr, size_t size)
{
        char** copy = malloc (sizeof (char*) * (size + 1));

        for (int i = 0; i < size; ++i)
                copy[i] = strdup (arr[i]);

        copy[size] = NULL;

        return copy;
}

/*
concats a null terminated array of strings into a single string
seperates strings with spaces
*/
char* string_from_arr (char** arr)
{
        int length = 0;
        char** tmp_arr = arr;

        while (*tmp_arr != NULL)
        {
                length += strlen (*tmp_arr) + 1;
                ++tmp_arr;
        }

        char* result = malloc (sizeof (char) * (length + 1));
        char* tmp_result = result;
        tmp_arr = arr;

        while (*tmp_arr != NULL)
        {
                strcpy (tmp_result, *tmp_arr);
                tmp_result[strlen (*tmp_arr)] = ' ';
                tmp_result += strlen (*tmp_arr) + 1;

                ++tmp_arr;
        }
        result[length] = '\0';

        return result;
}