/*
   umguo62_a1.c
   COMP 3430 Assignment 1

   This program implements a simple shell.
   Features implemented:
     -Searching the PATH environment variable
     -Command-line arguments
     -cd (change directory) command
     -wildcard pattern matching (globbing)
     
   To compile: gcc a1-skeleton.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>

// program constants
#define BUFSIZE 81
#define MAXARGS 100

// define a Boolean type
#define TRUE 1
#define FALSE 0
typedef int BOOLEAN;

// a macro to compare two strings for equality
#define equals(s1,s2) strcmp((s1),(s2)) == 0

int cmd_count = 1;      // print command count with prompt

// global declarations for command line arguments
char buffer[BUFSIZE];    // the input buffer
char *cmdline_args[MAXARGS];

/*
   get_cmd
   Read a command from standard input.
   Remove any leading whitespace.
   Returns: 
     1) the entered command
     2) length of the command (in reference parameter len)
*/
char *get_cmd(int *len){
  char *cmd = NULL;
  printf("%d%% ", cmd_count);
  cmd = fgets(buffer, BUFSIZE, stdin);
  if(!feof(stdin))
	{
    // overwrite newline character with \0
    buffer[strlen(buffer) - 1] = '\0';

    // remove any pesky leading whitespace
    while(*cmd && (*cmd == ' ' || *cmd == '\t'))
		{
      cmd++;
		}

    *len = strlen(cmd);
  }
  return cmd;
}//get_cmd

int main(int argc, char *argv[]){

  int i, pid;
  int ret_code;
  int len;
  char *the_cmd;    // pointer to a command
  
  the_cmd = get_cmd(&len);
  while(!feof(stdin))
	{
    if(len > 0)
		{  // a non-empty command was entered
			cmd_count++;
			pid = fork();
      if(pid)
			{  // parent process
        wait(NULL);
      }
			else
			{  // child process - execute the command
        ret_code = execl(the_cmd, the_cmd, NULL);
        if(ret_code != 0)
				{
          printf("error executing %s.\n", the_cmd);
          exit(TRUE);  // indicate that cmd failed
        }
			}// end else
		}// if(len > 0)

    the_cmd = get_cmd(&len);
  }// while

  printf("\nEnd of processing\n");
  return 0;
} /* end main */
