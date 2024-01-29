#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job.h"
#include "menial.h"

/*
trims all leading/trailing whitespaces
returns a substring of the original which must not be deallocated
if the original was allocated dynamically
*/
char* trim_whitespaces (char* str)
{
        char* tmp = str;

        while (isspace (*tmp))
                ++tmp;

        if (*tmp == '\0')
                return tmp;

        char* end = tmp + strlen (tmp) - 1;

        while (isspace (*end) && end > tmp)
                --end;

        end[1] = '\0';

        return tmp;
}

/*
splits a string into an array of words seperated by delim character
*/
char** parse_input_string (char* buffer)
{
        int count = 0;
        int whitespace = 0;
        char* trimmed = trim_whitespaces (buffer);
        char* tmp = trimmed;

        if (!*tmp)
                return NULL;

        while (*tmp)
        {
                if (*tmp == ' ' && whitespace == 0)
                {
                        whitespace = 1;
                        ++count;
                }
                else if (*tmp != ' ' && whitespace == 1)
                {
                        whitespace = 0;
                }

                ++tmp;
        }
        ++count;

        char** parsed = malloc (sizeof (char*) * (count + 1));

        if (parsed == NULL)
                return NULL;

        char* token = strtok (trimmed, " ");
        int index = 0;

        while (token != NULL)
        {
                parsed[index++] = strdup (token);
                token = strtok (NULL, " ");
        }

        parsed[index] = '\0';

        return parsed;
}

/*
takes in a null terminated array of cl arguments and constructs a job accordingly
returns NULL if the arguments are invalid
*/
job* parse_job (char** command)
{
        char** tmp = command;
        char** argv;
        int start = 0;
        int cur = start;
        int foreground = 1;
        int make_process = 0;
        process* first_process = NULL;

        while (*tmp)
        {
                if (strcmp (*tmp, "|") == 0)
                {
                        if (cur == start ||
                            *(tmp + 1) == NULL)
                        {
                                fprintf (stderr, "invalid syntax\n");
                                return NULL;
                        }

                        argv = copy_string_arr (command + start, cur - start);
                        start = cur + 1;
                        make_process = 1;
                }
                else if (strcmp (*tmp, ">") == 0)
                {
                        if (cur == start ||
                            *(tmp + 1) == NULL)
                        {
                                fprintf (stderr, "invalid syntax\n");
                                return NULL;
                        }

                        argv = copy_string_arr (command + start, cur - start);
                        make_process = 1;

                        
                }
                else if (*(tmp + 1) == NULL &&
                         strcmp (*tmp, "&") == 0)
                {
                        if (cur == start)
                        {
                                fprintf (stderr, "invalid syntax\n");
                                return NULL;
                        }

                        argv = copy_string_arr (command + start, cur - start);
                        foreground = 0;
                        make_process = 1;       
                }
                else if (*(tmp + 1) == NULL)
                {
                        argv = copy_string_arr (command + start, cur + 1 - start);
                        make_process = 1;
                }

                if (make_process)
                {
                        process* new_process = create_process (argv);
                        first_process = add_process (first_process, new_process);
                        make_process = 0;
                }

                ++cur;
                ++tmp;
        }

        char* command_string = string_from_arr (command);

        job* new_job = create_job (
                first_process,
                foreground,
                command_string
        );

        return new_job;
}