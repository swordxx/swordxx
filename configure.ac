# ---------------------------------------------------------------------
# Initialisation
# ---------------------------------------------------------------------

AC_INIT(sword, 1.5.5.99, sword-bugs@crosswire.org)
AC_CONFIG_SRCDIR(sword.bmp)
AC_PREREQ(2.52)
AC_REVISION($Revision: 1.20 $)

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(sword, 1.5.5.99)

AM_CONFIG_HEADER(config.h)


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
# Check libraries
# ---------------------------------------------------------------------
AC_CHECK_LIB(z, compress)
#AC_CHECK_LIB(stdc++, cerr)

# ---------------------------------------------------------------------
# With options
# ---------------------------------------------------------------------
AC_ARG_WITH(zlib,[  --with-zlib             allow zlib compressed modules [default=yes]],,with_zlib=yes)
AC_ARG_WITH(icu,[  --with-icu              use ICU for unicode [default=no]],,with_icu=no)
AC_ARG_WITH(conf,[  --with-conf             install a new sword.conf [default=yes]],,with_conf=yes)


# ---------------------------------------------------------------------
# Enable options
# ---------------------------------------------------------------------
AC_ARG_ENABLE(installmgr,[  --enable-installmgr     build install manager suport [default=no]],,enable_installmgr=no)
AC_ARG_ENABLE(debug,[  --enable-debug          build debug library [default=no]],,enable_debug=no)
AC_ARG_ENABLE(profile,[  --enable-profile        allow profiling [default=no]],,enable_profile=no)
AC_ARG_ENABLE(profilefn,[  --enable-profilefn      allow functioncheck profiling [default=no]],,enable_profilefn=no)
AC_ARG_ENABLE(tests,[  --enable-tests          build test programs [default=no]],,enable_tests=no)
AC_ARG_ENABLE(utilities,[  --enable-utilities      build utilities [default=yes]],,enable_utilities=yes)
AC_ARG_ENABLE(examples,[  --enable-examples       build examples [default=no]],,enable_examples=no)
AM_MAINTAINER_MODE


# ---------------------------------------------------------------------
# Find ICU tools
# ---------------------------------------------------------------------
if test x$with_icu = xyes; then
	AC_PATH_PROG([PKGDATA], [pkgdata],[/usr/sbin/pkgdata],[$PATH:/usr/sbin:.])
	AC_PATH_PROG([GENCCODE], [genccode], [/usr/sbin/genccode], [$PATH:/usr/sbin:.])
	AC_PATH_PROG([GENCMN], [gencmn], [/usr/sbin/gencmn], [$PATH:/usr/sbin:.])
	AC_PATH_PROG([GENRB], [genrb], [/usr/sbin/genrb], [$PATH:/usr/sbin:.])
fi

# ---------------------------------------------------------------------
# Debug and profile
# ---------------------------------------------------------------------

CXXFLAGS="$CXXFLAGS -ftemplate-depth-25"

if test x$enable_debug = xyes; then
  if test x$ac_cv_prog_cc_g = xyes; then
    CFLAGS="-g3 -O0"
  else
    CFLAGS="-O0"
  fi
  if test x$ac_cv_prog_cxx_g = xyes; then
    CXXFLAGS="-g3 -O0"
  else
    CXXFLAGS="-O0"
  fi
  CFLAGS="$CFLAGS -Werror"
  CXXFLAGS="$CXXFLAGS -Werror"
fi
#  AC_DEFINE(DEBUG)
#else
#  AC_DEFINE(NDEBUG)
#fi

if test x$enable_profile = xyes; then
  CFLAGS="$CFLAGS -pg"
  CXXFLAGS="$CXXFLAGS -pg"
fi
if test x$enable_profilefn = xyes; then
  CFLAGS="$CFLAGS -g -finstrument-functions"
  CXXFLAGS="$CXXFLAGS -g -finstrument-functions"
  LIBS="$LIBS -g -finstrument-functions -lfnccheck"
