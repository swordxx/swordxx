#!/usr/bin/perl

## This is a test app and has no other purpose.

use SOAP::Lite;

my ($books, $key, $options, $encoding, $markup, $searchtype, $locale, $script, $max) = @ARGV;

# uncomment to see parameters being passed to the SOAP server (for debugging)
# print "$books\n$key\n$options\n$encoding\n$markup\n$searchtype\n$locale\n$script\n$max\n";

print SOAP::Lite
    -> uri('http://bible.gotjesus.org/sapouni')
    -> proxy('http://bible.gotjesus.org/cgi-bin/sapouni.cgi')
    -> biblequery($books, $key, $options, $encoding, $markup, $searchtype, $locale, $script, $max)
    -> result;
