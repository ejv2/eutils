/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

typedef struct {
	long double val;
	char unknown;
} var_t;

typedef struct {
	unsigned char ncoff;
	var_t coff[26];
	long double c;
} expr_t;

typedef struct {
	int dims[2];
	long double **arr;
} mat_t;

int parse(expr_t *exp, const char *p);
