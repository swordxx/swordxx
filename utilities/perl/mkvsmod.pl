#!/usr/bin/perl

open (INF, $ARGV[0]);

`addvs -c ./`;
while (<INF>) {
    
    $line = $_;
    $line =~ s/[\r\n]//g;
    $line =~ /(.+\d+:\d+:?) +(.*)/;
    $ref = $1;
    $ver = $2;
    open (BUF, ">buffer");
    print BUF "$ver";
    close (BUF);
    $x = `addvs -a ./ \"$ref\" buffer`;
    print "$ref\n";

}

close (INF);
