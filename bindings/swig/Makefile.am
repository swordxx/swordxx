
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
	cd perl && perl Makefile.PL && make clean && rm -f MANIFEST* && rm -f *.old && perl Makefile.PL && make manifest
	cd perl && sed -e 's/-lsword/-lsword -lstdc++/' Makefile > Makefile.tmp && mv Makefile.tmp Makefile

tclswig: sword.i
	mkdir -p tcl
	swig -tcl -c++ -namespace -pkgversion $(VERSION) -o tcl/Sword.cxx sword.i
	
pythonswig: sword.i
	mkdir -p python
	swig -python -c++ -shadow -o python/Sword.cxx sword.i

phpswig: sword.i
	mkdir -p php
	swig -php -c++ -o php/Sword.cxx sword.i

