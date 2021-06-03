/*
 * Utility for generating POSIX random numbers
 * RNG source configurable
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef __linux__
#include <sys/random.h>
#endif

#ifdef __unix__
#include <unistd.h>
#endif

void usage()
{
	fputs("rand [options] [count] [source]\n", stderr);
	exit(-1);
}

void help()
{
	fputs("rand [options] [count] [source]\n\n", stderr);

	fputs("Options:\n", stderr);
	fputs("\t--help: this message\n", stderr);
	fputs("\t--usage: basic usage\n", stderr);

	fputs("Entropy sources:\n", stderr);
	fputs("\t-s: standard library\n", stderr);
#ifdef __linux__
	fputs("\t-l: linux syscall\n", stderr);
#endif

#ifdef BSD
	fputs("\t-e: BSD entropy source\n", stderr);
#endif
	fputs("\t-r: /dev/random\n", stderr);
	fputs("\t-u: /dev/urandom\n", stderr);
	fputs("\t[file]: contents of file will be used as a source\n", stderr);

	exit(-1);
}

void read_randFile(char *src, long *buf, long count)
{
	FILE *fsrc = fopen(src, "r");
	if (!fsrc) {
		fprintf(stderr, "E: Invalid entropy file source: '%s'\n", src);
		exit(-1);
		return;
	}

	int in = fread(buf, sizeof(long), count, fsrc);

	if (in < count) {
		fputs("W: Random data returned may be truncated\n", stderr);
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	if (argc < 2) {
		fputs("E: Not enough arguments\n", stderr);
		exit(-1);
	}

	if (strcmp(argv[1], "--help") == 0) {
		help();
	} else if (strcmp(argv[1], "--usage") == 0) {
		usage();
	}

	long count = strtol(argv[1], NULL, 10);
	char method = (argc > 2) ? tolower(argv[2][1]) : 's';

	if (count < 1) {
		fputs("E: Invalid count\n", stderr);
		exit(-1);
	}

	long *randBuf = calloc(count, sizeof(long));
	if (!randBuf) {
		fputs("E: Memory allocation failed\n", stderr);
		exit(-1);
	}

	switch (method)
	{
	case 's':
		for (long i = 0; i < count; i++) {
			randBuf[i] = (long)rand();
		}
		break;

#ifdef __linux__
	case 'l':
		getrandom(randBuf, sizeof(long) * count, 0x0);
		break;
#endif
#ifdef BSD
	case 'e':
		getentropy(randBuf, sizeof(long) * count);
		break;
#endif

	case 'r':
		read_randFile("/dev/random", randBuf, count);
		break;
	case 'u':
		read_randFile("/dev/urandom", randBuf, count);
		break;

	default:
		read_randFile(argv[2], randBuf, count);
		break;
	}


	for (long i = 0; i < count; i++) {
		printf("%li\n", randBuf[i]);
	}

	free(randBuf);
}
