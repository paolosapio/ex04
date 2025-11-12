


#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> /* para strerror si hace falta */
#include <stdbool.h>

/*
Allowed functions used: fork, waitpid, exit, alarm, sigaction, kill,
printf, strsignal, errno
*/

static volatile pid_t g_child_pid = 0;
static volatile sig_atomic_t g_timed_out = 0;

static void alarm_handler(int sig)
{
	(void)sig;
	/* marcar timeout y matar al hijo si existe */
	g_timed_out = 1;
	if (g_child_pid > 0)
	{
		/* intentar terminarlo de inmediato */
		kill(g_child_pid, SIGKILL);
	}
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t pid;
	int status;
	struct sigaction sa, old_sa;
	int ret_wait;
	unsigned int old_alarm = 0;
	int result = -1; /* por defecto -1 = error en nuestra función */

	/* preparar handler de SIGALRM */
	sa.sa_handler = alarm_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGALRM, &sa, &old_sa) == -1)
	{
		/* error */
		return -1;
	}

	pid = fork();
	if (pid < 0)
	{
		/* fork falló: restaurar sigaction y devolver error */
		sigaction(SIGALRM, &old_sa, NULL);
		return -1;
	}

	if (pid == 0)
	{
		/* hijo: ejecutar la función y salir */
		/* No debemos tocar handlers del padre ni alarmas */
		f();
		_exit(0);
	}

	/* padre */
	g_child_pid = pid;
	g_timed_out = 0;

	/* activar alarma si timeout > 0 */
	if (timeout > 0)
		old_alarm = alarm(timeout);

	/* Esperar al hijo; necesitamos detectar stops (WUNTRACED) y luego reaperlo */
	while (1)
	{
		errno = 0;
		ret_wait = waitpid(pid, &status, WUNTRACED);
		if (ret_wait == -1)
		{
			/* error en waitpid: intentar limpiar y devolver -1 */
			int saved_errno = errno;
			/* cancelar alarma y restaurar handler */
			alarm(0);
			sigaction(SIGALRM, &old_sa, NULL);
			(void)saved_errno;
			g_child_pid = 0;
			return -1;
		}

		/* Si ocurrió timeout (handler) -> seguramente el hijo fue KILL'd.
		   Vamos a seguir y dejar que la siguiente iteración confirme que terminó.
		   Pero si g_timed_out es 1 ahora, podemos romper y procesar como timeout. */
		if (g_timed_out)
		{
			/* cancelar alarma pendiente */
			alarm(0);
			/* asegurarnos de que el hijo está muerto; si no, intentar matarlo */
			kill(pid, SIGKILL);
			/* esperar a que termine definitivamente */
			errno = 0;
			if (waitpid(pid, &status, 0) == -1)
			{
				int saved_errno = errno;
				sigaction(SIGALRM, &old_sa, NULL);
				g_child_pid = 0;
				(void)saved_errno;
				return -1;
			}
			/* ahora status refleja la terminación tras SIGKILL */
			break;
		}

		/* Si el hijo fue detenido por una señal (SIGSTOP, SIGTSTP, etc)
		   consideramos que es una función "bad": notificamos (si verbose),
		   lo matamos y seguimos para reapear el proceso. */
		if (WIFSTOPPED(status))
		{
			int s = WSTOPSIG(status);
			if (verbose)
				printf("Bad function: %s\n", strsignal(s));
			/* forzar terminación y continuar para reapear */
			kill(pid, SIGKILL);
			/* loop: esperamos la terminación real en la siguiente iteración */
			continue;
		}

		/* Si terminó (por señal o por exit), salimos del loop y procesamos */
		if (WIFEXITED(status) || WIFSIGNALED(status))
			break;

		/* en principio no llegamos aquí, pero por seguridad continuamos esperando */
	}

	/* ya hemos salido del loop con 'status' final del hijo */
	/* cancelar la alarma y restaurar el handler original */
	alarm(0);
	if (sigaction(SIGALRM, &old_sa, NULL) == -1)
	{
		/* error al restaurar; aun así seguiremos intentando reaper y devolver -1 */
		g_child_pid = 0;
		return -1;
	}

	/* procesar casos */
	if (g_timed_out)
	{
		if (verbose)
			printf("Bad function: timed out after %u seconds\n", timeout);
		result = 0;
	}
	else if (WIFSIGNALED(status))
	{
		int s = WTERMSIG(status);
		if (verbose)
			printf("Bad function: %s\n", strsignal(s));
		result = 0;
	}
	else if (WIFEXITED(status))
	{
		int code = WEXITSTATUS(status);
		if (code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			result = 1;
		}
		else
		{
			if (verbose)
				printf("Bad function: exited with code %d\n", code);
			result = 0;
		}
	}
	else
	{
		/* caso inesperado */
		result = -1;
	}

	/* limpieza final */
	g_child_pid = 0;
	/* restaurar cualquier alarma previa (si la había) */
	if (old_alarm > 0)
		alarm(old_alarm);

	return result;
}