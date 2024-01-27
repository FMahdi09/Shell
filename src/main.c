#define EXTERN

#include "headers.h"
#include "globals.h"

int main (int argc, char* argv[])
{
        int done = 0;
        char* buffer = NULL;
        size_t char_read, size = 0;
        first_job = NULL;

        init_shell ();

        while (!done)
        {
                print_prompt ();

                char_read = getline (&buffer, &size, stdin);

                if (char_read < 0)
                {
                        perror ("getline");
                        return 1;
                }

                buffer[char_read - 1] = '\0';

                char** command = parse_input_string (buffer, ' ');

                if (command != NULL && strcmp (command[0], "exit") == 0)
                {
                        done = 1;
                        free_string_arr (command);
                }
                else if (command != NULL)
                {
                        job* new_job = parse_job (command);

                        if (new_job != NULL)
                        {
                                add_job (new_job);
                                launch_job (new_job, new_job->foreground);
                        }

                        free_string_arr (command);
                }

                update_jobs ();
        }
}