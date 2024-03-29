#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv)
{
	char *walk;
	char buf[BUFSIZ];
	long shift, work;

	if (argc < 2) {
		fputs("caesar: expected [shift]\n", stderr);
		return 1;
	}

	shift = strtol(argv[1], NULL, 10) % 26;
	while (fgets(buf, BUFSIZ, stdin)) {
		for (walk = buf; *walk; walk++) {
			work = tolower(*walk);
			if (work <= 'z' && work >= 'a') {
				if (work + shift < 'a') {
					work = work + shift + 26;
				} else if (work + shift > 'z') {
					work = (work + shift) - 26;
				} else {
					work += shift;
				}
			}
			putchar(work);
		}
	}
}
