dnl aclocal.m4 generated automatically by aclocal 1.4-p6

dnl Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN([AM_INIT_AUTOMAKE],
[AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
AC_REQUIRE([AC_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal-${am__api_version}, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake-${am__api_version}, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

# Copyright 2002  Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA

# AM_AUTOMAKE_VERSION(VERSION)
# ----------------------------
# Automake X.Y traces this macro to ensure aclocal.m4 has been
# generated from the m4 files accompanying Automake X.Y.
AC_DEFUN([AM_AUTOMAKE_VERSION],[am__api_version="1.4"])

# AM_SET_CURRENT_AUTOMAKE_VERSION
# -------------------------------
# Call AM_AUTOMAKE_VERSION so it can be traced.
# This function is AC_REQUIREd by AC_INIT_AUTOMAKE.
AC_DEFUN([AM_SET_CURRENT_AUTOMAKE_VERSION],
	 [AM_AUTOMAKE_VERSION([1.4-p6])])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN([AM_SANITY_CHECK],
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN([AM_MISSING_PROG],
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

dnl This file was created by Joachim Ansorg <jansorg@gmx.de>
dnl It provides macord for the autoconf package to find the Sword library on your system.

dnl ----------------------------------------------------------------------
dnl		Check wheter to use static linking
dnl		first parameter is the required version
dnl		second is whether to use static sword library
dnl ----------------------------------------------------------------------
AC_DEFUN(AC_CHECK_SWORD,
[
dnl AC_MSG_CHECKING([for a Sword installation])

dnl The option for the configure script
AC_ARG_WITH(sword-dir,
[  --with-sword-dir=DIR     Patch where Sword is being installed (default=/usr) ],
[
  ac_sword_dir=$withval
],ac_sword_dir=/usr
)

AC_ARG_ENABLE(static-sword,
[  --enable-static-sword    Link to the static Sword library],
 	ac_static_sword="YES",
 	[ ac_static_sword="$2" ]
)

dnl try to find Sword library files
AC_MSG_CHECKING([for Sword library files])
AC_REQUIRE([AC_FIND_ZLIB])
ac_sword_library_dirs="$ac_sword_dir/lib /usr/lib /usr/lib/sword /usr/local/lib /usr/local/lib/sword /usr/local/sword/lib"

if test "$ac_static_sword" = "YES"; then
	SEARCH_LIBS="libsword.a";
else
	SEARCH_LIBS="libsword.a libsword.so";
fi


AC_CACHE_VAL(ac_cv_sword_libdir, AC_FIND_FILE($SEARCH_LIBS, $ac_sword_library_dirs, ac_cv_sword_libdir))

if test "$ac_cv_sword_libdir" = "NO"; then
  AC_MSG_ERROR(SWORD library not found. Try to use configure with --with-sword-dir=/your/SWORD/path!);
fi

if test "$ac_static_sword" = "YES"; then
	LIB_SWORD="$ac_cv_sword_libdir/libsword.a";
else
	LIB_SWORD="-L$ac_cv_sword_libdir -lsword";
fi

#AC_SUBST(SWORD_LIBRARY_PATH)
AC_SUBST(LIB_SWORD)
all_libraries="$all_libraries -L$ac_cv_sword_libdir"

if test "$ac_static_sword" = "YES"; then
	MESSAGE="static library $ac_cv_sword_libdir/libsword.a";
else
	MESSAGE="$ac_cv_sword_libdir";
fi
AC_MSG_RESULT([$MESSAGE])



dnl -- try to find Swords include files --
AC_MSG_CHECKING([for Sword include files])
ac_sword_include_dirs="$ac_sword_dir/include/sword $ac_sword_dir/include /usr/include/sword /usr/include /usr/local/include/sword /usr/local/include /usr/local/sword/include /usr/local/sword/include/sword"

AC_CACHE_VAL(ac_cv_sword_incdir, AC_FIND_FILE(swmgr.h, $ac_sword_include_dirs, ac_cv_sword_incdir))

if test "$ac_cv_sword_incdir" = "NO"; then
	AC_MSG_ERROR([The Sword include file files were not found.
Please try to use configure with --with-sword-dir=/your/SWORD/path !
])
fi

SWORD_INCLUDES="-I$ac_cv_sword_incdir"
AC_SUBST(SWORD_INCLUDES)
all_includes="$all_includes -I$ac_cv_sword_incdir"

AC_MSG_RESULT([$ac_cv_sword_incdir])



dnl -- check if Sword matches the minimum version --
AC_MSG_CHECKING([if you have Sword $1 or later])

AC_CACHE_VAL(ac_cv_installed_sword_version,
[
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
ac_LD_LIBRARY_PATH_safe=$LD_LIBRARY_PATH
ac_LIBRARY_PATH="$LIBRARY_PATH"
ac_cxxflags_safe="$CXXFLAGS"
ac_ldflags_safe="$LDFLAGS"
ac_libs_safe="$LIBS"

CXXFLAGS="$CXXFLAGS -I$"
LDFLAGS="$LDFLAGS -L$ac_cv_sword_libdir"
LIBS="$LIB_SWORD -lz"
LD_LIBRARY_PATH="$ac_cv_sword_libdir"
export LD_LIBRARY_PATH
LIBRARY_PATH=
export LIBRARY_PATH

cat > conftest.$ac_ext <<EOF
#include <iostream.h>
#include <swversion.h>
using std::cout;
using std::endl;
using sword::SWVersion;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << SWVersion::currentVersion << endl;
	}
	else if (argc == 2) 
	{
		if (SWVersion(&argv[[1]]) < SWVersion::currentVersion || SWVersion(&argv[[1]]) == SWVersion::currentVersion)
		{
			cout << 0 << endl;
			return 0;
		}
		else	
		{
			cout << 1 << endl;
			return 1; //version not recent enough
		}
	}
	return 0;
}
EOF

ac_link='${CXX-g++} -o conftest $CXXFLAGS $all_includes $CPPFLAGS $LDFLAGS conftest.$ac_ext $LIBS 1>&5'
if AC_TRY_EVAL(ac_link) && test -s conftest; then
	if test -x conftest; then
		eval ac_cv_installed_sword_version=`./conftest 2>&5`
		echo "configure: ac_cv_installed_sword_version=$ac_cv_installed_sword_version" >&AC_FD_CC
		eval sword_test_returncode=`./conftest $1 2>&5`;
		echo "configure: sword_test_returncode=$sword_test_returncode" >&AC_FD_CC
	fi
else
  echo "configure: failed program was:" >&AC_FD_CC
  cat conftest.$ac_ext >&AC_FD_CC
fi

rm -f conftest*
CXXFLAGS="$ac_cxxflags_safe"
LDFLAGS="$ac_ldflags_safe"
LIBS="$ac_libs_safe"

LD_LIBRARY_PATH="$ac_LD_LIBRARY_PATH_safe"
export LD_LIBRARY_PATH
LIBRARY_PATH="$ac_LIBRARY_PATH"
export LIBRARY_PATH
AC_LANG_RESTORE
])

right_version="ok";
if test "x$sword_test_returncode" = "x1"; then
	echo "configure: changing right_version" >&AC_FD_CC
	right_version="wrong version";
fi;
	
AC_MSG_RESULT([$ac_cv_installed_sword_version])
echo "configure: right_version=$right_version" >&AC_FD_CC
if test "x$right_version" != "xok"; then
        AC_MSG_ERROR([Your Sword installation is not recent enough! $sword_test_returncode Please
upgrade to version $1!]);
fi;

])



AC_DEFUN(AC_FIND_ZLIB,
[
AC_REQUIRE([KDE_CHECK_EXTRA_LIBS])
AC_MSG_CHECKING([for libz])
AC_CACHE_VAL(ac_cv_lib_z,
[
AC_LANG_C
kde_save_LIBS="$LIBS"
LIBS="$all_libraries $USER_LDFLAGS -lz $LIBSOCKET"
kde_save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $all_includes $USER_INCLUDES"
AC_TRY_LINK(dnl
[
#include<zlib.h>
],
            [return (zlibVersion() == ZLIB_VERSION); ],
            eval "ac_cv_lib_z='-lz'",
            eval "ac_cv_lib_z=no")
LIBS="$kde_save_LIBS"
CFLAGS="$kde_save_CFLAGS"
])dnl
if test ! "$ac_cv_lib_z" = no; then
  AC_DEFINE_UNQUOTED(HAVE_LIBZ, 1, [Define if you have libz])
  LIBZ="$ac_cv_lib_z"
  AC_SUBST(LIBZ)
  AC_MSG_RESULT($ac_cv_lib_z)
else
  AC_MSG_ERROR(not found. Check your installation and look into config.log)
  LIBZ=""
  AC_SUBST(LIBZ)
fi
])

AC_DEFUN(KDE_CHECK_EXTRA_LIBS,
[
AC_MSG_CHECKING(for extra includes)
AC_ARG_WITH(extra-includes, [  --with-extra-includes=DIR
                          adds non standard include paths],
  kde_use_extra_includes="$withval",
  kde_use_extra_includes=NONE
)
kde_extra_includes=
if test -n "$kde_use_extra_includes" && \
   test "$kde_use_extra_includes" != "NONE"; then

   ac_save_ifs=$IFS
   IFS=':'
   for dir in $kde_use_extra_includes; do
     kde_extra_includes="$kde_extra_includes $dir"
     USER_INCLUDES="$USER_INCLUDES -I$dir"
   done
   IFS=$ac_save_ifs
   kde_use_extra_includes="added"
else
   kde_use_extra_includes="no"
fi
AC_SUBST(USER_INCLUDES)

AC_MSG_RESULT($kde_use_extra_includes)

kde_extra_libs=
AC_MSG_CHECKING(for extra libs)
AC_ARG_WITH(extra-libs, [  --with-extra-libs=DIR   adds non standard library paths],
  kde_use_extra_libs=$withval,
  kde_use_extra_libs=NONE
)
if test -n "$kde_use_extra_libs" && \
   test "$kde_use_extra_libs" != "NONE"; then

   ac_save_ifs=$IFS
   IFS=':'
   for dir in $kde_use_extra_libs; do
     kde_extra_libs="$kde_extra_libs $dir"
     KDE_EXTRA_RPATH="$KDE_EXTRA_RPATH -R $dir"
     USER_LDFLAGS="$USER_LDFLAGS -L$dir"
   done
   IFS=$ac_save_ifs
   kde_use_extra_libs="added"
else
   kde_use_extra_libs="no"
fi

AC_SUBST(USER_LDFLAGS)

AC_MSG_RESULT($kde_use_extra_libs)

])

dnl ------------------------------------------------------------------------
dnl Find a file (or one of more files in a list of dirs)
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN(AC_FIND_FILE,
[
$3=NO
for i in $2;
do
  for j in $1;
  do
    echo "configure: __oline__: $i/$j" >&AC_FD_CC
    if test -r "$i/$j"; then
      echo "taking that" >&AC_FD_CC
      $3=$i
      break 2
    fi
  done
done
])

#----------------------------------------------------------------
# Look for Perl5
#----------------------------------------------------------------

AC_DEFUN(SW_FIND_PERL,
[


PERLBIN=
PERLSWIG=

AC_ARG_WITH(perl5,[  --with-perl5=path       Set location of Perl5 executable],[ PERLBIN="$withval"], [PERLBIN=])

# First figure out what the name of Perl5 is

if test -z "$PERLBIN"; then
AC_CHECK_PROGS(PERL, perl perl5.6.1 perl5.6.0 perl5.004 perl5.003 perl5.002 perl5.001 perl5 perl)
else
PERL="$PERLBIN"
fi


AC_MSG_CHECKING(for Perl5 header files)
if test -n "$PERL"; then
	PERL5DIR=`($PERL -e 'use Config; print $Config{archlib};') 2>/dev/null`
	if test "$PERL5DIR" != ""; then
		dirs="$PERL5DIR $PERL5DIR/CORE"
		PERL5EXT=none
		PERLSWIG=perlswig
		for i in $dirs; do
			if test -r $i/perl.h; then
				AC_MSG_RESULT($i)
				PERL5EXT="$i"
				break;
			fi
		done
		if test "$PERL5EXT" = none; then
			PERL5EXT="$PERL5DIR/CORE"
			AC_MSG_RESULT(could not locate perl.h...using $PERL5EXT)
		fi

		AC_MSG_CHECKING(for Perl5 library)
		PERL5LIB=`($PERL -e 'use Config; $_=$Config{libperl}; s/^lib//; s/$Config{_a}$//; print $_') 2>/dev/null`
		if test "$PERL5LIB" = "" ; then
			AC_MSG_RESULT(not found)
		else
			AC_MSG_RESULT($PERL5LIB)
		fi
	else
		AC_MSG_RESULT(unable to determine perl5 configuration)
		PERL5EXT=$PERL5DIR
	fi
else
       	AC_MSG_RESULT(could not figure out how to run perl5)
#	PERL5EXT="/usr/local/lib/perl/archname/5.003/CORE"
fi

# Only cygwin (Windows) needs the library for dynamic linking
case $ac_sys_system/$ac_sys_release in
CYGWIN*) PERL5DYNAMICLINKING="-L$PERL5EXT -l$PERL5LIB";;
*)PERL5DYNAMICLINKING="";;
esac


AC_SUBST(PERL5EXT)
AC_SUBST(PERL5DYNAMICLINKING)
AC_SUBST(PERL5LIB)
AC_SUBST(PERLSWIG)

])

#----------------------------------------------------------------
# Look for Python
#----------------------------------------------------------------

AC_DEFUN(SW_FIND_PYTHON,
[

PYINCLUDE=
PYLIB=
PYPACKAGE=

# I don't think any of this commented stuff works anymore

#PYLINK="-lModules -lPython -lObjects -lParser"

#AC_ARG_WITH(py,[  --with-py=path          Set location of Python],[
#	PYPACKAGE="$withval"], [PYPACKAGE=])
#AC_ARG_WITH(pyincl,[  --with-pyincl=path      Set location of Python include directory],[
#	PYINCLUDE="$withval"], [PYINCLUDE=])
#AC_ARG_WITH(pylib,[  --with-pylib=path       Set location of Python library directory],[
#	PYLIB="$withval"], [PYLIB=])

#if test -z "$PYINCLUDE"; then
#   if test -n "$PYPACKAGE"; then
#	PYINCLUDE="$PYPACKAGE/include"
#   fi
#fi

#if test -z "$PYLIB"; then
#   if test -n "$PYPACKAGE"; then
#	PYLIB="$PYPACKAGE/lib"
#   fi
#fi

AC_ARG_WITH(python,[  --with-python=path       Set location of Python executable],[ PYBIN="$withval"], [PYBIN=])

# First figure out the name of the Python executable

if test -z "$PYBIN"; then
AC_CHECK_PROGS(PYTHON, $prefix/bin/python python python2.4 python2.3 python2.2 python2.1 python2.0 python1.6 python1.5 python1.4 python)
else
PYTHON="$PYBIN"
fi

if test -n "$PYTHON"; then
    AC_MSG_CHECKING(for Python prefix)
    PYPREFIX=`($PYTHON -c "import sys; print sys.prefix") 2>/dev/null`
    AC_MSG_RESULT($PYPREFIX)
    AC_MSG_CHECKING(for Python exec-prefix)
    PYEPREFIX=`($PYTHON -c "import sys; print sys.exec_prefix") 2>/dev/null`
    AC_MSG_RESULT($PYEPREFIX)


    # Note: I could not think of a standard way to get the version string from different versions.
    # This trick pulls it out of the file location for a standard library file.

    AC_MSG_CHECKING(for Python version)

    # Need to do this hack since autoconf replaces __file__ with the name of the configure file
    filehack="file__"
    PYVERSION=`($PYTHON -c "import string,operator; print operator.getitem(string.split(string.__$filehack,'/'),-2)")`
    AC_MSG_RESULT($PYVERSION)

    # Set the include directory

    AC_MSG_CHECKING(for Python header files)		
    if test -r $PYPREFIX/include/$PYVERSION/Python.h; then
        PYINCLUDE="-I$PYPREFIX/include/$PYVERSION -I$PYEPREFIX/lib/$PYVERSION/config"
    fi
    if test -z "$PYINCLUDE"; then
        if test -r $PYPREFIX/include/Py/Python.h; then
            PYINCLUDE="-I$PYPREFIX/include/Py -I$PYEPREFIX/lib/python/lib"
        fi
    fi
    AC_MSG_RESULT($PYINCLUDE)

    # Set the library directory blindly.   This probably won't work with older versions
    AC_MSG_CHECKING(for Python library)
    dirs="$PYVERSION/config $PYVERSION/lib python/lib"
    for i in $dirs; do
        if test -d $PYEPREFIX/lib/$i; then
           PYLIB="$PYEPREFIX/lib/$i"
           break
        fi
    done
    if test -z "$PYLIB"; then
        AC_MSG_RESULT(Not found)
    else
        AC_MSG_RESULT($PYLIB)
    fi

    # Check for really old versions
    if test -r $PYLIB/libPython.a; then
         PYLINK="-lModules -lPython -lObjects -lParser"
    else
         PYLINK="-l$PYVERSION"
    fi
fi

# Only cygwin (Windows) needs the library for dynamic linking
case $ac_sys_system/$ac_sys_release in
CYGWIN*) PYTHONDYNAMICLINKING="-L$PYLIB $PYLINK"
         PYINCLUDE="-DUSE_DL_IMPORT $PYINCLUDE"
         ;;
*)PYTHONDYNAMICLINKING="";;
esac


AC_SUBST(PYINCLUDE)
AC_SUBST(PYLIB)
AC_SUBST(PYLINK)
AC_SUBST(PYTHONDYNAMICLINKING)

])

