#!/usr/bin/perl

# This tool is only for locales in Latin-1, not UTF-8 (should such locales be supported at a later date)

use locale;

$abbrevs = 0;

open (INPUT, "$ARGV[0]") or print "Give the locale file as an argument.\n";
@loc = <INPUT>;
close (INPUT);
open (OUTPUT, ">$ARGV[0]");
foreach $line (@loc) {

    if ($line =~ /\[Book Abbrevs\]/) {
	$abbrevs = 1;
    }
    elsif ($abbrevs == 1) {
	$line = uc($line);
    }
    print OUTPUT $line;
}
close (OUTPUT);
