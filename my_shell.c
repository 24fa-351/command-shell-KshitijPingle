// Add a header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1000
int main(int argc, char *argv[])
{
    char line[MAX_LINE];
    while (1)
    {
        printf("my_shell> ");
        fgets(line, MAX_LINE, stdin);

        // Check if the user wants to exit the shell
        if ((strcmp(line, "exit\n") == 0) || (strcmp(line, "quit\n") == 0))
        {
            printf("Exiting my_shell\n");
            break;
        }

        // Remove the newline character from the end of the line
        line[strcspn(line, "\n")] = 0;

        printf("You entered: '%s'\n", line);
    }

    return 0;
}