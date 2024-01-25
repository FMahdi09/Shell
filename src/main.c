#define EXTERN

#include "headers.h"
#include "globals.h"

int main (int argc, char* argv[])
{
        init_shell ();

        process my_process =
        {
                .next = NULL,
                .argv = (char* []){"ls"}
        };

        job my_job =
        {
                .next = NULL,
                .first_process = &my_process,                
                .stderr = STDERR_FILENO,
                .stdin = STDIN_FILENO,
                .stdout = STDOUT_FILENO,
                .pgid = 0,
                .command = "ls"
        };

        launch_job(&my_job, 1);
}