/*
 * Unicode2Ascii
 * Quick utility for getting unicode character codes.
 * (Has nothing, in fact, to do with ASCII)
 *
 * For UNIX-related reasons, does not support a feature
 * of its predecessor where strings can be passed via
 * argv.
 */

#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

int raw = 0;
wchar_t buf[BUFSIZ];

int main(int argc, char **argv)
{
	/* allow us to use UTF-8 - if appropriate */
	setlocale(LC_ALL, "");

	if (argc > 1 && strcmp(argv[1], "-r") == 0) {
		raw = 1;
	}

	for (;;) {
		const wchar_t *ret = fgetws(buf, BUFSIZ-1, stdin);
		if (!ret)
			return 0;

		for (wchar_t *c = &buf[0]; *c != '\n' && *c != L'\0'; c++) {
			if (raw) {
				printf("%u ", (unsigned int)*c);
			} else {
				printf("U+%X ", (unsigned int)*c);
			}

		}

		putchar('\n');
	}
}
