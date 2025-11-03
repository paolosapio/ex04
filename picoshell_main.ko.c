#include <stdio.h> 
int picoshell(char **cmds[]) { return (0); } size_t array_len(char **cmd[])
{
    int i = 0; 
    while (*cmd[i]) { i++; } return (i); 
} 

int ft_strcmp(const char *s1, const char *s2)
{
    int i; i = 0;
    while (s1[i] && s1[i] == s2[i]) i++;
    return (s1[i] - s2[i]);
}

/* int ft_strncmp(const char *s1, const char *s2, size_t n) { int a; int b; unsigned char *s1cast; unsigned char *s2cast; s1cast = (unsigned char *)s1; s2cast = (unsigned char *)s2; a = 0; b = 0; while ((n != 0) && (s1[a] != '\0') && (s2[b] != '\0')) { if (s1cast[a] != s2cast[b]) return (s1cast[a] - s2cast[b]); a++; b++; n--; } if (n == 0) return (0); return (s1cast[a] - s2cast[b]); } */ int cmds_counter(int argc, char **argv, char pipe) { int i = 1; int n_pipes = 0; while (i < argc) { if (ft_strcmp(argv[i], &pipe) == 0) { n_pipes++; } i++; } return (n_pipes); } int get_largest_cmds(char **argv) { argv++; int i = 0; int count_a = 0; int count_total = 0; while (argv && argv[i]) { if (ft_strcmp(argv[i], "|") == 0) count_a = 0; if (count_a >= count_total) count_total = count_a; count_a++; i++; } return (count_total); } #include <stdlib.h> // for exit void cmds_filler(char **argv, int n_cmds, int largest_cmds_count, char *cmds[][largest_cmds_count]) { int current_cmd; int current_arg; int i = 0; *cmds[n_cmds] = NULL; current_cmd = 0; current_arg = 0; while (argv[current_arg] != NULL) { if (ft_strcmp(argv[current_arg], "|") == 0) { cmds[current_cmd][i] = NULL; current_arg++; current_cmd++; i = 0; continue ; } cmds[current_cmd][i] = argv[current_arg]; printf("cmds[%d][%d] %s\n", current_cmd, i,cmds[current_cmd][i]); current_arg++; i++; } cmds[current_cmd][i] = NULL; printf("\n"); printf("cmds_fill[0][2] %s\n", cmds[0][2]); printf("\n"); exit(1); } #define EXTRA_LAST_COMMAND 1 // ../a.out ls -j -k "|" cat "|" wc -l int main(int argc, char **argv) { int n_cmds = cmds_counter(argc, argv, '|') + EXTRA_LAST_COMMAND; int largest_cmds_count = get_largest_cmds(argv); char *cmds[n_cmds + 1][largest_cmds_count]; cmds_filler(argv + 1, n_cmds, largest_cmds_count, cmds); int current_cmd = 0; int current_arg = 0; printf("cmds[0][2] %s\n\n", cmds[0][2]); while (cmds[current_cmd] != NULL) { current_arg = 0; while (cmds[current_cmd][current_arg] != NULL) { printf("from main -> cmds[%d][%d]: %s\n", current_cmd, current_arg, cmds[current_cmd][current_arg]); current_arg++; } current_cmd++; } // picoshell(); return (0); } /* separar los comandos que me entran desd ela terminal en agrupaciones de arrai de strings el separador es estos arrays es el "|" ejemplo: si llega ./a.out ls -la "|" cat -e "|" wc -l el resultado es: array***: **array[0] = {"ls", "- la"} **array[1] = {"cat", "-e"} **array[2] = {"wc", "-l"} **array[3] = NULL */