#!/usr/bin/perl

# This program converts a given module into a compressed module of the same type.
# This is just an example to demomstrate the power of the Perl Sword bindings.

$appname = "mod2zmod.pl";

use Sword;

sub printUsage()
{
	print "\n$appname - Convert a module into a compressed module of the same type.\n";
	print "Usage: $appname <module> <datapth> [blocktype [compresstype]]\n";
	print("datapath: the directory in which to write the zModule\n");
        print("blockType  : (default 4)\n\t2 - verses\n\t3 - chapters\n\t4 - books\n");
        print("compressType: (default 1):\n\t1 - LZSS\n\t2 - Zip\n\n");

	exit(-1);
}

#main part of the program
if (scalar(@ARGV) < 2 || scalar(@ARGV) > 4) {
	printUsage;
}

#initialization stuff
$datapath = $ARGV[1];
$blockType = defined $ARGV[2] ? $ARGV[2] : 4;
$compressType = defined $ARGV[3] ? $ARGV[3] : 1;
$mgr = new Sword::SWMgr();
$module = $mgr->module($ARGV[0]);
$compressor = ($compressType == 1) ? new Sword::LZSSCompress() : new Sword::ZipCompress();

if ($module->Type() eq "Biblical Texts") {
	if (!Sword::zText::createModule( $datapath, $blockType )) {
		print "$appname: Couldn't create module in $datapath";
		exit(-1);
	}
        $newmod = new Sword::zText( $datapath, 0, 0, $blockType, $compressor );

} elsif ($module->Type() eq "Lexicons / Dictionaries") {
	if (!Sword::zLD::createModule( $datapath )){
		print "$appname: Couldn't create module in $datapath";
		exit(-1);
	}
	$newmod = new Sword::zLD( $datapath, 0, 0, $blockType, $compressor)
} elsif ($module->Type() eq "Commentaries") {
	if (!Sword::zCom::createModule( $datapath, $blockType )){
		print "$appname: Couldn't create module in $datapath";
		exit(-1);
	}
	$newmod = new Sword::zCom( $datapath, 0, 0, $blockType, $compressor)
}

#now copy the content!
$module->top();
$module->setSkipConsecutiveLinks(false);
do {
	$key = $module->Key();
	if (($buffer eq $module->getRawEntry()) &&($buffer ne "")) {
                print "Adding [", $key->getText(), "] link to: \n";
		$newmod->writeLink($key);
	}
	else {
		$buffer = $module->getRawEntry();
		if ($buffer ne "") {
		  $newmod->SetKey($key);
		  $newmod->write($buffer);
		  # print "Added ", $key->getText(), "\n";
		}
		else {
		  print "Skipping empty ", $key->getText(), "\n";
		}
	}
} while($module->next());

print "The new module is now available in $datapath!\n";
