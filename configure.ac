AC_INIT(sword, 1.5.3, sword-bugs@crosswire.org)
AC_CONFIG_SRCDIR(sword.bmp)
AC_PREREQ(2.52)
AC_REVISION($Revision: 1.1 $)

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(sword,1.5.3)

AM_CONFIG_HEADER(config.h)

AC_LANG(C++)
AC_PROG_CC
AC_PROG_CXX
#AC_PROG_RANLIB
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_C_BIGENDIAN

AC_CHECK_LIB(z, compress)
AC_CHECK_LIB(stdc++, cerr)
AC_ARG_WITH(zlib,[  --with-zlib             allow zlib compressed modules [default=yes]],,with_zlib=yes)
AC_ARG_WITH(icu,[  --with-icu              use ICU for unicode [default=no]],,with_icu=no)
#AC_ARG_ENABLE(shared,[  --enable-shared         build shared library [default=no]],,enable_shared=no)
AC_ARG_ENABLE(vcl,[  --enable-vcl            build VCL library [default=no]],,enable_vcl=no)
AC_ARG_ENABLE(debug,[  --enable-debug          build debug library [default=no]],,enable_debug=no)
AC_ARG_ENABLE(profile,[  --enable-profile        allow profiling [default=no]],,enable_profile=no)

AM_MAINTAINER_MODE

AC_SUBST(with_zlib)
AC_SUBST(with_icu)
AC_SUBST(CC)
#AC_SUBST(enable_shared)
AC_SUBST(enable_vcl)
AC_SUBST(enable_debug)
AC_SUBST(enable_profile)

AC_SUBST(target_cpu)
AC_SUBST(target_vendor)
AC_SUBST(target_os)

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

AM_CONDITIONAL(MINGW, test x$target_os = xmingw32*)
AM_CONDITIONAL(ICU, test x$with_icu = xyes)
AM_CONDITIONAL(ZLIB, test x$with_zlib = xyes)


AC_SUBST(target_system)
AC_CONFIG_FILES(Makefile tests/Makefile utilities/Makefile)
AC_OUTPUT
