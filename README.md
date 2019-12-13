# P01_Shell
By Jackie Lin and Peihua Huang

## Shell Features
***Features:***
* run commands (accounts for extra whitespace) including cd and exitxw
* supports multiple commands on one line (separated by a semicolon)
* implements simple redirection (supports "<", ">", ">>") and also supports multi-redirection
  * multi-redirection supports cmd > file1 < file2, cmd < file1 > file2, cmd < file1 >> file2, cmd >> file1 < file2, and cmd < file1 < file2
  * order of redirection doesn't matter
* implements simple piping of commands
  * pipe must following this format: cmd | cmd (and both commands must exist!)  

***Attempted:***
* attempted to implement multiple pipes in one command
* tried replacing ~/ with home directory
  * however, trying to cd into the home directory with ~ works   

***Bugs:***
* multi-redirection does not support two ">" signs or two ">>" signs or a combination of the two
  * for example, cmd > file1 >> file2 will not work as intended
* if these requirements for piping are not met, a broken pipe error may be thrown and exit the program
* if trying to cd into a directory that has spaces in the name, there may be an error
* putting too many semicolons next to each other may cause a segmentation fault and exit the program

## Files and Function Headers
***shell_functions.c***    
  
```
Handles all parsing functions (including multiple commands on one line)  
/*======== char **parse_args() ==========  
Inputs:  char *line  
         char *delimiter  
Returns: a command's tokens separated by delimiter  
    
 Every time the string delimiter appears in line, the token is added to an array called tokens  
 Very last item of tokens is always set to NULL  
====================*/ 
```
```
Strips a command of its delimiter  
/*======== char *strip() ==========    
 Inputs:  char *line    
      	   char delimiter    
 Returns: a command with its extra delimiter stripped off (usually whitespace)    
    
 First trims all the delimiters that appear in front a command    
 Then goes through rest of command and makes sure that at most only one delimiter between tokens    
 Finally, trims any delimiter at the end of a command    
 ====================*/
```
```    
Handles all commands that implement redirection (<, >, or >>)
/*======== void redirection() ==========
Inputs:  char ** tokens
Returns: void, modifies file table according to command

First checks if signs >, < or >> appear in the command tokens
If they do exist, stdout and stdin's spots in the file table are modified accordingly
====================*/
```
```
Handles all commands that implement pipes (|)
/*======== void piping() ==========
Inputs:  char ** tokens
Returns: void, uses popen() to pipe the two tokens accordingly

First checks if | appears in the command tokens
If command implements pipes, popen() is used to read and write from a pipe
====================*/
```
```
Runs all commands, including chdir()
/*======== void run_cmd() ==========
Inputs:  char **tokens
Returns: void, checks for redirection and cd before performing execvp on command tokens

First checks if cd appears in command tokens and runs chdir() accordingly
Then checks for redirection in the command
Also has error handling functionality (errno)
====================*/
```

***main.c***
  
Handles running the program and processing terminal input (with fgets and functions from shell_functions.c) as well as exiting the program, also frees any allocated memory
