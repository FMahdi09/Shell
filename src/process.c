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

/*
sets the status of given process
*/
int mark_process_status (pid_t pid, int status)
{
        job* j = first_job;
        process* p;

        while (j != NULL)
        {
                p = j->first_process;
                while (p != NULL)
                {
                        if (p->pid == pid)
                        {
                                p->status = status;

                                if (WIFSTOPPED (status))
                                        p->stopped = 1;
                                else
                                        p->completed = 1;

                                return 0;
                        }
                        p = p->next;
                }
                j = j->next;
        }

        return -1;
}

/*
adds process to the end of linked list
*/
process* add_process (process* head, process* new)
{
        if (head == NULL)
                return new;

        process* tmp = head;

        while (tmp != NULL)
        {
                if (tmp->next == NULL)
                {
                        tmp->next = new;
                        return head;
                }

                tmp = tmp->next;
        }

        return head;
}

/*
creates a new process with next set to null and argv set to provided string array
*/
process* create_process (char** argv)
{
        process* new_process = malloc (sizeof (process));
        new_process->next = NULL;
        new_process->argv = argv;
        new_process->completed = 0;
        new_process->stopped = 0;

        return new_process;
}

/*
deallocates all memory associated with a list of processes
*/
void free_process_list (process* p)
{
        process* tmp;

        while (p != NULL)
        {
                tmp = p;
                p = p->next;

                free_string_arr (tmp->argv);
                free (tmp);
        }
}