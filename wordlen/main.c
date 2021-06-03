/*
 * A utility to obtain the machine word length, along
 * with a few other useful data types
 */

#include <stdio.h>
#include <limits.h>

int main(int argc, char **argv)
{
	int wordlen = (int)sizeof(long)*CHAR_BIT;

	for (int i = 1; i < argc; i++) {
		if (argv[i][1] == 'q') {
			printf("%i\n", wordlen);
			return 0;
		} else if (argv[i][1] == 'v') {
			printf("Word length: %i\nExtended long size: %li\nPointer size: %li\nInteger size: %li\nChar size: %i\n",
					wordlen,
					sizeof(long long)*CHAR_BIT,
					sizeof(void*)*CHAR_BIT,
					sizeof(int)*CHAR_BIT,
					CHAR_BIT);
			return 0;
		} else if (argv[i][1] == 'u') {
			puts("word [-qvuh]: get machine word length");
			return 0;
		} else if (argv[i][1] == 'h') {
			puts("word [-qvuh]: get machine word length");
			puts("-q: Print word length and nothing else\n-v: Print length of various data types\n-u: Print usage\n-h: This message");
			return 0;
		}
	}

    	printf("Word length is %d bits on this system\n", wordlen);
    	return 0;
}
