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
		char *str;
		if (argc >= 2) {
			str = argv[count++];
		} else {
			char *in = fgets(buf, BUFSIZ, stdin);
			if (!in) {
				return 0;
			}

			str = buf;
		}

		for (unsigned long i = 0; i < strlen(str); i++) {
			if (str[i] == '\n')
				continue;

			printf("%d ", (int)str[i]);
		}

		putchar('\n');
	}

	return 0;
}