fi

# ---------------------------------------------------------------------
# Check for curl for installmgr suport
# ---------------------------------------------------------------------

CURL_CFLAGS=
CURL_LIBS=
AC_PATH_PROG(CURL_CONFIG, curl-config, no)
if test "$CURL_CONFIG" = "no" ; then
	echo "*** The curl-config script installed by curl could not be found"
	echo "*** compiling without install manager support"
	enable_installmgr=no
else
	echo "curl found - remote install options available"
	CURL_CFLAGS=`$CURL_CONFIG --cflags`
	CURL_LIBS=`$CURL_CONFIG --libs`
	CXXFLAGS="$CXXFLAGS -DCURLAVAILABLE"
	CFLAGS="$CFLAGS -DCURLAVAILABLE"
fi

# ---------------------------------------------------------------------
# Alter global conf directory only if not /etc
# ---------------------------------------------------------------------

if test "x$sysconfdir" = "x/etc"; then
  dir_confdef="no"
else
  dir_confdef="yes"
fi


# ---------------------------------------------------------------------
# Determine target system (obsolete?)
# ---------------------------------------------------------------------
case "$target_cpu" in
  i?86)
    target_system="intel"
    ;;
  sparc)
	case "$target_os" in
  	solaris*)
    	target_system="sparc_solaris"
    	;;
  	*)
    	target_system="gnu_bigendian"
    	;;
	esac
    ;;
  *)
    target_system="$target_cpu"
    ;;
esac

case "$target_os" in
  beos)
    target_system="beos"
    ;;
  *)
    target_system="$target_system"
    ;;
esac

case "$target_os" in
  mingw*)
    target_mingw32="yes"
    ;;
  *)
    target_mingw32="no"
    ;;
esac

# ---------------------------------------------------------------------
# Substitute variables into makefiles
# ---------------------------------------------------------------------
AC_SUBST(with_zlib)
AC_SUBST(with_icu)
AC_SUBST(with_conf)
AC_SUBST(dir_confdef)
AC_SUBST(CC)
AC_SUBST(CURL_CFLAGS)
AC_SUBST(CURL_LIBS)
AC_SUBST(enable_debug)
AC_SUBST(enable_profile)

AC_SUBST(target_cpu)
AC_SUBST(target_vendor)
AC_SUBST(target_os)

AC_SUBST(target_system)
AC_SUBST(target_mingw32)



# ---------------------------------------------------------------------
# Conditional variables
# ---------------------------------------------------------------------
AM_CONDITIONAL(MINGW, test x$target_mingw32 = xyes)
AM_CONDITIONAL(ICU, test x$with_icu = xyes)
AM_CONDITIONAL(ICUSWORD, test x$with_icu = xsword)
AM_CONDITIONAL(ZLIB, test x$with_zlib = xyes)
AM_CONDITIONAL(INSTCONF, test x$with_conf = xyes)
AM_CONDITIONAL(INSTALLMGR, test x$enable_installmgr = xyes)
AM_CONDITIONAL(CONFDEF, test x$dir_confdef = xyes)
#AM_CONDITIONAL(DEBUG, test x$with_debug = xyes)
#AM_CONDITIONAL(PROFILE, test x$with_profile = xyes)
AM_CONDITIONAL(BUILDTESTS, test x$enable_tests = xyes)
AM_CONDITIONAL(BUILDUTILS, test x$enable_utilities = xyes)
AM_CONDITIONAL(BUILDEXAMPLES, test x$enable_examples = xyes)

# ---------------------------------------------------------------------
# Final output
# ---------------------------------------------------------------------
AC_CONFIG_FILES(Makefile lib/Makefile tests/Makefile utilities/Makefile examples/Makefile \
	examples/cmdline/Makefile apps/Makefile apps/console/Makefile \
	apps/console/diatheke/Makefile icu/Makefile)
AC_OUTPUT
