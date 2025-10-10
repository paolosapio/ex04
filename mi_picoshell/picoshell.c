#include <stdio.h>

int    picoshell(char **cmds[])
{
	return (0);
}

/* size_t	array_len(char **cmd[])
{
	int i = 0;

	while (*cmd[i])
	{
		i++;
	}
		return (i);
} */

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	chr_counter(int argc, char **argv, char delim)
{
	int i = 1;
	int n_pipes = 0;

	while (i < argc)
	{
		if (ft_strcmp(argv[i], delim) == 0)
		{
			n_pipes++;
		}
		i++;
	}
	printf("n_pipes: %d\n", n_pipes);

	return (n_pipes);
}

void cmds_filler(char **argv, char **cmds[], int n_cmds)
{
	int current_cmd;
	int current_arg;

	// n_cmds = 3

	// "ls" "-la" [0, [0 -> 3, 1 -> 4]]
	// "cat" "-e" [1, [0 -> 4, 1 -> 3]]
	// "wc" "-l"  [2, [0 -> 3, 1 -> 3]]
	// NULL       [3]
	
	cmds[n_cmds] = NULL;
	current_cmd = 0;
	current_arg = 0;
	while (argv[current_arg] != NULL)
	{
		while (ft_strncmp(argv[current_arg], "|", 2) == 0)
		{
			cmds[current_cmd][current_arg] = argv[current_arg];
			current_arg++;
		}
		++current_cmd;
	}
}

#define EXTRA_LAST_COMMAND 1
int main(int argc, char **argv, char **env)
{
	(void)env;
	int i = 0;
	int n_cmds = chr_counter(argc, argv, '|') + EXTRA_LAST_COMMAND;
	char **cmds[n_cmds + 1];

	cmds_filler(argv + 1, cmds, n_cmds);
	while (argv[i])
	{
		printf("args:%s\n", argv[i]);
		// printf("cmds:%s\n", *cmds[i]);
		i++;
	}
	// picoshell();
	return (0);
}

/*
separar los comandos que me entran desd ela terminal en agrupaciones de
arrai de strings el separador es estos arrays es el "|"

ejemplo:

si llega ./a.out ls -la "|" cat -e "|" wc -l
el resultado es:

array***:

**array[0] = {"ls", "- la"}
**array[1] = {"cat", "-e"}
**array[2] = {"wc", "-l"}
**array[3] = NULL


*/