/*
 * bconv - binary conversion
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

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

void intout(long long x)
{
	if (x <= CHAR_MAX && x >= CHAR_MIN) {
		print_binary((char)x, sizeof(char)*8);
		return;
	} else if (x <= SHRT_MAX && x >= SHRT_MIN) {
		print_binary((short)x, sizeof(int)*8);
		return;
	} else if (x <= INT_MAX && x >= INT_MIN) {
		print_binary((int)x, sizeof(int)*8);
		return;
	} else if (x <= LONG_MAX && x >= LONG_MIN) {
		print_binary((long)x, sizeof(int)*8);
		return;
	} else {
		print_binary(x, sizeof(int)*8);
		return;
	}
}

int main(int argc, char **argv)
{
	char buf[BUFSIZ];

	if (argc > 1) {
		intout(strtoll(argv[1], NULL, 10));
		return 0;
	}

	while (fgets(buf, BUFSIZ - 1, stdin)) {
		intout(strtoll(buf, NULL, 10));
	}

	return 0;
}
