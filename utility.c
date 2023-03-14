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

void cd(char *args[]) {     /* ideas to do 'cd..' */
    if(args[1] == NULL) {
        system("pwd");
    }
    else if(strcmp(args[1], "..") == 1) {
        system("cd ..");
    }
    else if (chdir(args[1]) != 0) {
        perror("chdir() error");
    }

}

void clr() {
    system("clear");
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

    char directory[MAX_BUFFER];

    if(args[1] == NULL) {       // list in current directory if no argument provided
        system("ls -al");
    }
    else{
        strcpy(directory, args[1]);
        char filler[MAX_BUFFER] = "ls -al ";
        strcat(filler, directory);
        system(filler);  
    }

}

void env() {
    for (int i = 0; environ[i] != NULL; i++) {
        fprintf(stdout,"%s\n ", environ[i]);
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

void fk(char *args[]) {
    pid_t pid; 
    int status;

    pid = fork();  // create a new process
    if (pid < 0) { // error forking, unable to create child process.
        perror("command does not exist"); // come back to this, for error checking whether to use perror or not // 

    }

    // in child process //
    else if( pid == 0) {
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
