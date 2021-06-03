/*
 * Char2Ascii
 * Quick utility for getting ascii character codes
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc < 2) {
		puts("E: Expected at least 1 argument");
		exit(-1);
	}

	for (int i = 1; i < argc; i++) {
		char *stringParsed = argv[1];

		for (unsigned long i = 0; i < strlen(stringParsed); i++) {
			printf("%d ", (int)stringParsed[i]);
		}
	}

	puts("");
}
