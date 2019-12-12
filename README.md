# P01_Shell
By Jackie Lin and Peihua Huang (pd 10)

## Shell Features
* run commands (accounts for extra whitespace) including cd and exit
* supports multiple commands on one line (separated by a semicolon)
* implements simple redirection (supports "<", ">", ">>") and also supports multi-redirection

## Function Headers
char ** parse_args(char * line, char * delimiter);
char * strip(char * line, char delimiter);
void redirection(char ** tokens);
void piping(char ** tokens);
void run_cmd(char ** tokens);
