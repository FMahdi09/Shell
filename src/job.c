#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "job.h"

/*
prints out job info to standard output
format: <process group id> (<status>): <command>
*/
void print_job_info (job* j, const char* status)
{
        printf("%ld (%s): %s\n", (long)j->pgid, status, j->command);
}

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
deallocates all memory associated with a job
*/
void free_job (job* j)
{
        free_process_list (j->first_process);
        free (j->command);
        free (j);
}

/*
waits for job until all processes have either completed or stopped
*/
void wait_for_job (job* j)
{
        int status;
        pid_t pid;

        do
        {
                pid = waitpid (-1, &status, WUNTRACED);
        } while (!mark_process_status (pid, status)
                 && !job_is_completed (j)
                 && !job_is_stopped (j));
        
}

/*
put job in foreground, if cont is set sends SIGCONT to the jobs process group
*/
void put_job_in_foreground (job* j, int cont)
{
        tcsetpgrp (shell_terminal, j->pgid);

        wait_for_job (j);

        tcsetpgrp (shell_terminal, shell_pgid);
}


/*
launches a complete job and puts it in the foreground if specified
*/
void launch_job (job* j)
{
        process* p = j->first_process;
        int mypipe[2];

        while (p != NULL)
        {
                if (p->next)
                {
                        if (pipe (mypipe) < 0)
                        {
                                perror ("pipe");
                                exit (EXIT_FAILURE);
                        }

                        if (p->std_out == STDOUT_FILENO)
                        {
                                p->std_out = mypipe[1];
                        }
                        else
                        {
                                close (mypipe[1]);
                        }
                }

                pid_t pid = fork ();
                if (pid == 0)
                {
                        launch_process (p, j->pgid, j->foreground);
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
                                        j->pgid = pid;

                                setpgid (pid, j->pgid);
                        }
                }

                if (p->std_in != STDIN_FILENO)
                        close (p->std_in);
                if (p->std_out != STDOUT_FILENO)
                        close (p->std_out);
                
                p = p->next;
                
                if (p != NULL && p->std_in == STDIN_FILENO)
                {
                        p->std_in = mypipe[0];
                }
        }

        if(j->foreground)
                put_job_in_foreground (j, 0);
}

/*
adds job to the end of jobs linked list
*/
void add_job (job* new_job)
{
        if (first_job == NULL)
        {
                first_job = new_job;
                return;
        }

        job* tmp = first_job;

        while (tmp != NULL)
        {
                if (tmp->next == NULL)
                {
                        tmp->next = new_job;
                        return;
                }
                tmp = tmp->next;
        }
}

/*
removes a specific job from the list
*/
void remove_job (job* j)
{
        job* tmp = first_job;
        job* before_tmp = NULL;

        if (j == first_job)
        {
                first_job = first_job->next;
                free_job (j);
                return;
        }

        while (tmp != j)
        {
                before_tmp = tmp;
                tmp = tmp->next;
        }

        before_tmp->next = tmp->next;
        free_job (j);
}

/*
non-blocking check if any processes have information available
*/
void update_status ()
{
        int status;
        pid_t pid;

        do
        {
                pid = waitpid (-1, &status, WUNTRACED | WNOHANG);
        } while (!mark_process_status (pid, status));
}

/*
iterates over all jobs and checks if any processes have stopped or completed
updates status and notifies user
*/
void update_jobs ()
{
        update_status ();

        job* tmp = first_job;

        while (tmp != NULL)
        {
                if (job_is_completed (tmp))
                {
                        print_job_info (tmp, "completed");
                        job* to_remove = tmp;
                        tmp = tmp->next;
                        remove_job (to_remove);
                }
                else if (job_is_stopped (tmp) && !tmp->notified)
                {
                        print_job_info (tmp, "stopped");
                        tmp->notified = 1;
                        tmp = tmp->next;
                }
                else
                {
                        tmp = tmp->next;
                }
        }
}

/*
allocates and returns a job with provided values
the command argument needs to point to an allocted string 
which must not be manually deallocated after creating the job
*/
job* create_job (process* first_process, int foreground, char* command)
{
        job* new_job = malloc (sizeof (job));
        new_job->next = NULL;
        new_job->first_process = first_process;
        new_job->foreground = foreground;
        new_job->command = command;
        new_job->pgid = 0;

        return new_job;
}