#ifndef PARSING_H
#define PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void add_character_to_string(char *str, char c);

void split(char *cmd, char *words[], char delimiter);

bool find_absolute_path(char *cmd, char *absolute_path);

void execute_commands(char *words[], int input_fd, int output_fd);

#endif // PARSING_H
