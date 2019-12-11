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
      printf("%s \n", cmd[0]);
      if (cmd[1]){
        piping(cmd);
	for (j = 0; cmd[j] != NULL; j++){
	  free(cmd[j]);
	}
      }
      else{
        char ** tokens = parse_args(cmds[i], " ");
        run_cmd(tokens);
	for (j = 0; tokens[j] != NULL; j++){
	  free(tokens[j]);
	}
        free(tokens);
      }
      free(cmd);
      i++;
    }
    for (j = 0; cmds[j] != NULL; j++){
      free(cmds[j]);
    }
    free(cmds);
  }
  return 0;
}
