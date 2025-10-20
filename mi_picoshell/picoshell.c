
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>

int picoshell(char **cmds[])
{
	return (0);
}



close, fork, wait, exit, execvp, dup2, pipe
/*
	int		close(int fd);
	pid_t	fork(void);
	pid_t	wait(int *wstatus);
	void	_exit(int status);
	int		execvp(const char *file, char *const argv[]);
	int		dup2(int oldfd, int newfd);
	int		pipe(int pipefd[2]);
*/

