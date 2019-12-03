#include "shell_headers.h"

char ** parse_args( char * line ){
  char * curr = line;
  char ** tokens = malloc(6 * sizeof(curr));
  int i = 0;
  while (curr != NULL){
    tokens[i] = strsep(&curr, " ");
    i++;
  }
  return tokens;
}
