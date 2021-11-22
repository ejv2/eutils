#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "truth.h"

static char inbuf[BUFSIZ];

int main(int argc, char **argv)
{
	char *text;
	Statement *prg;

	if (argc <= 1) {
		fgets(inbuf, BUFSIZ, stdin);
		text = inbuf;
	} else {
		text = argv[1];
	}

	prg = parse(&text);
}
