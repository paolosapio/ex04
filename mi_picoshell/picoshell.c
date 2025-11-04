
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define HIJO 0
#define FD_READ_OUT 0
#define FD_WRITE_IN 1


int num_of_cmds(char **cmds[])
{
	int i = 0;
	
	while (cmds[i])
		i++;
	return (i);
}


int	picoshell(char **cmds[])
{
	int		pipe_fd[2];
	int		restore_system_fd[2];
	pid_t	family;
	int		n_cmds = num_of_cmds(cmds);
	int		wait_status;
	int		i = 0;

	printf("numero comandos: %d\n", n_cmds);	
	while (cmds[i])
	{
		if(pipe(pipe_fd) == -1)
			return (1);
		family = fork();
		if (family == -1)
			return (1);

		//HIJOS:
		if (family == HIJO)
		{
			int my_output;
			//caso primer hijo:
			/* if (i == 0)
			{
				my_output = pipe_fd[1];
				dup2(pipe_fd[FD_READ_OUT], 0);
				dup2(my_output, 1);
				close(pipe_fd[FD_WRITE_IN]);
				close(pipe_fd[FD_READ_OUT]);
				execvp(cmds[i][0], cmds[i]);
				exit(127); 
			} */

			//caso ultimo hijo:
			if (i  == n_cmds - 1)
			{
				my_output = 1;
				dup2(pipe_fd[FD_READ_OUT], 0);
				dup2(my_output, 1);
				close(pipe_fd[FD_WRITE_IN]);
				close(pipe_fd[FD_READ_OUT]);
				execvp(cmds[i][0], cmds[i]);
				exit(127);
			}else {
				my_output = restore_system_fd[1];
				dup2(pipe_fd[FD_READ_OUT], 0);
				dup2(my_output, 1);
				close(pipe_fd[FD_WRITE_IN]);
				close(pipe_fd[FD_READ_OUT]);
				execvp(cmds[i][0], cmds[i]);
				exit(127); 
		}
	}
		close(pipe_fd[1]);
		dup2(pipe_fd[0], restore_system_fd[0]);
		i++;
	}
	i = 0;
	while(i++ < n_cmds)
	{
		wait(&wait_status);

		if (WIFEXITED(wait_status))
		{
			exit(WEXITSTATUS(wait_status));
		}

		if (WIFSIGNALED(wait_status))
		{
			exit(WTERMSIG(wait_status));
		}
	}
}
























/* 
void picoshell(char **cmds[]) // dos comandos
{
	int		pipe_fd[2];
	pid_t	family;
	int		n_cmds = num_of_cmds(cmds);
	int		wait_status;
	
	pipe(pipe_fd);
	family = fork();
	if (family == HIJO)
	{
		close(pipe_fd[FD_READ_OUT]);
		dup2(pipe_fd[FD_WRITE_IN], 1);
		close(pipe_fd[FD_WRITE_IN]);
		execvp(cmds[0][0], cmds[0]);
		exit(127);
	}
	close(pipe_fd[FD_WRITE_IN]);
	family = fork();
	if (family == HIJO)
	{
		close(pipe_fd[FD_WRITE_IN]);
		dup2(pipe_fd[FD_READ_OUT], STDIN_FILENO);
		close(pipe_fd[FD_READ_OUT]);
		execvp(cmds[1][0], cmds[1]);
		exit(127);
	}
	close(pipe_fd[FD_READ_OUT]);
	int i = 0;
	
	while(i++ < n_cmds)
	{
		wait(&wait_status);

		if (WIFEXITED(wait_status))
		{
			exit(WEXITSTATUS(wait_status));
		}

		if (WIFSIGNALED(wait_status))
		{
			exit(WTERMSIG(wait_status));
		}
	}
} */

/* Hazlo tú: escribe el bucle while (wait(&status) > 0) completo, manejando los dos casos:
si el proceso terminó normalmente (WIFEXITED)
si terminó por señal (WIFSIGNALED)
y mostrando (por ejemplo) el código de salida o el número de señal. */
// Allowed functions:	close, fork, wait, exit, execvp, dup2, pipe

int main(void)
{
	char *cmd_echo[] = {"echo", "hello world", NULL};
    char *cmd_tr[] = {"tr", "a-z", "A-Z", NULL};
	char *cmd_cat[] = {"cat", "-e", NULL};

    char **cmds2[] = {	cmd_echo,
						cmd_tr,
						NULL};

    char **cmds3[] = {	cmd_echo,
						cmd_tr,
						cmd_cat,
						NULL};
	picoshell(cmds2);
	
	char *cmdcaca[] = {"caca", "hello world", NULL};
    char **cmdsERR[] = {cmdcaca, NULL};

    // printf("\nTest 2: echo hello world | tr a-z A-Z\n");

	// if (picoshell(cmds2) == -1)
    //     fprintf(stderr, "Error: command failed\n");


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
