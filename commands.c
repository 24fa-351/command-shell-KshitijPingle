// Add a header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "env_vars.h"
#include "parsing.h"
#include "commands.h"


void redirect_input(int input_fd)
{
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
}

void redirect_output(int output_fd)
{
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
}

void execute_command(char *words[], int input_fd, int output_fd, EnvVars *env_vars)
{
    char absolute_path[1000];

    for (int i = 0; words[i] != NULL; i++)
    {
        if (find_absolute_path(words[i], absolute_path))
        {
            //printf("Found absolute path: '%s'\n", absolute_path);
            break;
        }
    }

    // Check for special commands which I needed to implement
    for (int i = 0; words[i] != NULL; i++)
    {
        if (strcmp(words[i], "<") == 0)
        {
            less_than(words, input_fd, output_fd);
            return;
        }
        else if (strcmp(words[i], ">") == 0)
        {
            greater_than(words, input_fd, output_fd);
            return;
        }
        else if (strcmp(words[i], "cd") == 0)
        {
            // Change directory
            if (chdir(words[i + 1]) != 0)
            {
                perror("chdir");
            }
            return;
        }
        else if (strcmp(words[i], "set") == 0)
        {
            // Check if words has enough arguments
            if ((words[i + 2] != NULL) && (words[i + 1] != NULL))
            {
                set_env_var(env_vars, words[i + 1], words[i + 2]);
                printf("Set '%s' to '%s'\n", words[i + 1], words[i + 2]);
                return;
            }
            else
            {
                printf("Error: Not enough arguments for setting env variable\n");
                printf("Usage: set <key> <value>\n");
                return;
            }
        }
        else if (strcmp(words[i], "unset") == 0)
        {
            unset_env_var(env_vars, words[i + 1]);
            return;
        }
    }

    // Execute the command
    int child_pid = fork();

    if (child_pid == 0)
    {

        if (output_fd != 0)
        {
            redirect_output(output_fd);
        }

        //fprintf(stderr, "Executing command: '%s'\n", absolute_path);
        execve(absolute_path, words, NULL);
    }

    wait(NULL);

}

void greater_than(char *words[], int input_fd, int output_fd)
{
    // <command> [args] > <output_file>

    char absolute_path[1000];
    int command_index = 0;

    // Check if we need to redirect output
    for (int i = 0; words[i] != NULL; i++)
    {
        if (strcmp(words[i], ">") == 0)
        {
            //printf("Command: '%s'\n", words[i - 1]);
            //printf("Output file: '%s'\n", words[i + 1]);

            // <command> > <output_file>

            // Open the file for writing
            output_fd = open(words[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

            //printf("Output file descriptor: %d\n", output_fd);

            command_index = i - 1;

            break;
        }
    }

    find_absolute_path(words[command_index], absolute_path);
    //printf("Found absolute path: '%s'\n", absolute_path);

    int child_pid = fork();

    if (child_pid == 0)
    {
        if (output_fd != 0)
        {
            redirect_output(output_fd);
        }

        char *command[100];
        for (int i = 0; i < command_index; i++)
        {
            // Everything before the '>' is the command + arguments
            command[i] = words[i];
        }

        //fprintf(stderr, "Executing command: '%s'\n", absolute_path);
        for (int i = 0; command[i] != NULL; i++)
        {
            fprintf(stderr, "Command[%d] = '%s'\n", i, command[i]);
        }
        execve(absolute_path, command, NULL);
    }

    wait(NULL);
}

void less_than(char *words[], int input_fd, int output_fd) {
    // <command> [args] < <input_file>

    char absolute_path[1000];
    int command_index = 0;

    // Check if we need to redirect input
    for (int i = 0; words[i] != NULL; i++)
    {
        if (strcmp(words[i], "<") == 0)
        {
            //printf("Command: '%s'\n", words[i - 1]);
            //printf("Output file: '%s'\n", words[i + 1]);

            // <command> > <output_file>

            // Open the file for reading
            input_fd = open(words[i + 1], O_RDONLY);

            //printf("Output file descriptor: %d\n", output_fd);

            command_index = i - 1;

            break;
        }
    }

    find_absolute_path(words[command_index], absolute_path);
    //printf("Found absolute path: '%s'\n", absolute_path);

    int child_pid = fork();

    if (child_pid == 0)
    {
        if (input_fd != 0)
        {
            redirect_input(input_fd);
        }

        char *command[100];
        for (int i = 0; i < command_index; i++)
        {
            // Everything before the '<' is the command + arguments
            command[i] = words[i];
        }

        //fprintf(stderr, "Executing command: '%s'\n", absolute_path);
        for (int i = 0; command[i] != NULL; i++)
        {
            fprintf(stderr, "Command[%d] = '%s'\n", i, command[i]);
        }
        execve(absolute_path, command, NULL);
    }

    wait(NULL);
}