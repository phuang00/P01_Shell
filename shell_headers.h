#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

char ** parse_args(char * line, char * delimiter);
char * strip(char * line, char delimiter);
void redirection(char ** tokens);
void pipe(char ** tokens);
void run_cmd(char ** tokens);
