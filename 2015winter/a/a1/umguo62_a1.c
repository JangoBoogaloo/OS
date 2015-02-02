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

#define SEPERATION " "

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
     2) p_length of the command (in reference parameter p_len)
*/
char *get_cmd(int *p_len)
{
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

    *p_len = strlen(cmd);
  }
  return cmd;
}

int check_wildcard(char *arguments[], 
									 int arg_count,
									 glob_t *p_glob)
{
	int hasWild = FALSE;
	p_glob->gl_offs = 0;

	for(int i=0; i<arg_count && NULL !=arguments[i]; i++)
	{
		for(int j=0; '\0'!=arguments[i][j]; j++)
		{
			if('?' == arguments[i][j] || 
				 '*' == arguments[i][j] || 
				 '[' == arguments[i][j])
			{
				hasWild = TRUE;
				if(0 != p_glob->gl_offs)
				{ // append additional wildcard
					glob(arguments[i], GLOB_DOOFFS | GLOB_APPEND, NULL, p_glob);
				}
				else	
				{ // first wild card
					p_glob->gl_offs = i;
					glob(arguments[i], GLOB_DOOFFS, NULL, p_glob);
					//assign all previous non-wildcard variable
					for(int k=0; k<i; k++)
					{
						p_glob->gl_pathv[k] = arguments[k];
					}
				}
				
			}
		}
	}
	return hasWild;
}

int main(int argc, char *argv[])
{

  int i = -1;
	int pid = -1;
  int ret_code = -1;
  int len = -1;
  char *the_cmd = NULL;    // pointer to a command
	char home_dir[81] = {'\0'};
	char *arguments[MAXARGS]= {NULL};
	char *brkt = NULL;

	//preprocess, get process home directory
	if (getcwd(home_dir, sizeof(home_dir)) == NULL)
	{
		printf("Error executing getcwd\n");
	}

  the_cmd = get_cmd(&len);
  while(!feof(stdin))
	{
    if(len > 0)
		{  // a non-empty command was entered
			cmd_count++;

			pid = fork();
      if(pid)
			{  // parent process
				//wait for child process to finish
        wait(NULL);
      }
			else
			{  // child process - execute the command
				i=0;

				//parsing command arguments
				for(arguments[i] = strtok_r(the_cmd, SEPERATION, &brkt);
						arguments[i]; 
						arguments[++i] = strtok_r(NULL, SEPERATION, &brkt));

				if(equals(arguments[0], "cd"))
				{
					if('\0' == arguments[1])
					{//arguments[1] empty, go to home directory
						ret_code = chdir(home_dir);
					}
					else
					{
						ret_code = chdir(arguments[1]);
					}
				}
				else
				{
					glob_t g = {0};
					if(check_wildcard(arguments, MAXARGS, &g))
					{
						ret_code = execvp(arguments[0], g.gl_pathv);
					}
					else
					{
        		ret_code = execvp(the_cmd, arguments);
					}
				}

        if(ret_code != 0)
				{
          printf("error executing %s.\n", the_cmd);
        	exit(TRUE);  // indicate that cmd failed
        }
			}

		}

    the_cmd = get_cmd(&len);
  }

  printf("\nEnd of processing\n");
  return 0;
} 
