/*
 * Tally - Count an automated tally
 * Copyright (C) 2021 - Ethan Marshall
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <termios.h>

static const char *flags = "vhuq";
static int quiet = 0;
static struct termios termios, termios_orig;

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
	puts("Press any key to advance the tally");
	puts("Press 'q' to get a summary");
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
		case 'q':
			quiet = 1;
			break;
		case 'h':
			help();
			break;
		case 'u':
			usage();
			exit(0);
		default:
			fprintf(stderr, "%s: unknown option '%c'\n", argv[0], opt);
			break;
		}
	}
}

void init_tty(int fileno)
{
	if (!isatty(fileno) || !isatty(fileno)) {
		return;
	}
	tcgetattr(fileno, &termios_orig);
	memcpy(&termios, &termios_orig, sizeof(termios_orig));

	termios.c_lflag &= ~ICANON;
	termios.c_lflag &= ~ECHO;

	tcsetattr(fileno, TCSANOW, &termios);
}

void reset_tty(int fileno)
{
	tcsetattr(fileno, TCSANOW, &termios_orig);
}

int main(int argc, char **argv)
{
	FILE *out;
	handle_flags(argc, argv);

	out = fopen("/dev/tty", "r+");
	init_tty(fileno(out));

	int t;
	for (t = 1; getc(out) != 'q'; t++) {
		if (!quiet) {
			if (t != 0 && t % 5 == 0) {
				fprintf(out, "\r\x1b[9m||||\x1b[0m\n");
				continue;
			}

			putc('|', out);
		}
	}

	t--;
	fprintf(out, "\nTotal: %d Gates: %d Extra: %d\n", t, t / 5, t % 5);

	reset_tty(fileno(out));
	fclose(out);
}
