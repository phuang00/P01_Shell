#include "shell_headers.h"

int main() {
  char cmd[100];
  while (strcmp(cmd, "exit\n") != 0){
    fgets(cmd, 100, stdin);
    char ** tokens = parse_args(cmd);
    int ret = strncmp(tokens[0], "cd", 2);
    if (ret == 0){
      char name[100];
      strcpy(name, tokens[1]);
      name[strlen(name) - 1] = 0; //terminating null
      chdir(name);
      if (errno != 0){
        printf("Errno: %d %s\n", errno, strerror(errno));
      }
      printf("%s\n", getcwd(name, 100));
    }
  }
  return 0;
}
