# ---------------------------------------------------------------------
# Initialisation
# ---------------------------------------------------------------------
AC_INIT([swordbindings],[1.5.11],[sword-bugs@crosswire.org])
AC_CONFIG_SRCDIR(sword.i)
AC_PREREQ(2.59)
AC_REVISION($Revision: 1.2 $)
AM_INIT_AUTOMAKE(swordbindings,1.5.11)

# ---------------------------------------------------------------------
# Check Programs
# ---------------------------------------------------------------------
AC_LANG(C++)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
SW_PROG_SWIG

# ---------------------------------------------------------------------
# Check libraries
# ---------------------------------------------------------------------
SW_CHECK_SWORD(1.5.7)

# ---------------------------------------------------------------------
# Misc
# ---------------------------------------------------------------------
AM_MAINTAINER_MODE
AC_ARG_ENABLE(swig, [--disable-swig do not run swig to generate input files], enable_swig=$enableval, enable_swig=no)
AM_CONDITIONAL([RUNSWIG], [test x$enable_swig = xyes])

# ---------------------------------------------------------------------
# Check Scripting languages
# ---------------------------------------------------------------------
SW_FIND_PERL
SW_FIND_PYTHON
SW_FIND_PHP4
SW_FIND_TCL

# ---------------------------------------------------------------------
# Final output
# ---------------------------------------------------------------------
AC_CONFIG_FILES(Makefile)
AC_OUTPUT
