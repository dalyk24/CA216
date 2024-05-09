/*
    Name: Killian Daly
    Student number: 22739355
    I acknowledge DCU's academic integrity policy
*/

// Standard C libraries included for various functionalities
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "myshell.h"

// Function prototype
void displayPrompt();

int main(int argc, char **argv) {
    char cwd[1024];
    // Get current working directory and store in cwd
    if(getcwd(cwd, sizeof(cwd)) == NULL) {
        // Print error if getting current directory is unsuccessful
        perror("getcwd");
        return 1;
    }

    // Set the shell environment variable to the current directory
    if(setenv("shell", cwd, 1) == -1) {
        // Print error if setting shell variable is unsuccessful
        perror("setenv");
        return 1;
    }

    // Per the spec, two arguments will always be a batchfile so we enter batch file mode
    if(argc == 2) {
        // Open the batch file in read mode to, funny enough, read commands
        FILE *batchfile = fopen(argv[1], "r");
        if(batchfile == NULL) {
            // Print error if opening  is unsuccessful
            perror("fopen");
            return 1;
        }

        char buf[MAX_BUFFER];

        // Iterate through lines lines from the batch file, removing newline characters as we go
        while(fgets(buf, MAX_BUFFER, batchfile)) {
            if(buf[strlen(buf) - 1] == '\n') {
                // Null terminate line to avoid going off the end
                buf[strlen(buf) - 1] = '\0';
            }
            // Process command on line
            processCmdLine(buf);
        }

        // Close batch file
        fclose(batchfile);

        return 0;
    }
    else {
        // If ther are command line arguments, we iterate through them processing each command
        if(argc > 2) {
            for(int i = 1; i < argc; i++) {
                processCmdLine(argv[i]);
            }
        }

        // If there are no arguments, present command prompt for user input
        char buf[MAX_BUFFER];
        char *args[MAX_ARGS];
        char **arg;

        while(1) {
            // Print prompt before reading user input
            displayPrompt();

            if(fgets(buf, MAX_BUFFER, stdin)) {
                // Check for newline character, remove it and null terminate string
                if(buf[strlen(buf) - 1] == '\n') {
                    buf[strlen(buf) - 1] = '\0';
                }

                arg = args;
                *arg++ = strtok(buf, SEPARATORS);

                // Tokenize the input line and store tokens in args array
                while((*arg++ = strtok(NULL, SEPARATORS)));

                if(args[0]) {
                    // If the command is not internal, we check for & background execution character
                    if(!internalCommands(args)) {
                        int background = 0;
                        int i = 0;

                        // Store args in array, if & is present then we remove it and set background mode
                        while(args[i] != NULL) {
                            i++;
                        }
                        if(i > 0 && strcmp(args[i - 1], "&") == 0) {
                            // Setting background mode and removing & character
                            background = 1;
                            args[i - 1] = NULL;
                        }

                        // Create variable for process ID and store ID of child process for I/O redirection
                        pid_t pid;
                        switch(pid = fork()) {
                            // If process ID is -1 then fork fails
                            case -1:
                                perror("fork");
                                break;
                            // Execute the command and print error message if exec fails
                            case 0:
                                execvp(args[0], args);
                                perror("exec");
                                // Exit child process with failure status
                                exit(EXIT_FAILURE); 
                            default:
                                // If processes not exeucting in background mode, we wait for child process to finish 
                                if(!background) {
                                    waitpid(pid, NULL, 0);
                                } 
                                else {
                                    // Execute in background mode by not waiting for child process to finish
                                    printf("[%d] Running in background\n", pid);
                                }
                        }
                    }
                }
            } 
            else {
                // Exit loop if end of file is reached
                break;
            }
        }
    }

    return 0;
}

// Function to display prompt
void displayPrompt() {
    // Check if stdout is connected to a terminal to avoid printing prompts to output files needlessly
    if(isatty(fileno(stdout))) {
        char cwd[1024];

        // Get current directory to display in prompt
        if(getcwd(cwd, sizeof(cwd)) == NULL) {
            // Print error if unable to get current directory and exit
            perror("getcwd");
            exit(EXIT_FAILURE);
        }
        // Print prompt with current working directory
        printf("%s==>", cwd);
    }
}
