#include "shell_headers.h"

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

void run_arg(char ** tokens){
  if (strcmp(tokens[0], "cd") == 0){
    chdir(tokens[1]);
    if (errno != 0){
      printf("Errno: %d %s\n", errno, strerror(errno));
    }
    printf("%s\n", getcwd(tokens[1], 100));
  }
  else {
    if (fork() == 0){
      execvp(tokens[0], tokens);
    }
    else{
      wait(NULL);
    }
  }
}