#-------------------------------------------------------------------------
# Look for Php4
#-------------------------------------------------------------------------

AC_DEFUN(SW_FIND_PHP4,
[

PHP4BIN=

AC_ARG_WITH(php4,[  --with-php4=path	  Set location of PHP4 executable],[ PHP4BIN="$withval"], [PHP4BIN=])

if test -z "$PHP4BIN"; then
AC_CHECK_PROGS(PHP4, php, php4)
else
PHP4="$PHP4BIN"
fi
AC_MSG_CHECKING(for PHP4 header files)
dirs="/usr/include/php /usr/local/include/php /usr/local/apache/php"
for i in $dirs; do
	if test -r $i/php_config.h -o -r $i/php_version.h; then
		AC_MSG_RESULT($i)
		PHP4EXT="$i"
		PHP4INC="-I$PHP4EXT -I$PHP4EXT/Zend -I$PHP4EXT/main -I$PHP4EXT/TSRM"
		break;
	fi
done
if test -z "$PHP4INC"; then
	AC_MSG_RESULT(not found)
fi

AC_SUBST(PHP4INC)
	
])

#--------------------------------------------------------------------
#	Try to locate the Tcl package
#--------------------------------------------------------------------

AC_DEFUN(SW_FIND_TCL,
[


TCLINCLUDE=
TCLLIB=
TCLPACKAGE=
TCLSWIG=

AC_ARG_WITH(tclconfig,[  --with-tclconfig=path  Set location of tclConfig.sh],
        with_tclconfig="$withval")
AC_ARG_WITH(tcl,[  --with-tcl=path         Set location of Tcl package],[
	TCLPACKAGE="$withval"], [TCLPACKAGE=])
AC_ARG_WITH(tclincl,[  --with-tclincl=path     Set location of Tcl include directory],[
	TCLINCLUDE="-I$withval"], [TCLINCLUDE=])
AC_ARG_WITH(tcllib,[  --with-tcllib=path      Set location of Tcl library directory],[
	TCLLIB="-L$withval"], [TCLLIB=])

AC_MSG_CHECKING([for Tcl configuration])
# First check to see if --with-tclconfig was specified.
if test x"${with_tclconfig}" != x ; then
   if test -f "${with_tclconfig}/tclConfig.sh" ; then
      TCLCONFIG=`(cd ${with_tclconfig}; pwd)`
   else
      AC_MSG_ERROR([${with_tcl} directory doesn't contain tclConfig.sh])
   fi
fi
# check in a few common install locations
if test x"${TCLCONFIG}" = x ; then
    for i in `ls -d /usr/lib 2>/dev/null` \
	     `ls -d ${prefix}/lib 2>/dev/null` \
	    `ls -d /usr/local/lib 2>/dev/null` ; do
	if test -f "$i/tclConfig.sh" ; then
	    TCLCONFIG=`(cd $i; pwd)`
	    break
	fi
    done
fi
if test x"${TCLCONFIG}" = x ; then
    AC_MSG_RESULT(no)
else
    AC_MSG_RESULT(found $TCLCONFIG/tclConfig.sh)
    . $TCLCONFIG/tclConfig.sh
    TCLINCLUDE=-I$TCL_PREFIX/include
    TCLLIB=$TCL_LIB_SPEC
fi

if test -z "$TCLINCLUDE"; then
   if test -n "$TCLPACKAGE"; then
	TCLINCLUDE="-I$TCLPACKAGE/include"
   fi
fi

if test -z "$TCLLIB"; then
   if test -n "$TCLPACKAGE"; then
	TCLLIB="-L$TCLPACKAGE/lib -ltcl"
   fi
fi

AC_MSG_CHECKING(for Tcl header files)
if test -z "$TCLINCLUDE"; then
AC_TRY_CPP([#include <tcl.h>], , TCLINCLUDE="")
if test -z "$TCLINCLUDE"; then
	dirs="$prefix/include /usr/local/include /usr/include /opt/local/include"
	for i in $dirs ; do
		if test -r $i/tcl.h; then
			AC_MSG_RESULT($i)
			TCLINCLUDE="-I$i"
			break
		fi
	done
fi
if test -z "$TCLINCLUDE"; then
#	TCLINCLUDE="-I/usr/local/include"
    	AC_MSG_RESULT(not found)
fi
else
        AC_MSG_RESULT($TCLINCLUDE)
fi

AC_MSG_CHECKING(for Tcl library)
if test -z "$TCLLIB"; then
dirs="$prefix/lib /usr/local/lib /usr/lib /opt/local/lib"
for i in $dirs ; do
	if test -r $i/libtcl.a; then
	    AC_MSG_RESULT($i)
	    TCLLIB="-L$i -ltcl"
	    break
	fi
done
if test -z "$TCLLIB"; then
	AC_MSG_RESULT(not found)
#	TCLLIB="-L/usr/local/lib"
fi
else
AC_MSG_RESULT($TCLLIB)
fi

# Only cygwin (Windows) needs the library for dynamic linking
case $ac_sys_system/$ac_sys_release in
CYGWIN*) TCLDYNAMICLINKING="$TCLLIB";;
*)TCLDYNAMICLINKING="";;
esac

AC_SUBST(TCLINCLUDE)
AC_SUBST(TCLLIB)
AC_SUBST(TCLDYNAMICLINKING)
AC_SUBST(TCLSWIG)

])

