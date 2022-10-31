/*
 * truth - evaluate a simple truth table
 * Copyright (C) 2021 - Ethan Marshall
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

#include "truth.h"

bool verbose;
static char inbuf[BUFSIZ];

void handle_flags(const int argc, char *const *argv, char **text)
{
	const char *valid_flags = "v";
	char opt;

	while ((opt = getopt(argc, argv, valid_flags)) != -1) {
		switch (opt) {
		case 'v':
			verbose = true;
			break;
		case '?':
		default:
			USAGE(argv[0], "[-v] <table>");
			break;
		}
	}

	if (argc == optind) {
		fgets(inbuf, BUFSIZ, stdin);
		*text = inbuf;
		return;
	}

	/* program text is in remaining argv */
	*text = argv[optind];
}

int main(int argc, char **argv)
{
	char *text;
	int opcount;
	Statement *prg;

	handle_flags(argc, argv, &text);

	LOG("==========[---PARSE TRACE BEGINS---]==========");
	prg = parse(&text, &opcount);
	LOG("==========[---PARSE TRACE ENDS---]==========\n");

	LOG("==========[---PROGRAM TRACE BEGINS---]==========");
	run(prg, opcount);
	LOG("==========[---PROGRAM TRACE ENDS---]==========");
}
