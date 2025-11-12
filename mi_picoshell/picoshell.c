#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define HIJO 0

#define OUT_READ 0
#define IN_WRITE 1

int num_of_cmds(char **cmds[])
{
	int i = 0;
	
	while (cmds[i])
		i++;
	return (i);
}

int	picoshell(char **cmds[])
{
	int		previus_pipe_read_end = -1;
	int		pipe_fds[2];
	pid_t	family;
	int		n_cmds = num_of_cmds(cmds);
	int		wait_status;
	int		i = 0;

	while (cmds[i] != NULL) // WHILE GENERAL PARA CADA COMANDO
	{
		if (i < n_cmds - 1) // Si no es el último comando, creamos un nuevo pipe
		{
			if (pipe(pipe_fds) == -1) //inizialisamos pipe y control error
			{
				perror("pipe error");
				return (1);
			}
		}

		family = fork();
		if (family == -1)
		{
			perror("fork error");
			return (1);
		}

		// ====== HIJO ======
		if (family == HIJO)
		{
			// fprintf(stderr, "Ejecutando hijo %d: %s\n", i, cmds[i][0]);

			// Si no es el primer comando → redirige stdin desde pipe anterior
			if (i > 0)
			{
				dup2(previus_pipe_read_end, STDIN_FILENO);
				close(previus_pipe_read_end);
			}

			// Si no es el último comando → redirige stdout al pipe nuevo
			if (i < n_cmds - 1)
			{
				close(pipe_fds[OUT_READ]);
				dup2(pipe_fds[IN_WRITE], STDOUT_FILENO);
				close(pipe_fds[IN_WRITE]);
			}

			// Ejecuta el comando
			execvp(cmds[i][0], cmds[i]);
			perror("execvp error");
			exit(EXIT_FAILURE);
		}

		// ====== PADRE ======
		if (i > 0) //significa que no es el primer comando
		
			close(previus_pipe_read_end); // ya no se necesita el pipe anterior

		if (i < n_cmds - 1) // significa hasta el ultimo comando
		{
			close(pipe_fds[IN_WRITE]); // cerramos el write del pipe actual
			previus_pipe_read_end = pipe_fds[OUT_READ]; // guardamos el nuevo read
		}

		i++;
	}

	i = 0;
	while(i++ < n_cmds)
	{
		wait(&wait_status);
		if (WIFEXITED(wait_status))
		{
			// printf("hola pepe\n");
			return (WEXITSTATUS(wait_status));
		}
		if (WIFSIGNALED(wait_status))
		{
			// printf("hola alvaro\n");
			return (WTERMSIG(wait_status));
		}
	}
	return (0);
}