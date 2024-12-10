// Add a header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "parsing.h"
#include "commands.h"
#include "env_vars.h"

#define MAX_LINE 1000

int main(int argc, char *argv[])
{
    char line[MAX_LINE];
    EnvVars *env_vars = create_env_vars();
    // EnvVars *env_vars[100];

    while (1)
    {
        char s[100];
        bool loop_finished = false;
        char absolute_path[1000];
        char *words[1000];

        // Make sure commands are reset everytime 
        words[0] = NULL;

        char *current_dir = getcwd(s, 100);

        // BONUS: My WSL terminal has current_dir as blue, so I also added that
        printf("\033[34m%s", current_dir);
        printf("\033[0m my_shell>  ");
        // printf("my_shell>  ");
        fgets(line, MAX_LINE, stdin);

        // Remove the newline character from the end of the line
        line[strcspn(line, "\n")] = 0;

        printf("You entered: '%s'\n", line);

        split(line, words, ' ');

        for (int i = 0; words[i] != NULL; i++)
        {
            printf("words[%d] = '%s'\n", i, words[i]);
        }

        // Search for env vars with '$' and replace them with their values
        replace_env_vars(line, env_vars);

        // Default for now
        int input_fd = 1;
        int output_fd = 0;

        // Check if the user wants to exit the shell
        if ((strcmp(line, "exit") == 0) || (strcmp(line, "quit") == 0) || (strcmp(line, "q") == 0))
        {
            printf("Exiting my_shell\n");
            destroy_env_vars(env_vars);
            return 0;
        }

        if (strcmp(line, "\n") == 0)
        {
            continue;
        }

        // Check for special commands which I needed to implement
        for (int i = 0; words[i] != NULL; i++)
        {
            if (strcmp(words[i], "<") == 0)
            {
                less_than(words, input_fd, output_fd);
                loop_finished = true;
            }
            else if (strcmp(words[i], ">") == 0)
            {
                greater_than(words, input_fd, output_fd);
                loop_finished = true;
            }
            else if (strcmp(words[i], "cd") == 0)
            {
                // Change directory
                if (chdir(words[i + 1]) != 0)
                {
                    perror("chdir");
                }
                loop_finished = true;
            }
            else if (strcmp(words[i], "set") == 0)
            {
                // Check if words has enough arguments
                if ((words[i + 2] != NULL) && (words[i + 1] != NULL))
                {
                    set_env_var(env_vars, words[i + 1], words[i + 2]);
                    loop_finished = true;
                    printf("Set '%s' to '%s'\n", words[i + 1], words[i + 2]);
                }
                else
                {
                    printf("Not enough arguments for setting env variable\n");
                    loop_finished = true;
                    break;
                }
            }
            else if (strcmp(words[i], "unset") == 0)
            {
                unset_env_var(env_vars, words[i + 1]);
                loop_finished = true;
            }
        }

        if (loop_finished)
        {
            continue;
        }

        // If I don't have to do anything special for this command, then do it
        execute_command(words, input_fd, output_fd);
    }

    destroy_env_vars(env_vars);
    return 0;
}