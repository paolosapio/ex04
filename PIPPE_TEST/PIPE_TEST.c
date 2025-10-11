#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int dos_fd[2];

/*  dos_fd[0] = open("pipe", O_WRONLY);
    dos_fd[1] = open("pipe", O_RDONLY); */
    char frase[16];


    pipe(dos_fd);
    printf("primero fd%d\nsegundo fd%d\n", dos_fd[0], dos_fd[1]);

    write(dos_fd[1], "HOLA YOLANDA \n", 15);
    read(dos_fd[0], frase, 15);

    printf("la frase leida es:%s", frase);

    return (0);
}