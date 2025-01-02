/*
 * limexec - limited time execution
 * Copyright (C) 2025 - Ethan Marshall
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

static const char *version = "1.0.0";
static const char *opts = "s:t:uvh";

struct args {
	/* timeout given in milliseconds */
	unsigned long timeout;
	int signum;

	char *cmd;
	unsigned int argc;
	char **argv;
} args;

void usage()
{
	fputs("Usage: limexec [-s signal] [-t timeout] command [args...]\n", stderr);
}

struct args *parse_args(int argc, char **argv)
{
	args.timeout = 1000;
	args.signum = SIGTERM;
	args.argc = 0;
	args.cmd = NULL;
	args.argv = NULL;

	int opt;
	char *endptr = NULL;
	while ((opt = getopt(argc, argv, opts)) != -1) {
		switch (opt) {
		case 's':
		{
			args.signum = atoi(optarg);
			if (args.signum == 0)
			{
				fprintf(stderr, "limexec: invalid signal number '%s'\n", optarg);
			}
			break;
		}
		case 't':
		{
			args.timeout = strtoull(optarg, &endptr, 10);
			if (endptr && *endptr) {
			switch (*endptr) {
			case 'h':
			case 'H':
				args.timeout *= 60 * 60 * 1000;
				break;
			case 'M':
				args.timeout *= 60 * 1000;
				break;
			case 's':
				args.timeout *= 1000;
				break;
			case 'm':
				/* unchanged - stored as ms */
				break;
			case 'u':
				args.timeout /= 1000;
				break;
			default:
				fprintf(stderr, "limexec: invalid interval value '%s': unknown unit '%c'\n", optarg, *endptr);
				return NULL;
			}
			} else {
				args.timeout *= 1000;
			}
			break;
		}
		case 'v':
			fprintf(stderr, "limexec v%s -- Ethan Marshall 2025\n", version);
			fprintf(stderr, "https://github.com/ejv2/eutils/ -- https://ejv2.cc/\n");
			return NULL;
		case 'u':
		case 'h':
		default:
			usage();
			return NULL;
		}
	}

	if (optind >= argc) {
		fputs("limexec: expected command to run\n", stderr);
		return NULL;
	}

	args.cmd = argv[optind];
	args.argc = argc - optind - 1;
	if (args.argc > 0) {
		args.argv = &argv[optind + 1];
	} else {
		args.argv = NULL;
	}

	return &args;
}

void sigchild()
{
	exit(0);
}

int parent(pid_t childpid)
{
	signal(SIGCHLD, sigchild);

	struct timespec ts = (struct timespec){
		.tv_sec = args.timeout / 1000,
		.tv_nsec = (args.timeout % 1000) * 1000000
	};

	if (nanosleep(&ts, &ts) < 0) {
		switch (errno) {
		/* note: this happens when we get killed etc. */
		case EAGAIN:
		case EINTR:
			break;
		default:
			/* note: exit immediately here so we kill the child */
			fprintf(stderr, "limexec: nanosleep: %s\n", strerror(errno));
			break;
		}

		return 1;
	}

	if (kill(childpid, args.signum) != 0 && errno != ESRCH) {
		fprintf(stderr, "limexec: kill %d: %s\n", childpid, strerror(errno));
		return 1;
	}

	return 0;
}

void child()
{
	char *newargv[args.argc + 2];
	memset(newargv, 0, sizeof(newargv));
	newargv[0] = args.cmd;
	memcpy(&newargv[1], args.argv, args.argc * sizeof(const char *));
	newargv[args.argc + 1] = NULL;

	execvp(args.cmd, newargv);
	fprintf(stderr, "limexec: exec %s: %s\n", args.cmd, strerror(errno));
}

int main(int argc, char **argv)
{
	if (!parse_args(argc, argv))
	{
		return 1;
	}

	pid_t pid = fork();
	switch (pid) {
	case 0:
		child();
		return 3;
	case -1:
		fprintf(stderr, "limexec: fork: %s\n", strerror(errno));
		return 2;
	default:
		return parent(pid);
	}
}
