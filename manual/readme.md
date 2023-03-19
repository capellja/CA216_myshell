# MyShell User Manual
Press Q to quit User Manual
## Introduction
MyShell is a simple command line interpreter for Linux built using C. 
The shell is capable of executing internal commands such as "cd", "clr", "dir", "echo", "environ", "pause", and "quit". Additionally, it can also execute external commands that are stored in the system's path. The program also supports I/O redirection and batch processing from a file.
MyShell provides a basic shell environment similar to the Unix / Linux shell.

## Usage
Invocation
MyShell can be invoked by running "./myshell" in the directory where the executable is compiled.
You are welcomed by the prompt which shows your current working directory.
The shell reads the command line for the given command and following arguments. Errors will be raised of incorrect syntax is provided.

Arguments
MyShell can be invoked along with an argument. Given that this argument [batchfile] contains lines of commands, MyShell will read input from the file and perform said commands before exiting.

## Interal Commands

    CD
        Changes current directory
        Shows current directory if no argument is given
        Displays error if directory does not exist

    CLR
        Clears terminal screen.
        Returns prompt

    DIR
        Lists contents of given directory
        Displays contents of current directory if no argument is given.

    ENVIRON
        Displays list of environment variables

    ECHO
        Displays string given as an argument as a newline.
        String can consist of mutiple words separated by spaces / tabs, but are reduced to a single when shown.

    HELP
        Display the User Manual. One page at a time.
        Press 'Enter' to display next line.
        Press 'Space' to display the next page.
        Press 'Q' to quit.
        Use arrow keys to navigate the page.

    PAUSE
        Pause operation until 'Enter' key is pressed

    QUIT
        Quits the shell

## External Commands


## Environment

Environment variables are a type of dynamic variable in an operating system that contain information used by processes and applications. 
MyShell utilises examples of environment variables including PATH, which specifies the directories that the operating system should search for the executable program and SHELL, which specifies the default shell for the user.
These variables are set by the operating system, system programs, and user-defined scripts.
MyShell sets 'SHELL' to the path of the shell, and dynamically updates 'PATH' depending on the current working directory.