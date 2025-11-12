#include <unistd.h>		// pipe dup2 execvp close exit
#include <sys/types.h>	//fork
#include <stdlib.h>		//fork

#define HIJO 0
#define PIPE_WRITE 1
#define PIPE__READ 0

// int pipe(int pipefd[2]);
// int dup2(int oldfd, int newfd);

int	ft_popen(const char *file, char *const *argv, char type)
{
	int fd_pipe[2];
	pid_t pid;

	if ((type != 'r' && type != 'w') || file == NULL || argv == NULL)
        return (-1);

	if (pipe(fd_pipe) == -1)
        return (-1);

    if (type == 'r')
	{
		pid = fork();
		if (pid == 0)
		{
			close(fd_pipe[PIPE__READ]);
			dup2(fd_pipe[PIPE_WRITE], 1);
			close(fd_pipe[PIPE_WRITE]);
			execvp(file, argv);
			exit(1);
		}
		close(fd_pipe[PIPE_WRITE]);
		return (fd_pipe[PIPE__READ]);
	}

	if (type == 'w')
	{
		pid = fork();
		if (pid == 0)
		{
			close(fd_pipe[PIPE_WRITE]);
			dup2(fd_pipe[PIPE__READ], 0);
			close(fd_pipe[PIPE__READ]);
			execvp(file, argv);
			exit(1);
		}
		close(fd_pipe[PIPE__READ]);
		return (fd_pipe[PIPE_WRITE]);
	};
	return (-1);
}

/* int	ft_popen(const char *file, char *const *argv, char type)
{
	int fd_pipe[2];
	pid_t family;

	if (type != 'r' && type != 'w')
		return (-1);

	if (pipe(fd_pipe))
		return (-1);

	family = fork();
	if (family == -1)
	{
		close(fd_pipe[PIPE__READ]);
		close(fd_pipe[PIPE_WRITE]);
		return (-1);
	}

	if (family == HIJO)
	{
		close(fd_pipe[PIPE__READ]);
		dup2(fd_pipe[PIPE_WRITE], 1);
		close(fd_pipe[PIPE_WRITE]);
		execvp(file, argv);
		exit(1);
	}

	if (type == 'r')
	{	
		close(fd_pipe[PIPE_WRITE]);
		return (fd_pipe[PIPE__READ]);
	}
    else
    {
        close(fd_pipe[PIPE__READ]);
        return (fd_pipe[PIPE_WRITE]);
    }
} */

/* #include <stdio.h> //popen

int main()
{
    int fd;
	char *const args[] = {"ls", NULL};

	fd = ft_popen("ls", args, 'r');
	char buffer[10000];
	int byte_read;
	while (1)
	{
		byte_read = read(fd, buffer, 100);
		if(byte_read <= 0)
			break ;
		buffer[byte_read] = '\0';
		printf("TEST: %s", buffer);
	}
	return (0);
} */

/* int main()
{
    int fd;
	char *const args[] = {"cat", "-e", NULL};

	fd = ft_popen("cat", args, 'w');
	char buffer[10000];
	int byte_read;
	while (1)
	{
		byte_read = read(0, buffer, 100);
		if(byte_read <= 0)
			break ;
		buffer[byte_read] = '\0';
		dprintf(fd, "TEST: %s", buffer);
	}
	return (0);
} */