/*
Assignment name		:	sandbox
Expected files		:	sandbox.c
Allowed functions	:	fork, waitpid, exit, alarm, sigaction, kill, printf, strsignal, errno
===============================================================================

Write the following function:

#include <stdbool.h>
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)

This function must test if the function f is a nice function or a bad function,
you will return 1 if f is nice , 0 if f is bad or -1 in case of an error in
your function.
!TRADUCIDO EN SAPIESCO:
!    la sandbox detecta si la funcion recibida es buena o mala:
!        return (1)  si OK:
!        return (0)  si KO;
!        return (-1) si falla algo en mi funcion SANDBOX (porejemplo argumentos malos);

A function is considered bad if it is terminated or stopped by a signal
(segfault, abort...), if it exit with any other exit code than 0 or if it
times out.
!TRADUCIDO EN SAPIESCO:
!    la funcion se considera mala si:
!        es parada da una sgnal tipo segfoult, abort, ect:
!        si sale con exit y no es 0;
//!        si se qeuda atascada (timeout);

If verbose is true, you must write the appropriate message among the following:
!TRADUCIDO EN SAPIESCO:
!   si verbose es true tenemos que escribir los siguentes mensajes:
        "Nice function!\n"
        "Bad function: exited with code <exit_code>\n"
        "Bad function: <signal description>\n"
        "Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using
wait).

We will test your code with very bad functions.
*/


/* 
Parámetros:

pid → indica qué hijo esperar:
        > 0 → espera al hijo con ese PID exacto.
        -1 → espera a cualquier hijo (igual que wait()).
        < -1 → espera a cualquier hijo cuyo PGID = |pid|.
        0 → espera a cualquier hijo del mismo grupo de procesos que el padre.

        status → puntero a entero donde se guarda el estado de salida del hijo.
        Después puedes usar macros para interpretarlo:
        WIFEXITED(status) → ¿el hijo terminó normalmente?
        WEXITSTATUS(status) → valor que devolvió exit().
        WIFSIGNALED(status) → ¿el hijo fue terminado por señal?
        WTERMSIG(status) → qué señal lo mató.
        WIFSTOPPED(status) → ¿el hijo fue detenido (stop)?
        WSTOPSIG(status) → señal que lo detuvo.
        
        options → controla el comportamiento:
        0 → espera bloqueante, hasta que el hijo cambie de estado.
        WNOHANG → no bloqueante, devuelve 0 si no hay hijo terminado aún.
        WUNTRACED → también retorna si el hijo fue detenido por señal.
        WCONTINUED → también retorna si el hijo fue continuado después de stop.
        */
       //debemos comprobar la *f si esta bien o mal
       
       
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t family = fork();
    pid_t return_proceso_hijo;
    int status;
    int exit_code;

	if (family == -1)
		return (-1);
	
	if (family == 0) //en el hijo
	{
		alarm(timeout);
		f();
		exit(0);
	}

	return_proceso_hijo = waitpid(family, &status, 0);
	if (return_proceso_hijo == -1)
	{
		if (errno == EINTR)
		{
			kill(family, SIGKILL);
			waitpid(family, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
		return (-1);
	}

	if (WIFEXITED(status)) //si ha salido con un exit
	{
		exit_code = WEXITSTATUS(status);
		if (exit_code == 0)
		{
			if (verbose == true)
				printf("Nice function!\n");
			return (1);
		}
        if (verbose)
		    printf("Bad function: exited with code %d\n", exit_code);
		return (0);
	}
	if (WIFSIGNALED(status)) //si ha salido con una signal
	{
		if (WTERMSIG(status) == SIGALRM)
		{
			if (verbose == true)
				printf("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
		if (verbose == true)
			printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
		return (0);
	}

	return (-1);
}

/* 
void printer_caca(void)
{

    while (1)
    {}

    // char *caca = NULL;
    write(1, "\n", 1);
    write(1, "💩💩💩💩💩\n", 22);
    write(1, "💩 caca 💩\n", 15);
    write(1, "💩💩💩💩💩\n", 22);
    write(1, "\n", 1);
    // printf("%c", caca[10]); //!provoca un segfault
    // exit(99);
}

#include <stdio.h>

// fork, waitpid, exit, alarm, sigaction, kill, printf, strsignal, errno
int main(void)
{

    bool            verbose = false;
    unsigned int    timeout = 3;
    int             result;

    result = sandbox(printer_caca, timeout, verbose);
    printf("result sandbox is: %d\n", result);
    return (0);
}
 








 vieja version:

 // sandbox.c
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose){
  pid_t child;
  int status;
  int res;

  (void)verbose;
  child = fork();
  if(child < 0) return -1;
  if(child == 0){
    alarm(timeout);
    f();
    exit(0);
  }
  
  alarm(timeout);
  res = waitpid(child, &status, 0);
  if(res == -1){
    return -1;
  }
  if(res == child){
    if(WIFEXITED(status)){
      if(WEXITSTATUS(status) == 0)
        return(printf("Nice function!\n"), 1);
      else if(WEXITSTATUS(status) == SIGALRM)
        return 
      printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
      return 0;
    }
    else if(WIFSIGNALED(status)){
      printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
      return 0;
    }
  }
  return -1;
}


void printer_caca(void)
{

    while (1)
    {}

    char *caca = NULL;
    write(1, "\n", 1);
    write(1, "💩💩💩💩💩\n", 22);
    write(1, "💩 caca 💩\n", 15);
    write(1, "💩💩💩💩💩\n", 22);
    write(1, "\n", 1);
    // printf("%c", caca[10]); //!provoca un segfault
    // exit(99);
}

#include <stdio.h>

// fork, waitpid, exit, alarm, sigaction, kill, printf, strsignal, errno
int main(void)
{

    bool            verbose = false;
    unsigned int    timeout = 0;
    int             result;

    result = sandbox(printer_caca, timeout, verbose);
    printf("result sandbox is: %d\n", result);
    return (0);
}
 */