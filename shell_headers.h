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

// parse_args takes in a command and a delimiter (either "," or ";"), and returns their tokens
char ** parse_args(char * line, char * delimiter);

//strip takes in a command and a delimiter (usually ' '), and returns the command stripped of the delimiter
//in the case of ' ', strip() removes the extra whitespace in the command
char * strip(char * line, char delimiter);

//redirection takes in the tokens of a command and checks to see if any of the tokens are "<" or ">", then modifies the file table accordingly
void redirection(char ** tokens);

//piping takes in the tokens of a command and checks to see if any of the tokens are "|", then uses popen() to pipe accordingly
void piping(char ** tokens);

//run_cmd takes in the tokens of a command and incorporates modular design to parse_args on that command, check for redirection and check for piping
//it also handles chdir() and exit(), which cannot be done via fork and exec
void run_cmd(char ** tokens);
