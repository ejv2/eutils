/*
 * Tally - Count an automated tally
 * Copyright (C) 2021 - Ethan Marshall
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <getopt.h>

static const char *flags = "vhuq";

static int quiet = 0;

void usage()
{
	puts("tally: count an automated tally");
	puts("Copyright (C) 2021 - Ethan Marshall");
}

void version()
{
	usage();
	puts("Version: v1.0.0");
	exit(0);
}

void help()
{
	usage();
	exit(0);
}

void handle_flags(int argc, char **argv)
{
	char opt;
	while ((opt = getopt(argc, argv, flags)) != -1) {
		switch (opt) {
		case 'v':
			version();
			break;
		case 'h':
			help();
			break;
		case 'u':
			usage();
			exit(0);
		case 'q':
			quiet = 1;
			break;
		default:
			fprintf(stderr, "%s: unknown option '%c'\n", argv[0], opt);
			break;
		}
	}
}

void init_tty()
{
	/* don't line buffer */
	system("stty -F /dev/tty cbreak min 1");
	/* don't show output on the TTY */
	system("stty -F /dev/tty -echo");
}

int main(int argc, char **argv)
{
	handle_flags(argc, argv);

	if (!quiet) {
		usage();
		puts("\nPress any key to advance the tally");
	}

	init_tty();

	int t;
	for (t = 1; getchar() != 'q'; t++) {
		if (t != 0 && t % 5 == 0) {
			printf("\r\x1b[9m||||\x1b[0m\n");
			continue;
		}

		putchar('|');
	}

	t--;
	printf("\nTotal: %d Gates: %d Extra: %d\n", t, t / 5, t % 5);
}
