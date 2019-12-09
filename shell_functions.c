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

void run_cmd(char ** tokens){
  if (strcmp(tokens[0], "cd") == 0){
    chdir(tokens[1]);
    if (errno != 0){
      printf("Errno: %d %s\n", errno, strerror(errno));
    }
    printf("%s\n", getcwd(tokens[1], 100));
  }
  else if (strcmp(tokens[0], "")){
    if (fork() == 0){
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
          fd = open(output, O_CREAT | O_WRONLY, 0644);
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
