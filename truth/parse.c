/*
 * Recursive descent parser for truthlang
 * Copyright (C) 2021 - Ethan Marshall
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "truth.h"

extern bool verbose;

static const int opermax = 4;
static int opers = 0;
static int oplookup[sizeof(char) * 255];
static int opseen[sizeof(char) * 255];
char operands[sizeof(char) * 255] = {0};

void parseparam(char **program, Statement *stat, int pos)
{
	char oper = -1;
	int found = 0;

	LOG("\tNAMED PARAMETER");
	while (isalnum(**program)) {
		if (oper == -1) {
			if (!opseen[**program]) {
				LOG("\t\tParameter declaration");

				oper = **program;
				operands[opers] = oper;

				opseen[oper]++;
				oplookup[oper] = opers;

				stat->operands[pos].data.oper = opers;

				opers++;
			} else {
				stat->operands[pos].data.oper = oplookup[oper];
				LOG("\t\tParameter reuse");
			}

		}

		(*program)++;
		found++;
	}

	(*program)--;

	if (found != 1) {
		ERR("Invalid operand");
	}
}

Statement *parse(char **program, int *opcount)
{
	LOG("------START STATEMENT------");
	Statement *stat = malloc(sizeof(Statement));

	char *c;
	int params = 0;
	int expects = -1;
	bool operfound = false;
	char slice[opermax + 1];
	for (c = *program; *c != '\00' && *c != ')'; c++) {
		strncpy(slice, c, opermax);
		slice[opermax] = '\00';

		/* Full size operations */
		/* TODO: Add support for shorthands (& / | / !) */
		if (strncmp("AND", slice, 3) == 0) {
			expects = 2;

			if (params != 1) {
				ERR("Expected one operand preceeding AND");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			LOG("\tAND");
			stat->op = AND;

			c += 3;
			operfound = true;
			continue;
		} else if (strncmp("NOT", slice, 3) == 0) {
			expects = 1;

			if (params > 0) {
				ERR("NOT expects single operand to follow, not preceed");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			LOG("\tNOT");
			stat->op = NOT;

			c += 3;
			operfound = true;
			continue;
		} else if (strncmp("XOR", slice, 3) == 0) {
			expects = 2;

			if (params != 1) {
				ERR("Expected one operand preceeding XOR");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			LOG("\tXOR");
			stat->op = XOR;

			c += 3;
			operfound = true;
			continue;
		} else if (strncmp("OR", slice, 2) == 0) {
			expects = 2;

			if (params != 1) {
				ERR("Expected one operand preceeding OR");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			LOG("\tOR");
			stat->op = OR;

			c += 2;
			operfound = true;
			continue;
		} else if (strncmp("NOR", slice, 3) == 0) {
			expects = 2;

			if (params != 1) {
				ERR("Expected one operand preceeding OR");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			LOG("\tNOR");
			stat->op = NOR;

			c += 3;
			operfound = true;
			continue;
		} else if (strncmp("NAND", slice, 4) == 0) {
			expects = 2;

			if (params != 1) {
				ERR("Expected one operand preceeding OR");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			LOG("\tNAND");
			stat->op = NAND;

			c += 4;
			operfound = true;
			continue;
		}

		if (isspace(*c)) {
			continue;
		} else if (*c == '(') {
			if (expects != -1 && params > expects) {
				ERR("Too many parameters provided");
			}

			LOG("\tSTATEMENT");
			stat->operands[params].t = StatementOperand;
			c++;
			stat->operands[params].data.s = (struct Statement *)parse(&c, &opers);
			params++;
		} else if (isalnum(*c)) {
			if (expects != -1 && params > expects) {
				ERR("Too many parameters provided");
			}

			stat->operands[params].t = NumericOperand;
			parseparam(&c, stat, params);
			params++;
		} else {
			ERR("Syntax error. Invalid identifier");
		}
	}

	if (expects != -1 && params != expects) {
		ERR("Wrong number of operands provided");
	} else if (!operfound) {
		ERR("Statement must contain an operation");
	}

	LOG("------END STATEMENT------");

	*program = c;
	*opcount = opers;
	return stat;
}
