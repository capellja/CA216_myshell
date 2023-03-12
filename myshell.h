#ifndef MYSHELL_H
#define MYSHELL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void clr();
void echo(char *args[]);
void env();
void dir(char *args[]);
void pwd();
void cd(char *args[]);
int pause();


#endif