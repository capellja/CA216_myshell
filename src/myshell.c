/////////////////////////////////////////////////////////////////////////

// OPERATING SYSTEMS 
/*  CA216 PROJECT 1 - MYSHELL */

// Author : James Capellan : (21477856)

/////////////////////////////////////////////////////////////////////////
/********************************************************************
 * This program is structured based on the skeleton provided by the following source
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

// child and parent pid's for testing
pid_t pid;
pid_t parent_pid; 
int status;


int main (int argc, char ** argv)
{   
    char buf[MAX_BUFFER];       // line buffer
    char * args[MAX_ARGS];      // pointers to arg strings
    char ** arg;    
    char * input;

    /*  setting "SHELL" environment variable to path to shell  */
    char *pwd = strdup(getenv("PWD"));  // duplicate PWD for path to shell

    char *shellvar = malloc(strlen(pwd) + strlen("/myshell"));  // malloc shellvar for pwd + /myshell
    if(shellvar == NULL) {
        printf("Memory allocation error");  // mem alloc error check
    }
    strcat(shellvar, pwd);  // "" + pwd
    strcat(shellvar, "/myshell");   // pwd + /myshell
    setenv("SHELL", shellvar, 1);   // setenv() to set "SHELL" as path to shell
    // https://www.tutorialspoint.com/c_standard_library/c_function_getenv.htm

    /* prompt - show current working directory */
    char *prompt = pwd;
    strcat(prompt, ": ");  
    
    parent_pid = getpid(); // for testing
    int flag =  0;          // batchfile check

    /*  testing for batchfile   */
    if (argc != 1) {
        if(argv[1] != NULL) {
            if (freopen(argv[1], "r", stdin) == NULL) { // use freopen() to set stdin as batchfile
                perror("Failed to open file");  // error checking if file doesn't exist
                return 1;
             }
            else {
                flag = 1;   // in batchfile, remove prompt
            }
        }
    }
    // https://www.educative.io/answers/how-to-use-freopen-in-cpp

    /* keep reading input until "quit" command or eof of redirected input */
    while (!feof(stdin)) { 
        /* get command line from input */

    	/*  check if in batchfile   */
        if(flag != 1) {
            fputs (prompt, stdout); // else write prompt
        }

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenise input
            while ((*arg++ = strtok(NULL,SEPARATORS))); // last entry will be NULL 

            input = args[0]; // command input, first arg
            if (input) { 

                /*  cd  */
                if (!strcmp(input,"cd")) {
                    if(args[1] != NULL) {
                        cd(args);
                        prompt = strcat(strdup(getenv("PWD")), ": ");   // update prompt to current directory
                    }
                    else {
                        printf("Directory : %s\n", getenv("PWD"));   // return current directory if no path specified
                    }
                    continue;
                }

                /* quit */
                else if (!strcmp(input,"quit"))
                    exit(0);    // break out of 'while' loop

                /*  pause   */
                else if (!strcmp(input,"pause")) {
                     pausecommand();
                     continue;
                }

                /*  help    */
                else if (!strcmp(input,"help")) {
                    help();
                    continue;
                    }

                /*  fork following commands as child processes 
                fork() returns -1, 0, 1. 
                Use of switch allows organisation and forking of internal commands
                Cases:
                -1 = failed
                0 = child
                1 = parent
                */
               // https://stackoverflow.com/questions/44297208/using-switch-statements-to-fork-two-processes

                switch (pid = fork()) {
                
                    case -1: 
                        perror("command does not exist");   // fork failed
                    /* child process */
                    case 0:

                        IOredirect(args);   // call IORedirect function to check for redirect symbols, return commands, output or input.
                        
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
                        /*  external commands    */
                        else { 
                            if(execvp(input, args) == -1) {  // execute command using execvp() with args 
                                fprintf(stdout,"Command %s does not exist\n", input);   // error checking if command exists
                                break;
                            }
                        }
                        exit(0);
                    /* parent  */
                    default: 

                        /*  background processing */
                         // check for "&" in args
                         // https://stackoverflow.com/questions/46860148/c-how-do-i-run-a-program-in-the-background-using-exec
                        int backgroundflag = 0;
                        for(int i = 1; args[i] != NULL; i++) {  // iterate through args
                            if(strcmp(args[i], "&") == 0) {
                                backgroundflag = 1; //flag =1 if present
                            }
                        }

                        if(backgroundflag == 1) {
                            waitpid(pid, NULL, 0);  // set WAIT to NULL so parent will not wait for child process to finish
                            printf("Running command '%s' in background\n", args[0]);
                            break;
                        }
                        else if(backgroundflag == 0) {
                            waitpid(pid, &status, 0);  // wait for the child process to finish
                            //printf("this is working, %s\n", args[0]);
                            break;
                        }
                }
            }
        }
    }
    free(shellvar);     //free memory
    shellvar = NULL;

    return 0; 
}

/* Acknowledgement of the DCU Academic Integrity Policy:
* I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.
* I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.
* I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.
* I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.
* I have used the DCU library referencing guidelines and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.
*/