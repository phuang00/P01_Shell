#include "shell_headers.h"

char ** parse_args(char * line, char * delimiter){
  //char * curr = line;
  char * curr = strip(line, ' ');
  char ** tokens = malloc(6 * sizeof(curr));
  int i = 0;
  while (curr != NULL){
    tokens[i] = strsep(&curr, delimiter);
    i++;
  }
  tokens[i] = NULL;
  free(curr);
  return tokens;
}

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
}

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
    //printf("%s\n", getcwd(tokens[1], 100));
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
