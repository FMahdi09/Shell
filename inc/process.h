#ifndef PROCESS
#define PROCESS

#include <sys/types.h>

/* STRUCTS */

typedef struct process
{
        struct process* next;
        char** argv;
        pid_t pid;
        int completed;
        int stopped;
        int status;
} process;

/* FUNCTION DECLARATIONS */

void launch_process (process* p, pid_t pgid, int infile, int outfile, int errfile, int foreground);
int mark_process_status (pid_t pid, int status);
process* add_process (process* head, process* new);
process* create_process (char** argv);
void free_process_list (process* p);

#endif