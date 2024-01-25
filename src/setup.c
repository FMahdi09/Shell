#include "headers.h"
#include "globals.h"

/* checks if the shell is running in interactive mode
// If it is:
//      - puts itself in the foreground
//      - ignores all interactive and job-control signals
//      - puts itself in its own process group
//      - grabs control of the terminal
*/
void initShell ()
{
        shell_terminal = STDIN_FILENO;
        shell_is_interactive = isatty(shell_terminal);

        if (shell_is_interactive)
        {
                while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
                        kill (shell_pgid, SIGTTIN);

                signal (SIGINT, SIG_IGN);
                signal (SIGQUIT, SIG_IGN);
                signal (SIGTSTP, SIG_IGN);
                signal (SIGTTIN, SIG_IGN);
                signal (SIGTTOU, SIG_IGN);
                signal (SIGCHLD, SIG_IGN);

                shell_pid = getpid ();
                if (setpgid (shell_pid, shell_pgid) < 0)
                {
                        perror ("Unable to put the shell into its own process group");
                        exit(1);      
                }

                tcsetpgrp (shell_terminal, shell_pgid);
        }
}