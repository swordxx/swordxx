#####################################################################################################
#
# A system to, hopefully, build the Sword bindings which are asked for.  Currently I only know and
# therefore only support Python & Perl bindings.  YMMV for it, please submit patches or suggestions.
# I will try to get them integrated into the system if I can.  If there are bindings that you need
# other than the Python|Perl/Swig bindings, then you should probably speak up now.

#####################################################################################################
# SWIG
# We need to find the SWIG executable in order to be able to do this, right?
FIND_PACKAGE(SWIG)

IF(NOT SWIG_FOUND)
	MESSAGE(FATAL_ERROR "Swig not found.  Bindings will not be built.")
ELSE(NOT SWIG_FOUND)
	MESSAGE(STATUS "Swig found at ${SWIG_EXECUTABLE}")
	
	SET(SWORD_SWIG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig")
	SET(SWORD_SWIG_BINARY "${CMAKE_CURRENT_BINARY_DIR}/bindings/swig")
	
	# This code that is commented out is basically how CMake suggests that you do it.  I, on the
	# other hand, can't seem to get it to do that.  The manual seems to work though, so go with that
	# instead.
	#INCLUDE(${SWIG_USE_FILE})
	
	#FIND_PACKAGE(PythonLibs)
	#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
	#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig)
	
	#SET(CMAKE_SWIG_FLAGS "")
	
	#SET_SOURCE_FILES_PROPERTIES(sword.i PROPERTIES CPLUSPLUS ON)
	#SET_SOURCE_FILES_PROPERTIES(sword.i PROPERTIES SWIG_FLAGS "-shadow")
	
	#SWIG_ADD_MODULE(sword python
	#	"${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig/sword.i" 
		#"${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig/python/Sword.cxx"
	#	)
	#SWIG_LINK_LIBRARIES(sword ${PYTHON_LIBRARIES})
	
	# Borrowed this from the CMake UseSWIG.cmake file
	GET_DIRECTORY_PROPERTY(cmake_include_directories INCLUDE_DIRECTORIES)
	SET(SWIG_INCLUDES)
	FOREACH(direct ${cmake_include_directories})
		SET(SWIG_INCLUDES ${SWIG_INCLUDES} "-I${direct}")
	ENDFOREACH(direct ${cmake_include_directories})
# 	MESSAGE(STATUS "${SWIG_INCLUDES} include directories")

	IF(SWORD_BINDINGS MATCHES ".*Python.*")
		# This should add the pythonswig target to be built when the "make" command is executed
		ADD_CUSTOM_TARGET(pythonswig ALL
				mkdir -p "${SWORD_SWIG_BINARY}/python"
			COMMAND	${SWIG_EXECUTABLE}  "-w503,+509" -python -c++ -shadow -o "${SWORD_SWIG_BINARY}/python/Sword.cxx" "-I${SWORD_SWIG_SOURCE}" ${SWIG_INCLUDES} "${SWORD_SWIG_SOURCE}/sword.i"
			COMMAND	echo "#! /usr/bin/python" > ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo "" >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo "from distutils.core import setup, Extension" >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo "setup (name = \"sword\"," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " version = \"${SWORD_VERSION}\"," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " maintainer = \"Sword Developers\"," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " maintainer_email = \"sword-devel@crosswire.org\"," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " url = \"http://www.crosswire.org/sword\"," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " py_modules = [\"Sword\"]," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " include_dirs=['${SWORD_SWIG_SOURCE}', '${CMAKE_CURRENT_SOURCE_DIR}/include', '${SWORD_SWIG_SOURCE}/..', '${SWORD_SWIG_SOURCE}/../..']," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " ext_modules = [Extension(\"_Sword\", [\"Sword.cxx\"]," >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " libraries=[('sword')], " >> ${SWORD_SWIG_BINARY}/python/setup.py
                        COMMAND echo " library_dirs=[('${CMAKE_CURRENT_BINARY_DIR}')], " >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo " )], " >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo ")" >> ${SWORD_SWIG_BINARY}/python/setup.py
			COMMAND echo "Python bindings built, to install change into ${SWORD_SWIG_BINARY}/python and type 'python setup.py install'"
			DEPENDS sword
			WORKING_DIRECTORY "${SWORD_SWIG_SOURCE}"
			VERBATIM
		)
		
		MESSAGE(STATUS "Configured for building Python bindings.")
	ENDIF(SWORD_BINDINGS MATCHES ".*Python.*")
	
	IF(SWORD_BINDINGS MATCHES ".*Perl.*")
		FIND_PACKAGE(Perl REQUIRED)
		IF(PERL_FOUND)
			# This is a terrible hack.  Don't leave it stay here.  It's ugly
			SET(LIB_SWORD "${CMAKE_CURRENT_BINARY_DIR}/libsword.a")
# 			MESSAGE(STATUS "LIB_SWORD is ${LIB_SWORD}")
			# This should add the perlswig target to be build with the "make" command is executed
			ADD_CUSTOM_TARGET(perlswig ALL
				mkdir -p "${SWORD_SWIG_BINARY}/perl"
				COMMAND ${SWIG_EXECUTABLE} "-w503,+509" -perl -c++ -shadow -o "${SWORD_SWIG_BINARY}/perl/Sword.cxx" "-I${SWORD_SWIG_SOURCE}" ${SWIG_INCLUDES} "${SWORD_SWIG_SOURCE}/sword.i"
				COMMAND echo "Writing ${SWORD_SWIG_BINARY}/perl/Makefile.PL"
				COMMAND echo "#! /usr/bin/perl" > ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "use ExtUtils::MakeMaker;" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "# See lib/ExtUtils/MakeMaker.pm for details of how to influence" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "# the contents of the Makefile that is written." >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "WriteMakefile(" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "         'NAME'          => 'Sword'," >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "         'VERSION'       => '${SWORD_VERSION}'," >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "         'INC'           => '-I${CMAKE_CURRENT_SOURCE_DIR}/include -I${SWORD_SWIG_SOURCE}'," >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "         'DEFINE'        => '-DSWIG'," >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "         'LIBS'          => '-lsword -lz'," >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "         'PREREQ_PM'     => {}, # e.g., Module::Name => 1.1" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "                 ($] >= 5.005 ?         ## Add these new keywords supported since 5.005" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "                 (ABSTRACT => 'Sword Project perl bindings', # retrieve abstract from module" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "                 AUTHOR          => 'Sword Project <sword-devel@crosswire.org>') : ())," >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo ");" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "rename 'Makefile', 'Makefile.old' or die \"can't rename Makefile\";" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "open(INPUT, '<Makefile.old') or die \"can't open input Makefile.old\";" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "open(OUTPUT, '>Makefile') or die \"can't open output Makefile\";" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "while (<INPUT>) {" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo " s/\\-lsword/\\-lsword \\-lstdc\\+\\+/;" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo " print OUTPUT \"$_\";" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "}" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "close INPUT;" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND echo "close OUTPUT;" >> ${SWORD_SWIG_BINARY}/perl/Makefile.PL
				COMMAND cd ${SWORD_SWIG_BINARY}/perl && ${PERL_EXECUTABLE} Makefile.PL && make clean
				COMMAND echo "\\.old" > ${SWORD_SWIG_BINARY}/perl/MANIFEST.SKIP
				COMMAND echo "~$" >> ${SWORD_SWIG_BINARY}/perl/MANIFEST.SKIP
				COMMAND echo "\\.bak" >> ${SWORD_SWIG_BINARY}/perl/MANIFEST.SKIP
				COMMAND echo "^CVS" >> ${SWORD_SWIG_BINARY}/perl/MANIFEST.SKIP
				COMMAND echo "Makefile$" >> ${SWORD_SWIG_BINARY}/perl/MANIFEST.SKIP
				COMMAND cd ${SWORD_SWIG_BINARY}/perl && ${PERL_EXECUTABLE} Makefile.PL && make manifest
				COMMAND echo "Perl bindings built, to install change into ${SWORD_SWIG_BINARY}/perl and type 'make install'"
				DEPENDS sword
				WORKING_DIRECTORY "${SWORD_SWIG_SOURCE}"
				VERBATIM
			)
			
			MESSAGE(STATUS "Configured for building Perl bindings.")
		ELSE(PERL_FOUND)
			MESSAGE(FATAL_ERROR "Perl not found. Can't create perl bindings without Perl to bind")
		ENDIF(PERL_FOUND)
	ENDIF(SWORD_BINDINGS MATCHES ".*Perl.*")
ENDIF(NOT SWIG_FOUND)
