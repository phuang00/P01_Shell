#include "shell_headers.h"

int main() {
  char line[100];
  while (strcmp(line, "exit") != 0){
    fgets(line, 100, stdin);
    line[strlen(line) - 1] = 0;
    char ** cmds = parse_args(line, ";");
    int i = 0;
    int j;
    while (cmds[i] != NULL && strcmp(cmds[i], "exit") != 0){
      char ** cmd = parse_args(cmds[i], "|");
      if (cmd[1]){
        piping(cmd);
      }
      else{
        char ** tokens = parse_args(cmds[i], " ");
        run_cmd(tokens);
        free(tokens);
      }
      free(cmd);
      i++;
    }
    free(cmds);
  }
  return 0;
}
