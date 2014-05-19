/*
	shell.c
	COMP 3430 Operating Systems
	Lab 1
	
	Simple shell program.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFSIZE 81

int main(int argc, char *argv[]){

  int pid;
  int ret_code;		// return code
  int len;		// length of entered command
  char buffer[BUFSIZE];	// room for 80 chars plus \0
  char *cmd;		// pointer to entered command

  // Print a prompt and read a command from standard input
  printf(" > ");
  cmd = fgets(buffer, BUFSIZE, stdin);
  while(cmd != NULL){
    // check for the newline character and overwrite with \0
    len = strlen(buffer);
    if(buffer[len-1] == '\n')
      buffer[len-1] = '\0';
    pid=fork();  // fork child process to execute cmd
    if(pid!=0){
      /* parent process waits here */
			printf("Parent Wait\n");

      wait(NULL);
    } else {
      /* child process executes here */
      ret_code = execl(cmd, cmd, NULL);
      if(ret_code != 0)
        printf("%s.\n", strerror(errno));
      exit(0);
    }
    
    printf(" > ");
    cmd = fgets(buffer, BUFSIZE, stdin);
  } /* end while */
  printf("All done.\n");
	return 0;
} /* end main */
