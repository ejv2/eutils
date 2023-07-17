#!/bin/perl
#
# Taylor Series printer for sin and cosin, formatted for use in qalc or desmos.
#

use strict;
use warnings;
use diagnostics;

use feature 'say';

# Enable sin
my $esin = 1;
# Enable cos
my $ecos = 1;
# Enable power parentheses
my $opar = "", my $cpar = "";
# Number of terms
my $tcount = 15;

my $help = <<'EOF';
tayser -- print out a trigonometric Taylor Series

Options:

	-s|--sin    -- print sin only
	-c|--cos    -- print cos only
	-n|--number -- print this number of terms
	-p|--parpow -- parenthesize powers of x
	-h|--help   -- this message

Written by Ethan Marshall - 2023
EOF


sub do_sin {
	my ($count) = @_;
	if ($count == 0) {
		return "x ";
	}

	my $exp = (2 * $count) + 1;
	my $oper = "+";
	if ($count % 2 == 1) {
		$oper = "-";
	}

	return "$oper (x^$opar$exp$cpar)/($exp!) ";
}

sub do_cos {
	my ($count) = @_;

	if ($count == 0) {
		return "1 ";
	}

	my $oper = "+";
	if ($count % 2 == 1) {
		$oper = "-";
	}
	my $iter = 2 * $count;

	return "$oper (x^$opar$iter$cpar)/($iter!) ";
}

my $nextcount = 0;
foreach my $a (@ARGV) {
	if ($nextcount) {
		$nextcount = 0;
		$tcount = $a;
		next;
	}

	if ($a eq "-s" || $a eq "--sin") {
		$esin = 1;
		$ecos = 0;
	} elsif ($a eq "-c" || $a eq "--cos") {
		$ecos = 1;
		$esin = 0;
	} elsif ($a eq "-n" || $a eq "--number") {
		$nextcount = 1;
	} elsif ($a eq "-p" || $a eq "--parpow") {
		$opar = "(", $cpar = ")";
	} elsif ($a eq "-h" || $a eq "--help") {
		printf "$help";
		exit 1;
	} else {
		say "tayser: unrecognised option -- $a";
		exit 1;
	}
}
if ($nextcount) {
	say "tayser: expected count";
	exit 1;
}

my $sinbuf = "";
my $cosbuf = "";

for (my $counter = 0; $counter < $tcount; $counter++) {
	if ($esin) {
		$sinbuf = $sinbuf . do_sin($counter);
	}
	if ($ecos) {
		$cosbuf = $cosbuf . do_cos($counter);
	}
}

if ($esin) {
	say "sin: $sinbuf";
}
if ($ecos) {
	say "cos: $cosbuf";
}
