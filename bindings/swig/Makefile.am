
all: $(PERLSWIG) $(TCLSWIG) $(PYTHONSWIG) $(PHPSWIG)
	echo "built"

perlswig: sword.i
	mkdir -p perl
	swig -perl -c++ -shadow -o perl/Sword.cxx sword.i
	echo "writing perl/Makefile.PL"
	@echo "#!/usr/bin/perl" > perl/Makefile.PL
	@echo "" >> perl/Makefile.PL
	@echo "use ExtUtils::MakeMaker;" >> perl/Makefile.PL
	@echo "" >> perl/Makefile.PL
	@echo "# See lib/ExtUtils/MakeMaker.pm for details of how to influence" >> perl/Makefile.PL
	@echo "# the contents of the Makefile that is written." >> perl/Makefile.PL
	@echo "WriteMakefile(" >> perl/Makefile.PL
	@echo "		'NAME'		=> 'Sword'," >> perl/Makefile.PL
	@echo "		'VERSION'	=> '$(VERSION)'," >> perl/Makefile.PL 
	@echo "		'INC'		=> '$(SWORD_INCLUDES)'," >> perl/Makefile.PL
	@echo "		'DEFINE'	=> '-DSWIG'," >> perl/Makefile.PL
	@echo "		'LIBS'		=> '$(LIB_SWORD) -lz'," >> perl/Makefile.PL
	@echo "		'PREREQ_PM'	=> {}, # e.g., Module::Name => 1.1" >> perl/Makefile.PL
	@echo "			($$] >= 5.005 ?		## Add these new keywords supported since 5.005" >> perl/Makefile.PL
	@echo "			(ABSTRACT => 'Sword Project perl bindings', # retrieve abstract from module" >> perl/Makefile.PL
	@echo "			AUTHOR		=> 'Sword Project <sword-devel@crosswire.org>') : ())," >> perl/Makefile.PL 
	@echo ");" >> perl/Makefile.PL
	@echo "" >> perl/Makefile.PL
	@echo "rename 'Makefile', 'Makefile.old' or die \"can't rename Makefile\";" >> perl/Makefile.PL
	@echo "open(INPUT, '<Makefile.old') or die \"can't open input Makefile.old\";" >> perl/Makefile.PL
	@echo "open(OUTPUT, '>Makefile') or die \"can't open output Makefile\";" >> perl/Makefile.PL
	@echo "while (<INPUT>) {" >> perl/Makefile.PL
	@echo "	s/\-lsword/\-lsword \-lstdc\+\+/;" >> perl/Makefile.PL
	@echo "	print OUTPUT \"$$""_\";" >> perl/Makefile.PL
	@echo "}" >> perl/Makefile.PL
	@echo "" >> perl/Makefile.PL
	@echo "close INPUT;" >> perl/Makefile.PL
	@echo "close OUTPUT;" >> perl/Makefile.PL	
	cd perl && $(PERL) Makefile.PL && make clean
	@echo "\.old" > perl/MANIFEST.SKIP
	@echo "~$$" >> perl/MANIFEST.SKIP
	@echo "\.bak" >> perl/MANIFEST.SKIP
	@echo "^CVS" >> perl/MANIFEST.SKIP
	@echo "Makefile$$" >> perl/MANIFEST.SKIP
	cd perl && $(PERL) Makefile.PL && make manifest

tclswig: sword.i
	mkdir -p tcl
	swig -tcl -c++ -namespace -pkgversion $(VERSION) -o tcl/Sword.cxx sword.i
	
pythonswig: sword.i
	mkdir -p python
	swig -python -c++ -shadow -o python/Sword.cxx sword.i

phpswig: sword.i
	mkdir -p php
	swig -php -c++ -o php/Sword.cxx sword.i

