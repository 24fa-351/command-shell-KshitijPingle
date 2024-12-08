// Add header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// Note: All functions in this file are from the in person coding we did in class for the redirect assignment

void add_character_to_string(char *str, char c)
{
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

// splits string by spaces; adds a NULL into the array after the last word
void split(char *cmd, char *words[], char delimiter)
{
    int word_count = 0;
    char *next_char = cmd;
    char current_word[10000];
    strcpy(current_word, ""); // Initialize the string to be empty

    // strdup() : A way to allocate space for a string and also copy it. Equivalent to malloc and a strcpy

    while (*next_char != '\0')
    {
        if (*next_char == delimiter)
        {
            words[word_count++] = strdup(current_word); // Add the word, then zero it out
            strcpy(current_word, "");
        }
        else
        {
            add_character_to_string(current_word, *next_char);
        }
        ++next_char;
    }

    words[word_count++] = strdup(current_word);
    words[word_count] = NULL;
}

bool find_absolute_path(char *cmd, char *absolute_path)
{
    char *directories[1000];

    split(getenv("PATH"), directories, ':');

    // look in array until I find the paththing + cmd

    for (int i = 0; directories[i] != NULL; i++)
    {
        char path[1000];
        strcpy(path, directories[i]);
        add_character_to_string(path, '/');
        strcat(path, cmd);

        if (access(path, X_OK) == 0)
        {
            strcpy(absolute_path, path);
            return true;
        }
    }

    return false;
}
