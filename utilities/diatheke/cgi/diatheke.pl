#!/usr/bin/perl

#version 3.1

$diatheke = "nice /usr/bin/diatheke";  # location of diatheke command line program -- if you are using a MS Windows server, you might need to remove the "nice"
$defaultfontface = "Arial, Helvetica, sans-serif"; # default font name
$sword_path = "/home/sword";  # SWORD_PATH environment variable you want to use
$maxverses = 50; # maximum number of verses diatheke will return per query (prevents people from asking for Gen1:1-Rev22:21)
$defaultbook = "KJV"; # book to query when none is selected, but a verse/search is entered
$deflocale = en;  # this is just the default for cases where user has not selected a locale and his browser does not reveal one -- you can also set locale using locael=<locale> in the GET URL


###############################################################################
## You should not need to edit anything below this line.
## Unless you want to modify functionality of course. :)
###############################################################################

$ENV{'SWORD_PATH'} = $sword_path;

print "Content-type: text/html\n\n";


if ($ENV{'HTTP_COOKIE'}) {

    $cookie = $ENV{'HTTP_COOKIE'};
    $cookie =~ s/\; /=/g;
    %cookiedata = split(/=/, $cookie);
    
    $defversion = $cookiedata{DEFTRANS};
    $locale = $cookiedata{LOCALE};
}

if ($defversion eq "") {
    $defversion = 'KJV';
}
if ($locale eq "") {
    $locale = $ENV{'HTTP_ACCEPT_LANGUAGE'};
    if ($locale eq "") {
	$locale = $deflocale;
    }
}

$locale =~ s/(..).*/$1/;


$hostname = $ENV{'REMOTE_ADDR'};
@values = split(/\&/,$ENV{'QUERY_STRING'});
$n = 0;
$palm = 0;
$footnotes = 0;
$strongs = 0;
$arg = "-";
foreach $i (@values) {
    ($varname, $mydata) = split(/=/,$i);
    if ($varname ne "Submit" && $varname ne "lookup") {
	if ($varname eq "verse") {
	    $verse = $mydata;
	    $verse =~ tr/+/ /;
	    $verse =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
	}
	elsif ($varname eq "search") {
	    $search = $mydata;
	}
	elsif ($varname eq "strongs") {
	    $arg .= "n";
	    $strongs = 1;
	}
	elsif ($varname eq "footnotes") {
	    $arg .= "f";
	    $footnotes = 1;
	}
	elsif ($varname eq "palm") {
	    $palm = 1;
	}
	elsif ($varname eq "locale") {
	    $locale = $mydata;
	}
	elsif ($mydata eq "on" || $mydata eq "ON") {
	    $versions[$n] = $varname;
	    $n++;
	}
    }
}

if ($n == 0) {
    $versions[0] = $defaultbook;
    $n++;
}

if ($verse eq "") {

    @versionlist = `$diatheke -m 2> /dev/null`;    
    @versionlist2 = @versionlist;
    @localelist = `$diatheke -l 2> /dev/null`;

    print <<DEF1;
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>Diatheke Insta-Interlinear Bible</title>
</head>

<body>

<form method="get" action="diatheke.pl">
  <p /><input type="radio" name="search" checked value="off"><font face="Arial, Helvetica, sans-serif">Verse/Commentary
  Lookup</font><br />
  <input type="radio" name="search" value="on"><font face="Arial, Helvetica, sans-serif">Word
  Search&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  Verse or Word:</font><input type="text" name="verse" size="20"><input type="submit" name="Submit" value="Submit"><input type="reset" name="Reset" value="Reset">
  <p /><input type="checkbox" name="strongs" value="on"><font size="-1" face="Arial, Helvetica, sans-serif">Show
  Stong's Numbers when available (Strong's numbered modules are marked by *)</font>
  <p /><input type="checkbox" name="footnotes" value="on"><font size="-1" face="Arial, Helvetica, sans-serif">Show
  Footnotes when available</font><br />
  &nbsp;
  <table BORDER="0" WIDTH="100%">
DEF1
    
    foreach $line (@versionlist) {
	chomp($line);

	if ($line eq "Biblical Texts:") {
	    print "<tr><td><font face=$defaultfontface><b>Biblical Texts:</b></font><br /></td></tr>";
	}
	elsif ($line eq "Commentaries:") {
	    print "<tr><td><font face=$defaultfontface><b>Commentaries:</b></font></td></tr>";
	}
	elsif ($line eq "Dictionaries:") {
	    print "<tr><td><font face=$defaultfontface><b>Dictionaries & Lexica:</b></font></td></tr>";
	}
	else {
	    $line =~ s/([^:]+) : (.+)/<tr><td><input type=\"checkbox\" name=\"$1\" value=\"on\"><font size=\"-1\" face=$defaultfontface>$2 ($1)<\/font><\/td><\/tr>/;
	    print "$line\n";
	}

    }

    print <<DEF2;
        </table>
