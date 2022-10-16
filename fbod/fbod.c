#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define LENGTH(x) sizeof(x) / sizeof(x[0])
#define MAX_FORCES 255
#define EPSILON 1E-5F

struct force_t {
	long double i, j;
};

int parse_column(const char *in, struct force_t *out)
{
	char *next;

	/* sanity check */
	if (!in || in[0] != '[') {
		fputs("panic: incorrect parser method: parse_column\n", stderr);
		abort();
	}

	out->i = strtold(&in[1], &next);
	if (*next != ' ' && *next != '\t') {
		fputs("fbod: invalid column syntax\n", stderr);
		return 0;
	}

	out->j = strtold(&next[1], &next);
	if (*next != ']') {
		fputs("fbod: invalid column syntax\n", stderr);
		return 0;
	}

	return 1;
}

int parse_basis(char *in, struct force_t *out)
{
	char *endptr = &in[1];
	int neg = 0, is = 0, js = 0;
	long double work;

	if (!in || in[0] != '(') {
		fputs("panic: incorrect parser method: parse_basis\n", stderr);
		abort();
	}

	for (;;) {
		work = strtold(endptr, &endptr);
		if (neg)
			work = -work;

		switch (*endptr) {
		case 'i':
			out->i += work;
			is = 1, neg = 0;
			break;
		case 'j':
			out->j += work;
			js = 1, neg = 0;
			break;
		case '-':
			neg = 1;
			break;
		case ' ':
		case '+':
			break;
		case ')':
		case '\0':
			goto out;
			break;
		default:
			fputs("fbod: invalid vector syntax: expect (ai + bj) or (bj + ai)\n", stderr);
			return 0;
		}

		endptr++;
	}

out:
	if (!is && !js) {
		fputs("fbod: invalid vector syntax: expect (ai + bj) or (bj + ai)\n", stderr);
		return 0;
	}

	return 1;
}

int parse_conventional(char *in, struct force_t *out)
{
	char *work0, *work1;
	long double iwork0, iwork1;

	if (!in) {
		fputs("panic: incorrect parser method: parse_conventional\n", stderr);
		abort();
	}

	work0 = strtok(in, " ");
	work1 = strtok(NULL, " ");
	if (!work0 || !work1) {
		fputs("fbod: invalid vector syntax: expect <magnitude> <direction in degrees>\n", stderr);
		return 0;
	}
	iwork0 = strtold(work0, NULL);
	/* angle: degress to radians */
	iwork1 = (strtold(work1, NULL) / 360.0) * 2.0 * M_PI;

	/*
	 * note: angle is measured in redians from the y-axis
	 * eg. a horizontal vector along the x-axis has angle of 90 degrees (M_PI_2 radians)
	 */
	out->i = iwork0 * sin(iwork1);
	out->j = iwork0 * cos(iwork1);

	return 1;
}

int parse_forces(struct force_t *fbuf, unsigned int maxlen)
{
	unsigned int curind = 0;
	char *walk, *nl;
	char buf[BUFSIZ];
	while (fgets(buf, BUFSIZ - 1, stdin)) {
		if (curind >= maxlen) {
			fprintf(stderr, "fbod: warning: too many input vectors; stopping at %u", maxlen);
			return 1;
		}
		memset(&fbuf[curind], 0, sizeof(fbuf[curind]));

		/* strip newline and leading whitespace */
		if ((nl = strchr(buf, '\n'))) {
			*nl = 0;
		}
		for (walk = buf; *walk && isspace(*walk); walk++) {
			*walk = 0;
		}

		switch (buf[0]) {
		case '[':  /* column vector */
			if (!parse_column(buf, &fbuf[curind++]))
				return 0;
			break;
		case '(':  /* in terms of i and j */
			if (!parse_basis(buf, &fbuf[curind++]))
				return 0;
			break;
		case '\0': /* ignore blank lines */
			break;
		default:  /* assume in magnitude and angle */
			if (!parse_conventional(buf, &fbuf[curind++]))
				return 0;
			break;
		}
	}

	return curind;
}

int main(int argc, char **argv)
{
	char *voff = "       ", *hoff =  "           ";
	int i, j, buflen, off;
	long double nx, ny, px, py;
	long double mag, theta;
	struct force_t fbuf[MAX_FORCES];
	struct force_t result[4];
	char obuf[1024], padding[50];

	memset(padding, ' ', sizeof(padding));
	if (!(buflen = parse_forces(fbuf, MAX_FORCES))) {
		return 1;
	}

	px = py = nx = ny = 0;
	for (i = 0; i < buflen; i++) {
		if (fbuf[i].i < 0) {
			nx += fabsl(fbuf[i].i);
		} else {
			px += fbuf[i].i;
		}

		if (fbuf[i].j < 0) {
			ny += fabsl(fbuf[i].j);
		} else {
			py += fbuf[i].j;
		}
	}

	off = snprintf(obuf, 1024, "%.2LfN", nx);
	if (off >= LENGTH(padding)) {
		fprintf(stderr, "fbod: fp result too large");
		abort();
	}

	padding[off] = 0;
	if (nx < EPSILON && nx >= 0)
		voff = "      ";
	else
		hoff = "";

	/* upward arrow */
	if (py > EPSILON) {
		printf("%s%s%.2LfN\n%s%s^\n", voff, padding, py, voff, padding);
		for (i = 0; i < 3; i++) {
			printf("%s%s|\n", padding, voff);
		}
	}

	/* horizontal arrows */
	if (nx > EPSILON)
		printf("%.2LfN<------", nx);
	printf("%sO", hoff);
	if (px > EPSILON)
		printf("------>%.2LfN", px);
	putchar('\n');

	/* upward arrows */
	if (ny > EPSILON) {
		for (i = 0; i < 3; i++) {
			printf("%s%s|\n", padding, voff);
		}
		printf("%s%sV\n%s%s%.2LfN\n", voff, padding, voff, padding, ny);
	}

	/* magnitude and direction of resultant */
	mag = sqrtl((px - nx) * (px - nx) + (py - ny) * ((py - ny)));
	theta = (atanl((px - nx) / (py - ny)) / M_PI) * 180;

	/* add axis rotations to theta */
	if (py - ny < -EPSILON)
		theta += 180;
	else if (px - nx < -EPSILON)
		theta += 360;

	printf("\nresultant: x: %Lf, y: %Lf, mag: %Lf, theta: %Lf\n", px - nx, py - ny, mag, theta);
}
