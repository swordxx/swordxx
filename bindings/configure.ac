# ---------------------------------------------------------------------
# Initialisation
# ---------------------------------------------------------------------

# Version change: Change line 8 only !
# Change it immediately after a release

AC_INIT(swordbindings, 1.5.9, sword-bugs@crosswire.org)
AC_CONFIG_SRCDIR([flatapi.cpp])
AC_PREREQ(2.52)
AC_REVISION($Revision$)

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE

#AM_CONFIG_HEADER(include/config.h)


# ---------------------------------------------------------------------
# Check Programs
# ---------------------------------------------------------------------
AC_LANG(C++)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_C_BIGENDIAN

# ---------------------------------------------------------------------
# Java
# ---------------------------------------------------------------------
AC_CHECK_CLASSPATH
AC_PROG_JAVAC
AC_PROG_JAVA
AC_PROG_JAR

# ---------------------------------------------------------------------
# With options
# ---------------------------------------------------------------------
AC_ARG_WITH(tomcathome,
	AC_HELP_STRING([--with-tomcathome],
    [tomcat home directory (default=[/usr/local/tomcat])]),,
    with_tomcathome=[/usr/local/tomcat])


# ---------------------------------------------------------------------
# Enable options
# ---------------------------------------------------------------------
AC_ARG_ENABLE(corba,
	AC_HELP_STRING([--enable-corba],[build swordorbserver (default=no)]),,enable_corba=no)

AM_MAINTAINER_MODE

# ---------------------------------------------------------------------
# Check libraries
# ---------------------------------------------------------------------

# ---------------------------------------------------------------------
# Find pkg-config
# ---------------------------------------------------------------------
use_pkgconfig=yes
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test x$PKG_CONFIG = xno ; then
  AC_MSG_WARN([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/])
  AC_MSG_ERROR([not using pkg-config])
  use_pkgconfig=no
fi

if $PKG_CONFIG --atleast-pkgconfig-version 0.14 ; then
  :
else
  AC_MSG_WARN([*** pkg-config too old; version 0.14 or better required.])
  AC_MSG_ERROR([not using pkg-config])
  use_pkgconfig=no
fi

# ---------------------------------------------------------------------
# Find sword
# ---------------------------------------------------------------------
SWORD_CFLAGS=
SWORD_LIBS=
if test x$use_pkgconfig = xyes ; then
  SWORD_LIBS=`$PKG_CONFIG --libs sword`
  SWORD_CFLAGS=`$PKG_CONFIG --cflags sword`
fi

# ---------------------------------------------------------------------
# Tomcat install directories
# ---------------------------------------------------------------------
TOMCAT_HOME=$with_tomcathome

# ---------------------------------------------------------------------
# Find orbit-config
# ---------------------------------------------------------------------
ORBIT_LIBS=
ORBIT_CFLAGS=
if test x$enable_corba = xyes ; then
  if test x$use_pkgconfig = xyes ; then
    ORBIT_LIBS+=`$PKG_CONFIG --libs ORBit-2.0-cpp`
    ORBIT_CFLAGS+=`$PKG_CONFIG --cflags ORBit-2.0`
    ORBIT_CXXFLAGS+=`$PKG_CONFIG --cflags ORBit-2.0-cpp`
  fi
fi

# ---------------------------------------------------------------------
# Substitute variables into makefiles
# ---------------------------------------------------------------------
AC_SUBST(SWORD_LIBS)
AC_SUBST(SWORD_CFLAGS)
AC_SUBST(ORBIT_LIBS)
AC_SUBST(ORBIT_CFLAGS)
AC_SUBST(ORBIT_CXXFLAGS)
AC_SUBST(TOMCAT_HOME)

# ---------------------------------------------------------------------
# Conditional variables
# ---------------------------------------------------------------------
AM_CONDITIONAL(CORBA, test x$enable_corba = xyes)

# ---------------------------------------------------------------------
# Final output
# ---------------------------------------------------------------------
AC_CONFIG_FILES(Makefile corba/Makefile corba/orbitcpp/Makefile corba/java/Makefile)
AC_OUTPUT

