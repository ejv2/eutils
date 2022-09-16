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
	const char *valid_flags = "v:";
	char opt;

	/* 
	* no option passed
	*/
	if (argc == 1) {
		fgets(inbuf, BUFSIZ, stdin);
		*text = inbuf;
		return;
	}

	*text = argv[1];
	while ((opt = getopt(argc, argv, valid_flags)) != -1) {
		switch (opt) {
		case 'v':
			verbose = true;

			/* 
			 * if -v was first passed 
			 * instead of table
			 */
			if (optarg) {
			    *text = optarg;
			}

			break;
		case '?':
		default:
			USAGE(argv[0], "<table> [-v]");
			break;
		}
	}
}

int main(int argc, char **argv)
{
	char *text;
	Statement *prg;

	handle_flags(argc, argv, &text);

	LOG("==========[---PARSE TRACE BEGINS---]==========");
	prg = parse(&text);
	LOG("==========[---PARSE TRACE ENDS---]==========\n");

	LOG("==========[---PROGRAM TRACE BEGINS---]==========");
	run(prg);
	LOG("==========[---PROGRAM TRACE ENDS---]==========");
}
