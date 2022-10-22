/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

typedef struct {
	unsigned char ncoff;
	int mask;
	long double coff[26];
	long double c;
} expr_t;

/* indices into the dimensions array */
enum {
	Equations = 0,
	Unknowns
};

typedef struct {
	/* rows x columns */
	int dims[2];
	long double **rows;
	long double *eval;
} mat_t;

int parse(expr_t *exp, const char *p);

mat_t gauss_reduce(mat_t *mat);

mat_t exp_mat(expr_t *exp, size_t n);
mat_t mat_dup(mat_t *mat);
int mat_verify(mat_t *mat);
void mat_destroy(mat_t *mat);
void mat_swap(mat_t *mat, int i, int j);
void mat_mul(mat_t *mat, int row, long double product);
void mat_sub(mat_t *mat, int r1, int r2, long double n);