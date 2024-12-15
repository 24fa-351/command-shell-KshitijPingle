#!/bin/bash

# Author information
#   Author name : Kshitij Pingle
#   Author email: kpingle@csu.fullerton.edu
#   Author section: 351-13
#   Author CWID : 885626978 

# Program Description: Shell file to compile the program


# compile the program
gcc -o my_shell my_shell.c parsing.c commands.c env_vars.c
