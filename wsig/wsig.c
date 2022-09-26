/*
 * wsig - wait for a signal
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <inttypes.h>

void out()
{
	exit(0);
}

int main(int argc, char **argv)
{
	sigset_t set;
	int sig = SIGINT;

	if (argc > 1) {
		sig = strtoimax(argv[1], NULL, 10);
		if (sig <= 0) {
			fprintf(stderr, "%s: invalid signal '%d'\n", argv[0], sig);
			return 1;
		}
	}

	sigfillset(&set);
	sigdelset(&set, sig);
	sigprocmask(SIG_SETMASK, &set, NULL);

	errno = 0;
	signal(sig, out);
	if (errno != 0) {
		perror("signal");
		return 2;
	}

	/*
	 * use minimal resources by continually waiting for non-existent
	 * buffering IO
	 */
	for (;;) {
		getchar();
	}

	return 0;
}
