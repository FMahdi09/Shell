#include "headers.h"
#include "globals.h"

/*
launches the given process and sets std in, out and err
if shell is in interactive mode:
        - sets the pgid
        - puts it into the foreground if specified
*/
void launch_process (process* p, pid_t pgid, int infile, int outfile, int errfile, int foreground)
{
        if (shell_is_interactive)
        {
                pid_t pid = getpid ();
                if (pgid == 0)
                        pgid = pid;

                setpgid (pid, pgid);
                if (foreground)
                        tcsetpgrp (shell_terminal, pgid);

                signal (SIGINT, SIG_DFL);
                signal (SIGQUIT, SIG_DFL);
                signal (SIGTSTP, SIG_DFL);
                signal (SIGTTIN, SIG_DFL);
                signal (SIGTTOU, SIG_DFL);
                signal (SIGCHLD, SIG_DFL);
        }

        if (infile != STDIN_FILENO)
        {
                dup2 (infile, STDIN_FILENO);
                close (infile);
        }
        if (outfile != STDOUT_FILENO)
        {
                dup2 (outfile, STDOUT_FILENO);
                close (outfile);
        }
        if (errfile != STDERR_FILENO)
        {
                dup2 (errfile, STDERR_FILENO);
                close (errfile);
        }

        execvp (p->argv[0], p->argv);
        perror ("launch process execvp");
        exit (1);
}