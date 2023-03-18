#ifndef MYSHELL_H
#define MYSHELL_H

void clr();
void echo(char *args[]);
void env();
void dir(char *args[]);
void cd(char *args[]);
void pausecommand(); // in-built 'pause' command exists -> rename to pausecommand to avoid conflict //
void fk(char *args[]);
void IOredirect(char *args[]);

#endif