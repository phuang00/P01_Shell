# P01_Shell
By Jackie Lin and Peihua Huang (period 10)

## Shell Features
* run commands (accounts for extra whitespace) including cd and exitxw
* supports multiple commands on one line (separated by a semicolon)
* implements simple redirection (supports "<", ">", ">>") and also supports multi-redirection
  * multi-redirection supports cmd > file1 < file2, cmd < file1 > file2, cmd < file1 >> file2, cmd >> file1 < file2, and cmd < file1 < file2
  * order of redirection doesn't matter
* implements simple piping of commands

Limitations:
* multi-redirection does not support two ">" signs or two ">>" signs or a combination of the two
  * for example, cmd > file1 >> file2 will not work as intended
* pipe must following this format: cmd | cmd (and both commands must exist!)
  * if these requirements are not met, a broken pipe error may be thrown and the program will exit
* does not support multiple pipes in one command

## Function Headers
* char ** parse_args(char * line, char * delimiter);
* char * strip(char * line, char delimiter);
* void redirection(char ** tokens);
* void piping(char ** tokens);
* void run_cmd(char ** tokens);
