#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "myshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64   

extern char **environ;

void cd(char *args[]) {
    char cwd[MAX_BUFFER];
    if(args[1] == NULL) {
        system("pwd");
    }
    else if (chdir(args[1]) != 0) {
        perror("chdir() error");
    }

}

void clr() {
    system("clear");
}

// pwd for testing
void pwd() {
   system("pwd");
}

void echo(char *args[]) {
    char output[MAX_BUFFER] = ""; // Maximum output length of 1024

    // Concatenate all arguments into a single string
    for (int i = 1; args[i] != NULL; i++) {
        strcat(output, args[i]);
        strcat(output, " ");                     // come back to this //
    }
    fprintf(stdout,"%s\n", output);

}

void dir(char *args[]) {

    char directory[MAX_BUFFER];
    strcpy(directory, args[1]);
    char filler[MAX_BUFFER] = "ls -al ";
    strcat(filler, directory);
    system(filler);

}

void env() {
    for (int i = 0; environ[i] != NULL; i++) {
        fprintf(stdout,"%s\n ", environ[i]);
    }

}

int pause() {
    fprintf(stdout,"press Enter to continue...");
    fflush(stdout); // flush stdout buffer to ensure prompt is displayed
    while (getchar() != '\n') { // loop until Enter is pressed
        // do nothing
    }
    fprintf(stdout,"continuing...\n");

}
