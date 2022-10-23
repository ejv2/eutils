/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <stdio.h>
#include <math.h>

#include "solve.h"

int main(int argc, char **argv)
{
	size_t i, j;
	int nexp = 0;
	expr_t exp[255];
	mat_t amat, smat;
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
	for (i = 0; i < amat.dims[Equations]; i++) {
		putchar('[');
		for (size_t j = 0; j < amat.dims[Unknowns]; j++) {
			printf("%Lf ", amat.rows[i][j]);
		}
		printf("| %Lf ]\n", amat.eval[i]);
	}
	puts("------------");

	smat = gauss_reduce(&amat);
	for (i = 0; i < smat.dims[Equations]; i++) {
		putchar('[');
		for (size_t j = 0; j < smat.dims[Unknowns]; j++) {
			printf("%Lf ", smat.rows[i][j]);
		}
		printf("| %Lf ]\n", smat.eval[i]);
	}

	if (smat.dims[Unknowns] > nexp) {
		fprintf(stderr,
				"%s: system in insufficiently constrained (got %d expressions "
				"but %d unknowns\n",
				argv[0], nexp, smat.dims[Unknowns]);
		return 1;
	}

	/*
	 * Check if reduction succeeded
	 * Reduction led to a solution if at least one row exists which can
	 * be used to back substitute for a solution per variable.
	 * The final coefficient should be solved for by a matrix row, the
	 * remainder by back substitution.
	 */
	for (i = 0; i < smat.dims[Equations]; i++) {
		for (j = 0; j < smat.dims[Unknowns]; j++) {
			if (isinfl(smat.rows[i][j]) || isnanl(smat.rows[i][j]) ||
				(smat.rows[i][j] == 0 && j >= i) ||
				(smat.rows[i][j] != 0 && j < i)) {
				fprintf(stderr, "%s: system is inconsistent or unsolvable\n", argv[0]);
				return 2;
			}
		}
	}

	mat_destroy(&amat);
	mat_destroy(&smat);
}
