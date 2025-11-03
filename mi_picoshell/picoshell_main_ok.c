#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXTRA_LAST_COMMAND 1

// -----------------------------------------------------
// FUNZIONI UTILI
// -----------------------------------------------------

int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

// Conta le pipes → numero di comandi = n_pipes + 1
int cmds_counter(int argc, char **argv)
{
    int i = 1;
    int n_pipes = 0;

    while (i < argc)
    {
        if (argv[i][0] == '|' && argv[i][1] == '\0')
            n_pipes++;
        i++;
    }
    return n_pipes;
}

// Trova il numero massimo di argomenti di un comando
int get_largest_cmds(char **argv)
{
    int i = 1;
    int count = 0;
    int max = 0;

    while (argv[i])
    {
        if (argv[i][0] == '|' && argv[i][1] == '\0')
            count = 0;
        else
            count++;
        if (count > max)
            max = count;
        i++;
    }
    return max + 1; // +1 per il NULL finale di ogni comando
}

// -----------------------------------------------------
// FUNZIONE PER POPOLARE L'ARRAY DI COMANDI
// -----------------------------------------------------

void cmds_filler(char **argv, int n_cmds, int largest_cmds_count, char *cmds[][largest_cmds_count])
{
    int current_cmd = 0;
    int current_arg = 0;
    int i = 0;

    while (argv[current_arg] && current_cmd < n_cmds)
    {
        if (argv[current_arg][0] == '|' && argv[current_arg][1] == '\0')
        {
            cmds[current_cmd][i] = NULL;
            current_cmd++;
            current_arg++;
            i = 0;
            continue;
        }

        if (i >= largest_cmds_count - 1)
        {
            fprintf(stderr, "Warning: command %d has too many args (max %d)\n",
                current_cmd, largest_cmds_count - 1);
            // salta fino alla pipe
            while (argv[current_arg] && !(argv[current_arg][0] == '|' && argv[current_arg][1] == '\0'))
                current_arg++;
            continue;
        }

        cmds[current_cmd][i] = argv[current_arg];
        i++;
        current_arg++;
    }

    // Chiudi l'ultimo comando
    cmds[current_cmd][i] = NULL;

	printf("cmds[0][2] %s\n\n", cmds[0][2] );
    // Sentinella finale per l'array di comandi
    if (current_cmd + 1 <= n_cmds)
        cmds[current_cmd + 1][0] = NULL;
}

// -----------------------------------------------------
// MAIN
// -----------------------------------------------------

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s cmd [args] \"|\" cmd2 [args] ...\n", argv[0]);
        return 1;
    }

    int n_cmds = cmds_counter(argc, argv) + EXTRA_LAST_COMMAND;
    int largest_cmds_count = get_largest_cmds(argv);

    // Array VLA sullo stack
    char *cmds[n_cmds + 1][largest_cmds_count];

    // Popola l'array di comandi
    cmds_filler(argv + 1, n_cmds, largest_cmds_count, cmds);

    // Stampa dei comandi per debug
    int current_cmd = 0;
    while (current_cmd < n_cmds && cmds[current_cmd][0] != NULL)
    {
        int current_arg = 0;
        while (cmds[current_cmd][current_arg] != NULL)
        {
            printf("cmds[%d][%d] = %s\n", current_cmd, current_arg, cmds[current_cmd][current_arg]);
            current_arg++;
        }
        current_cmd++;
    }

    return 0;
}
