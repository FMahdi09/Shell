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