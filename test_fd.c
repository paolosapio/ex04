#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *const args[] = {"ls", NULL};

	printf("%d", execvp("ls", args));
}