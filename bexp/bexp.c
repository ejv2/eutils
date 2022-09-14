/*
 * bexp - binary explorer
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <getopt.h>
#include <arpa/inet.h>

extern char *optarg;
static const char *flags = "uhn:";

void print_binary(long long string, unsigned int maxlen)
{
	unsigned long long mask;

	for (long long i = 0; i < maxlen; i++) {
		/* needed to prevent overflows when mixing types
		 * types of different sizes.
		 */
		mask = 1L << i;
		if ((mask & string) != 0)
			putchar('1');
		else
			putchar('0');
	}
	putchar('\n');
}

/*
 * This is surprisingly hard to get right...
 *
 * The c99 standard says that a signed int must be 32 bits two's complement.
 * However, C also tries to "help" us by abstracting over endinanness by making
 * numbers from C always work as though written in big endian, so bit shifting
 * is out of the question.
 *
 * To avoid losing portability, I found the best way is to simply use htonl and
 * work out by elimination what could possibly be our order. The libc on the
 * platform simply pastes in the appropriate assembly instructions for us.
 * If htonl on a signed int returns the same value as the plain int, we are big
 * endian.
 *
 * Returns >0 if platform is big endian.
 */
int detect_endian()
{
	unsigned int test = 1;
	return htonl(test) == test;
}

char **parse_cmd(char *buf, int *cmd, int *argc)
{
	const int amax = 5;
	int i, compact = 0;
	int state = 0, acount = 0;
	char **abuf = malloc(sizeof(char *) * amax);

	if (!abuf)
		return NULL;
	memset(abuf, 0, sizeof(char *) * amax);

	for (char *walk = buf; *walk != 0 && *walk != '\n'; walk++) {
		switch (state) {
			case 0:		/* copy to cmd buf */
				*cmd = *walk;
				state++;
				break;
			case 1:		/* discard up to space */
				if (*walk == ' ' || *walk == '\t')
					state++;
				break;
			case 2:		/* script whitespace between args */
				if (*walk != ' ' || *walk != '\t') {
					abuf[acount] = walk;
					acount++;

					state++;
				}
				break;
			case 3:		/* copy arg */
				if (*walk == ' ' || *walk == '\t') {
					*walk = 0;
					state = 2;
					break;
				}
				if (acount >= amax)
					goto out;
				break;
			default:
				fputs("bexp: illegal parser state!\n", stderr);
				abort();
				/* NOTREACHED */
		}
	}

out:
	*argc = acount;
	return abuf;
}

int main(int argc, char **argv)
{
	const int endian = detect_endian();
	const char *end = detect_endian() ? "big" : "little";
	signed long long buf = 0;
	long long work;
	int opt, choice;
	int sign;
	int cargc;
	int curend = endian;
	char **cargv;
	char in[1024];

	while ((opt = getopt(argc, argv, flags)) != -1) {
		switch (opt) {
			case 'n':
				buf = strtoll(optarg, NULL, 10);
				break;
			case 'u':
				sign = 0;
				break;
			case 'h':
				puts("n - set bit string to integer value");
				puts("u - make bit string unsigned");
				puts("h - this message");
			default:
				return 1;
		}
	}

	for (;;) {
		puts("Enter '?' or 'h' for help");
		printf("\nsize: %lu endian: %s\n", sizeof(buf)*8, end);

		for (;;) {
			print_binary(buf, sizeof(buf) * 8);
			fputs("Operation>", stdout);

			if (!fgets(in, 1023, stdin))
				return 0;
			cargv = parse_cmd(in, &choice, &cargc);
			if (!cargv)
				return 127;

			switch (choice) {
			case 'h':
			case '?':
				puts("Available commands:");
				puts("? h\t\tThis message");
				puts("t <index>\tToggle bit");
				puts("c <index>\tClear bit");
				puts("s <index>\tSet bit");
				puts("n <value>\tSet bit string to value (two's complement binary)");
				puts("-\t\tToggle sign");
				puts("f\t\tFill bit string");
				puts("e\t\tEmpty bit string");
				puts("p\t\tPrint bit string as number");
				puts("q\t\tExit the program");
				puts("\n[NOTE]: Display of bits depends on host endinanness");
				break;
			case 't': 
				if (cargc != 1) {
					puts("expected one argument <index>");
					break;
				}
				work = strtoull(cargv[0], NULL, 10) - 1;
				if (work < 0)
					break;
				buf ^= (1LL << work);
				break;
			case 'c':
				if (cargc != 1) {
					puts("expected one argument <index>");
					break;
				}
				work = strtoull(cargv[0], NULL, 10) - 1;
				if (work < 0)
					break;
				buf &= ~(1LL << work);
				break;
			case 's':
				if (cargc != 1) {
					puts("expected one argument <index>");
					break;
				}
				work = strtoull(cargv[0], NULL, 10) - 1;
				if (work < 0)
					break;
				buf |= (1LL << work);
				break;
			case '-':
				buf = -buf;
				break;
			case 'e':
				buf = 0;
				break;
			case 'f':
				buf |= ~0L;
				break;
			case 'n':
				if (cargc != 1) {
					puts("expected one argument <value>");
					break;
				}
				buf = strtoll(cargv[0], NULL, 10);
				break;
			case 'p':
				if (sign)
					printf("%lld\n", buf);
				else
					printf("%llu\n", buf);
				break;
			case 'q':
				goto exit;
			default:
				printf("unknown option: '%c'\n", choice);
				break;
			}

			putchar('\n');
			free(cargv);
		}
	}
exit:
	free(cargv);
	return 0;
}
