#!/usr/bin/perl

#******************************************************************************
#
#  sword.pl -	This looks like a test program to check binding functionality
#
# $Id$
#
# Copyright 2002-2009 CrossWire Bible Society (http://www.crosswire.org)
#	CrossWire Bible Society
#	P. O. Box 2528
#	Tempe, AZ  85280-2528
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation version 2.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#

use Sword;

print "Version (should be 1.0): " , $Sword::VERSION , "\n";

print "Create SWConfig object!\n";
$config = new Sword::SWConfig("test.conf");

print "Load\n";
$config->Load();

print "Set value ... ";
print $config->set("Group", "Entry", "Value");
print " finished \n";

print "Get value ... ";
print $config->get("Group", "Entry");
print " finished \n";

$config->Save();

#testing SWMgr
print "testing SWMgr\n";
#$localemgr = Sword::LocaleMgr::getSystemLocaleMgr();
#$localemgr->setDefaultLocaleName("de");

$mgr = new Sword::SWMgr();
print "init ... ";
#$module = $mgr->module("GerLut1545-temp");
$module = $mgr->getModule("WEB");
print "Printing WEB Module information: \n";
print "Name:\t", $module->Name(),"\nDescription:\t", $module->Description(), "\nLang:\t", $module->Lang(), "\n";

$key = new Sword::VerseKey("Matthew 3:16");
#$key->setPersist(1);
$module->SetKey($key);

for ($i = 0; $i < 15; $i++) {
  print "(", $module->KeyText() ,")\t", $module->StripText(), "\n";
  $key->increment();
  $module->SetKey($key);
}
$key->increment(103);
$module->SetKey($key);
print "(", $module->KeyText() ,")\t", $module->StripText(), "\n";

#testing write interface
$key->setText("John 3:16");
$module->SetKey($key);
$module->setEntry("This is a test entry! This tests the write abilities of the Sword Perl classes", 78);
print "(", $module->KeyText() ,")\t", $module->StripText(), "\n";

print "Searching for God: ";
$list = $module->doSearch("God");
print $list->Count(), " entries found!\n";
#for ( $i = 0; $i < $list->Count(); $i++) {
#  print "(", $i, ")\t", $list->GetElement()->getText(), "\n";
#  $list->next();
#}

print "Creating new module! Writing search result...";
#Sword::RawText::createModule("/usr/share/sword/modules/texts/ztext/testmodule/");
#$newmod = new Sword::RawText("/usr/share/sword/modules/texts/ztext/testmodule/");

#$key->setText("Genesis 1:1");
$newkey = $key->clone();
#$newmod->SetKey($newkey);
#
#for ($i = 0; $i < $list->Count(); $i++, $list->next()) {
#	$key->setText($list->GetElement()->getText());
#	$newkey->setText($list->GetElement()->getText());	
#
#	$newmod->write( $module->StripText() );
#}

print "Now create the LD module\n";

 mkdir("ldmod");
Sword::zText::createModule("ldmod/",4);

print "first step}\n";

$newmod = new Sword::zText("ldmod/");

print "Created module;\n";

$newkey = $newmod->CreateKey();
#$newkey->setPersist(1);
$newkey->setText(" ");
$module->SetKey($newkey);

print "Loop! \n";

for ($i = 0; $i < $list->Count(); $i++) {
	print $list->GetElement()->getText() . "\n";

        $key->setText($list->GetElement()->getText());
        $newkey->setText($list->GetElement()->getText());

	$newmod->SetKey($newkey);

	$entry = $module->StripText();
        $newmod->setEntry( $entry, length $entry );
	$list->increment();
}

