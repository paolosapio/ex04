#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
 #include <string.h>

 #define PIPE_ESCRIBIR 1
 #define PIPE_LEER 0
int test_pipe(char **cdms[])
{
	int fd_pipe[2];
	char readed[5];
	pid_t family;
	
	pipe(fd_pipe);
	family = fork();
	if (family == 0)
	{
		dup2(fd_pipe[PIPE_ESCRIBIR], 1);
		execvp(cdms[0][0], cdms[0]);
		fprintf(stderr, "hola dan\n");
	}
	fprintf(stderr, "devuelto read: %zd\n", read(fd_pipe[PIPE_LEER], readed, strlen(cdms[0][1])));
	fprintf(stderr, "leido: %s\n", readed);
	return (0);
}

int	main()
{
	char *cmd_echo[] = {"echo", "hello world", NULL};
	char *cmd_tr[] = {"tr", "a-z", "A-Z", NULL};
	char *cmd_ls[] = {"ls", "-l", NULL};
	char **cmd_chain[] = {cmd_echo, cmd_tr, NULL};
	char **cmd_chain1[] = {cmd_echo, cmd_ls, NULL};
	
	char *cmd_txt[] = {"I'm text", "hello world", NULL};
	char *cmd_nulls[] = {NULL, NULL, NULL};
	char **cmd_Error[] = {cmd_txt, cmd_nulls, NULL};
	
	test_pipe(cmd_chain);	
	// picoshell_simple(cmd_chain);
	
	// picoshell(cmd_chain);
	// picoshell(cmd_chain1);
	
	//printf("El programa llega hasta aqui\n");
	return (0);
}