# General defines

version := 1.5.3


# ownership (used by 'make install')
# use group := wheel for MAC OS X instead of root

user      := root
group     := root
#group     := wheel

# this WILL NOT WORK for you. only inhouse patched version of
# sapphire will omit enciphering code when set to yes, so leave
# as no, unless you really know what you're doing

legalexport := no

##### Customizable stuff. Set it how you like it #####

# if building dll with gcc on windoze (www.mingw.org)
# note you must set need_local_regex and buildshare to yes
if MINGW
mingw = yes
need_local_regex := yes
else

# need regular expression stuff?
need_local_regex := no

endif

# VCL library- set to yes if you would like to build
buildvcl := @enable_vcl@


# Debugging options etc...
debug   := @enable_debug@
profile := @enable_profile@


# endian, etc. support for other hardware

system := @target_system@

#system := intel
#system := sparc_solaris
#system := macosx
#system := arm
#system := gnu_bigendian
#system := beos

# SPARC Solaris users should define system as sparc_solaris; SPARC users running Linux or other OSes with glibc, like Debian GNU/Linux should use gnu_bigendian instead

# to build on FreeBSD, simply use gmake instead of make

# to build for Darwin on PPC, set system to macosx


# zLib support
zlib := @with_zlib@

# icu support
icu := @with_icu@

# Global config directory
globalconfdir := @sysconfdir@

SUBDIRS = lib . tests utilities
INCLUDES = -I $(top_srcdir)/include
EXTRA_DIST = sword.spec sword.kdevprj sword.bmp options.conf
sysconf_DATA = samples/recommended/sword.conf

noinst_PROGRAMS = testlib
testlib_SOURCES = testlib.cpp
testlib_LDADD = -Llib -lsword

include locales.d/Makefile.am
include samples/Makefile.am

restore:
	cd $(top_srcdir) && ./scripts/restoremake.sh

install-data-am: installlocaleDATA installmodsDATA

uninstall-am: uninstall-info-am uninstalllocaleDATA \
	uninstallmodsDATA uninstall-sysconfDATA
