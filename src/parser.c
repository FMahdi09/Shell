#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "job.h"
#include "menial.h"
#include "my_vector.h"

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
        int foreground = 1;
        int infile = STDIN_FILENO;
        int outfile = STDOUT_FILENO;
        int make_process = 0;
        char** full_command = command;
        char** argv;
        my_vector* arguments = create_vector ();
        process* first_process = NULL;

        while (*command)
        {
                if (strcmp (*command, "|") == 0)
                {
                        if (arguments->cur_size == 0 ||
                            *(command + 1) == NULL)
                        {
                                fprintf (stderr, "invalid syntax\n");
                                free_vector (arguments);
                                free_process_list (first_process);
                                return NULL;
                        }

                        argv = copy_string_arr (arguments->argv, arguments->cur_size);
                        make_process = 1;
                }
                else if (strcmp (*command, ">") == 0    ||
                         strcmp (*command, "<") == 0)
                {
                        if (arguments->cur_size == 0            ||
                            *(command + 1) == NULL              ||
                            strcmp (*(command + 1), "|") == 0   ||
                            strcmp (*(command + 1), ">") == 0   ||
                            strcmp (*(command + 1), "<") == 0)
                        {
                                fprintf (stderr, "invalid syntax\n");
                                free_vector (arguments);
                                free_process_list (first_process);
                                return NULL;
                        }

                        int options;

                        if (strcmp (*command, "<") == 0)
                                options = O_RDONLY;
                        else if (strcmp (*command, ">") == 0)
                                options = O_CREAT | O_WRONLY | O_TRUNC;

                        int fd = open (*(command + 1), options, S_IRWXG | S_IRWXU);

                        if (fd == -1)
                        {
                                perror (*(command + 1));
                                free_vector (arguments);
                                free_process_list (first_process);
                                return NULL;
                        }

                        if (strcmp (*command, "<") == 0)
                                infile = fd;
                        else if (strcmp (*command, ">") == 0)
                                outfile = fd;

                        ++command;

                        if (*(command + 1) == NULL)
                        {
                                make_process = 1;
                                argv = copy_string_arr (arguments->argv, arguments->cur_size);
                        }
                }
                else if (*(command + 1) == NULL)
                {
                        if (strcmp (*command, "&") == 0)
                                foreground = 0;
                        else
                                add_string (arguments, *command);

                        if (arguments->cur_size == 0)
                        {
                                fprintf (stderr, "invalid syntax\n");
                                free_vector (arguments);
                                free_process_list (first_process);
                                return NULL;
                        }

                        argv = copy_string_arr (arguments->argv, arguments->cur_size);
                        make_process = 1;
                }
                else
                {
                        add_string (arguments, *command);
                }

                if (make_process)
                {
                        process* new_process = create_process (argv, outfile, infile);
                        first_process = add_process (first_process, new_process);
                        infile = STDIN_FILENO;
                        outfile = STDOUT_FILENO;
                        free_vector (arguments);
                        arguments = create_vector ();
                        make_process = 0;
                }

                ++command;
        }

        free_vector (arguments);
        
        char* command_string = string_from_arr (full_command);

        job* new_job = create_job (
                first_process,
                foreground,
                command_string
        );

        return new_job;
}