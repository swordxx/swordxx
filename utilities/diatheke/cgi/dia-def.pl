#!/usr/bin/perl

#change this variable to hostname of your server
@values = split(/\&/,$ENV{'QUERY_STRING'});
$DOMAIN = $ENV{'REMOTE_ADDR'};
$EXPIRES = 'Fri Dec 31 23:59:00 GMT 2005';

foreach $i (@values) {
    ($varname, $mydata) = split(/=/,$i);
    if ($varname eq "defversion") {
	$COOKIE = "DEFTRANS=$mydata ; expires=$EXPIRES";
    }
    elsif ($varname eq "settrans") {
	$COOKIE = "SETTRANS=$mydata ; expires=$EXPIRES";
    }
    elsif ($varname eq "setcomm") {
	$COOKIE = "SETCOMM=$mydata ; expires=$EXPIRES";
    }
    elsif ($varname eq "setld") {
	$COOKIE = "SETLD=$mydata ; expires=$EXPIRES";
    }
}

# Set the cookie and send the user the thank you page.
print "Set-cookie: $COOKIE\n";
print "Content-type: text/html\n\n";  #Note extra newline to mark
                                        #end of header.

print "<html><meta http-equiv=\"refresh\" content=\"0\; URL=$ENV{'HTTP_REFERER'}\"></html>";









