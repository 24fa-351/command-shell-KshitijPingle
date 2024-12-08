// Add a header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

#include "parsing.h"
#include "commands.h"


void execute_commands(char *words[], int input_fd, int output_fd)
{
    char absolute_path[1000];

    for (int i = 0; words[i] != NULL; i++)
    {
        if (find_absolute_path(words[i], absolute_path))
        {
            printf("Found absolute path: '%s'\n", absolute_path);
            break;
        }
    }

    // Execute the command
    int child_pid = fork();

    if (child_pid == 0)
    {

        if (output_fd != 0)
        {
            // Think of dup2 as a reverse assignment statement
            // Think of it like 'STDOUT_FILENO = output_fd'
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        fprintf(stderr, "Executing command: '%s'\n", absolute_path);
        execve(absolute_path, words, NULL);
    }

    wait(NULL);

}