/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <stdio.h>

#include "solve.h"

int main(int argc, char **argv)
{
	int nexp = 0;
	expr_t exp[255];
	char buf[BUFSIZ];

	while (fgets(buf, BUFSIZ - 1, stdin)) {
		if (nexp >= 255) {
			fprintf(stderr, "%s: math error -- too many equations, max 255\n", argv[0]);
			return 1;
		}

		if (!parse(&exp[nexp++], buf)) {
			fprintf(stderr, "%s: syntax error -- line %d on stdin\n", argv[0], nexp);
			return 1;
		}
	}

	for (int i = 0; i < nexp; i++) {
		for (int j = 0; j < 26; j++) {
			/* printf("%d %d\n", exp[i].mask, j); */
			if (exp[i].mask & (1 << j)) {
				printf("%c: %Lf\n", 'a' + j, exp[i].coff[j]);
			}
		}
	}
}
