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
	mat_t amat;
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

	amat = exp_mat(exp, nexp);
	for (size_t i = 0; i < amat.dims[Equations]; i++) {
		putchar('[');
		for (size_t j = 0; j < amat.dims[Unknowns]; j++) {
			printf("%Lf ", amat.rows[i][j]);
		}
		printf("| %Lf ]\n", amat.eval[i]);
	}

	mat_destroy(&amat);
}
