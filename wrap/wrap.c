/*
 * wrap - wrap lines to a specified length
 * Copyright (C) 2023 - Ethan Marshall
 */

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

const char *flags = "pn:l:Ldhu";
int preserve = 0;
int pref = 0, ipref = 0;
int dash = 0;
char *prefbuf = "";
unsigned long wraplen = 80;

void usage()
{
	fputs("Usage: wrap [OPTIONS]... [FILES]...\n", stderr);
	fputs("If no FILES given, stdin is read\n\n", stderr);

	fputs("-p: preserve newlines on input stream\n", stderr);
	fputs("-n [length]: wrap at length characters\n", stderr);
	fputs("-l [prefix]: lead wrapped lines with prefix\n", stderr);
	fputs("-L: include prefix from -l in new line length\n", stderr);
	fputs("-d: include a '-' character when wrapping mid-word\n", stderr);
	fputs("-h: this message\n", stderr);
	fputs("-u: also this message\n", stderr);
	exit(1);
}

void wrap_file(FILE *f)
{
	unsigned long len = 0;
	wchar_t r;
	/* to prevent accidental double spacing */
	int lastnl = 0;
	/* to prevent leading spaces */
	int wrapped = 0;

	while ((r = getwc(f)) != WEOF) {
		wrapped = 0;

		if (len++ == wraplen) {
			if (iswalnum(r) && dash) {
				putwchar(L'-');
			}

			putwchar('\n');
			wprintf(L"%s", prefbuf);
			len = pref * ipref;
			len = 0;
			wrapped = 1;
		}
		if (r == '\n') {
			if (preserve)
				len = 0;
			else {
				if (!lastnl) {
					putwchar(' ');
					lastnl = 1;
				}
			}
		} else {
			lastnl = 0;
		}

		if ((r != '\n' || preserve) && !(wrapped && r == ' '))
			putwchar(r);
	}
}

int flag_parse(int argc, char **argv)
{
	int opt;
	while ((opt = getopt(argc, argv, flags)) != -1) {
		switch (opt) {
		case 'p':
			preserve = 1;
			break;
		case 'n':
			wraplen = strtoul(optarg, NULL, 10);
			if (!wraplen) {
				fprintf(stderr, "wrap: %lu: invalid wrap length\n", wraplen);
				return 0;
			}
			break;
		case 'l':
			pref = strlen(optarg);
			prefbuf = strdup(optarg);
			break;
		case 'L':
			ipref = 1;
			break;
		case 'd':
			dash = 1;
			break;
		case 'h':
		case 'u':
			usage();
			break;
		default:
			return 0;
		}
	}

	return 1;
}

int main(int argc, char **argv)
{
	int seenstdin = 0;
	int numf = 0, fcap = 1;
	FILE **files = malloc(sizeof(FILE *) * fcap);

	setlocale(LC_ALL, "");
	if (!flag_parse(argc, argv))
		return 1;

	for (int i = optind; i < argc; i++) {
		FILE *f;

		if (strcmp(argv[i], "-") != 0) {
			f = fopen(argv[i], "r");
		} else {
			if (seenstdin) {
				fputs("wrap: cannot wrap stdin multiple times\n", stderr);
				continue;
			}
			f = stdin;
			seenstdin = 1;
		}

		if (!f) {
			perror(argv[i]);
			continue;
		}

		if (numf == fcap) {
			fcap *= 2;
			files = realloc(files, sizeof(FILE *) * fcap);
			if (!files) {
				perror("realloc");
				return 127;
			}
		}

		files[numf++] = f;
	}

	if (numf == 0) {
		*files = stdin;
		numf++;
	}

	for (int i = 0; i < numf; i++) {
		wrap_file(files[i]);
		fclose(files[i]);

		if (!preserve)
			putwchar('\n');
	}

	if (pref)
		free(prefbuf);
	free(files);
}
