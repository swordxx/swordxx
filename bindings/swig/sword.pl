#!/usr/bin/perl

use sword;

print "Version (should be 1.0): " , $sword::VERSION , "\n";

print "Create SWConfig object!\n";
$config = new sword::SWConfig("test.conf");

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
$localemgr = sword::LocaleMgr::systemLocaleMgr();
$localemgr->setDefaultLocaleName("de");

$mgr = new sword::SWMgr();
$module = $mgr->module("GerLut1545-temp");
print "Printing WEB Module information: \n";
print "Name:\t", $module->Name(),"\nDescription:\t", $module->Description(), "\nLang:\t", $module->Lang(), "\n";

$key = new sword::VerseKey("Matthew 3:16");
$key->setPersist(1);
$module->SetKey($key);

for ($i = 0; $i < 15; $i++) {
  print "(", $module->KeyText() ,")\t", $module->StripText(), "\n";
  $key->next();
}
$key->increment(103);
print "(", $module->KeyText() ,")\t", $module->StripText(), "\n";

#testing write interface
$key->setText("John 3:16");
#$module->SetKey($key);
$module->write("This is a test entry! This tests the write abilities of the Sword Perl classes");
print "(", $module->KeyText() ,")\t", $module->StripText(), "\n";
