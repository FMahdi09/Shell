#include "headers.h"
#include "globals.h"

/*
returns the active job with the indicated pgid
returns NULL if pgid is invalid
*/
job* find_job (pid_t pgid)
{
        job* j = first_job;

        while (j != NULL)
        {
                if (j->pgid == pgid)
                        return j;

                j = j->next;
        }

        return NULL;
}

/*
returns true if all processes in the job have stopped or completed
*/
int job_is_stopped (job* j)
{
        process* p = j->first_process;

        while (p != NULL)
        {
                if (!p->completed && !p->stopped)
                        return 0;

                p = p->next;
        }

        return 1;
}

/*
returns true if all processes in the job have completed
*/
int job_is_completed (job* j)
{
        process* p = j->first_process;

        while (p != NULL)
        {
                if (!p->completed)
                        return 0;

                p = p->next;
        }

        return 1;
}

/*
launches a complete job and puts it in the foreground if specified
*/
void launch_job (job* j, int foreground)
{
        process* p = j->first_process;
        int mypipe[2];
        int infile = j->stdin;
        int outfile;

        while (p != NULL)
        {
                if (p->next)
                {
                        if (pipe (mypipe) < 0)
                        {
                                perror ("pipe");
                                exit (1);
                        }
                        outfile = mypipe[1];
                }
                else
                {
                        outfile = j->stdout;
                }

                pid_t pid = fork ();
                if (pid == 0)
                {
                        launch_process (p, j->pgid, infile, outfile, j->stderr, foreground);
                }
                else if (pid < 0)
                {
                        perror ("fork");
                        exit (1);
                }
                else
                {
                        p->pid = pid;
                        if (shell_is_interactive)
                        {
                                if (j->pgid == 0)
                                {
                                        j->pgid = pid;
                                        setpgid(pid, j->pgid);
                                }
                        }
                }

                if (infile != STDIN_FILENO)
                        close (infile);
                if (outfile != STDOUT_FILENO)
                        close (outfile);
                
                infile = mypipe[0];
                p = p->next;
        }

        print_job_info (j, "launched");
}

/*
put job in foreground, if cont is set sends SIGCONT to the jobs process group
*/
void put_job_in_foreground (job* j, int cont)
{
        tcsetpgrp(shell_terminal, j->pgid);
}