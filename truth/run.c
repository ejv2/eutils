/*
 * Interpreter for truthlang
 * Copyright (C) 2021 - Ethan Marshall
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "truth.h"

extern bool verbose;
extern char operands[sizeof(long long) * 8];

/*
 * Array returned will be, in reality, an array of
 * buffers which are sizeof(char) * opers in length
 *
 * To get an individual entry, multiply sizeof(char) * opers
 * and take an offset.
 */
static bool *calc_perms(int opers, int *len)
{
	long perms = pow(2, opers);
	bool *alloc = malloc(sizeof(bool) * (perms * opers));

	/* To calculate perms, we are just going to
	 * find the max permutations and count up, querying
	 * the bits in binary up to the permutations max.
	 *
	 * This means that there can be at most sizeof(long long)*8
	 * operands at once - usually 64.
	 */
	long long count;
	for (count = 0; count < perms; count++) {
		bool *entry = alloc + ((sizeof(char) * opers) * count);

		/* Do a bit test */
		for (short i = 0; i < opers; i++) {
			int mask = 0x1 << i;
			entry[i] = ((count & mask) != 0);
		}
	}

	*len = perms;
	return alloc;
}

/* Called recursively using progressively less
 * parameter space until all sub-statements are
 * solved
 */
int solve(Statement *prg, bool *params, int count)
{
	LOG("------START STATEMENT------");
	switch (prg->op) {
		case AND: {
			int o0, o1;
			if (prg->operands[0].t == StatementOperand) {
				o0 = solve((Statement*)prg->operands[0].data.s, params, count - 1);
			} else {
				o0 = params[0];
			}

			if (prg->operands[1].t == StatementOperand) {
				o1 = solve((Statement*)prg->operands[1].data.s, params+1, count - 1);
			} else {
				o1 = params[1];
			}

			return o0 && o1;
		}

		case OR: {
			int o0, o1;
			if (prg->operands[0].t == StatementOperand) {
				o0 = solve((Statement*)prg->operands[0].data.s, params, count - 1);
			} else {
				o0 = params[0];
			}

			if (prg->operands[1].t == StatementOperand) {
				o1 = solve((Statement*)prg->operands[1].data.s, params+1, count - 1);
			} else {
				o1 = params[1];
			}

			return o0 || o1;
		}

		case XOR: {
			int o0, o1;
			if (prg->operands[0].t == StatementOperand) {
				o0 = solve((Statement*)prg->operands[0].data.s, params, count - 1);
			} else {
				o0 = params[0];
			}

			if (prg->operands[1].t == StatementOperand) {
				o1 = solve((Statement*)prg->operands[1].data.s, params+1, count - 1);
			} else {
				o1 = params[1];
			}

			return o0 ^ o1;
		}

		case NOT: {
			int o0;

			if (prg->operands[0].t == StatementOperand) {
				o0 = solve((Statement*)prg->operands[0].data.s, params, count - 1);
			} else {
				o0 = params[0];
			}

			return !o0;
		}

		default:
			ERR("Illegal instruction encountered");
	}

	LOG("------END STATEMENT------");
	return 0;
}

void run(Statement *program)
{
	LOG("Run begins. Calculating permutations");
	int len;
	int opers = strlen(operands);
	bool *space = calc_perms(opers, &len);

	/* Print table headers */
	for (int i = 0; i < opers; i++) {
		printf("%-8c", operands[i]);
	}
	printf("%-8s\n", "Q/P");

	LOG("Ready. Computing Q recursively.");
	for (int i = 0; i < len; i++) {
		int q = solve(program, space + ((sizeof(char) * opers) * i), opers);

		for (int b = 0; b < opers; b++) {
			printf("%-8d", space[((sizeof(char) * opers) * i) + b]);
		}
		printf("%-8d\n", q);
	}

	return;
}
