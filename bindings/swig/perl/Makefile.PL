#!/usr/bin/perl

use ExtUtils::MakeMaker;

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
		'NAME'		=> 'Sword',
		'VERSION'	=> '1.5.5',
		'INC'		=> '-I/scratch/src/tmp/include/sword',
		'DEFINE'	=> '-DSWIG',
		'LIBS'		=> '-L/scratch/src/tmp/lib -lsword -lz',
		'PREREQ_PM'	=> {}, # e.g., Module::Name => 1.1
			($] >= 5.005 ?		## Add these new keywords supported since 5.005
			(ABSTRACT => 'Sword Project perl bindings', # retrieve abstract from module
			AUTHOR		=> 'Sword Project <sword-devel@crosswire.org>') : ()),
);
