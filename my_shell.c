// Add a header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"

#define MAX_LINE 1000
int main(int argc, char *argv[])
{
    char line[MAX_LINE];
    while (1)
    {
        printf("my_shell> ");
        fgets(line, MAX_LINE, stdin);

        // Default for now
        int input_fd = 1;
        int output_fd = 0;

        // Check if the user wants to exit the shell
        if ((strcmp(line, "exit\n") == 0) || (strcmp(line, "quit\n") == 0))
        {
            printf("Exiting my_shell\n");
            break;
        }

        if (strcmp(line, "\n") == 0)
        {
            continue;
        }

        // Remove the newline character from the end of the line
        line[strcspn(line, "\n")] = 0;

        printf("You entered: '%s'\n", line);

        char absolute_path[1000];
        char *words[1000];

        split(line, words, ' ');

        for (int i = 0; words[i] != NULL; i++)
        {
            printf("words[%d] = '%s'\n", i, words[i]);
        }

        // Look if we got any valid commands
        execute_commands(words, input_fd, output_fd);
        
    }

    return 0;
}