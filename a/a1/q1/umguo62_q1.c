#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>

#define CASE_SHIFT 32

static const char cat_str[] = {'C','A','T'};

int main(int argc, char *argv[])
{
	//pipe file descriptor
  int pfd[2];	
	char buf;
	int file_dec = open("raw.log", O_WRONLY);	

  int ret_signal = -1;
	char cat_origin_str[3];
	int cat_index = 0;
	
  if(-1 == pipe(pfd)) 
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int pid = fork();

	//check fork error
  if(-1 == pid)
  {
		perror("fork");
		exit(EXIT_FAILURE);	   
  }
	
  if(0 != pid) 
  { //parent process

		close(pfd[1]); //write end is not needed	
		while(read(pfd[0], &buf, 1)>0)
		{
			write(file_dec, &buf, 1);
			if(buf == cat_str[cat_index] || buf == (cat_str[cat_index]+CASE_SHIFT))
			{
				cat_origin_str[cat_index] = buf;
				cat_index++;
				if(cat_index>2)
				{	
					write(STDOUT_FILENO, "dog", 3);
					cat_index = 0;
				}
			}
			else
			{	
				assert(cat_index <3);
				write(STDOUT_FILENO, cat_origin_str, cat_index);
				cat_index = 0;
				write(STDOUT_FILENO, &buf, 1);
			}

		}

		close(pfd[0]);
		write(file_dec, "\n", 1);
		close(file_dec);
  } 
	else
	{ //child process
		close(pfd[0]); //read end is not needed
  	
		//link standard output to write end of pipe
		dup2(pfd[1], STDOUT_FILENO); 	
		ret_signal = execv(argv[1], argv);
		if(-1 == ret_signal)
		{
			write(STDOUT_FILENO, "Error\n", 6);
			close(pfd[1]);
		}
	}

	return 0;
}
