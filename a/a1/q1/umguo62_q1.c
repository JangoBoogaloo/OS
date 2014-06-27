#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define READ_FD 0
#define WRITE_FD 1

typedef enum { false, true } bool;

static pid_t pid;
static int pipefd[2];
static sig_atomic_t isRunning = true;
static int logfile;

static void sigHandle(int sigNum)
{
	if (SIGTERM == sigNum)
		isRunning = false;
}

static void closeFiles()
{
	close(logfile);
	close(pipefd[READ_FD]);
	close(pipefd[WRITE_FD]);
}

static void handleError(int ret, const char *msg)
{
	if(ret != -1)
		return;
	perror(msg);
	closeFiles();
	exit(EXIT_FAILURE);
}

void doOutput(int file, char data) {
	write(file, &data, 1);
}

int main(int argc, char *argv[])
{
	logfile = open("log", O_WRONLY | O_CREAT | O_TRUNC, 0700);
	signal(SIGTERM, sigHandle);
	handleError(pipe(pipefd), "Pipe Error");
	printf("Program Starts\n");

	//handleError(dup2(logfile, STDERR_FILENO), "Log File Error");
	pid = fork();

	if(pid == 0)
	{

		close(pipefd[READ_FD]);
		//Child write standard output to pipe
		//dup2(logfile, STDOUT_FILENO);
		dup2(pipefd[WRITE_FD], STDOUT_FILENO);
		execv(argv[1], &argv[1]);
		perror("Error");
	}
	else
	{
		int data;

		close(pipefd[WRITE_FD]);

		while(isRunning && read(pipefd[READ_FD], &data, 1) >0)
			doOutput(STDOUT_FILENO, (char)data);

		printf("Main Finished\n");
		kill(pid, SIGTERM);
		waitpid(pid, NULL, 0); 
		close(pipefd[READ_FD]);
	}
	
	closeFiles();

	return 0;
}
