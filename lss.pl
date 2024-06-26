#!/usr/bin/perl
# lss - least sum squares calculator
# Copyright (C) 2024 - Ethan Marshall

use feature "say";

# Statistical data
# Data count
my $n = 0;
# Sum of x
my $sx = 0;
# Sum of y
my $sy = 0;
# Sum of x squared
my $sxx = 0;
# Sum of x*y for each pair
my $sxy = 0;

# Guards against impossible regression lines
my $tn = 0;
my %seen = ();

while (1) {
	my $line = <stdin>;
	last unless defined $line;

	chomp($line);
	my @segs = split(",", $line);

	if (scalar(@segs) != 2) {
		say "Invalid datum syntax; need [x,y]";
		exit 1;
	}

	$sx += $segs[0];
	$sy += $segs[1];

	$sxx += $segs[0]**2;
	$sxy += $segs[0] * $segs[1];

	$n++;

	if (not defined($seen{$line + 0})) {
		$tn++;
		$seen{$line + 0} = 1;
	}
}

if ($tn < 2) {
	say "Need at least two data points with unique x coordinates";
	exit 2;
}

my $c = (($sxx * $sy) - ($sxy * $sx))/(($n * $sxx) - $sx**2);
my $m = ($n*$sxy - $sx*$sy)/($n*$sxx - $sx**2);

my $ca = abs($c);
my $sym = ($c >= 0) ? '+' : '-';

say "y = ${m}x $sym $ca";
