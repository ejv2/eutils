#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char buf[BUFSIZ];
	long double parse;

	/*
	 * M_PI radians is 180 degrees
	 */
	while (fgets(buf, BUFSIZ, stdin)) {
		parse = strtof(buf, NULL);
		printf("%Lf\n", (parse/M_PI)*180);
	}
}
