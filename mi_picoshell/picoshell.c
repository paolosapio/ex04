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

int one_cmds(char **cmds[])
{
	pid_t	family;
	int		exitcode = 42;

	family = fork();
	if (family == HIJO)
	{
		exitcode = execvp(cmds[0][0], cmds[0]);
		printf("exitcode: [%d]\n", exitcode);

		exit(exitcode);
	}
	// int a = 0;
	while (/* a =  */wait(NULL) >= 0) // wait espera el hijo y se queda con su grito de dolor
	{
		// printf("un hijo a muerto: %d\n", a);
	}
	return (0);
}



/* int one_cmds(char **cmds[])
{
	pid_t family;
	int exitcode = 42;

	printf("cmds[0][0]:%s\n", cmds[0][0]);
	family = fork();
	if (family == HIJO)
	{
		exitcode = execvp(cmds[0][0], cmds[0]);
		printf("[%d]\n", exitcode);
		if (exitcode == -1)
		{
			printf("Error: command failed\n");
			exit (-1);
		}
	}
	int a;
	while (a = wait(NULL) > 0)
	{
		printf("wait(NULL) %d\n", a);
	}
	return (exitcode);
} */

#define FD_READ_OUT 0
#define FD_WRITE_IN 1

int two_cmds(char **cmds[])
{
	int pipe_fd[2];
	pid_t family;

	pipe(pipe_fd);
	family = fork();
	if (family == HIJO)
	{
		dup2(pipe_fd[FD_WRITE_IN], 1);
		close(pipe_fd[FD_WRITE_IN]);
		close(pipe_fd[FD_READ_OUT]);
		execvp(cmds[0][0], cmds[0]);
		exit(1);
	}
	close(pipe_fd[FD_WRITE_IN]);
	family = fork();
	if (family == HIJO)
	{
		dup2(pipe_fd[FD_READ_OUT], STDIN_FILENO);
		close(pipe_fd[FD_WRITE_IN]);
		close(pipe_fd[FD_READ_OUT]);
		printf("ERROR %d\n", execvp(cmds[1][0], cmds[1]));
		exit(1);
	}
	close(pipe_fd[FD_READ_OUT]);

	while(wait(NULL) > 0)
	{
		printf("hola!\n");
	}
	return (0);
}

int picoshell(char **cmds[])
{
	// if	(one_cmds(cmds) == -1)
	// 	return (-1);
	two_cmds(cmds);
	return (0);
}

int main(void)
{
	
	char *cmd_echo[] = {"echo", "hello world", NULL};
    char *cmd_tr[] = {"tr", "a-z", "A-Z", NULL};
    char **cmds2[] = {	cmd_echo, 
						cmd_tr, 
						NULL}; // <----------
	
	char *cmdcaca[] = {"caca", "hello world", NULL};
    char **cmdsERR[] = {cmdcaca, cmd_tr, NULL};

    // printf("\nTest 2: echo hello world | tr a-z A-Z\n");

	// if (picoshell(cmds2) == -1)
    //     fprintf(stderr, "Error: command failed\n");


    if (picoshell(cmds2) == -1)
        fprintf(stderr, "Error: command failed\n");
	// fprintf(stderr, "viva la vida\n");

	/*char *cmd4[] = {"echo", "hello world", NULL};
    char **cmds2[] = {cmd4, NULL};

    printf("\nTest 3: echo hello world\n");
    if (picoshell(cmds2) == 1)
        fprintf(stderr, "Error: command failed\n"); */
	/*
    // Ejemplo 1: ls -l | grep .c | wc -l
    char *cmd1[] = {"ls", "-l", NULL};
    char *cmd2[] = {"grep", ".c", NULL};
    char *cmd3[] = {"wc", "-l", NULL};
    char **cmds1[] = {cmd1, cmd2, cmd3, NULL};

    printf("Test 1: ls -l | grep .c | wc -l\n");
    if (picoshell(cmds1) == 1)
        fprintf(stderr, "Error: command failed\n");

    // Ejemplo 2: echo hello world | tr a-z A-Z
    char *cmd4[] = {"echo", "hello world", NULL};
    char *cmd5[] = {"tr", "a-z", "A-Z", NULL};
    char **cmds2[] = {cmd4, cmd5, NULL};

    printf("\nTest 2: echo hello world | tr a-z A-Z\n");
    if (picoshell(cmds2) == 1)
        fprintf(stderr, "Error: command failed\n");

    // Ejemplo 3: comando inexistente (para probar error)
    char *cmd6[] = {"noexiste", NULL};
    char **cmds3[] = {cmd6, NULL};

    printf("\nTest 3: comando inexistente\n");
    if (picoshell(cmds3) == 1)
        fprintf(stderr, "Error: command failed\n");
*/
    return 0;
}
