#include <unistd.h>		// pipe dup2 execvp close exit
#include <sys/types.h>	//fork
#include <stdlib.h>		//fork

int ft_popen(const char *file, char *const *argv, char type)
{
	int fd_pipe[2];
	pid_t pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);

	if (pipe(fd_pipe) == -1)
		return (-1);

	pid = fork();
	if (pid < 0)
		return (-1);

	if (pid == 0) // hijo
	{
		if (type == 'r')
		{
			close(fd_pipe[0]);
			dup2(fd_pipe[1], STDOUT_FILENO);
		}
		else // 'w'
		{
			close(fd_pipe[1]);
			dup2(fd_pipe[0], STDIN_FILENO);
		}
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		execvp(file, argv);
		perror("execvp"); // debug si falla
		exit(1);
	}

	// padre
	if (type == 'r')
		close(fd_pipe[1]);
	else
		close(fd_pipe[0]);

	return (type == 'r' ? fd_pipe[0] : fd_pipe[1]);
}
