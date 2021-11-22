#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ERR(msg) fprintf(stderr, "Error: %s\n", msg); exit(1)

typedef enum { AND = 1, OR, NOT } Operation;
typedef enum { NullOperand = 0, StatementOperand = 1, NumericOperand } OperandType;
typedef struct {
	OperandType t;
	union {
		struct Statement *s;
		char oper;
	} data;
} Operand;
typedef struct {
	Operand operands[2];
	Operation op;
} Statement;

static const int opermax = 3;

static char inbuf[BUFSIZ];
static int opers = 0;
static char operands[128] = {0};

void parseparam(char **program, Statement *stat, int pos)
{
	char oper = -1;

	puts("START NAMED PARAMETER");
	while (isalnum(**program)) {
		printf("\tPARAMPARSE: %c\n", **program);

		operands[opers++] = **program;

		if (oper == -1) {
			oper = **program;
			stat->operands[pos].data.oper = oper;
		}

		(*program)++;
	}

	(*program)--;
	printf("Operand ends on '%c'\n", **program);
	puts("END NAMED PARAMETER");
}

Statement *parse(char **program)
{
	puts("------START RECURSION------");
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
		if (strcmp("AND", slice) == 0) {
			expects = 2;

			if (params != 1) {
				ERR("Expected one operand preceeding AND");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			puts("====[AND]====");
			stat->op = AND;

			c += 3;
			operfound = true;
			continue;
		} else if (strcmp("NOT", slice) == 0) {
			expects = 1;

			if (params > 0) {
				ERR("NOT expects single operand to follow, not preceed");
			} else if (operfound) {
				ERR("Expected only a single operation");
			}

			puts("====[NOT]====");
			stat->op = NOT;

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

			puts("====[OR]====");
			stat->op = OR;

			c += 2;
			operfound = true;
			continue;
		}

		if (isspace(*c)) {
			continue;
		} else if (*c == '(') {
			if (expects != -1 && params > expects) {
				ERR("Too many parameters provided");
			}

			puts("START STATEMENT PARAMETER");
			stat->operands[params].t = StatementOperand;
			c++;
			stat->operands[params].data.s = (struct Statement *)parse(&c);
			params++;
			puts("END STATEMENT PARAMETER");
		} else {
			if (expects != -1 && params > expects) {
				ERR("Too many parameters provided");
			}

			stat->operands[params].t = NumericOperand;
			parseparam(&c, stat, params);
			params++;
		}
	}

	if (expects != -1 && params != expects) {
		ERR("Wrong number of operands provided");
	}

	puts("------END RECURSION------");
	printf("Statement ends on '%c'\n", *c);

	*program = c;
	return stat;
}

int main(int argc, char **argv)
{
	char *text;
	Statement *prg;

	if (argc <= 1) {
		fgets(inbuf, BUFSIZ, stdin);
		text = inbuf;
	} else {
		text = argv[1];
	}

	prg = parse(&text);
}
