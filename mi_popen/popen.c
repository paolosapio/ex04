#include <unistd.h>		// pipe dup2 execvp close exit
#include <sys/types.h>	//fork
#include <stdlib.h>		//fork

// int pipe(int pipefd[2]);
// int dup2(int oldfd, int newfd);

int	ft_popen(const char *file, char *const *argv, char type)
{
	int fd_pipe[2];
	pid_t pid;

	// if (type != 'r' || type != 'w') || file == NULL || argv[][])

	pipe(fd_pipe);
	if (type == 'r')
	{
		pid = fork();
		if (pid == 0)
		{
			close(fd_pipe[0]);
			dup2(fd_pipe[1], 1);
			execvp(file, argv);
			exit(1);
		}
		close(fd_pipe[1]);
		return (fd_pipe[0]);
	}


	if (type == 'w')
	{
		pid = fork();
		if (pid == 0)
		{
			close(fd_pipe[1]);
			dup2(fd_pipe[0], 0);
			execvp(file, argv);
			exit(1);
		}
		close(fd_pipe[0]);
		return (fd_pipe[1]);
	};
	return (-1);
}

#include <stdio.h> //popen

// int main()
// {
//     int fd;
// 	char *const args[] = {"ls", NULL};

// 	fd = ft_popen("ls", args, 'r');
// 	char buffer[10000];
// 	int byte_read;
// 	while (1)
// 	{
// 		byte_read = read(fd, buffer, 100);
// 		if(byte_read <= 0)
// 			break ;
// 		buffer[byte_read] = '\0';
// 		printf("TEST: %s", buffer);
// 	}
// 	return (0);
// }

int main()
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
}