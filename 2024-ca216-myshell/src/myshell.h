/*
    Name: Killian Daly
    Student number: 22739355
    I acknowledge DCU's academic integrity policy
*/

// Include guards, , ensuring that the contents are only included once in compilation
#ifndef MYSHELL_H
#define MYSHELL_H

// Standard C libraries included for various functionalities
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

// Defines the maximum size of the input buffer for storing command lines
#define MAX_BUFFER 1024
// Defines the maximum number of arguments that can be passed to a command
#define MAX_ARGS 64
//  Defines the characters used to separate arguments in a command line
#define SEPARATORS " \t\n"

// Handles internal shell commands
int internalCommands(char **args);
// Handles external shell commands
void externalCommands(char **args);
// Processes the command line input by the user
void processCmdLine(char *line);

// End of the include guards
#endif /* MYSHELL_H */
