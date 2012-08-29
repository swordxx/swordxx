# This method works, but there is no established way to do the installation
#FIND_PACKAGE(PythonLibs REQUIRED)
#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
#
#SET_SOURCE_FILES_PROPERTIES(../sword.i PROPERTIES CPLUSPLUS ON)
#SET_SOURCE_FILES_PROPERTIES(../sword.i PROPERTIES SWIG_FLAGS "")
#
#SWIG_ADD_MODULE(Sword python
#	"../sword.i"
#)
#
#SWIG_LINK_LIBRARIES(Sword ${PYTHON_LIBRARIES} ${SWORD_LINK_NAME})

FIND_PACKAGE(PythonInterp REQUIRED)

SET(PY_SCRIPT "#!${PYTHON_EXECUTABLE} python

from distutils.core import setup, Extension
setup(name='sword',version='${SWORD_VERSION}',
maintainer='Sword Developers',
maintainer_email='sword-devel@crosswire.org',
url='http://www.crosswire.org/sword',
py_modules=['Sword'],
include_dirs=['${CMAKE_CURRENT_SOURCE_DIR}', '${CMAKE_SOURCE_DIR}/include', '${CMAKE_CURRENT_SOURCE_DIR}/..', '${CMAKE_CURRENT_SOURCE_DIR}../../'],
ext_modules = [Extension('_Sword',['Sword.cxx'],
libraries=[('sword')],
library_dirs=[('${CMAKE_BINARY_DIR}')],
)]
,)"
)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/setup.py "${PY_SCRIPT}")

SWORD_SWIG_GENERATE(python)

ADD_CUSTOM_TARGET(swordswig ALL
	COMMAND python setup.py build
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Sword.cxx ${SWORD_LINK_NAME}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

INSTALL(SCRIPT install.cmake)

MESSAGE(STATUS "Python: yes")
