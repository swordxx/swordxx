#!/usr/bin/perl

# cipherkeygen.pl

# generates a cipher key of the format \d{4}[a-zA-Z]{4}\d{4}[a-zA-Z]{4}.
# because I'm lazy and not random enough.
# and because the utilities/perl directory is kinda bare...


# let's get a base key of \d{4}[A-Z]{4}\d{4}[A-Z]{4}
$key = int(rand() * 10) . int(rand() * 10) . int(rand() * 10) . int(rand() * 10) . chr(int(rand() * 26) + 0x41) . chr(int(rand() * 26) + 0x41) . chr(int(rand() * 26) + 0x41) . chr(int(rand() * 26) + 0x41) . int(rand() * 10) . int(rand() * 10) . int(rand() * 10) . int(rand() * 10) . chr(int(rand() * 26) + 0x41) . chr(int(rand() * 26) + 0x41) . chr(int(rand() * 26) + 0x41) . chr(int(rand() * 26) + 0x41);

# now randomly lowercase the letters, printing as we go
foreach $c (unpack ("cccccccccccccccc", $key)) {
    $c = chr($c);
    if (rand() < 0.5) {
	$c = lc($c);
    }
    print $c;
}
print "\n";



