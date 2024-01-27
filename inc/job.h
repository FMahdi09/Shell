#ifndef JOB
#define JOB

#include <sys/types.h>

#include "process.h"

/* STRUCTS */

typedef struct job
{
        struct job* next;
        char* command;
        process* first_process;
        pid_t pgid;
        int notified;
        int foreground;
        int stdin;
        int stdout;
        int stderr;
} job;

/* FUNCTION DECLARATIONS */

void launch_job (job* j, int foreground);
void add_job (job* new_job);
void print_job_info (job* j, const char* status);
void update_jobs ();
job* create_job (process* first_process, int in, int out, int err, int foreground, char* command);

#endif