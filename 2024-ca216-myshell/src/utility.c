/*
    Name: Killian Daly
    Student number: 22739355
    I acknowledge DCU's academic integrity policy
*/

// Standard C libraries included for various functionalities
#include "myshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Include for open() function
#include <fcntl.h>

// The purpose of this function is to handle internal commands
int internalCommands(char **args) {
    // Check for input/output redirection
    int i = 0;
    const char *inpFile = NULL;
    const char *outFile = NULL;
    // Create variables for file descriptors
    int inpDesc, outDesc;

    // Iterate over args looking for redirection symbols for input and output
    while(args[i] != NULL) {
        // If redirect input symbol is present, we set file for stdin to next argument and remove redirect symbol from args
        // The terminal recognises the < character and handles all further logic for stdin redirection
        if(strcmp(args[i], "<") == 0) {
            inpFile = args[i + 1];
            // Remove redirection symbols from args
            args[i] = NULL;
            args[i + 1] = NULL;
        } 
        // If redirect output symbol is present, we set file for stdout to next argument and remove redirect symbol from args
        // The terminal handles truncate and concatenate behaviour for > and >> so no further logic is needed
        else if(strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
            outFile = args[i + 1];
            // Remove redirection symbols from args
            args[i] = NULL;
            args[i + 1] = NULL;
        } 
        i++;
    }

    // Redirect stdin to file if we have found stdin file to create
    if(inpFile) {
        // Open the input file for reading
        inpDesc = open(inpFile, O_RDONLY);
        if(inpDesc == -1) {
            // Print error message if file cannot be opened
            perror("open input file");
            return 1;
        }

        // Redirect stdin to the input file
        dup2(inpDesc, STDIN_FILENO);
        // Close the file descriptor after redirection
        close(inpDesc);
    }

    // Redirect stdout to file if we have found stdout file to create
    if(outFile) {
        // Open the output file for writing, creating if it doesn't exist, and truncate if it does
        outDesc = open(outFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        // If file value is -1 then it does not exist and we print error
        if(outDesc == -1) {
            perror("open output file");
            return 1;
        }

        // Redirect stdout to the output file
        dup2(outDesc, STDOUT_FILENO);
        // Close the file descriptor after redirection
        close(outDesc);
    }

    // Perform internal commands, by going through series of if and else if statements to if command matches any
    // For each command, we perform string comparison to see if there are 0 differences between the input and internal command
    // We return 1 to indicate an internal command has been found and executed
    if(strcmp(args[0], "clr") == 0) {
        // Clear the terminal screen
        system("clear");
        return 1;
    } 
    else if(strcmp(args[0], "quit") == 0) {
        // Exit the shell with success status
        exit(EXIT_SUCCESS);
    } 
    else if(strcmp(args[0], "dir") == 0) {
        // List files and directories in long format
        system("ls -al");
        return 1;
    } 
    else if(strcmp(args[0], "echo") == 0) {
        // Print the argument following the echo command
        printf("%s\n", args[1]);
        return 1;
    } 
    else if(strcmp(args[0], "environ") == 0) {
        // Display environment variables
        system("env");
        return 1;
    } 
    else if (strcmp(args[0], "cd") == 0) {
        // Handle change directory command
        // If argument present then attempt to change directory
        if(args[1]) {

            if(chdir(args[1]) != 0) {
                // Report error if directory change fails
                perror("cd");
            } 
            else {
                // Update PWD environment variable
                setenv("PWD", args[1], 1);
                // Get the current working directory
                char cwd[1024];
                if(getcwd(cwd, sizeof(cwd)) == NULL) {
                    // Report error if getting current directory fails
                    perror("getcwd");
                    return 1;
                }

                // Update shell environment variable with the current directory
                if(setenv("shell", cwd, 1) == -1) {
                    // Report error if setting shell variable fails
                    perror("setenv");
                    return 1;
                }
            }
        } 
        // If no argument present then get the current working directory
        else {
            char cwd[1024];
            if(getcwd(cwd, sizeof(cwd)) != NULL) {
                // Print the current directory
                printf("Current directory: %s\n", cwd);
            } 
            else {
                // Report error if getting current directory fails
                perror("getcwd");
            }
        }
        return 1;
    } 
    else if(strcmp(args[0], "help") == 0) {
        // Print help message from readme file, per spec we can assume it is in current working directory
        system("more ../manual/readme.md");
        return 1;
    } 
    else if(strcmp(args[0], "pause") == 0) {
        // Pause execution until enter key is pressed by user by checking for newline character
        while(getchar() != '\n');
        return 1;
    }

    // Return 0 if the command is not an internal command
    return 0;
}

// Function to handle external commands
void externalCommands(char **args) {
    // Check if the last argument is & character
    // Flag to indicate if the command should run in the background
    int background = 0; 
    int i = 0;

    // Iterate through the arguments to find the end of the argument list
    while (args[i] != NULL) {
        i++;
    }

    // Check if the last argument is "&"
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        // Set background flag to true
        background = 1; 
        // Remove "&" from the arguments list
        args[i - 1] = NULL; 
    }

    // Fork and execute the command
    pid_t pid;
    switch (pid = fork()) {
        case -1:
            // Print error message if fork fails
            perror("fork"); 
            break;
        // Child process code block
        case 0: 
            // Execute the command
            execvp(args[0], args);
            // Print error message if exec fails
            perror("exec");
            // Exit child process with failure status
            exit(EXIT_FAILURE);
        // Parent process
        default:
            if (!background) {
                // Wait for child process to finish
                waitpid(pid, NULL, 0);
            }
    }
}

// Function to process command line input
void processCmdLine(char *line) {
    // Allocate memory for the arguments array
    char **args = malloc(MAX_ARGS * sizeof(char*));

    if (args == NULL) {
        // Print error if memory allocation fails
        perror("malloc"); 
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, SEPARATORS);
    int count = 0;

    // Tokenize the input line and store tokens in the arguments array
    while (token != NULL) {
        args[count] = strdup(token);
        if (args[count] == NULL) {
            // Print error if string duplication fails
            perror("strdup"); 
            exit(EXIT_FAILURE);
        }
        count++;
        token = strtok(NULL, SEPARATORS);
    }
    args[count] = NULL;

    // Check if the command is an internal command, otherwise execute external command
    if(args[0]) {
        if(!internalCommands(args)) {
            externalCommands(args);
        }
    }

    // Free memory allocated for arguments
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
}
