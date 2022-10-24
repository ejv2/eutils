/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "solve.h"

int main(int argc, char **argv)
{
	size_t i, j;
	int verbose = 0, nexp = 0;
	expr_t exp[255];
	mat_t amat, smat;
	unsigned int nsolutions;
	long double *solutions;
	char buf[BUFSIZ];

	if (argc > 1) {
		if (strcmp(argv[1], "-v") == 0) {
			verbose = 1;
		} else if (strcmp(argv[1], "-h") == 0) {
			puts("solve [-hv]: solve a linear system of equations using Gauss-Jordan elimination");
			puts("Copyright (C) 2022 - Ethan Marshall");
			puts("\n-h:\tThis message");
			puts("-v:\tPrint intermediate matrices in elimination process to stdout");
			return 1;
		}
	}

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
	if (nexp == 0) {
		fprintf(stderr, "%s: math error -- system must contain at least one equation\n", argv[0]);
		return 1;
	}

	amat = exp_mat(exp, nexp);
	smat = gauss_reduce(&amat);

	if (verbose) {
		for (i = 0; i < amat.dims[Equations]; i++) {
			putchar('[');
			for (size_t j = 0; j < amat.dims[Unknowns]; j++) {
				printf("%Lf ", amat.rows[i][j]);
			}
			printf("| %Lf ]\n", amat.eval[i]);
		}
		puts("------------");
		for (i = 0; i < smat.dims[Equations]; i++) {
			putchar('[');
			for (size_t j = 0; j < smat.dims[Unknowns]; j++) {
				printf("%Lf ", smat.rows[i][j]);
			}
			printf("| %Lf ]\n", smat.eval[i]);
		}
	}

	if (smat.dims[Unknowns] > nexp) {
		fprintf(stderr,
				"%s: system in insufficiently constrained (got %d expressions "
				"but %d unknowns\n",
				argv[0], nexp, smat.dims[Unknowns]);
		return 2;
	}

	solutions = malloc(sizeof(long double) * smat.dims[Unknowns]);
	nsolutions = solve(&smat, solutions);
	if (nsolutions == 0) {
		fprintf(stderr, "%s: system is inconsistent or unsolvable\n", argv[0]);
		return 2;
	} else if (nsolutions < smat.dims[Unknowns]) {
		fprintf(stderr, "%s: warning: could not solve for all unknowns (%d of %d)\n", argv[0], nsolutions, smat.dims[Unknowns]);
	}

	for (i = 0; i < nsolutions; i++) {
		printf("%c:\t%Lf\n", exp->coffsym[i], solutions[i]);
	}

	mat_destroy(&amat);
	mat_destroy(&smat);
	free(solutions);
}
