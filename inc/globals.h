#ifndef GLOBALS
#define GLOBALS

#include "headers.h"

#ifndef EXTERN
#define EXTERN extern
#endif

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

/* GLOBAL VARIABLES */

// FD of the standard input
EXTERN int shell_terminal;

// flag showing if we are in interactive mode
EXTERN int shell_is_interactive;

// process group id of the shell
EXTERN pid_t shell_pgid;

// process id of the shell
EXTERN pid_t shell_pid;

// name of user logged into the controlling terminal
EXTERN char user[MAX_BUF_LEN];

// system hostname
EXTERN char hostname[MAX_BUF_LEN];

// current working directory
EXTERN char cwd[MAX_BUF_LEN];

// head of linked list of active jobs
EXTERN job* first_job;

/* FUNCTION DECLARATIONS */

void init_shell ();
void print_prompt ();
void launch_process (process* p, pid_t pgid, int infile, int outfile, int errfile, int foreground);
void launch_job (job* j, int foreground);
void print_job_info (job* j, const char* status);
int mark_process_status (pid_t pid, int status);
void wait_for_job (job* j);
char** parse_input_string (char* buffer, const char delim);
char* trim_whitespaces (char* str);
job* parse_job (char** command);
void free_string_arr (char** arr);
char** copy_string_arr (char** arr, size_t size);
process* add_process (process* head, process* new);
process* create_process (char** argv);
void add_job (job* new_job);
void free_process_list (process* p);
void update_status ();
void update_jobs ();
void free_job (job* j);

#endif