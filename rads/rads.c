#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char buf[BUFSIZ];
	long double parse;

	/*
	 * 360 degrees is 2M_PI radians
	 */
	while (fgets(buf, BUFSIZ, stdin)) {
		parse = strtof(buf, NULL);
		printf("%Lf\n", (parse/360)*2*M_PI);
	}
}
