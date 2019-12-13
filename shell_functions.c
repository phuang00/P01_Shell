#include "shell_headers.h"


//Handles all parsing functions (including multiple commands on one line)
/*======== char **parse_args() ==========
Inputs:  char *line
         char *delimiter
Returns: a command's tokens separated by delimiter

 Every time the string delimiter appears in line, the token is added to an array called tokens
 Very last item of tokens is always set to NULL
====================*/
char ** parse_args(char * line, char * delimiter){
  char * curr = line;
  char ** tokens = malloc(6 * sizeof(curr));
  int i = 0;
  while (curr != NULL){
    tokens[i] = strsep(&curr, delimiter);
    i++;
  }
  tokens[i] = NULL;
  return tokens;
}


//Strips a command of its delimiter
/*======== char *strip() ==========
 Inputs:  char *line
      	   char delimiter
 Returns: a command with its extra delimiter stripped off (usually whitespace)

 First trims all the delimiters that appear in front a command
 Then goes through rest of command and makes sure that at most only one delimiter between tokens
 Finally, trims any delimiter at the end of a command
 ====================*/
char * strip(char * line, char delimiter){
  char input[100];
  char output[100];
  strcpy(input, line);
  int n = strlen(line);
  int i = 0, j = 0; //i keeps track of output, j keeps track of input
  int isSpace = 0; //boolean for whether or not character is space
  while (j < n && input[j] == delimiter){ //gets rid of preceding whitespace
    j++;
  }
  while (j < n){ //loops through rest of string for extra spaces
    if (input[j] != delimiter){
      isSpace = 0; //space not found
      output[i] = input[j];
      i++;
      j++;
    }
    else{
      if (isSpace){ //space already found
        j++;
      }
      else {
        isSpace = 1; //space found
        output[i] = input[j];
        i++;
        j++;
      }
    }
  }
  if (i > 0 && output[i - 1] == ' ') output[i - 1] = 0;
  output[i] = 0;
  char * ret = malloc(100 * sizeof(char));
  strcpy(ret, output);
  return ret;
}


//Handles all commands that implement redirection (<, >, or >>)
/*======== void redirection() ==========
Inputs:  char ** tokens
Returns: void, modifies file table according to command

First checks if signs >, < or >> appear in the command tokens
If they do exist, stdout and stdin's spots in the file table are modified accordingly
====================*/
void redirection(char ** tokens){
  int i;
  int in = 0;
  int out = 0;
  char input[100];
  char output[100];
  int backup = 0;
  for (i = 0; tokens[i] != 0; i++){
    if (strcmp(tokens[i], "<") == 0){
      strcpy(input, tokens[i + 1]);
      tokens[i] = NULL;
      in = 1;
    }
    else if (strcmp(tokens[i], ">") == 0){
      tokens[i] = NULL;
      strcpy(output, tokens[i + 1]);
      out = 1;
    }
    else if (strcmp(tokens[i], ">>") == 0){
      tokens[i] = NULL;
      strcpy(output, tokens[i + 1]);
      out = 2;
    }
  }
  if (in){
    int fd = open(input, O_RDONLY, 0);
    if (fd < 0){
      printf("%s: %s\n", input, strerror(errno));
    }
    backup = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    close(fd);
  }
  if (out){
    int fd;
    if (out == 2){
      fd = open(output, O_CREAT | O_WRONLY | O_APPEND, 0644);
    }
    else{
      fd = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    }
    if (fd < 0){
      printf("%s: %s\n", output, strerror(errno));
    }
    backup = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);
  }
}


//Handles all commands that implement pipes (|)
/*======== void piping() ==========
Inputs:  char ** tokens
Returns: void, uses popen() to pipe the two tokens accordingly

First checks if | appears in the command tokens
If command implements pipes, popen() is used to read and write from a pipe
====================*/
void piping(char ** tokens){
  FILE *input = popen(tokens[0], "r");
  if (!input){
    printf("%s\n", strerror(errno));
  }
  FILE *output = popen(tokens[1], "w");
  if (!output){
    printf("%s\n", strerror(errno));
  }
  char buffer[100];
  while (fgets(buffer, 100, input)){
    fputs(buffer, output);
  }
  pclose(input);
  pclose(output);
  //popen(cmd1, r);
  //popen(cmd2, w);
}


//Runs all commands, including chdir()
/*======== void run_cmd() ==========
Inputs:  char **tokens
Returns: void, checks for redirection and cd before performing execvp on command tokens

First checks if cd appears in command tokens and runs chdir() accordingly
Then checks for redirection in the command
Also has error handling functionality (errno)
====================*/
void run_cmd(char ** tokens){
  if (strcmp(tokens[0], "cd") == 0){
    if (tokens[1] == NULL || strcmp(tokens[1], "~") == 0){
      tokens[1] = getenv("HOME");
    }
    chdir(tokens[1]);
    if (errno != 0){
      printf("Errno: %d %s\n", errno, strerror(errno));
      errno = 0;
    }
  }
  else if (strcmp(tokens[0], "")){
    if (fork() == 0){
      redirection(tokens);
      execvp(tokens[0], tokens);
      if (errno != 0){
        printf("%s: command not found\n", tokens[0]);
        kill(getpid(), SIGTERM);
      }
    }
    else{
      if (errno != 0){
        printf("Errno: %d %s\n", errno, strerror(errno));
      }
      wait(NULL);
    }
  }
}
