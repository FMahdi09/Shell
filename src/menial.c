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
prints out job info to stdout
*/
void print_job_info (job* j, const char* status)
{
        printf("%ld (%s): %s\n", (long)j->pgid, status, j->command);
}