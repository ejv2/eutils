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

	errno = 0;
	signal(sig, out);
	if (errno != 0) {
		perror("signal");
		return 2;
	}

	sigsuspend(&set);
	return 0;
}
