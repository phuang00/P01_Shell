#include "shell_headers.h"

int main() {
  char cmd[100];
  fgets(cmd, 100, stdin);
  while (strcmp(cmd, "exit\n") != 0){
    fgets(cmd, 100, stdin);
  }
  printf("hi\n");
  return 0;
}
