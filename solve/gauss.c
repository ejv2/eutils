/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "solve.h"

/*
 * Detects the row which a row should be placed in the reduced augmented
 * matrix
 *
 * returns n, where n is the target row
 */
int g_rowpos(mat_t *mat, int row)
{
	size_t i;
	int off = -1;
	long double *rw;

	if (!mat)
		return -1;
	if (row >= mat->dims[Equations])
		return -1;

	rw = mat->rows[row];
	for (i = 0; i < mat->dims[Unknowns]; i++) {
		if (rw[i] != 0) {
			off = i;
			break;
		}
	}

	if (off < 0)
		return mat->dims[Unknowns];
	return off;
}

/*
 * Preprocesses a system such that every equation references every variable
 */
void g_preprocess(mat_t *mat)
{
	size_t i, j;
	int safe = -1, nfixers = 0;
	int fixers[mat->dims[Equations]];

	if (!mat)
		return;

	/* First determine a safe row and the targets of preprocessing.
	 *
	 * A safe row is as follows:
	 * 	- Has every unknown with a non-zero coefficient
	 * 	- Has a nonzero solution
	 *
	 * A target of preprocessing is as follows:
	 * 	- Has a zero coefficient on one or more variables
	 * 	- Has a zero solution but non-zero coefficients
	 */
	for (i = 0; i < mat->dims[Equations]; i++) {
		int notzero = 1;
		for (j = 0; j < mat->dims[Unknowns]; j++) {
			if (mat->rows[i][j] == 0)
				notzero = 0;
		}

		if (notzero)
			safe = i;
		else
			fixers[nfixers++] = i;
	}

	/* no safe rows - give up */
	if (safe < 0)
		return;

	for (i = 0; i < nfixers; i++) {
		mat_sub(mat, fixers[i], safe, -1);
	}
}

mat_t gauss_reduce(mat_t *mat)
{
	char seen[mat->dims[Equations]];
	int rowpos[mat->dims[Equations]];
	size_t i, row;
	mat_t r;

	memset(seen, 0, sizeof(seen));
	memset(&r, 0, sizeof(r));
	if (!mat)
		return r;
	r = mat_dup(mat);
	if (!mat_verify(&r))
		return (mat_t){{0, 0}, NULL, NULL};

	/* Optimization attempt
	 * Build the work matrix in order of coefficients, such that a matrix
	 * that could be re-arranged to be in echelon form will be in echelon
	 * form.
	 *
	 * example:
	 * 0 1 3 4  ---> 2 8 9 4
	 * 2 8 9 4       0 1 3 4
	 * 0 0 1 4       0 0 1 4
	 * 2 4 6 8       2 4 6 8
	 *
	 * Echelon form dictates that row n should contain 1 at nth column and
	 * zeroes before it.
	 */
	for (row = 0; row < r.dims[Equations]; row++) {
		rowpos[row] = g_rowpos(mat, row);
	}

	int off = 0;
	for (i = 0; i < r.dims[Unknowns]; i++) {
		for (row = 0; row < r.dims[Equations]; row++) {
			if (rowpos[row] == i && !seen[row]) {
				memcpy(r.rows[off], mat->rows[row], sizeof(long double) * mat->dims[Unknowns]);
				r.eval[off] = mat->eval[row];

				off++;
				seen[row] = 1;
			}
		}
	}
	g_preprocess(mat);

	/* 
	 * Gaussian Elimination
	 * Matrix is now in "easily" cancellable position - perform elimination.
	 *
	 * Our aim is to get a matrix in echelon form, as stated above.
	 * We do this by subtracting multiples of rows such that columns <n are
	 * zero.
	 * We can then subtract this further to eliminate terms below also.
	 *
	 * So, we start at row one and then back substitute for the first
	 * solution later. This means that every row is a linear combination of
	 * that below it - thereby being solvable.
	 */
	for (row = 1; row < r.dims[Equations]; row++) {
		for (i = 0; i < (row % r.dims[Unknowns]); i++) {
			long double ratio = r.rows[row][i] / r.rows[i][i];
			if (isinf(ratio) || r.rows[i][i] == 0)
				goto out;
			mat_sub(&r, row, i, ratio);
		}

		mat_mul(&r, row, 1/r.rows[row][row % r.dims[Unknowns]]);
	}

out:
	return r;
}

/*
 * Solves for mat->dims[Unknowns] solutions and copies into buf.
 * Returns the number of solutions found.
 */
unsigned int solve(mat_t *mat, long double *buf)
{
	if (!mat || !buf)
		return 0;

	long unknown, irow, icol;
	int off = 0;
	char solmask[mat->dims[Unknowns]];
	char eqmask[mat->dims[Equations]];

	/* init structures */
	memset(buf, 0, sizeof(long double) * mat->dims[Unknowns]);
	memset(solmask, 0, sizeof(solmask));
	memset(eqmask, 0, sizeof(eqmask));

	/*
	 * We have found a solution if a row contains:
	 * 	- The variable we wish to solve for (with a non-zero coefficient)
	 * 	- Only variables we have already solved for
	 */
	for (unknown = mat->dims[Unknowns] - 1; unknown >= 0; unknown--) {
		for (irow = mat->dims[Equations] - 1; irow >= 0; irow--) {
			long double c = mat->eval[irow];
			long double *row = mat->rows[irow];
			int valid = 1;
			long double scale = 1;

			if (eqmask[irow])
				continue;

			for (icol = 0; icol < mat->dims[Unknowns]; icol++) {
				if (icol == unknown) {
					/* Not a valid solution - unknown eliminated */
					if (row[icol] == 0 || !isnormal(row[icol])) {
						valid = 0;
						break;
					}
					scale = row[icol];
					continue;
				}
				if (row[icol] != 0) {
					/* Not a valid solution - not yet solved for an unknown */
					if (!solmask[icol]) {
						valid = 0;
						break;
					}
					/* Not a valid solution - coefficient is nan or inf */
					if (!isnormal(row[icol])) {
						valid = 0;
						break;
					}
					c -= buf[icol] * row[icol];
				}
			}
			if (!valid)
				continue;

			buf[mat->dims[Unknowns] - 1 - off++] = c / scale;
			solmask[unknown] = 1;
			break;
		}
	}

	return off;
}
