#include "shell_headers.h"

int main() {
  char cmd[100];
  while (strcmp(cmd, "exit") != 0){
    fgets(cmd, 100, stdin);
    cmd[strlen(cmd) - 1] = 0;
    char ** tokens = parse_args(cmd);
    if (strcmp(tokens[0], "cd") == 0){
      chdir(tokens[1]);
      if (errno != 0){
        printf("Errno: %d %s\n", errno, strerror(errno));
      }
      printf("%s\n", getcwd(tokens[1], 100));
    }
    else if (strcmp(cmd, "exit") != 0){
      int id = fork();
      if (!id){
        execvp(tokens[0], tokens);
      }
    }
  }
  return 0;
}
