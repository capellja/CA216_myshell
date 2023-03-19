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
pid_t parent_pid;
int status;


int main (int argc, char ** argv)
{   
    char buf[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;    
    char * input;


    char *pwd = strdup(getenv("PWD"));

    char *shellvar = malloc(strlen(pwd) + strlen("/myshell"));
    if(shellvar == NULL) {
        printf("Memory allocation error");
    }
    strcat(shellvar, pwd);
    strcat(shellvar, "/myshell");
    setenv("SHELL", shellvar, 1);

    char *prompt = pwd;
    strcat(prompt, ": ");  

    
    parent_pid = getpid();
    int flag =  0;          // shell prompt

    if (argc != 1) {

        if(argv[1] != NULL) {                   // batchfile 
            if (freopen(argv[1], "r", stdin) == NULL) {
                perror("Failed to open file");
                return 1;
             }
            else {
                flag = 1;
            }
        }
    }

    /* keep reading input until "quit" command or eof of redirected input */

    while (!feof(stdin)) { 
        /* get command line from input */

        if(flag != 1) {
            fputs (prompt, stdout); // write prompt
        }

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenise input
            while ((*arg++ = strtok(NULL,SEPARATORS))); // last entry will be NULL 

            input = args[0]; // command input
            if (input) {                     // if there's anything there
                /* check for internal/external command */

                if (!strcmp(input,"cd")) {  // "cd" command
                    if(args[1] != NULL) {
                        cd(args);
                        prompt = strcat(strdup(getenv("PWD")), ": ");
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

                else if (!strcmp(input,"help")) {  // "help" command
                    help();
                    continue;
                    }

                switch (pid = fork()) {
                    case -1: // could not fork 
                        perror("command does not exist");
                    case 0: // in child process

                        IOredirect(args);
                        
                        if (!strcmp(input,"clr")) { // "clear" command
                            clr();
                        }
               
                        else if (!strcmp(input,"dir")) { // "show directory" command
                            dir(args);
                        }

                        else if (!strcmp(input,"echo")) {  // "echo" command
                            echo(args);
                        }

                        else if (!strcmp(input,"environ")) {     // "environ" command
                            env();
                        }

                        else {     // external commands
                            if(execvp(input, args) == -1) {  // execute command using child process #  
                                fprintf(stdout,"Command %s does not exist\n", input);   // error checking
                                break;
                            }
                        }
                        exit(0);
                    default: //parent process

                        int flag = 0;
                        for(int i = 1; args[i] != NULL; i++) {
                            if(strcmp(args[i], "&") == 0) {
                                flag = 1;
                            }
                        }

                        if(flag == 1) {
                            waitpid(pid, NULL, 0);  // wait for the child process to finish 
                            printf("Running command '%s' in background\n", args[0]);
                            break;
                        }
                        else if(flag == 0) {
                            waitpid(pid, &status, 0);  // wait for the child process to finish
                            printf("this is working, %s\n", args[0]);
                            break;
                        }
                        
                     /*
                        for(int i = 1; args[i] != NULL; i++) {
                            if(strcmp(args[i], "&") == 0) {
                                wait(NULL);
                                printf("Running command '%s' in background\n", args[0]);

                            } 
                        }
                
                        
                            

                        

                    
                        for(int i = 1; args[i] != NULL; i++) {
                            if(strcmp(args[i], "&") == 0) {
                                printf("test");
                            }
                        
                        waitpid(pid, &status, 0);  // wait for the child process to finish
                        }
                        */
                        /*
                            for(int i = 0; args[i + 1] != NULL; i++) {
                                if(strcmp(args[i], "&") == 0) {
                                    waitpid(pid, &status, 0);  // wait for the child process to finish 
                                }
                                else{
                                    wait(NULL);  // wait for the child process to finish 
                                }
                            }
                        */
                    
                }
            }
        }
    }

    
    //free(prompt);
    free(shellvar);

    //prompt = NULL;
    shellvar = NULL;

    return 0; 
}