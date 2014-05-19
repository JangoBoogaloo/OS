/**********************************************************************
  Module: q1.c
  Author: Jim Young
  Date:   2013 Jan 3

  Purpose: To execute a program specified via the command line and to
		modify it's ouput to replace all instances of 'cat' with 'dog'.

  Changes: 
    2013 Jan 3 [ Jim Young ]
      - initial version

  NOTES: no output is buffered so for heavy-output applications this will
         not be efficien.
**********************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

/* booleans setup */
#ifndef TRUE
  #define TRUE 1
#endif

#ifndef FALSE
  #define FALSE 0
#endif

/* simple constants to help make code more redable */
static int const READ_END = 0; /* read/write #s for pipe */
static int const WRITE_END = 1;
static int pipe_fd[2]; /* holds interprocess pipe */

static pid_t childPid; 

static int const MIN_ARGS = 2;
static int const EXEC_ARG = 1;

static int const RETURN_OK = 0;
static char const * const LOGFILE = ".spy.txt";

static char const* FIND = "cat";
static char const* REPLACE = "dog";

static sig_atomic_t running = TRUE; /* so signals can kill us gracefully*/

static int posInSearch = 0; /* keep track of where we are in FIND */



/* searches for FIND and puts REPLACE, otherwise a passthrough
	if it looks like FIND a byte isn't tossed out each time, we wait if it looks like a token is found 
*/
void doOutput(int file, char data) {
	if (toupper(data) == toupper(FIND[posInSearch])) { /* looks like a token */
		posInSearch++;
		if (posInSearch == strlen(FIND)) { // found it!!
			write(file, REPLACE, strlen(REPLACE));
			posInSearch = 0;
		}

	} else if (posInSearch != 0) { /* not FIND, and we were mid search, back out!! */
		write(file, FIND, posInSearch); /* write out witheld bytes.. */
		write(file, &data, 1);
		posInSearch = 0;

	} else { /* nothing found, just output  */
		write(file, &data, 1);
	}
}


/* Handles SIGTERM - allows for clean exit on kill */
void handler(int signum) {
	if (SIGTERM == signum)
		running = 0;
}

/* Do child code- run process in argv[EXEC_ARG], and setup the pipes so that
   its stdout now points to my pipe descriped in pipe_fd */
void runChild(int argc, char *argv[]) {
	close(pipe_fd[READ_END]);
	dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
	execv(argv[EXEC_ARG], &argv[EXEC_ARG]);
	fprintf(stderr,"Exec error on %s\n", argv[EXEC_ARG]); /* unreachable */
	perror("Error:"); /* only reach here on error */
}


/* do parent code - listen on pipe from child's output, and modify it to 
   replace all instances of FIND with REPLACE */
void runParent() {	
	int data;
	int outfile;

	close(pipe_fd[WRITE_END]);
	outfile = open(LOGFILE, O_WRONLY | O_CREAT | O_TRUNC, 0700);
	if (outfile == -1)
		perror ("Error opening log file");
	else {	
		while (running && read(pipe_fd[READ_END], &data, 1) > 0) { /* 0 on read means EOF */
			doOutput(STDOUT_FILENO, (char)data);
			write(outfile, &data, 1);
		}
	}
		
	/*cleanup. kill child and close files*/
	kill(childPid, SIGTERM);
	waitpid(childPid, NULL, 0); /* can hang us if the kill was unsuccessful */
	if (outfile != -1)
		close(outfile);
	close(pipe_fd[READ_END]);
}


int main(int argc, char *argv[]) {

	signal(SIGTERM, handler);

	if (MIN_ARGS <= argc) {
		pipe(pipe_fd);
		childPid = fork();

		if (0 == childPid) /* Child */
			runChild(argc, argv);
		else /* parent */
			runParent();
		

	} else  /* wrong usage */
		fprintf(stderr, "Usage: %s <executable>\n", argv[0]);

	return RETURN_OK;
}
