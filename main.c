#include "shell_headers.h"

int main() {
  char cmd[100];
  while (strcmp(cmd, "exit") != 0){
    fgets(cmd, 100, stdin);
    cmd[strlen(cmd) - 1] = 0;
    char * args;
    char * input = cmd;
    while (input){
      args = strsep(&input, ";");
      char ** tokens = parse_args(args);
      if (strcmp(tokens[0], "cd") == 0){
        chdir(tokens[1]);
        if (errno != 0){
          printf("Errno: %d %s\n", errno, strerror(errno));
        }
        printf("%s\n", getcwd(tokens[1], 100));
      }
      else if (strcmp(args, "exit") != 0){
        if (fork() == 0){
          execvp(tokens[0], tokens);
        }
        else{
          wait(NULL);
        }
      }
    }
  }
  return 0;
}
