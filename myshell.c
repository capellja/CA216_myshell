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

#include "myshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators


int main (int argc, char ** argv)
{   
    char buf[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;    
    
    if(argv[1] != NULL) {                   // batchfile 
        FILE * commands = freopen(argv[1], "r", stdin);
        if(commands == NULL) {
            perror("Failed to open file");
            return 1;
        } 

    }

    char *prompt = ": ";                   // shell prompt

    char cwd[MAX_BUFFER];
    if (getcwd(cwd, sizeof(cwd)) != NULL) { // show current working directory in prompt
            strcat(cwd, prompt);
            prompt = (cwd);
        } else {
            perror("getcwd() error");
            return 1;
     }
 
    /* keep reading input until "quit" command or eof of redirected input */

    while (!feof(stdin)) { 
        /* get command line from input */
        fputs (prompt, stdout); // write prompt

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenise input

            while ((*arg++ = strtok(NULL,SEPARATORS)));

            // last entry will be NULL 
            if (args[0]) {                     // if there's anything there
                /* check for internal/external command */

                if (!strcmp(args[0],"cd")) {  // "cd" command
                    cd(args);

                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                                strcat(cwd, ": ");
                                prompt = (cwd);
                            } else {
                                perror("getcwd() error");
                                return 1;
                            }

                    continue;
                }

                if (!strcmp(args[0],"clr")) { // "clear" command
                    clr();
                    continue;
                }

                if (!strcmp(args[0],"dir")) { // "show directory" command
                    dir(args);
                    continue;
                }

                if (!strcmp(args[0],"quit"))   // "quit" command
                    break;                     // break out of 'while' loop
            
                if (!strcmp(args[0],"echo")) {  // "echo" command
                    echo(args);
                    continue;
                }

                if (!strcmp(args[0],"environ")) {     // "environ" command
                    env();
                    continue;
                }

                if (!strcmp(args[0],"pause")) {  // "pause" command
                     pausecommand();
                     continue;
                }

                if (!strcmp(args[0],"help")) {  // "help" command
                     continue;
                }

                else {
                    fk(args);
                   // fprintf(stdout,"command not found \n"); //
                    continue;

                }
                
                /* else pass command onto OS (or in this instance, print them out) */

            }
        }
    }
    return 0; 
}