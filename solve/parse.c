/*
 * solve - generalised Gauss-Jordan solver
 * Copyright (C) 2022 - Ethan Marshall
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "solve.h"

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
			if (*walk == '=') {
				walk--;
				state = 5;
			}
			if (*walk == '-') {
				neg = 1;
				state = 3;
			} else if (*walk == '+') {
				neg = 0;
				state = 3;
			}
			break;
		case 3:	/* parse coefficient */
			coff = strtold(walk, &end);
			if (coff == 0)
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
			if (!isdigit(*walk))
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
