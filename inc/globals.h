#ifndef GLOBALS
#define GLOBALS

#include "headers.h"

#ifndef EXTERN
#define EXTERN extern
#endif

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

/* FUNCTION DECLARATIONS */

void init_shell ();
void print_prompt ();

#endif