/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "solve.h"

/* converts an arbitrary math expression to an augmented matrix */
mat_t exp_mat(expr_t *exp, size_t n)
{
	unsigned char cur = 0;
	size_t i, j;
	mat_t ret;

	memset(&ret, 0, sizeof(ret));
	if (!exp || !n)
		return ret;

	for (i = 0; i < n; i++) {
		if (exp[i].ncoff > ret.dims[Unknowns])
			ret.dims[Unknowns] = exp[i].ncoff;
	}
	ret.dims[Equations] = n;
	ret.rows = malloc(sizeof(long double *) * ret.dims[Equations]);
	ret.eval = malloc(sizeof(long double) * ret.dims[Equations]);

	for (i = 0; i < ret.dims[Equations]; i++) {
		ret.rows[i] = malloc(sizeof(long double) * ret.dims[Unknowns]);
		ret.eval[i] = exp[i].c;
		j = cur = 0;

		memset(ret.rows[i], 0, sizeof(long double) * ret.dims[Unknowns]);
		while (j < ret.dims[Unknowns] && j < exp[i].ncoff) {
			if ((exp[i].mask & (1 << (unsigned int)cur)) > 0) {
				ret.rows[i][j] = exp[i].coff[cur];
				j++;
			}

			cur++;
		}
	}

	return ret;
}

mat_t mat_dup(mat_t *mat)
{
	size_t i;
	mat_t r;

	memset(&r, 0, sizeof(r));
	if (!mat)
		return r;

	r.dims[Equations] = mat->dims[Equations];
	r.dims[Unknowns] = mat->dims[Unknowns];
	r.rows = malloc(sizeof(long double *) * r.dims[Equations]);
	r.eval = malloc(sizeof(long double) * r.dims[Equations]);

	for (i = 0; i < r.dims[Equations]; i++) {
		r.rows[i] = malloc(sizeof(long double) * r.dims[Unknowns]);
		memcpy(r.rows[i], mat->rows[i], sizeof(long double) * r.dims[Unknowns]);
	}
	memcpy(r.eval, mat->eval, sizeof(long double) * mat->dims[Equations]);

	return r;
}

/* checks if mat can safely be used */
int mat_verify(mat_t *mat)
{
	return mat && mat->rows && mat->eval && mat->dims[0] != 0 && mat->dims[1] != 0;
}

/* frees buffers associated with a matrix */
void mat_destroy(mat_t *mat)
{
	if (!mat)
		return;

	for (size_t i = 0; i < mat->dims[Equations]; i++) {
		free(mat->rows[i]);
	}

	free(mat->rows);
	free(mat->eval);
	memset(mat, 0, sizeof(*mat));
}

/* swaps two rows in a matrix */
void mat_swap(mat_t *mat, int i, int j)
{
	long double tmp, *tmpr;

	if (!mat)
		return;

	/* swap row itself */
	tmpr = mat->rows[i];
	mat->rows[i] = mat->rows[j];
	mat->rows[j] = tmpr;

	/* swap solution */
	tmp = mat->eval[i];
	mat->eval[i] = mat->eval[j];
	mat->eval[j] = tmp;
}

/* multiplies mat[row] by product */
void mat_mul(mat_t *mat, int row, long double product)
{
	size_t i;

	if (!mat)
		return;
	if (row > mat->dims[Equations])
		return;

	for (i = 0; i < mat->dims[Unknowns]; i++) {
		mat->rows[row][i] *= product;
	}
	mat->eval[row] *= product;
}

/* r1 - nr2 -> r1 */
void mat_sub(mat_t *mat, int r1, int r2, long double n)
{
	size_t i;

	if (!mat)
		return;
	if (r1 >= mat->dims[Equations] || r2 >= mat->dims[Equations])
		return;

	for (i = 0; i < mat->dims[Unknowns]; i++) {
		mat->rows[r1][i] -= n * mat->rows[r2][i];
	}
	mat->eval[r1] -= n * mat->eval[r2];
}

/* parse a raw math expression from a NULL-terminated buffer */
int parse(expr_t *exp, const char *p)
{
	const char *walk = p;
	char *end = NULL;
	int state = 0;
	int neg = 0;
	long double coff = 0;

	if (!exp || !p)
		return 0;

	/* init struct */
	exp->ncoff = exp->c = exp->mask = 0;
	memset(exp->coff, 0, sizeof(exp->coff));
	memset(exp->coffsym, 0, sizeof(exp->coffsym));

	for (const char *walk = p; *walk && *walk != '\n'; walk++) {
		switch (state) {
		case 0:	/* strip leading whitespace */
			if (!isspace(*walk)) {
				state = 1;
				walk--;
			}
			break;
		case 1:	/* decision - coff or unknown */
			if (isdigit(*walk) || *walk == '-' || *walk == '.') {
				state = 3;
			} else if (isalpha(*walk)) {
				if (neg)
					coff = -1;
				else
					coff = 1;
				state = 4;
			} else {
				return 0;
			}
			walk--;
			break;
		case 2:	/* +/- sign */
			if (isspace(*walk))
				break;
			if (isalpha(*walk))
				return 0;
			if (*walk == '=') {
				walk--;
				state = 5;
			}
			if (*walk == '-') {
				neg = 1;
				state = 0;
			} else if (*walk == '+') {
				neg = 0;
				state = 0;
			}
			break;
		case 3:	/* parse coefficient */
			if (*walk == '+')
				break;
			if (*walk == '-') {
				neg = 1;
				break;
			}
			if (isalpha(*walk)) {
				state = 4;
				if (neg)
					coff = -1;
				else
					coff = 1;
				walk--;
				break;
			}
			coff = strtold(walk, &end);
			if (end == walk)
				return 0;
			if (neg)
				coff = -coff;
			walk = end - 1;
			state = 4;
			break;
		case 4:	/* parse unknown */
			if (!isalpha(*walk) || *walk < 'a' || *walk > 'z')
				return 0;
			exp->coff[*walk - 'a'] += coff;
			exp->coffsym[exp->ncoff] = *walk;
			exp->ncoff++;
			exp->mask |= (1 << (*walk - 'a'));
			state = 2;
			break;
		case 5:	/* parse equality */
			if (isspace(*walk))
				break;
			if (*walk != '=')
				return 0;
			state = 6;
			break;
		case 6:	/* parse equality value */
			if (isspace(*walk))
				break;
			if (!isdigit(*walk) && *walk != '-' && *walk != '.')
				return 0;
			exp->c = strtold(walk, &end);
			if (*end != '\n' && *end != '\0')
				return 0;
			goto out;
			break;
		default:
			abort();
			break;
		}
	}

	/* skipped by goto out
	 * if flow reaches here, error occurred */
	return 0;

out:
	if (exp->ncoff <= 0) {
		return 0;
	}

	return 1;
}
