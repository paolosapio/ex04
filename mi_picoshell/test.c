//close, fork, wait, exit, execvp, dup2, pipe
/*
	int		close(int fd);
	pid_t	fork(void);
	pid_t	wait(int *wstatus);
	void	_exit(int status);
	int		execvp(const char *file, char *const argv[]);
	int		dup2(int oldfd, int newfd);
	int		pipe(int pipefd[2]);
*/


#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define HIJO 0

#define PIPE_FD_READ_OUT 0
#define PIPE_FD_WRITE_IN 1

int main(void)
{
	int pipe_fd[2];

	pipe(pipe_fd);

	printf("%zd\n",write(pipe_fd[PIPE_FD_READ_OUT], "hola", 4));
	return (0);
}
