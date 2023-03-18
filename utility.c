#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "myshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64   

extern char **environ;

void cd(char *args[]) {     
    char cwd[MAX_BUFFER];
    
    if (chdir(args[1]) != 0) {
        perror("chdir() error");
    }
    else {
        getcwd(cwd, sizeof(cwd));
        setenv("PWD", cwd, 1);
    }

}

void clr() {
    printf("\033[2J\033[H"); // ANSI escape codes to clear screen
}

void echo(char *args[]) {
    char output[MAX_BUFFER] = ""; // Maximum output length of 1024

    // Concatenate all arguments into a single string
    for (int i = 1; args[i] != NULL; i++) {
        strcat(output, args[i]);
        strcat(output, " ");                     // come back to this - extra whitespace at end //
    }
    fprintf(stdout,"%s\n", output);

}

void dir(char *args[]) {

    char *argv[MAX_ARGS];  // create new array to hold new order of arguments
    argv[0] = "ls";
    argv[1] = "-al";
    int i;

    for ( i = 1; argv[i] != NULL; i++) {    // increment through args and add values into new argv +1 in position to accomodate "-al"
        argv[i + 1] = args[i];
    }
    argv[i + 1] = NULL;     // last argument is NULL

    if(execvp("ls", argv) == -1) {
        perror("execvp fail");
        exit(EXIT_FAILURE);  // error check
    }
    
}


void env() {
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n ", environ[i]);
    }

}

void pausecommand() {
    fprintf(stdout,"press Enter to continue...");
    fflush(stdout); // flush stdout buffer to ensure prompt is displayed
    while (getchar() != '\n') { // loop until Enter is pressed
        // do nothing
    }
    fprintf(stdout,"continuing...\n");

}

void IOredirect(char *args[]) {

    for(int i = 1; args[i] != NULL; i++) {
        if(strcmp(args[i], "<") == 0) {
            if (freopen(args[i+1], "r", stdin) == NULL) {
                printf("Error : cannot open file %s for input\n", args[i+1]);
            }
            else {
                args[i] = NULL;   // removes symbol and file name
                args[i + 1] = NULL;
                break;
            }
        }
        
        else if (strcmp(args[i], ">") == 0) {

            if (freopen(args[i+1], "w", stdout) == NULL) {
                printf("Error : cannot open file %s for output\n", args[i+1]);
            }
            else {
                args[i] = NULL;
                args[i + 1] = NULL;
                break;
            }
        }

        else if (strcmp(args[i], ">>") == 0) {

            if (freopen(args[i+1], "a", stdout) == NULL) {
                printf("Error : cannot open file %s for output\n", args[i+1]);
            }
            else {
                args[i] = NULL;
                args[i + 1] = NULL;
                break;
            }
        }        

    }
}
    

/*
void fk(char *args[]) {
    pid_t pid; 
    int status;

    pid = fork();  // create a new process

    if (pid < 0) { // error forking, unable to create child process.
        perror("command does not exist"); // come back to this, for error checking whether to use perror or not // 

    }

    // in child process //
    else if( pid == 0) {
        for(int i = 1; args[i] != NULL; i++) {
            if (strcmp(args[i], "<")) {
                ++i;
                
            }
        }

        if(execvp(args[0], args) == -1) {  // execute command using child process 
            fprintf(stdout,"Command %s does not exist.\n", args[0]);
        }
        exit(EXIT_FAILURE);
    }

    else {  // parent process
        waitpid(pid, &status, 0);  // wait for the child process to finish
            if (WIFEXITED(status)) {
            }
    }

}
*/ 

