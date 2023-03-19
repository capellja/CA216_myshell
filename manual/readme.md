# MyShell User Manual

Press Q to quit User Manual
## Introduction

MyShell is a simple command line interpreter for Linux built using C programming language. 
The shell is capable of executing internal commands such as "cd", "clr", "dir", "echo", "environ", "pause", and "quit". Additionally, it can also execute external commands that are stored in the system's path. The program also supports I/O redirection and input from a batchfile.
MyShell provides a basic shell environment similar to the Unix / Linux shell.

## Usage

#### Invocation

    eg.
        [command] arg1

MyShell can be invoked by running "./myshell" in the directory where the executable is compiled.
You are greeted by the prompt which shows your current working directory.
The shell reads the command line for the given command and following arguments. Errors will be raised of incorrect syntax is provided.


#### Arguments

    eg.
        ./myshell [batchfile]

MyShell can be invoked along with an argument. Given that this argument is a file and [batchfile] contains lines of commands, MyShell will read input from the file and perform said commands before exiting.
note: myshell has a bug with environ.

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
        Pause operation until 'Enter' key is pressed,

    QUIT
        Quits the shell.

## External Commands

MyShell has the ability to perform external commands (eg. ls, python3, pwd, etc..) An Error message will be raised if the external command is not recognised.

## Environment

- Environment variables are a type of dynamic variable in an operating system that contain information used by processes and applications. 
- MyShell utilises examples of environment variables including PATH, which specifies the directories that the operating system should search for the executable program and SHELL, which specifies the default shell for the user.
- These variables are set by the operating system, system programs, and user-defined scripts.
- MyShell sets 'SHELL' to the path of the shell, and dynamically updates 'PATH' depending on the current working directory.

## Processes

MyShell forks its parent process to create an identical child processes.
The shell runs it's external and internal commands through these child processes. This allows features such as background processing to be implemented.

## Background Processing

    eg.
        command argument &
    
MyShell supports background execution of commands if '&' is present as the final argument.
MyShell returns the message, "Running command [command] in the background>. While typically the parent process(shell) waits for the child process(command) to be done, this feature allows the child process to run in the background and the user is given access to the command line immediately.

## I/O Redirection

MyShell supports I/O redirection of standard input (stdin) and standard output(stdout).
This implemenatation allows you read input or provide output to/ from an external source such as a file.

    eg.
        [command]  [arg1] [arg2]   < [inputfile] > [outputfile]

- Where the stdin is replaced by inputfile and the stdout is replaced by outputfile.

### MyShell can handle three types of I/O Redirection

    1. Input [<] 
    2. Output [>]
    - Will create a new file if it doesn't exist, otherwise it will be overwritten
    3. Append [>>]
    - Same as above, however output will be appended to the end of file.


#### References

- I/O redirection ; Oualline, S. (1997). Practical C programming. " O'Reilly Media, Inc.".
- Proccesses ; Gehani, N., & Roome, W. D. (1989). The concurrent C programming language. Silicon Press.
- Command line processing, General Linux ; Newham, C. (2005). Learning the bash shell: Unix shell programming. " O'Reilly Media, Inc.".
- Background processing in shell; Ward, B. (2021). How Linux works: What every superuser should know. no starch press.

/*
 James Capellan : 21477856
 Acknowledgement of the DCU Academic Integrity Policy:
 I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.
 I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.
 I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.
 I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.
 I have used the DCU library referencing guidelines and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.
*/