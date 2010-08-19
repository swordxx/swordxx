#####################################################################################################
#
# A system to, hopefully, build the Sword bindings which are asked for.  Currently I only know and
# therefore only support the Python bindings.  YMMV for it, please submit patches or suggestions.
# I will try to get them integrated into the system if I can.  If there are bindings that you need
# other than the Python/Swig bindings, then you should probably speak up now.

#####################################################################################################
# SWIG
# We need to find the SWIG executable in order to be able to do this, right?
FIND_PACKAGE(SWIG)

IF(NOT SWIG_FOUND)
	MESSAGE(FATAL_ERROR "Swig not found.  Bindings will not be built.")
ELSE(NOT SWIG_FOUND)
	MESSAGE(STATUS "Swig found at ${SWIG_EXECUTABLE}")
	
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
	MESSAGE(STATUS "${SWIG_INCLUDES} include directories")
	# This should add the pythonswig target to be built when the "make" command is executed
 	ADD_CUSTOM_TARGET(pythonswig ALL
 			mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python"
 		COMMAND	${SWIG_EXECUTABLE} -python -c++ -shadow -o "${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/Sword.cxx" -I${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig ${SWIG_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig/sword.i
 		COMMAND	echo "#! /usr/bin/python" > ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo "" >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo "from distutils.core import setup, Extension" >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo "setup (name = \"sword\"," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " version = \"${SWORD_VERSION}\"," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " maintainer = \"Sword Developers\"," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " maintainer_email = \"sword-devel@crosswire.org\"," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " url = \"http://www.crosswire.org/sword\"," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " py_modules = [\"Sword\"]," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " include_dirs=['${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig', '${CMAKE_CURRENT_SOURCE_DIR}/include', '..', '../..']," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " ext_modules = [Extension(\"_Sword\", [\"Sword.cxx\"]," >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " libraries=[('sword')], " >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo " )], " >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		COMMAND echo ")" >> ${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python/setup.py
 		DEPENDS sword
 		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bindings/swig"
 		VERBATIM
 	)
ENDIF(NOT SWIG_FOUND)