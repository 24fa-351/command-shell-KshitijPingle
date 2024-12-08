#ifndef COMMANDS_H
#define COMMANDS_H


void redirect_input(int input_fd);

void redirect_output(int output_fd);

void execute_command(char *words[], int input_fd, int output_fd);

void greater_than(char *words[], int input_fd, int output_fd);

#endif // COMMANDS_H
