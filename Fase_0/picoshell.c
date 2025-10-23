#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define HIJO 0
 #define PIPE_ESCRIBIR 1
 #define PIPE_LEER 0

void picoshell(char **cmds[])
{
	pid_t	family;
	int		exitcode = 0;
	int		fd[2];
	int		input = 0;
	int		output = 1;

	int		i = 0;

	pipe(fd);
	while (cmds[i] != NULL)
	{
		family = fork();
		if (family == -1)
		printf("Error fork");
		if (family == HIJO)
		{
			int	child_output = 1; // si es == 1 imprimo por paantalla (poruqe soy el unico o ultimo comando) y si no doy mi salida al siguente comando;
			dup2(child_output, 1);
			execvp(cmds[i][0], cmds[i]);
			dprintf(2, "Error in %s\n", cmds[i][0]);
			dprintf(2, "exitcode a:%d\n", exitcode);
			exit(1);
		}
		//close(fd[1]);
		//dup2(fd[1], fd[0]);
		i++;
	}
	int a = 0;
	while ((a = wait(NULL)) > 0)
	{
		printf("exitcode b:%d\n", a);
	}
}

void picoshell_simple(char **cmds[])
{
	pid_t	family;
	int		exitcode = 0;
	
	family = fork();
	if (family == -1)
		printf("Error fork");
	if (family == HIJO)
	{
		exitcode = execvp(cmds[0][0], cmds[0]);
		printf("Error in %s\n", cmds[0][0]);
		exit(exitcode);
	}
	else
	{ //mmmm.....
		printf("Finalizo el proceso hijo\n");
		while (wait(NULL) > 0)
		{
		}
	}
	
}

int	main()
{
	char *cmd_echo[] = {"echo", "viva la vida", NULL};
	char *cmd_tr[] = {"tr", "a-z", "A-Z", NULL};
	char *cmd_cat[] = {"cat", "-e", NULL};
	char **cmd_chain[] = {cmd_echo, cmd_tr, NULL};
	char **cmd_chain1[] = {cmd_echo, cmd_cat, NULL};
	
	char *cmd_txt[] = {"I'm text", "hello world", NULL};
	char *cmd_nulls[] = {NULL, NULL, NULL};
	char **cmd_Error[] = {cmd_txt, cmd_nulls, NULL};
	
	// picoshell_simple(cmd_Error);	
	// picoshell_simple(cmd_chain);
	
	picoshell(cmd_chain1);
	// picoshell(cmd_Error);
	
	//printf("El programa llega hasta aqui\n");
	return (0);
}
