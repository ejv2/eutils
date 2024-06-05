#!/bin/perl
#
# smult -- sums to and multiplies to calculator
#
# This is based on the quadratic formula.
# Let q be a quadratic equal to (x - a)(x - b).
# q has the form x^2 - (a + b)x + ab
# Now let s = a + b; p = ab
# a and b are both roots so:
# 	a = 0.5(s + sqrt(s^2 -4p)
# 	b = 0.5(s - sqrt(s^2 - 4p)
#
# If no real numbers exhist which satisfy the requirements:
# 	s^2 - 4p < 0

use strict;
use warnings;

if (scalar(@ARGV) < 2) {
	printf("need 2 arguments, got %d\n", scalar(@ARGV));
	exit 1;
}

# sum
my $s = $ARGV[0];
# product
my $p = $ARGV[1];

# discriminant < 0
if ($s**2 < (4 * $p)) {
	printf("smult: no such real numbers\n");
	exit 2;
}

my $a = 0.5 * ($s + sqrt($s**2 - (4 * $p)));
my $b = 0.5 * ($s - sqrt($s**2 - (4 * $p)));

printf("a: %f, b: %f\n", $a, $b);
