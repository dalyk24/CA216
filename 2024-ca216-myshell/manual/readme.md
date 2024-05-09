# CA216 Operating Systems: myshell

## Description
The myshell project is a command-line shell program, which allows users to enter commands into a terminal prompt and execute them by pressing Enter. It provides a set of internal commands and supports external commands.

The project specification can be found at: https://loop.dcu.ie/mod/assign/view.php?id=2326066


## Commands
"cd":   
    Change the current directory to argument. If the argument is not present, reports the current directory.

"clr":       
    Clear the screen.

"dir":     
    List the contents of a directory e.g. ls <directory> will list the contents of the given directory.

"environ":  
    List all the environment strings.

"echo":
    Echo on the display followed by a new line.

"help": 
    Display this user manual.

"pause": 
    Pause operation of the shell until 'Enter' is pressed.

"quit": 
    Quit the shell.

External commands:
    The shell supports execution of external commands within the system e.g. pwd will print the current working directory.


## Environment
The myshell program creates the "shell" environment variable and updates the "PWD" environment variable, with both variables containing the current working directory. The command environ can get all the environment variables.


## Input/Output Redirection
There is support for input and output redirection of commands within myshell.

"<": 
    Redirects standard input to a file e.g. "< input.txt" will execute all input.txt commands then exit.

">": 
    Redirects standard output to a file, creating the file if it doesn't exist and overwriting if it does e.g. "> output.txt" will redirect the output of the pwd command to output.txt.

">>": 
    Redirects standard output to a file, creating the file if it doesn't exist and appending if it does e.g. ">> output.txt" will append the output of the pwd command to output.txt.


## Background and Foreground Execution
Foreground execution:
    If a command line does not end with the "&" character then preceding commands are executed in the foreground. So, the program waits for the command to execute before prompting the user for another command.

Background execution:
    If a command line ends with the "&" character then preceding commands are executed in the background. So, the user can continue entering commands while the previous command executes.


## Sources
Graham Healy, CA216 Lab04/05: Building a Shell, https://loop.dcu.ie/mod/book/view.php?id=2322719&chapterid=492183

Linux manual exec(3), https://man7.org/linux/man-pages/man3/exec.3.html

An introduction to makefiles, https://www.gnu.org/software/make/manual/html_node/Introduction.html

C - Header Files, https://www.tutorialspoint.com/cprogramming/c_header_files.html

Tai Tan Mai, CA284 Systems Programming, https://loop.dcu.ie/course/view.php?id=61366

Linux manual fork(2), https://man7.org/linux/man-pages/man2/fork.2.html

Name: Killian Daly

Student number: 22739355

I acknowledge DCU's academic integrity policy
