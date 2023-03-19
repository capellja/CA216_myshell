#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "myshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                               // max arg buffer

extern char **environ;                                // extern char for environ()

/*  cd   */
void cd(char *args[]) {     
    char cwd[MAX_BUFFER];
    
    if (chdir(args[1]) != 0) {                          // chdir() to change directory
        perror("chdir() error");                      // if chdir() fails, return error
    }
    else {
        getcwd(cwd, sizeof(cwd));               // use getcwd() for current directory
        setenv("PWD", cwd, 1);                   // update environment variable, 'PWD' to cwd
    }
}

/*  clear   */
void clr() {
    char * temp[MAX_BUFFER] = {NULL};
    execvp("clear", temp);
    // printf("\033[2J\033[H"); // ANSI escape codes to clear screen
}

/*  echo    */
void echo(char *args[]) {
    char* echoOutput[MAX_BUFFER] ;                      // intialise output array
    int i;

    //  iterate through args and store them in output
    echoOutput[0] = "echo";                                    // first element has to be echo
    for (i = 1; args[i] != NULL; i++) {
        echoOutput[i] = args[i];
    }
    echoOutput[i + 1] = NULL;                               // last element has to be NULL

    if(execvp("echo", echoOutput) == -1) {          // call execvp(), with echo as the command and the output array
        perror("execvp fail");
        exit(EXIT_FAILURE);                                     // error check
    }
}

/*  dir    */
void dir(char *args[]) {
    char *outputdir[MAX_ARGS];                   // initialise outputdir array
    outputdir[0] = "ls";                                   // first and second arguments must be ls, and -al in order for execvp() to run it as "ls -al"
    outputdir[1] = "-al";       
    int i;

    // increment through args and insert them into outputdir, but one position forward in index to accomodate, '-al'
    for ( i = 1; outputdir[i] != NULL; i++) {
        outputdir[i + 1] = args[i];
    }
    outputdir[i + 1] = NULL;                         // last argument is NULL

    if(execvp("ls", outputdir) == -1) {
        perror("execvp fail");
        exit(EXIT_FAILURE);  // error check
    }
}

/*  env    */
void env() {
    for (int i = 0; environ[i] != NULL; i++) {  //  iterate through exter char **environ, print
        fprintf(stdout, "%s\n ", environ[i]);
    }
}

/*  pause   */
void pausecommand() {
    fprintf(stdout,"press Enter to continue...");
    fflush(stdout);                                // flush stdout buffer to ensure prompt is displayed
    while (getchar() != '\n') {               // loop until Enter is pressed
        // do nothing
    }
    fprintf(stdout,"continuing...\n");
}

/*  IORedirection   */
void IOredirect(char *args[]) {

    // iterate through args and look for redirect symbol
    for(int i = 1; args[i] != NULL; i++) {

        if(strcmp(args[i], "<") == 0) {
            if (freopen(args[i+1], "r", stdin) == NULL) {                               // use freopen() to redirect standard input into the file (args[i+1])
                printf("Error : cannot open file %s for input\n", args[i+1]);   // error checking if file exists
            }
            else {
                args[i] = NULL;         // removes redirect symbol and file name by setting them to NULL
                args[i + 1] = NULL;
                break;
            }
        }

        else if (strcmp(args[i], ">") == 0) {       // use freopen() to redirect standard output into the file (args[i+1])
            if (freopen(args[i+1], "w", stdout) == NULL) {
                printf("Error : cannot open file %s for output\n", args[i+1]);
            }
            else {
                args[i] = NULL;
                args[i + 1] = NULL;
                break;
            }
        }

        else if (strcmp(args[i], ">>") == 0) {      //append         
            if (freopen(args[i+1], "a", stdout) == NULL) {      // redirect standard output, 'a' specifics appending to the file, rather than writing over it
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

/*  manual  */ 
void help() {
    char path[MAX_BUFFER];
    getcwd(path, sizeof(path));         // get cwd
    char * read = malloc(strlen("cat ") * sizeof(path) + strlen(" | more"));    // malloc to read for full path including additonals
    strcat(read, "cat ");       //concat cat into empty read
    strcat(read, path);        // "cat" + cwd
    strcat(read, "/../manual/readme.md | more -p");     // "cat" + "cwd" + "/../manual/readme.md | more =p"

    system(read);       // call system to output readme.md, | more to view contents of textfile
    free(read);            // free memory
}

/* PROTOTYPE FOR CALLING FORK THROUGH A FUNCTION
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