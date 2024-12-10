#ifndef COMMANDS_H
#define COMMANDS_H

#include "env_vars.h"

void redirect_input(int input_fd);

void redirect_output(int output_fd);

void execute_command(char *words[], int input_fd, int output_fd, EnvVars *env_vars);

void greater_than(char *words[], int input_fd, int output_fd);

void less_than(char *words[], int input_fd, int output_fd);

#endif // COMMANDS_H
