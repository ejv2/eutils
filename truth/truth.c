#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "truth.h"

bool verbose;

static char inbuf[BUFSIZ];

int main(int argc, char **argv)
{
	char *text;
	Statement *prg;

	if (argc <= 1) {
		fgets(inbuf, BUFSIZ, stdin);
		text = inbuf;
	} else {
		if (strcmp(argv[1], "-v") == 0) {
			verbose = true;

			argv++;
			argc--;
		}

		if (argc >= 2) {
			text = argv[1];
		} else {
			fgets(inbuf, BUFSIZ, stdin);
			text = inbuf;
		}
	}

	prg = parse(&text);
}
