#!/usr/bin/perl

$vplfile = $ARGV[0];

if ($vplfile eq "") {
    die("linkvers.pl Syntax:\n./linkvers.pl <vpl file> [1 - checking mode on].\nMust be run AFTER vpl2mod is completed.\n");
}

$check = $ARGV[1];

open(INF,$vplfile) or die;
while (<INF>) {
    $line = $_;

    $line =~ /([\w ]+:[\d\-]+)\s+(.*)/;
    $vref = $1;

    if ($vref =~ /\-/) {
	$vref =~ /(.*:)(\d+)\-(\d+)/;
	$ch = $1;
	$fv = $2;
	$lv = $3;
	if ($fv + 1 == $lv) {
	    $sv = $lv;
	}
	else {
	    $sv = $fv + 1;
	    $sv .= "-" . $lv;
	}
	$first = $ch . $fv;
	$last = $ch . $sv;

	if ($check ne "") {
	    print "$first\t\t$last\n";
	} else {
	    `addvs -l ./ \"$first\" \"$last\"`;
	}
    }
}
close(INF);




