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
extern char operands[sizeof(char) * 8];

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
	int o0, o1;

	LOG("------START STATEMENT------");
	if (prg->operands[0].t == StatementOperand) {
		o0 = solve((Statement*)prg->operands[0].data.s, params, count - 1);
	} else {
		o0 = params[prg->operands[0].data.oper];
	}
	if (prg->operands[1].t == StatementOperand) {
		o1 = solve((Statement*)prg->operands[1].data.s, params, count - 1);
	} else {
		o1 = params[prg->operands[1].data.oper];
	}

	switch (prg->op) {
		case AND:
			return o0 && o1;
		case OR:
			return o0 || o1;
		case XOR:
			return o0 ^ o1;
		case NOR:
			return !(o0 || o1);

		case NAND:
			return !(o0 && o1);

		case NOT:
			return !o0;

		default:
			ERR("Illegal instruction encountered");
	}

	LOG("------END STATEMENT------");
	return 0;
}

void run(Statement *program, int opcount)
{
	LOG("Run begins. Calculating permutations");
	int len;
	bool *space = calc_perms(opcount, &len);

	/* Print table headers */
	for (int i = 0; i < opcount; i++) {
		printf("%-8c", operands[i]);
	}
	printf("%-8s\n", "Q/P");

	LOG("Ready. Computing Q recursively.");
	for (int i = 0; i < len; i++) {
		int q = solve(program, space + ((sizeof(char) * opcount) * i), opcount);

		for (int b = 0; b < opcount; b++) {
			printf("%-8d", space[((sizeof(char) * opcount) * i) + b]);
		}
		printf("%-8d\n", q);
	}

	return;
}
