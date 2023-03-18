/* 
strtokeg - skeleton shell using strtok to parse command line
usage:
strtokeg
reads in a line of keyboard input at a time, parsing it into
tokens that are separated by white spaces (set by #define SEPARATORS).
can use redirected input if the first token is a recognised internal command, 
then that command is executed. otherwise the tokens are printed on the display.

internal commands:
clear - clears the screen
quit - exits from the program
********************************************************************
version: 1.0
date:    December 2003
author:  Ian G Graham
School of Information Technology
Griffith University, Gold Coast
ian.graham@griffith.edu.au
copyright (c) Ian G Graham, 2003. All rights reserved.
This code can be used for teaching purposes, but no warranty,
explicit or implicit, is provided.
*******************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "myshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators

pid_t pid; 
int status;


int main (int argc, char ** argv)
{   
    char buf[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;    
    char * input;
    char cwd[MAX_BUFFER];
    char *prompt = "";       
    int flag =  0;          // shell prompt


    
    if(argv[1] != NULL) {                   // batchfile 
        FILE * commands = freopen(argv[1], "r", stdin);
        flag = 1;
        if(commands == NULL) {
            perror("Failed to open file");
            return 1;
        } 

    }

    if (getcwd(cwd, sizeof(cwd)) != NULL) { // show current working directory in prompt
        prompt = getenv("SHELL");
        setenv("SHELL", cwd, 1);
        strcat(prompt, ": ");
        } else {
            perror("getcwd() error");
            return 1;
        }
     

    prompt = getenv("PWD");
    setenv("SHELL", prompt, 1);
    strcat(prompt, ": ");

 
    /* keep reading input until "quit" command or eof of redirected input */

    while (!feof(stdin)) { 
        /* get command line from input */

        pid = getpid();

        if(flag != 1) {
            fputs (prompt, stdout); // write prompt
        }

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenise input
            while ((*arg++ = strtok(NULL,SEPARATORS))); // last entry will be NULL 

            input = args[0]; // command input
            printf("%s\n", input);
            if (input) {                     // if there's anything there
                /* check for internal/external command */

                if (!strcmp(input,"cd")) {  // "cd" command
                    if(args[1] != NULL) {
                        cd(args);
                        prompt = strcat(getenv("PWD"), ": ");
                    }
                    else {
                        printf("Directory : %s\n", getenv("PWD"));   // error checking
                    }
                    continue;
                }

                else if (!strcmp(input,"quit"))   // "quit" command
                    exit(0);                    // break out of 'while' loop

                else if (!strcmp(input,"pause")) {  // "pause" command
                     pausecommand();
                     continue;
                }

                switch (pid = fork()) {
                    case -1: // could not fork 
                        perror("command does not exist");
                    case 0: // in child process

                        // IOredirect(args);
                        
                        if (!strcmp(input,"clr")) { // "clear" command
                            clr();
                            continue;
                        }
               
                        else if (!strcmp(input,"dir")) { // "show directory" command
                            dir(args);
                            continue;
                        }

                        else if (!strcmp(input,"echo")) {  // "echo" command
                            echo(args);
                            continue;
                        }

                        else if (!strcmp(input,"environ")) {     // "environ" command
                            continue;
                        }

                        else if (!strcmp(input,"help")) {  // "help" command
                            // help()
                        }

                        else {     // external commands
                            if(execvp(input, args) == -1) {  // execute command using child process 
                                fprintf(stdout,"Command %s does not exist.\n", input);   // error checking
                            }
                        }
                        exit(0);
                    default: //parent process
                        waitpid(pid, &status, 0);  // wait for the child process to finish
                        /* for(int i = 0; args[i + 1] != NULL; i++) {
                            waitpid(pid, &status, 0);  // wait for the child process to finish
                            printf("test");

                            if(strcmp(args[i], "&") != 0) {
                                wait(NULL); // PICKUP FROM HERE LS NOT PRINTING BELOW LINE
                            }
                            else{
                                waitpid(pid, &status, 0);  // wait for the child process to finish 
                            }
                        waitpid(pid, &status, 0);  // wait for the child process to finish

                        }
                        */
                    
                }
            }
        }
    }
    return 0; 
}