#!/usr/bin/perl

#change this variable to hostname of your server
$hostname = "bible.yourhost.com";

@values = split(/\&/,$ENV{'QUERY_STRING'});
foreach $i (@values) {
    ($varname, $mydata) = split(/=/,$i);
    if ($varname eq "defversion") {
	$COOKIE = "DEFTRANS=$mydata";
    }
    else if ($varname eq "settrans") {
	$COOKIE = "SETTRANS=$mydata";
    }
    else if ($varname eq "setcomm") {
	$COOKIE = "SETCOMM=$mydata";
    }
    else if ($varname eq "setld") {
	$COOKIE = "SETLD=$mydata";
    }
}

$DOMAIN = '$hostname';
$EXPIRES = 'Fri Dec 31 23:59:00 GMT 2000';

# Set the cookie and send the user the thank you page.
print "Set-cookie: $COOKIE\n";
print "Content-type: text/html\n\n";  #Note extra newline to mark
                                        #end of header.

print "<html><meta http-equiv=\"refresh\" content=\"0\; URL=http://$hostname\"></html>";









