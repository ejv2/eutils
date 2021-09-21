/*
 * Char2Ascii
 * Quick utility for getting ascii character codes
 */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

char buf[BUFSIZ];

int main(int argc, char **argv)
{
	int count = 1;
	while ((argc >= 2 && count < argc) || argc < 2) {
		char *stringParsed;
		if (argc >= 2) {
			stringParsed = argv[count++];
		} else {
			char *ptr = fgets(buf, BUFSIZ, stdin);
			if (!ptr) {
				return 0;
			}

			stringParsed = buf;
		}

		for (unsigned long i = 0; i < strlen(stringParsed); i++) {
			printf("%d ", (int)stringParsed[i]);
		}

		putchar('\n');
	}

	return 0;
}
