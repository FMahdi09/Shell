#include "headers.h"
#include "globals.h"

/* 
prints the shell prompt to standard output
format: <user>@<hostmachine>:<working_directory>€
*/
void print_prompt ()
{
        printf("%s@%s:%s€ ", user, hostname, cwd);
}

/*
prints out job info to standard output
format: <process group id> (<status>): <command>
*/
void print_job_info (job* j, const char* status)
{
        printf("%ld (%s): %s\n", (long)j->pgid, status, j->command);
}

/*
deallocates memory of an array of strings which is null terminated
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
returns a copy of string array and null terminates it
*/
char** copy_string_arr (char** arr, size_t size)
{
        char** copy = malloc (sizeof (char*) * (size + 1));

        for (int i = 0; i < size; ++i)
                copy[i] = strdup (arr[i]);

        copy[size] = NULL;

        return copy;
}
