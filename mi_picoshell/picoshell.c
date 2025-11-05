
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define HIJO 0

#define FD_READ_OUT 0
#define FD_WRITE_IN 1

#define BKP_STDIN 10
#define BKP_STDOUT 11

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
	int		pipe_child[2];
	int		bkp_sys_fd[2];
	pid_t	family;
	int		n_cmds = num_of_cmds(cmds);
	int		wait_status;
	int		i = 0;

	// Hacemos copia de los descriptores originales del sistema
	dup2(STDIN_FILENO, 10);
	dup2(STDOUT_FILENO, 11);
	bkp_sys_fd[BKP_STDIN] = 10;
	bkp_sys_fd[BKP_STDOUT] = 11;

	fprintf(stderr, "numero comandos: %d\n", n_cmds);

	while (cmds[i])
	{
		// Si no es el último comando, creamos un nuevo pipe
		if (i < n_cmds - 1)
		{
			if (pipe(pipe_child) == -1)
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
			fprintf(stderr, "Ejecutando hijo %d: %s\n", i, cmds[i][0]);
			fflush(stderr);

			// Si no es el primer comando → redirige stdin desde pipe anterior
			if (i > 0)
			{
				dup2(previus_pipe_read_end, STDIN_FILENO);
				close(previus_pipe_read_end);
			}

			// Si no es el último comando → redirige stdout al pipe nuevo
			if (i < n_cmds - 1)
			{
				close(pipe_child[FD_READ_OUT]);
				dup2(pipe_child[FD_WRITE_IN], STDOUT_FILENO);
				close(pipe_child[FD_WRITE_IN]);
			}

			// Ejecuta el comando
			if (execvp(cmds[i][0], cmds[i]) == -1)
			{
				perror("execvp error");
				exit(EXIT_FAILURE);
			}
		}

		// ====== PADRE ======
		if (i > 0)
			close(previus_pipe_read_end); // ya no se necesita el pipe anterior

		if (i < n_cmds - 1)
		{
			close(pipe_child[FD_WRITE_IN]); // cerramos el write del pipe actual
			previus_pipe_read_end = pipe_child[FD_READ_OUT]; // guardamos el nuevo read
		}

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

// Allowed functions:	close, fork, wait, exit, execvp, dup2, pipe

int main(void)
{
    char *cmd_ls[] = {"ls", NULL};
    char *cmd_wc[] = {"wc", "-l", NULL};
    char **cmds2[] = {cmd_ls, cmd_wc, NULL};

    // debug
    printf("cmds2[0][0] = %s\n", cmds2[0][0]);
    printf("cmds2[1][0] = %s\n", cmds2[1][0]);
    printf("cmds2[1][1] = %s\n", cmds2[1][1]);
    printf("cmds2[2] = %p (debería ser NULL)\n", (void*)cmds2[2]);

    printf("\n==== TEST ls | wc -l ====\n");
    picoshell(cmds2);
    return 0;

/*     // test largo: 5 pipes seguidos
    char *cmd_a[] = {"seq", "-10", "10", NULL};
    char *cmd_b[] = {"sort", "-R", NULL};
    char *cmd_c[] = {"tail", "-n", "7", NULL};
    char *cmd_d[] = {"cat", "-e", NULL};
    char *cmd_e[] = {"cat", "-e", NULL};
    char **cmds5[] = {cmd_a, cmd_b, cmd_c, cmd_d, cmd_e, NULL};
    printf("\n==== TEST 5 cmds ====\n");
    picoshell(cmds5); */
    return 0;
}

/* 	char *cmd_echo[] = {"echo", "viva la vida", NULL};
    char *cmd_tr[] = {"tr", "a-z", "A-Z", NULL};
	char *cmd_cat[] = {"cat", "-e", NULL};

    char **cmds2[] = {	cmd_echo,
						cmd_tr,
						NULL};

    char **cmds3[] = {	cmd_echo,
						cmd_tr,
						cmd_cat,
						cmd_cat,
						cmd_cat,
						cmd_cat,
						cmd_cat,
						cmd_cat,
						NULL};

	picoshell(cmds3); */
	
/* 	char *cmdcaca[] = {"caca", "hello world", NULL};
    char **cmdsERR[] = {cmdcaca, NULL};
 */
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

