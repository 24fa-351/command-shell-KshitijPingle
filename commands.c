// Add a header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "parsing.h"
#include "commands.h"


void redirect_input(int input_fd)
{
    // Think of dup2 as a reverse assignment statement
    // Think of it like 'STDIN_FILENO = input_fd'
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
}

void redirect_output(int output_fd)
{
    // Think of dup2 as a reverse assignment statement
    // Think of it like 'STDOUT_FILENO = output_fd'
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);

}

void execute_command(char *words[], int input_fd, int output_fd)
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

void greater_than(char *words[], int input_fd, int output_fd)
{
    char absolute_path[1000];
    int command_index = 0;

    // Check if we need to redirect output
    for (int i = 0; words[i] != NULL; i++)
    {
        if (strcmp(words[i], ">") == 0)
        {
            printf("Command: '%s'\n", words[i - 1]);
            printf("Output file: '%s'\n", words[i + 1]);

            // <command> > <output_file>

            // Open the file for writing
            output_fd = open(words[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

            printf("Output file descriptor: %d\n", output_fd);

            command_index = i - 1;

            break;
        }
    }

    find_absolute_path(words[command_index], absolute_path);
    printf("Found absolute path: '%s'\n", absolute_path);

    // Execute the command
    int child_pid = fork();

    if (child_pid == 0)
    {
        if (output_fd != 0)
        {
            redirect_output(output_fd);
        }

        fprintf(stderr, "Executing command: '%s'\n", absolute_path);
        execve(absolute_path, words, NULL);
    }

    wait(NULL);
}