</form>

<form method="get" action="dia-def.pl">
  Select default Bible version for cross-references:&nbsp;<select name="defversion" size="1">
  
DEF2
    
    $biblesflag = 1;
    foreach $line (@versionlist2) {
	if ($biblesflag == 1) {
	    chomp ($line);
	    if ($line eq "Biblical Texts:") {
	    }
	    elsif ($line eq "Commentaries:") {
		$biblesflag = 0;
	    }
	    else {
		$line =~ s/([^:]+) : (.+)/<option value=\"$1\">$2 ($1)<\/option>/;
		print "$line\n";
	    }
	}
    }

    print <<DEF3;
</select><input type="submit" name="Submit" value="Submit"></form><br/><form method="get" action="dia-def.pl">Select locale:&nbsp;
<select name="locale" size="1"><option value="">browser default</option>
<option value="en">en</option>
DEF3
    foreach $line (@localelist) {
	chomp($line);
	print "<option value=\"$line\">$line<\/option>";
    }
print <<DEF4
</select>
<input type="submit" name="Submit" value="Submit">
</form>
</body>
</html>
DEF4

}
else {



if ($palm == 0) {
print <<END;

<html><head>
<title>Diatheke Interlinear Bible</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta name="palmcomputingplatform" content="true">
<meta name="historylisttext" content="Diatheke">
<style type="text/css">
#divBottom{position:absolute; visibility:hidden; font-family:arial,helvetica; height:30; width:100; font-size:10pt; font-weight:bold}
#A:link, A:visited, A:active{text-decoration: none}
</style>
<script type="text/javascript" language="JavaScript">
/********************************************************************************
Copyright (C) 1999 Thomas Brattli
This script is made by and copyrighted to Thomas Brattli at www.bratta.com
Visit for more great scripts. This may be used freely as long as this msg is intact!
I will also appriciate any links you could give me.
********************************************************************************/
//Default browsercheck, added to all scripts!
function checkBrowser(){
    this.ver=navigator.appVersion;
    this.dom=document.getElementById?1:0;
    this.ie5=(this.ver.indexOf("MSIE 5")>-1 && this.dom)?1:0;
    this.ie4=(document.all && !this.dom)?1:0;
    this.ns5=(this.dom && parseInt(this.ver) >= 5) ?1:0;
    this.ns4=(document.layers && !this.dom)?1:0;
    this.bw=(this.ie5 || this.ie4 || this.ns4 || this.ns5);
    return this;
}
bw=new checkBrowser()
/********************************************************************************
Remeber to set the look of the divBottom layer in the stylesheet (if you wan't
another font or something)
********************************************************************************/
/*Set these values, gright for how much from the right you wan't the layer to go
and gbottom for how much from the bottom you want it*/
var gright=160
var gbottom=80



/********************************************************************************
Constructing the ChangeText object
********************************************************************************/
function makeObj(obj,nest){
    nest=(!nest) ? '':'document.'+nest+'.';
    this.css=bw.dom? document.getElementById(obj).style:bw.ie4?document.all[obj].style:bw.ns4?eval(nest+"document.layers." +obj):0;
    this.moveIt=b_moveIt;
}
function b_moveIt(x,y){this.x=x; this.y=y; this.css.left=this.x; this.css.top=this.y}

/********************************************************************************
Initilizing the page, getting height and width to moveto and calls the 
object constructor
********************************************************************************/
    function geoInit(){
	oGeo=new makeObj('divBottom');
	pageWidth=(bw.ie4 || bw.ie5)?document.body.offsetWidth-4:innerWidth;
	pageHeight=(bw.ie4 || bw.ie5)?document.body.offsetHeight-2:innerHeight;
	checkIt();
	// sets the resize handler.
	onresize=resized;
	if(bw.ie4 || bw.ie5) window.onscroll=checkIt;
	// shows the div
	oGeo.css.visibility='visible';
    }
/********************************************************************************
This function executes onscroll in ie and every 30 millisecond in ns
and checks if the user have scrolled, and if it has it moves the layer.
********************************************************************************/
function checkIt(){
    if(bw.ie4 || bw.ie5) oGeo.moveIt(document.body.scrollLeft +pageWidth-gright,document.body.scrollTop+pageHeight-gbottom);
    else if(bw.ns4){
	oGeo.moveIt(window.pageXOffset+pageWidth-gright, window.pageYOffset+pageHeight-gbottom);
	setTimeout('checkIt()',30);
    }
}

//Adds a onresize event handler to handle the resizing of the window.
function resized(){
    pageWidth=(bw.ie4 || bw.ie5)?document.body.offsetWidth-4:innerWidth;
    pageHeight=(bw.ie4 || bw.ie5)?document.body.offsetHeight-2:innerHeight;
    if(bw.ie4 || bw.ie5) checkIt()
}


//Calls the geoInit onload
if(bw.bw && !bw.ns5) onload=geoInit;

//Here we will write the div out so that lower browser won't see it.'
if(bw.bw && !bw.ns5) document.write('<div id="divBottom"><center>Powered by<br /><img src="http://www.crosswire.org/sword/pbsword.gif"><br /><a href="http://www.crosswire.org/">www.crosswire.org</a></center></div>')
</script>

</head>

<body bgcolor="#FFFFFF"><font face="$defaultfontface">

END
							      }
else {
print <<END

<html><head>
<title>HANDiatheke</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta name="palmcomputingplatform" content="true">
<meta name="historylisttext" content="HANDiatheke">
</head>
<body bgcolor="#FFFFFF"><font face="$defaultfontface">
END
}
for ($i = 0; $i < $n; $i++) {
    
    if ($search eq "on") {
	
	$line = `$diatheke -s $versions[$i] \"$verse\" $locale 2> /dev/null`;

    }
    else {
	if ($versions[$i] eq "MHC" || $versions[$i] eq "RWP" || $versions[$i] eq "DTN" || $versions[$i] eq "Family" || $versions[$i] eq "Geneva" || $versions[$i] eq "JFB" || $versions[$i] eq "PNT" || $versions[$i] eq "TSK" || $versions[$i] eq "MHCC") {
	    $arg .= "c";
	    $line = `$diatheke $arg $versions[$i] \"$verse\" $locale thml $maxverses 2> /dev/null`;
	}
	elsif ($versions[$i] eq "Vines" || $versions[$i] eq "Naves" || $versions[$i] eq "Eastons" || $versions[$i] eq "StrongsGreek" || $versions[$i] eq "StrongsHebrew" || $versions[$i] eq "Thayer" || $versions[$i] eq "BDB" || $versions[$i] eq "Hitchcocks" || $versions[$i] eq "ISBE" || $versions[$i] eq "Smiths" || $versions[$i] eq "MCS" || $versions[$i] eq "Anchor" || $versions[$i] eq "WBE") {
	    $arg .= "d";
	    $line = `$diatheke $arg $versions[$i] \"$verse\" $locale thml $maxverses 2> /dev/null`;
	}
	else {
	    $arg .= "b";
	    $line = `$diatheke $arg $versions[$i] \"$verse\" $locale thml $maxverses 2> /dev/null`;
	}
    }
    chomp($line);

# Parse and link to Strong's references if present
    
#    $line =~ s/\n/<br />/g;
    
#    $line =~ s/<FI>/<i>/g;
#    $line =~ s/<Fi>/<\/i>/g;
#    $line =~ s/<CM>//g;
#    $line =~ s/<FNgreek>/<FNBSTGreek>/g;
#    $line =~ s/<FNhebrew>/<FNBSTHebrew>/g;
#    $line =~ s/<FN([^>]+)>/<\/font><font face=\"$1\" size=+1>/g;
#    $line =~ s/<Fn>/<\/font><font face=\"$defaultfontface\">/g;
#    $line =~ s/<RF>/\(Footnote: /g;
#    $line =~ s/<Rf>/\)/g;		    
    
    $line =~ s/<sync type=\"Strongs\" value=\"\w?H([0-9]+)\" \/>/<a href=\"diatheke.pl?verse=$1&StrongsHebrew=on\">&lt;$1&gt;\<\/a\>/g;
    $line =~ s/<sync type=\"Strongs\" value=\"\w?G([0-9]+)\" \/>/<a href=\"diatheke.pl?verse=$1&&StrongsGreek=on\">&lt;$1&gt;\<\/a\>/g;


    if ($versions[$i] eq "StrongsHebrew") {
	$line =~ s/(see HEBREW for )([0-9]+)/<a href=\"diatheke.pl?verse=$2&StrongsHebrew=on\">$1$2\<\/a\>/g;
    }
    elsif($versions[$i] eq "StrongsGreek") {
	$line =~ s/(see GREEK for )([0-9]+)/<a href=\"diatheke.pl?verse=$2&StrongsGreek=on\">$1$2\<\/a\>/g;
    }
    elsif ($versions[$i] eq "BDB") {
	$line =~ s/([0-9][0-9][0-9][0-9]+)/<a href=\"diatheke.pl?verse=$1&BDB=on\">$1\<\/a\>/g;
    }
    elsif($versions[$i] eq "Thayer") {
	$line =~ s/([0-9][0-9][0-9][0-9]+)/<a href=\"diatheke.pl?verse=$1&Thayer=on\">$1\<\/a\>/g;
    }
    #case for ThML format files
    elsif($versions[$i] eq "JFB" || $versions[$i] eq "MHC") {
	$line =~ s/<scripRef version=\"([^\"]+)\" passage=\"([^\"]+)\">/<a href=\"diatheke.pl?verse=$2&$1=on\">/g;
	$line =~ s/<\/scripRef>/<\/a>/g;
    }
    #case for GBF & other non-ThML files
    elsif($versions[$i] eq "MHC" || $versions[$i] eq "RWP" || $versions[$i] eq "DTN" || $versions[$i] eq "Family" || $versions[$i] eq "Geneva" || $versions[$i] eq "PNT" || $versions[$i] eq "TSK" || $versions[$i] eq "Vines" || $versions[$i] eq "Naves" || $versions[$i] eq "Eastons" || $versions[$i] eq "Hitchcocks" || $versions[$i] eq "ISBE" || $versions[$i] eq "Smiths"|| $versions[$i] eq "Gill") {
	$book = $verse;
	$book =~ s/^([A-Za-z0-9]+) [0-9]+:[0-9]+.*/$1/;
	$chapter = $verse;
	$chapter =~ s/[A-Za-z0-9]+ ([0-9]+):[0-9]+.*/$1/;
	
	$line =~ s/\#*([1-9]*[A-Z][a-z]+\.*) ([0-9]+):([0-9]+-*,*[0-9]*)\|*/<a href=\"diatheke.pl?verse=$1+$2%3A$3&$defversion=on\">$1 $2:$3\<\/a\>/g;
	$line =~ s/\#([0-9]+):([0-9]+-*,*[0-9]*)\|*/<a href=\"diatheke.pl?verse=$book+$1%3A$2&$defversion=on\">$book $1:$2\<\/a\>/g;
	$line =~ s/\#([0-9]+-*,*[0-9]*)\|*/<a href=\"diatheke.pl?verse=$book+$chapter%3A$1&$defversion=on\">$book $chapter:$1\<\/a\>/g;
    }
#--------- Change full book names to abbreviations
#>>
    if ($search ne "on") {
	

	$line =~ s/^Genesis/Gen/g;
	$line =~ s/^Exodus/Ex/g;
	$line =~ s/^Leviticus/Lev/g;
	$line =~ s/^Numbers/Num/g;
	$line =~ s/^Deuteronomy/Deut/g;
	$line =~ s/^Joshua/Jos/g;
	$line =~ s/^Judges/Jdg/g;
	$line =~ s/^II Samuel/2Sam/g;
	$line =~ s/^I Samuel/1Sam/g;
	$line =~ s/^II Kings/2Kgs/g;
	$line =~ s/^I Kings/1Kgs/g;
	$line =~ s/^II Chronicles/2Chr/g;
	$line =~ s/^I Chronicles/1Chr/g;
	$line =~ s/^Nehemiah/Neh/g;
	$line =~ s/^Psalms/Ps/g;
	$line =~ s/^Proverbs/Prov/g;
	$line =~ s/^Ecclesiastes/Eccl/g;
	$line =~ s/^Song of Solomon/Song/g;
	$line =~ s/^Isaiah/Isa/g;
	$line =~ s/^Jeremiah/Jer/g;
	$line =~ s/^Lamentations/Lam/g;
	$line =~ s/^Ezekiel/Ezek/g;
	$line =~ s/^Daniel/Dan/g;
	$line =~ s/^Hosea/Hos/g;
	$line =~ s/^Obadiah/Obad/g;
	$line =~ s/^Jonah/Jnh/g;
	$line =~ s/^Nahum/Nah/g;
	$line =~ s/^Habakkuk/Hab/g;
	$line =~ s/^Zephaniah/Zeph/g;
	$line =~ s/^Haggai/Hag/g;
	$line =~ s/^Zechariah/Zech/g;
	$line =~ s/^Malachi/Mal/g;
	
	$line =~ s/^Revelation of John/Rev/g;
	$line =~ s/^Matthew/Mt/g;
	$line =~ s/^Mark/Mk/g;
	$line =~ s/^Luke/Lk/g;
	$line =~ s/^John/Jn/g;
	$line =~ s/^Romans/Rom/g;
	$line =~ s/^II Corinthians/2Cor/g;
	$line =~ s/^I Corinthians/1Cor/g;
	$line =~ s/^Galatians/Gal/g;
	$line =~ s/^Ephesians/Eph/g;
	$line =~ s/^Philippians/Php/g;
	$line =~ s/^Colossians/Col/g;
	$line =~ s/^II Thessalonians/2Thes/g;
	$line =~ s/^I Thessalonians/1Thes/g;
	$line =~ s/^II Timothy/2Tim/g;
	$line =~ s/^I Timothy/1Tim/g;
	$line =~ s/^Titus/Tit/g;
	$line =~ s/^Philemon/Phlm/g;
	$line =~ s/^Hebrews/Heb/g;
	$line =~ s/^James/Jas/g;
	$line =~ s/^II Peter/2Pet/g;
	$line =~ s/^I Peter/1Pet/g;
	$line =~ s/^III John/3Jn/g;
	$line =~ s/^II John/2Jn/g;
	$line =~ s/^I John/1Jn/g;


	$line =~ s/\nGenesis/Gen/g;
	$line =~ s/\nExodus/Ex/g;
	$line =~ s/\nLeviticus/Lev/g;
	$line =~ s/\nNumbers/Num/g;
	$line =~ s/\nDeuteronomy/Deut/g;
	$line =~ s/\nJoshua/Jos/g;
	$line =~ s/\nJudges/Jdg/g;
	$line =~ s/\nII Samuel/2Sam/g;
	$line =~ s/\nI Samuel/1Sam/g;
	$line =~ s/\nII Kings/2Kgs/g;
	$line =~ s/\nI Kings/1Kgs/g;
	$line =~ s/\nII Chronicles/2Chr/g;
	$line =~ s/\nI Chronicles/1Chr/g;
	$line =~ s/\nNehemiah/Neh/g;
	$line =~ s/\nPsalms/Ps/g;
	$line =~ s/\nProverbs/Prov/g;
	$line =~ s/\nEcclesiastes/Eccl/g;
	$line =~ s/\nSong of Solomon/Song/g;
	$line =~ s/\nIsaiah/Isa/g;
	$line =~ s/\nJeremiah/Jer/g;
	$line =~ s/\nLamentations/Lam/g;
	$line =~ s/\nEzekiel/Ezek/g;
	$line =~ s/\nDaniel/Dan/g;
	$line =~ s/\nHosea/Hos/g;
	$line =~ s/\nObadiah/Obad/g;
	$line =~ s/\nJonah/Jnh/g;
	$line =~ s/\nNahum/Nah/g;
	$line =~ s/\nHabakkuk/Hab/g;
	$line =~ s/\nZephaniah/Zeph/g;
	$line =~ s/\nHaggai/Hag/g;
	$line =~ s/\nZechariah/Zech/g;
	$line =~ s/\nMalachi/Mal/g;
	
	$line =~ s/\nRevelation of John/Rev/g;
	$line =~ s/\nMatthew/Mt/g;
	$line =~ s/\nMark/Mk/g;
	$line =~ s/\nLuke/Lk/g;
	$line =~ s/\nJohn/Jn/g;
	$line =~ s/\nRomans/Rom/g;
	$line =~ s/\nII Corinthians/2Cor/g;
	$line =~ s/\nI Corinthians/1Cor/g;
	$line =~ s/\nGalatians/Gal/g;
	$line =~ s/\nEphesians/Eph/g;
	$line =~ s/\nPhilippians/Php/g;
	$line =~ s/\nColossians/Col/g;
	$line =~ s/\nII Thessalonians/2Thes/g;
	$line =~ s/\nI Thessalonians/1Thes/g;
	$line =~ s/\nII Timothy/2Tim/g;
	$line =~ s/\nI Timothy/1Tim/g;
	$line =~ s/\nTitus/Tit/g;
	$line =~ s/\nPhilemon/Phlm/g;
	$line =~ s/\nHebrews/Heb/g;
	$line =~ s/\nJames/Jas/g;
	$line =~ s/\nII Peter/2Pet/g;
	$line =~ s/\nI Peter/1Pet/g;
	$line =~ s/\nIII John/3Jn/g;
	$line =~ s/\nII John/2Jn/g;
	$line =~ s/\nI John/1Jn/g;
	
    }
    if ($search eq "on") {


    $line =~ s/Genesis/Gen/g;
    $line =~ s/Exodus/Ex/g;
    $line =~ s/Leviticus/Lev/g;
    $line =~ s/Numbers/Num/g;
    $line =~ s/Deuteronomy/Deut/g;
    $line =~ s/Joshua/Jos/g;
    $line =~ s/Judges/Jdg/g;
    $line =~ s/II Samuel/2Sam/g;
    $line =~ s/I Samuel/1Sam/g;
    $line =~ s/II Kings/2Kgs/g;
    $line =~ s/I Kings/1Kgs/g;
    $line =~ s/II Chronicles/2Chr/g;
    $line =~ s/I Chronicles/1Chr/g;
    $line =~ s/Nehemiah/Neh/g;
    $line =~ s/Psalms/Ps/g;
    $line =~ s/Proverbs/Prov/g;
    $line =~ s/Ecclesiastes/Eccl/g;
    $line =~ s/Song of Solomon/Song/g;
    $line =~ s/Isaiah/Isa/g;
    $line =~ s/Jeremiah/Jer/g;
    $line =~ s/Lamentations/Lam/g;
    $line =~ s/Ezekiel/Ezek/g;
    $line =~ s/Daniel/Dan/g;
    $line =~ s/Hosea/Hos/g;
    $line =~ s/Obadiah/Obad/g;
    $line =~ s/Jonah/Jnh/g;
    $line =~ s/Nahum/Nah/g;
    $line =~ s/Habakkuk/Hab/g;
    $line =~ s/Zephaniah/Zeph/g;
    $line =~ s/Haggai/Hag/g;
    $line =~ s/Zechariah/Zech/g;
    $line =~ s/Malachi/Mal/g;

    $line =~ s/Revelation of John/Rev/g;
    $line =~ s/Matthew/Mt/g;
    $line =~ s/Mark/Mk/g;
    $line =~ s/Luke/Lk/g;
    $line =~ s/John/Jn/g;
    $line =~ s/Romans/Rom/g;
    $line =~ s/II Corinthians/2Cor/g;
    $line =~ s/I Corinthians/1Cor/g;
    $line =~ s/Galatians/Gal/g;
    $line =~ s/Ephesians/Eph/g;
    $line =~ s/Philippians/Php/g;
    $line =~ s/Colossians/Col/g;
    $line =~ s/II Thessalonians/2Thes/g;
    $line =~ s/I Thessalonians/1Thes/g;
    $line =~ s/II Timothy/2Tim/g;
    $line =~ s/I Timothy/1Tim/g;
    $line =~ s/Titus/Tit/g;
    $line =~ s/Philemon/Phlm/g;
    $line =~ s/Hebrews/Heb/g;
    $line =~ s/James/Jas/g;
    $line =~ s/II Peter/2Pet/g;
    $line =~ s/I Peter/1Pet/g;
    $line =~ s/III John/3Jn/g;
    $line =~ s/II John/2Jn/g;
    $line =~ s/I John/1Jn/g;


#<<
	$line =~ s/([0-9]*[A-Za-z]+) ([0-9]+):([0-9]+)/<a href=\"diatheke.pl?verse=$1+$2%3A$3&$versions[$i]=on\">$1 $2:$3\<\/a\>/g;
    }




# for the old HREFCom version of JFB
#    if ($versions[$i] eq "JFB") {
#	$line =~ s/(http:[^ ]+) /<a href="$1">$1<\/a>/g;
#    }

    if ($palm == 1) {
	$line =~ s/href=\"diatheke.pl([^\"]+)\"/href=\"http:\/\/bible.gotjesus.org\/cgi-bin\/diatheke.pl$1&palm=on\"/g;
    }

    if ($footnotes == 0) {
	$line =~ s/<note[^<]+<\/note>//g;
    }
    if ($strongs == 0) {
	$line =~ s/<a href=[^>]+Strongs(Greek|Hebrew)[^<]+<\/a>//g;
    }
    
    print "$line <br /><br />\n";
}

if ($palm == 1) {
    print "<hr>Powered by Diatheke (bible.gotjesus.org) and the SWORD Project (www.crosswire.org).";
}

print "</font></body></html>";

}
