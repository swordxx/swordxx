# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



























SHELL = /bin/sh

srcdir = .
top_srcdir = ..

prefix = /usr
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = /etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/sword
pkglibdir = $(libdir)/sword
pkgincludedir = $(includedir)/sword
top_builddir = ..

ACLOCAL = ${SHELL} /home/scribe/src/sword/missing --run aclocal
AUTOCONF = ${SHELL} /home/scribe/src/sword/missing --run autoconf
AUTOMAKE = ${SHELL} /home/scribe/src/sword/missing --run automake
AUTOHEADER = ${SHELL} /home/scribe/src/sword/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = i686-pc-linux-gnu
AMTAR = ${SHELL} /home/scribe/src/sword/missing --run tar
AS = as
AWK = gawk
CC = gcc
CXX = g++
DEPDIR = .deps
DLLTOOL = dlltool
ECHO = echo
EXEEXT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
MAINT = #
OBJDUMP = objdump
OBJEXT = o
PACKAGE = sword
RANLIB = ranlib
STRIP = strip
VERSION = 1.5.3
am__include = include
am__quote = 
dir_confdef = no
enable_debug = yes
enable_profile = no
enable_vcl = no
install_sh = /home/scribe/src/sword/install-sh
target_cpu = i686
target_mingw32 = no
target_os = linux-gnu
target_system = intel
target_vendor = pc
with_conf = no
with_icu = no
with_zlib = yes

AUTOMAKE_OPTIONS = 1.5
INCLUDES = -I$(top_srcdir)/include

# Global config directory
globalconfdir := /etc

lib_LTLIBRARIES = libsword.la

libsword_la_SOURCES = $(keysdir)/swkey.cpp $(keysdir)/listkey.cpp $(keysdir)/versekey.cpp $(keysdir)/strkey.cpp $(keysdir)/treekey.cpp $(keysdir)/treekeyidx.cpp $(utilfunsdir)/Greek2Greek.cpp $(utilfunsdir)/utilstr.cpp $(utilfunsdir)/unixstr.cpp $(utilfunsdir)/swunicod.cpp $(utilfunsdir)/swversion.cpp $(SWREGEX) $(utilfunsdir)/roman.c $(mgrdir)/swconfig.cpp $(mgrdir)/swmgr.cpp $(mgrdir)/swfiltermgr.cpp $(mgrdir)/encfiltmgr.cpp $(mgrdir)/markupfiltmgr.cpp $(mgrdir)/filemgr.cpp $(mgrdir)/swlocale.cpp $(mgrdir)/localemgr.cpp $(frontenddir)/swdisp.cpp $(frontenddir)/swlog.cpp $(modulesdir)/swmodule.cpp $(commondir)/rawstr.cpp $(commondir)/rawstr4.cpp $(commondir)/swcomprs.cpp $(commondir)/lzsscomprs.cpp $(SWZLIB) $(commondir)/rawverse.cpp $(commondir)/swcipher.cpp $(commondir)/zverse.cpp $(commondir)/zstr.cpp $(commondir)/entriesblk.cpp $(commondir)/sapphire.cpp $(filtersdir)/swbasicfilter.cpp $(filtersdir)/gbfhtml.cpp $(filtersdir)/gbfhtmlhref.cpp $(filtersdir)/gbfplain.cpp $(filtersdir)/gbfrtf.cpp $(filtersdir)/plainhtml.cpp $(filtersdir)/rwphtml.cpp $(filtersdir)/rwprtf.cpp $(filtersdir)/cipherfil.cpp $(filtersdir)/rtfhtml.cpp $(filtersdir)/gbfstrongs.cpp $(filtersdir)/gbffootnotes.cpp $(filtersdir)/gbfheadings.cpp $(filtersdir)/gbfmorph.cpp $(filtersdir)/plainfootnotes.cpp $(filtersdir)/thmlstrongs.cpp $(filtersdir)/thmlfootnotes.cpp $(filtersdir)/thmlheadings.cpp $(filtersdir)/thmlmorph.cpp $(filtersdir)/thmllemma.cpp $(filtersdir)/thmlscripref.cpp $(filtersdir)/thmlvariants.cpp $(filtersdir)/gbfthml.cpp $(filtersdir)/thmlgbf.cpp $(filtersdir)/thmlrtf.cpp $(filtersdir)/thmlhtml.cpp $(filtersdir)/thmlhtmlhref.cpp $(filtersdir)/thmlplain.cpp $(filtersdir)/unicodertf.cpp $(filtersdir)/latin1utf8.cpp $(filtersdir)/latin1utf16.cpp $(filtersdir)/utf8utf16.cpp $(filtersdir)/utf16utf8.cpp $(filtersdir)/scsuutf8.cpp $(filtersdir)/utf8html.cpp $(filtersdir)/utf8latin1.cpp $(filtersdir)/thmlolb.cpp $(SWICUSRC) $(filtersdir)/utf8cantillation.cpp $(filtersdir)/utf8hebrewpoints.cpp $(filtersdir)/utf8greekaccents.cpp $(genbookdir)/swgenbook.cpp $(rawgenbookdir)/rawgenbook.cpp $(textsdir)/swtext.cpp $(rawtextdir)/rawtext.cpp $(ztextdir)/ztext.cpp $(rawgbfdir)/rawgbf.cpp $(commentsdir)/swcom.cpp $(rawcomdir)/rawcom.cpp $(rawfilesdir)/rawfiles.cpp $(zcomdir)/zcom.cpp $(hrefcomdir)/hrefcom.cpp $(lexdictdir)/swld.cpp $(rawlddir)/rawld.cpp $(rawld4dir)/rawld4.cpp $(zlddir)/zld.cpp $(bindingsdir)/flatapi.cpp
libsword_la_LIBADD = 
libsword_la_LDFLAGS = -version-info 1

keysdir = $(top_srcdir)/src/keys

utilfunsdir = $(top_srcdir)/src/utilfuns

#SWREGEX = $(utilfunsdir)/regex.c
SWREGEX = 

mgrdir = $(top_srcdir)/src/mgr

#globdef = -DGLOBCONFPATH=\"${globalconfdir}/sword.conf\"
globdef = 

DEFS = $(globdef) -DUNIX $(ICUDEFS)

frontenddir = $(top_srcdir)/src/frontend

modulesdir = $(top_srcdir)/src/modules

commondir = $(top_srcdir)/src/modules/common

SWZLIB = $(commondir)/zipcomprs.cpp
#SWZLIB = 

filtersdir = $(top_srcdir)/src/modules/filters

#ICUDEFS = -D_ICU_
ICUDEFS = 
#SWICUSRC = $(filtersdir)/utf8transliterator.cpp $(filtersdir)/utf8nfc.cpp $(filtersdir)/utf8nfkd.cpp $(filtersdir)/utf8arshaping.cpp $(filtersdir)/utf8bidireorder.cpp
SWICUSRC = 

genbookdir = $(top_srcdir)/src/modules/genbook

rawgenbookdir = $(top_srcdir)/src/modules/genbook/rawgenbook

textsdir = $(top_srcdir)/src/modules/texts

rawtextdir = $(top_srcdir)/src/modules/texts/rawtext

ztextdir = $(top_srcdir)/src/modules/texts/ztext

rawgbfdir = $(top_srcdir)/src/modules/texts/rawgbf

commentsdir = $(top_srcdir)/src/modules/comments

rawcomdir = $(top_srcdir)/src/modules/comments/rawcom

rawfilesdir = $(top_srcdir)/src/modules/comments/rawfiles

zcomdir = $(top_srcdir)/src/modules/comments/zcom

hrefcomdir = $(top_srcdir)/src/modules/comments/hrefcom

lexdictdir = $(top_srcdir)/src/modules/lexdict

rawlddir = $(top_srcdir)/src/modules/lexdict/rawld

rawld4dir = $(top_srcdir)/src/modules/lexdict/rawld4

zlddir = $(top_srcdir)/src/modules/lexdict/zld

bindingsdir = $(top_srcdir)/bindings

swincludedir = $(top_srcdir)/include

pkginclude_HEADERS = $(swincludedir)/Greek2Greek.h $(swincludedir)/GreekChars.h $(swincludedir)/canon.h $(swincludedir)/cipherfil.h $(swincludedir)/defs.h $(swincludedir)/echomod.h $(swincludedir)/encfiltmgr.h $(swincludedir)/entriesblk.h $(swincludedir)/femain.h $(swincludedir)/filemgr.h $(swincludedir)/flatapi.h $(swincludedir)/gbffootnotes.h $(swincludedir)/gbfheadings.h $(swincludedir)/gbfhtml.h $(swincludedir)/gbfhtmlhref.h $(swincludedir)/gbfmorph.h $(swincludedir)/gbfplain.h $(swincludedir)/gbfrtf.h $(swincludedir)/gbfstrongs.h $(swincludedir)/gbfthml.h $(swincludedir)/hebrewmcim.h $(swincludedir)/hrefcom.h $(swincludedir)/latin1utf16.h $(swincludedir)/latin1utf8.h $(swincludedir)/listkey.h $(swincludedir)/localemgr.h $(swincludedir)/lzsscomprs.h $(swincludedir)/markupfiltmgr.h $(swincludedir)/nullim.h $(swincludedir)/plainfootnotes.h $(swincludedir)/plainhtml.h $(swincludedir)/rawcom.h $(swincludedir)/rawfiles.h $(swincludedir)/rawgbf.h $(swincludedir)/rawgenbook.h $(swincludedir)/rawld.h $(swincludedir)/rawld4.h $(swincludedir)/rawstr.h $(swincludedir)/rawstr4.h $(swincludedir)/rawtext.h $(swincludedir)/rawverse.h $(swincludedir)/regex.h $(swincludedir)/roman.h $(swincludedir)/rtfhtml.h $(swincludedir)/rwphtml.h $(swincludedir)/rwprtf.h $(swincludedir)/sapphire.h $(swincludedir)/scsuutf8.h $(swincludedir)/strkey.h $(swincludedir)/swbasicfilter.h $(swincludedir)/swbyteswap.h $(swincludedir)/swcipher.h $(swincludedir)/swcom.h $(swincludedir)/swcomprs.h $(swincludedir)/swconfig.h $(swincludedir)/swdisp.h $(swincludedir)/swdisprtf.h $(swincludedir)/swdisprtfchap.h $(swincludedir)/swfilter.h $(swincludedir)/swfiltermgr.h $(swincludedir)/swgenbook.h $(swincludedir)/swinputmeth.h $(swincludedir)/swkey.h $(swincludedir)/swld.h $(swincludedir)/swlocale.h $(swincludedir)/swlog.h $(swincludedir)/swmacs.h $(swincludedir)/swmgr.h $(swincludedir)/swmodule.h $(swincludedir)/swobject.h $(swincludedir)/swtext.h $(swincludedir)/swunicod.h $(swincludedir)/swversion.h $(swincludedir)/swwinlog.h $(swincludedir)/sysdata.h $(swincludedir)/tbdisp.h $(swincludedir)/thmlfootnotes.h $(swincludedir)/thmlgbf.h $(swincludedir)/thmlheadings.h $(swincludedir)/thmlhtml.h $(swincludedir)/thmlhtmlhref.h $(swincludedir)/thmllemma.h $(swincludedir)/thmlmorph.h $(swincludedir)/thmlolb.h $(swincludedir)/thmlplain.h $(swincludedir)/thmlrtf.h $(swincludedir)/thmlscripref.h $(swincludedir)/thmlstrongs.h $(swincludedir)/thmlvariants.h $(swincludedir)/treekey.h $(swincludedir)/treekeyidx.h $(swincludedir)/unicodertf.h $(swincludedir)/unixstr.h $(swincludedir)/untgz.h $(swincludedir)/utf16utf8.h $(swincludedir)/utf8arshaping.h $(swincludedir)/utf8bidireorder.h $(swincludedir)/utf8cantillation.h $(swincludedir)/utf8greekaccents.h $(swincludedir)/utf8hebrewpoints.h $(swincludedir)/utf8html.h $(swincludedir)/utf8latin1.h $(swincludedir)/utf8nfc.h $(swincludedir)/utf8nfkd.h $(swincludedir)/utf8transliterator.h $(swincludedir)/utf8utf16.h $(swincludedir)/utilconf.h $(swincludedir)/utilfuns.h $(swincludedir)/utilstr.h $(swincludedir)/versekey.h $(swincludedir)/zcom.h $(swincludedir)/zconf.h $(swincludedir)/zipcomprs.h $(swincludedir)/zld.h $(swincludedir)/zlib.h $(swincludedir)/ztext.h $(swincludedir)/zverse.h
subdir = lib
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
LTLIBRARIES = $(lib_LTLIBRARIES)

libsword_la_DEPENDENCIES =
am_libsword_la_OBJECTS = swkey.lo \
	listkey.lo versekey.lo \
	strkey.lo treekey.lo \
	treekeyidx.lo Greek2Greek.lo \
	utilstr.lo unixstr.lo \
	swunicod.lo swversion.lo \
	roman.lo swconfig.lo \
	swmgr.lo swfiltermgr.lo \
	encfiltmgr.lo \
	markupfiltmgr.lo filemgr.lo \
	swlocale.lo localemgr.lo \
	swdisp.lo swlog.lo \
	swmodule.lo rawstr.lo \
	rawstr4.lo swcomprs.lo \
	lzsscomprs.lo zipcomprs.lo \
	rawverse.lo swcipher.lo \
	zverse.lo zstr.lo \
	entriesblk.lo sapphire.lo \
	swbasicfilter.lo gbfhtml.lo \
	gbfhtmlhref.lo gbfplain.lo \
	gbfrtf.lo plainhtml.lo \
	rwphtml.lo rwprtf.lo \
	cipherfil.lo rtfhtml.lo \
	gbfstrongs.lo \
	gbffootnotes.lo \
	gbfheadings.lo gbfmorph.lo \
	plainfootnotes.lo \
	thmlstrongs.lo \
	thmlfootnotes.lo \
	thmlheadings.lo thmlmorph.lo \
	thmllemma.lo thmlscripref.lo \
	thmlvariants.lo gbfthml.lo \
	thmlgbf.lo thmlrtf.lo \
	thmlhtml.lo thmlhtmlhref.lo \
	thmlplain.lo unicodertf.lo \
	latin1utf8.lo latin1utf16.lo \
	utf8utf16.lo utf16utf8.lo \
	scsuutf8.lo utf8html.lo \
	utf8latin1.lo thmlolb.lo \
	utf8cantillation.lo \
	utf8hebrewpoints.lo \
	utf8greekaccents.lo \
	swgenbook.lo rawgenbook.lo \
	swtext.lo rawtext.lo \
	ztext.lo rawgbf.lo swcom.lo \
	rawcom.lo rawfiles.lo \
	zcom.lo hrefcom.lo swld.lo \
	rawld.lo rawld4.lo zld.lo \
	flatapi.lo
#am_libsword_la_OBJECTS = swkey.lo \
#	listkey.lo versekey.lo \
#	strkey.lo treekey.lo \
#	treekeyidx.lo \
#	Greek2Greek.lo utilstr.lo \
#	unixstr.lo swunicod.lo \
#	swversion.lo roman.lo \
#	swconfig.lo swmgr.lo \
#	swfiltermgr.lo \
#	encfiltmgr.lo \
#	markupfiltmgr.lo filemgr.lo \
#	swlocale.lo localemgr.lo \
#	swdisp.lo swlog.lo \
#	swmodule.lo rawstr.lo \
#	rawstr4.lo swcomprs.lo \
#	lzsscomprs.lo rawverse.lo \
#	swcipher.lo zverse.lo \
#	zstr.lo entriesblk.lo \
#	sapphire.lo \
#	swbasicfilter.lo gbfhtml.lo \
#	gbfhtmlhref.lo gbfplain.lo \
#	gbfrtf.lo plainhtml.lo \
#	rwphtml.lo rwprtf.lo \
#	cipherfil.lo rtfhtml.lo \
#	gbfstrongs.lo \
#	gbffootnotes.lo \
#	gbfheadings.lo gbfmorph.lo \
#	plainfootnotes.lo \
#	thmlstrongs.lo \
#	thmlfootnotes.lo \
#	thmlheadings.lo \
#	thmlmorph.lo thmllemma.lo \
#	thmlscripref.lo \
#	thmlvariants.lo gbfthml.lo \
#	thmlgbf.lo thmlrtf.lo \
#	thmlhtml.lo thmlhtmlhref.lo \
#	thmlplain.lo unicodertf.lo \
#	latin1utf8.lo \
#	latin1utf16.lo utf8utf16.lo \
#	utf16utf8.lo scsuutf8.lo \
#	utf8html.lo utf8latin1.lo \
#	thmlolb.lo \
#	utf8cantillation.lo \
#	utf8hebrewpoints.lo \
#	utf8greekaccents.lo \
#	swgenbook.lo rawgenbook.lo \
#	swtext.lo rawtext.lo \
#	ztext.lo rawgbf.lo swcom.lo \
#	rawcom.lo rawfiles.lo \
#	zcom.lo hrefcom.lo swld.lo \
#	rawld.lo rawld4.lo zld.lo \
#	flatapi.lo
#am_libsword_la_OBJECTS = swkey.lo \
#	listkey.lo versekey.lo \
#	strkey.lo treekey.lo \
#	treekeyidx.lo Greek2Greek.lo \
#	utilstr.lo unixstr.lo \
#	swunicod.lo swversion.lo \
#	regex.lo roman.lo swconfig.lo \
#	swmgr.lo swfiltermgr.lo \
#	encfiltmgr.lo \
#	markupfiltmgr.lo filemgr.lo \
#	swlocale.lo localemgr.lo \
#	swdisp.lo swlog.lo \
#	swmodule.lo rawstr.lo \
#	rawstr4.lo swcomprs.lo \
#	lzsscomprs.lo zipcomprs.lo \
#	rawverse.lo swcipher.lo \
#	zverse.lo zstr.lo \
#	entriesblk.lo sapphire.lo \
#	swbasicfilter.lo gbfhtml.lo \
#	gbfhtmlhref.lo gbfplain.lo \
#	gbfrtf.lo plainhtml.lo \
#	rwphtml.lo rwprtf.lo \
#	cipherfil.lo rtfhtml.lo \
#	gbfstrongs.lo gbffootnotes.lo \
#	gbfheadings.lo gbfmorph.lo \
#	plainfootnotes.lo \
#	thmlstrongs.lo \
#	thmlfootnotes.lo \
#	thmlheadings.lo thmlmorph.lo \
#	thmllemma.lo thmlscripref.lo \
#	thmlvariants.lo gbfthml.lo \
#	thmlgbf.lo thmlrtf.lo \
#	thmlhtml.lo thmlhtmlhref.lo \
#	thmlplain.lo unicodertf.lo \
#	latin1utf8.lo latin1utf16.lo \
#	utf8utf16.lo utf16utf8.lo \
#	scsuutf8.lo utf8html.lo \
#	utf8latin1.lo thmlolb.lo \
#	utf8cantillation.lo \
#	utf8hebrewpoints.lo \
#	utf8greekaccents.lo \
#	swgenbook.lo rawgenbook.lo \
#	swtext.lo rawtext.lo ztext.lo \
#	rawgbf.lo swcom.lo rawcom.lo \
#	rawfiles.lo zcom.lo \
#	hrefcom.lo swld.lo rawld.lo \
#	rawld4.lo zld.lo flatapi.lo
##am_libsword_la_OBJECTS = swkey.lo \
##	listkey.lo versekey.lo \
##	strkey.lo treekey.lo \
##	treekeyidx.lo Greek2Greek.lo \
##	utilstr.lo unixstr.lo \
##	swunicod.lo swversion.lo \
##	regex.lo roman.lo \
##	swconfig.lo swmgr.lo \
##	swfiltermgr.lo encfiltmgr.lo \
##	markupfiltmgr.lo filemgr.lo \
##	swlocale.lo localemgr.lo \
##	swdisp.lo swlog.lo \
##	swmodule.lo rawstr.lo \
##	rawstr4.lo swcomprs.lo \
##	lzsscomprs.lo rawverse.lo \
##	swcipher.lo zverse.lo \
##	zstr.lo entriesblk.lo \
##	sapphire.lo swbasicfilter.lo \
##	gbfhtml.lo gbfhtmlhref.lo \
##	gbfplain.lo gbfrtf.lo \
##	plainhtml.lo rwphtml.lo \
##	rwprtf.lo cipherfil.lo \
##	rtfhtml.lo gbfstrongs.lo \
##	gbffootnotes.lo \
##	gbfheadings.lo gbfmorph.lo \
##	plainfootnotes.lo \
##	thmlstrongs.lo \
##	thmlfootnotes.lo \
##	thmlheadings.lo thmlmorph.lo \
##	thmllemma.lo thmlscripref.lo \
##	thmlvariants.lo gbfthml.lo \
##	thmlgbf.lo thmlrtf.lo \
##	thmlhtml.lo thmlhtmlhref.lo \
##	thmlplain.lo unicodertf.lo \
##	latin1utf8.lo latin1utf16.lo \
##	utf8utf16.lo utf16utf8.lo \
##	scsuutf8.lo utf8html.lo \
##	utf8latin1.lo thmlolb.lo \
##	utf8cantillation.lo \
##	utf8hebrewpoints.lo \
##	utf8greekaccents.lo \
##	swgenbook.lo rawgenbook.lo \
##	swtext.lo rawtext.lo \
##	ztext.lo rawgbf.lo swcom.lo \
##	rawcom.lo rawfiles.lo \
##	zcom.lo hrefcom.lo swld.lo \
##	rawld.lo rawld4.lo zld.lo \
##	flatapi.lo
#am_libsword_la_OBJECTS = swkey.lo \
#	listkey.lo versekey.lo \
#	strkey.lo treekey.lo \
#	treekeyidx.lo Greek2Greek.lo \
#	utilstr.lo unixstr.lo \
#	swunicod.lo swversion.lo \
#	roman.lo swconfig.lo swmgr.lo \
#	swfiltermgr.lo encfiltmgr.lo \
#	markupfiltmgr.lo filemgr.lo \
#	swlocale.lo localemgr.lo \
#	swdisp.lo swlog.lo \
#	swmodule.lo rawstr.lo \
#	rawstr4.lo swcomprs.lo \
#	lzsscomprs.lo zipcomprs.lo \
#	rawverse.lo swcipher.lo \
#	zverse.lo zstr.lo \
#	entriesblk.lo sapphire.lo \
#	swbasicfilter.lo gbfhtml.lo \
#	gbfhtmlhref.lo gbfplain.lo \
#	gbfrtf.lo plainhtml.lo \
#	rwphtml.lo rwprtf.lo \
#	cipherfil.lo rtfhtml.lo \
#	gbfstrongs.lo gbffootnotes.lo \
#	gbfheadings.lo gbfmorph.lo \
#	plainfootnotes.lo \
#	thmlstrongs.lo \
#	thmlfootnotes.lo \
#	thmlheadings.lo thmlmorph.lo \
#	thmllemma.lo thmlscripref.lo \
#	thmlvariants.lo gbfthml.lo \
#	thmlgbf.lo thmlrtf.lo \
#	thmlhtml.lo thmlhtmlhref.lo \
#	thmlplain.lo unicodertf.lo \
#	latin1utf8.lo latin1utf16.lo \
#	utf8utf16.lo utf16utf8.lo \
#	scsuutf8.lo utf8html.lo \
#	utf8latin1.lo thmlolb.lo \
#	utf8transliterator.lo \
#	utf8nfc.lo utf8nfkd.lo \
#	utf8arshaping.lo \
#	utf8bidireorder.lo \
#	utf8cantillation.lo \
#	utf8hebrewpoints.lo \
#	utf8greekaccents.lo \
#	swgenbook.lo rawgenbook.lo \
#	swtext.lo rawtext.lo ztext.lo \
#	rawgbf.lo swcom.lo rawcom.lo \
#	rawfiles.lo zcom.lo \
#	hrefcom.lo swld.lo rawld.lo \
#	rawld4.lo zld.lo flatapi.lo
##am_libsword_la_OBJECTS = swkey.lo \
##	listkey.lo versekey.lo \
##	strkey.lo treekey.lo \
##	treekeyidx.lo Greek2Greek.lo \
##	utilstr.lo unixstr.lo \
##	swunicod.lo swversion.lo \
##	roman.lo swconfig.lo \
##	swmgr.lo swfiltermgr.lo \
##	encfiltmgr.lo \
##	markupfiltmgr.lo filemgr.lo \
##	swlocale.lo localemgr.lo \
##	swdisp.lo swlog.lo \
##	swmodule.lo rawstr.lo \
##	rawstr4.lo swcomprs.lo \
##	lzsscomprs.lo rawverse.lo \
##	swcipher.lo zverse.lo \
##	zstr.lo entriesblk.lo \
##	sapphire.lo swbasicfilter.lo \
##	gbfhtml.lo gbfhtmlhref.lo \
##	gbfplain.lo gbfrtf.lo \
##	plainhtml.lo rwphtml.lo \
##	rwprtf.lo cipherfil.lo \
##	rtfhtml.lo gbfstrongs.lo \
##	gbffootnotes.lo \
##	gbfheadings.lo gbfmorph.lo \
##	plainfootnotes.lo \
##	thmlstrongs.lo \
##	thmlfootnotes.lo \
##	thmlheadings.lo thmlmorph.lo \
##	thmllemma.lo thmlscripref.lo \
##	thmlvariants.lo gbfthml.lo \
##	thmlgbf.lo thmlrtf.lo \
##	thmlhtml.lo thmlhtmlhref.lo \
##	thmlplain.lo unicodertf.lo \
##	latin1utf8.lo latin1utf16.lo \
##	utf8utf16.lo utf16utf8.lo \
##	scsuutf8.lo utf8html.lo \
##	utf8latin1.lo thmlolb.lo \
##	utf8transliterator.lo \
##	utf8nfc.lo utf8nfkd.lo \
##	utf8arshaping.lo \
##	utf8bidireorder.lo \
##	utf8cantillation.lo \
##	utf8hebrewpoints.lo \
##	utf8greekaccents.lo \
##	swgenbook.lo rawgenbook.lo \
##	swtext.lo rawtext.lo \
##	ztext.lo rawgbf.lo swcom.lo \
##	rawcom.lo rawfiles.lo \
##	zcom.lo hrefcom.lo swld.lo \
##	rawld.lo rawld4.lo zld.lo \
##	flatapi.lo
##am_libsword_la_OBJECTS = swkey.lo \
##	listkey.lo versekey.lo \
##	strkey.lo treekey.lo \
##	treekeyidx.lo Greek2Greek.lo \
##	utilstr.lo unixstr.lo \
##	swunicod.lo swversion.lo \
##	regex.lo roman.lo swconfig.lo \
##	swmgr.lo swfiltermgr.lo \
##	encfiltmgr.lo markupfiltmgr.lo \
##	filemgr.lo swlocale.lo \
##	localemgr.lo swdisp.lo \
##	swlog.lo swmodule.lo rawstr.lo \
##	rawstr4.lo swcomprs.lo \
##	lzsscomprs.lo zipcomprs.lo \
##	rawverse.lo swcipher.lo \
##	zverse.lo zstr.lo \
##	entriesblk.lo sapphire.lo \
##	swbasicfilter.lo gbfhtml.lo \
##	gbfhtmlhref.lo gbfplain.lo \
##	gbfrtf.lo plainhtml.lo \
##	rwphtml.lo rwprtf.lo \
##	cipherfil.lo rtfhtml.lo \
##	gbfstrongs.lo gbffootnotes.lo \
##	gbfheadings.lo gbfmorph.lo \
##	plainfootnotes.lo \
##	thmlstrongs.lo \
##	thmlfootnotes.lo \
##	thmlheadings.lo thmlmorph.lo \
##	thmllemma.lo thmlscripref.lo \
##	thmlvariants.lo gbfthml.lo \
##	thmlgbf.lo thmlrtf.lo \
##	thmlhtml.lo thmlhtmlhref.lo \
##	thmlplain.lo unicodertf.lo \
##	latin1utf8.lo latin1utf16.lo \
##	utf8utf16.lo utf16utf8.lo \
##	scsuutf8.lo utf8html.lo \
##	utf8latin1.lo thmlolb.lo \
##	utf8transliterator.lo \
##	utf8nfc.lo utf8nfkd.lo \
##	utf8arshaping.lo \
##	utf8bidireorder.lo \
##	utf8cantillation.lo \
##	utf8hebrewpoints.lo \
##	utf8greekaccents.lo \
##	swgenbook.lo rawgenbook.lo \
##	swtext.lo rawtext.lo ztext.lo \
##	rawgbf.lo swcom.lo rawcom.lo \
##	rawfiles.lo zcom.lo hrefcom.lo \
##	swld.lo rawld.lo rawld4.lo \
##	zld.lo flatapi.lo
###am_libsword_la_OBJECTS = swkey.lo \
###	listkey.lo versekey.lo \
###	strkey.lo treekey.lo \
###	treekeyidx.lo Greek2Greek.lo \
###	utilstr.lo unixstr.lo \
###	swunicod.lo swversion.lo \
###	regex.lo roman.lo swconfig.lo \
###	swmgr.lo swfiltermgr.lo \
###	encfiltmgr.lo \
###	markupfiltmgr.lo filemgr.lo \
###	swlocale.lo localemgr.lo \
###	swdisp.lo swlog.lo \
###	swmodule.lo rawstr.lo \
###	rawstr4.lo swcomprs.lo \
###	lzsscomprs.lo rawverse.lo \
###	swcipher.lo zverse.lo zstr.lo \
###	entriesblk.lo sapphire.lo \
###	swbasicfilter.lo gbfhtml.lo \
###	gbfhtmlhref.lo gbfplain.lo \
###	gbfrtf.lo plainhtml.lo \
###	rwphtml.lo rwprtf.lo \
###	cipherfil.lo rtfhtml.lo \
###	gbfstrongs.lo gbffootnotes.lo \
###	gbfheadings.lo gbfmorph.lo \
###	plainfootnotes.lo \
###	thmlstrongs.lo \
###	thmlfootnotes.lo \
###	thmlheadings.lo thmlmorph.lo \
###	thmllemma.lo thmlscripref.lo \
###	thmlvariants.lo gbfthml.lo \
###	thmlgbf.lo thmlrtf.lo \
###	thmlhtml.lo thmlhtmlhref.lo \
###	thmlplain.lo unicodertf.lo \
###	latin1utf8.lo latin1utf16.lo \
###	utf8utf16.lo utf16utf8.lo \
###	scsuutf8.lo utf8html.lo \
###	utf8latin1.lo thmlolb.lo \
###	utf8transliterator.lo \
###	utf8nfc.lo utf8nfkd.lo \
###	utf8arshaping.lo \
###	utf8bidireorder.lo \
###	utf8cantillation.lo \
###	utf8hebrewpoints.lo \
###	utf8greekaccents.lo \
###	swgenbook.lo rawgenbook.lo \
###	swtext.lo rawtext.lo ztext.lo \
###	rawgbf.lo swcom.lo rawcom.lo \
###	rawfiles.lo zcom.lo \
###	hrefcom.lo swld.lo rawld.lo \
###	rawld4.lo zld.lo flatapi.lo
libsword_la_OBJECTS = $(am_libsword_la_OBJECTS)
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I$(top_builddir)
CPPFLAGS = 
LDFLAGS = 
LIBS = -lstdc++ -lz 
depcomp = $(SHELL) $(top_srcdir)/depcomp
DEP_FILES = $(DEPDIR)/Greek2Greek.Plo \
	$(DEPDIR)/cipherfil.Plo $(DEPDIR)/encfiltmgr.Plo \
	$(DEPDIR)/entriesblk.Plo $(DEPDIR)/filemgr.Plo \
	$(DEPDIR)/flatapi.Plo $(DEPDIR)/gbffootnotes.Plo \
	$(DEPDIR)/gbfheadings.Plo $(DEPDIR)/gbfhtml.Plo \
	$(DEPDIR)/gbfhtmlhref.Plo $(DEPDIR)/gbfmorph.Plo \
	$(DEPDIR)/gbfplain.Plo $(DEPDIR)/gbfrtf.Plo \
	$(DEPDIR)/gbfstrongs.Plo $(DEPDIR)/gbfthml.Plo \
	$(DEPDIR)/hrefcom.Plo $(DEPDIR)/latin1utf16.Plo \
	$(DEPDIR)/latin1utf8.Plo $(DEPDIR)/listkey.Plo \
	$(DEPDIR)/localemgr.Plo $(DEPDIR)/lzsscomprs.Plo \
	$(DEPDIR)/markupfiltmgr.Plo \
	$(DEPDIR)/plainfootnotes.Plo \
	$(DEPDIR)/plainhtml.Plo $(DEPDIR)/rawcom.Plo \
	$(DEPDIR)/rawfiles.Plo $(DEPDIR)/rawgbf.Plo \
	$(DEPDIR)/rawgenbook.Plo $(DEPDIR)/rawld.Plo \
	$(DEPDIR)/rawld4.Plo $(DEPDIR)/rawstr.Plo \
	$(DEPDIR)/rawstr4.Plo $(DEPDIR)/rawtext.Plo \
	$(DEPDIR)/rawverse.Plo $(DEPDIR)/regex.Plo \
	$(DEPDIR)/roman.Plo $(DEPDIR)/rtfhtml.Plo \
	$(DEPDIR)/rwphtml.Plo $(DEPDIR)/rwprtf.Plo \
	$(DEPDIR)/sapphire.Plo $(DEPDIR)/scsuutf8.Plo \
	$(DEPDIR)/strkey.Plo $(DEPDIR)/swbasicfilter.Plo \
	$(DEPDIR)/swcipher.Plo $(DEPDIR)/swcom.Plo \
	$(DEPDIR)/swcomprs.Plo $(DEPDIR)/swconfig.Plo \
	$(DEPDIR)/swdisp.Plo $(DEPDIR)/swfiltermgr.Plo \
	$(DEPDIR)/swgenbook.Plo $(DEPDIR)/swkey.Plo \
	$(DEPDIR)/swld.Plo $(DEPDIR)/swlocale.Plo \
	$(DEPDIR)/swlog.Plo $(DEPDIR)/swmgr.Plo \
	$(DEPDIR)/swmodule.Plo $(DEPDIR)/swtext.Plo \
	$(DEPDIR)/swunicod.Plo $(DEPDIR)/swversion.Plo \
	$(DEPDIR)/thmlfootnotes.Plo $(DEPDIR)/thmlgbf.Plo \
	$(DEPDIR)/thmlheadings.Plo $(DEPDIR)/thmlhtml.Plo \
	$(DEPDIR)/thmlhtmlhref.Plo $(DEPDIR)/thmllemma.Plo \
	$(DEPDIR)/thmlmorph.Plo $(DEPDIR)/thmlolb.Plo \
	$(DEPDIR)/thmlplain.Plo $(DEPDIR)/thmlrtf.Plo \
	$(DEPDIR)/thmlscripref.Plo \
	$(DEPDIR)/thmlstrongs.Plo \
	$(DEPDIR)/thmlvariants.Plo $(DEPDIR)/treekey.Plo \
	$(DEPDIR)/treekeyidx.Plo $(DEPDIR)/unicodertf.Plo \
	$(DEPDIR)/unixstr.Plo $(DEPDIR)/utf16utf8.Plo \
	$(DEPDIR)/utf8arshaping.Plo \
	$(DEPDIR)/utf8bidireorder.Plo \
	$(DEPDIR)/utf8cantillation.Plo \
	$(DEPDIR)/utf8greekaccents.Plo \
	$(DEPDIR)/utf8hebrewpoints.Plo \
	$(DEPDIR)/utf8html.Plo $(DEPDIR)/utf8latin1.Plo \
	$(DEPDIR)/utf8nfc.Plo $(DEPDIR)/utf8nfkd.Plo \
	$(DEPDIR)/utf8transliterator.Plo \
	$(DEPDIR)/utf8utf16.Plo $(DEPDIR)/utilstr.Plo \
	$(DEPDIR)/versekey.Plo $(DEPDIR)/zcom.Plo \
	$(DEPDIR)/zipcomprs.Plo $(DEPDIR)/zld.Plo \
	$(DEPDIR)/zstr.Plo $(DEPDIR)/ztext.Plo \
	$(DEPDIR)/zverse.Plo
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) \
	$(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
CFLAGS = -g -O0 -Werror
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
CXXFLAGS = -g -O0 -Werror
DIST_SOURCES = $(libsword_la_SOURCES)
HEADERS = $(pkginclude_HEADERS)

DIST_COMMON = README $(pkginclude_HEADERS) Makefile.am Makefile.in
SOURCES = $(libsword_la_SOURCES)

all: all-am

.SUFFIXES:
.SUFFIXES: .c .cpp .lo .o .obj

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool
$(srcdir)/Makefile.in: # Makefile.am $(srcdir)/../src/keys/Makefile.am $(srcdir)/../src/utilfuns/Makefile.am $(srcdir)/../src/mgr/Makefile.am $(srcdir)/../src/frontend/Makefile.am $(srcdir)/../src/modules/Makefile.am $(srcdir)/../src/modules/common/Makefile.am $(srcdir)/../src/modules/filters/Makefile.am $(srcdir)/../src/modules/genbook/Makefile.am $(srcdir)/../src/modules/genbook/rawgenbook/Makefile.am $(srcdir)/../src/modules/texts/Makefile.am $(srcdir)/../src/modules/texts/rawtext/Makefile.am $(srcdir)/../src/modules/texts/ztext/Makefile.am $(srcdir)/../src/modules/texts/rawgbf/Makefile.am $(srcdir)/../src/modules/comments/Makefile.am $(srcdir)/../src/modules/comments/rawcom/Makefile.am $(srcdir)/../src/modules/comments/rawfiles/Makefile.am $(srcdir)/../src/modules/comments/zcom/Makefile.am $(srcdir)/../src/modules/comments/hrefcom/Makefile.am $(srcdir)/../src/modules/lexdict/Makefile.am $(srcdir)/../src/modules/lexdict/rawld/Makefile.am $(srcdir)/../src/modules/lexdict/rawld4/Makefile.am $(srcdir)/../src/modules/lexdict/zld/Makefile.am $(srcdir)/../bindings/Makefile.am $(srcdir)/../include/Makefile.am $(top_srcdir)/configure.ac $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --foreign  lib/Makefile
Makefile: # $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status
install-libLTLIBRARIES: $(lib_LTLIBRARIES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	@list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(LIBTOOL) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(libdir)/$$p"; \
	    $(LIBTOOL) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done

uninstall-libLTLIBRARIES:
	@$(NORMAL_UNINSTALL)
	@list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  echo " $(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(libdir)/$$p"; \
	  $(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(libdir)/$$p; \
	done

clean-libLTLIBRARIES:
	-test -z "$(lib_LTLIBRARIES)" || rm -f $(lib_LTLIBRARIES)
swkey.lo: $(keysdir)/swkey.cpp
listkey.lo: $(keysdir)/listkey.cpp
versekey.lo: $(keysdir)/versekey.cpp
strkey.lo: $(keysdir)/strkey.cpp
treekey.lo: $(keysdir)/treekey.cpp
treekeyidx.lo: $(keysdir)/treekeyidx.cpp
Greek2Greek.lo: $(utilfunsdir)/Greek2Greek.cpp
utilstr.lo: $(utilfunsdir)/utilstr.cpp
unixstr.lo: $(utilfunsdir)/unixstr.cpp
swunicod.lo: $(utilfunsdir)/swunicod.cpp
swversion.lo: $(utilfunsdir)/swversion.cpp
roman.lo: $(utilfunsdir)/roman.c
swconfig.lo: $(mgrdir)/swconfig.cpp
swmgr.lo: $(mgrdir)/swmgr.cpp
swfiltermgr.lo: $(mgrdir)/swfiltermgr.cpp
encfiltmgr.lo: $(mgrdir)/encfiltmgr.cpp
markupfiltmgr.lo: $(mgrdir)/markupfiltmgr.cpp
filemgr.lo: $(mgrdir)/filemgr.cpp
swlocale.lo: $(mgrdir)/swlocale.cpp
localemgr.lo: $(mgrdir)/localemgr.cpp
swdisp.lo: $(frontenddir)/swdisp.cpp
swlog.lo: $(frontenddir)/swlog.cpp
swmodule.lo: $(modulesdir)/swmodule.cpp
rawstr.lo: $(commondir)/rawstr.cpp
rawstr4.lo: $(commondir)/rawstr4.cpp
swcomprs.lo: $(commondir)/swcomprs.cpp
lzsscomprs.lo: $(commondir)/lzsscomprs.cpp
zipcomprs.lo: $(commondir)/zipcomprs.cpp
rawverse.lo: $(commondir)/rawverse.cpp
swcipher.lo: $(commondir)/swcipher.cpp
zverse.lo: $(commondir)/zverse.cpp
zstr.lo: $(commondir)/zstr.cpp
entriesblk.lo: $(commondir)/entriesblk.cpp
sapphire.lo: $(commondir)/sapphire.cpp
swbasicfilter.lo: $(filtersdir)/swbasicfilter.cpp
gbfhtml.lo: $(filtersdir)/gbfhtml.cpp
gbfhtmlhref.lo: $(filtersdir)/gbfhtmlhref.cpp
gbfplain.lo: $(filtersdir)/gbfplain.cpp
gbfrtf.lo: $(filtersdir)/gbfrtf.cpp
plainhtml.lo: $(filtersdir)/plainhtml.cpp
rwphtml.lo: $(filtersdir)/rwphtml.cpp
rwprtf.lo: $(filtersdir)/rwprtf.cpp
cipherfil.lo: $(filtersdir)/cipherfil.cpp
rtfhtml.lo: $(filtersdir)/rtfhtml.cpp
gbfstrongs.lo: $(filtersdir)/gbfstrongs.cpp
gbffootnotes.lo: $(filtersdir)/gbffootnotes.cpp
gbfheadings.lo: $(filtersdir)/gbfheadings.cpp
gbfmorph.lo: $(filtersdir)/gbfmorph.cpp
plainfootnotes.lo: $(filtersdir)/plainfootnotes.cpp
thmlstrongs.lo: $(filtersdir)/thmlstrongs.cpp
thmlfootnotes.lo: $(filtersdir)/thmlfootnotes.cpp
thmlheadings.lo: $(filtersdir)/thmlheadings.cpp
thmlmorph.lo: $(filtersdir)/thmlmorph.cpp
thmllemma.lo: $(filtersdir)/thmllemma.cpp
thmlscripref.lo: $(filtersdir)/thmlscripref.cpp
thmlvariants.lo: $(filtersdir)/thmlvariants.cpp
gbfthml.lo: $(filtersdir)/gbfthml.cpp
thmlgbf.lo: $(filtersdir)/thmlgbf.cpp
thmlrtf.lo: $(filtersdir)/thmlrtf.cpp
thmlhtml.lo: $(filtersdir)/thmlhtml.cpp
thmlhtmlhref.lo: $(filtersdir)/thmlhtmlhref.cpp
thmlplain.lo: $(filtersdir)/thmlplain.cpp
unicodertf.lo: $(filtersdir)/unicodertf.cpp
latin1utf8.lo: $(filtersdir)/latin1utf8.cpp
latin1utf16.lo: $(filtersdir)/latin1utf16.cpp
utf8utf16.lo: $(filtersdir)/utf8utf16.cpp
utf16utf8.lo: $(filtersdir)/utf16utf8.cpp
scsuutf8.lo: $(filtersdir)/scsuutf8.cpp
utf8html.lo: $(filtersdir)/utf8html.cpp
utf8latin1.lo: $(filtersdir)/utf8latin1.cpp
thmlolb.lo: $(filtersdir)/thmlolb.cpp
utf8cantillation.lo: $(filtersdir)/utf8cantillation.cpp
utf8hebrewpoints.lo: $(filtersdir)/utf8hebrewpoints.cpp
utf8greekaccents.lo: $(filtersdir)/utf8greekaccents.cpp
swgenbook.lo: $(genbookdir)/swgenbook.cpp
rawgenbook.lo: $(rawgenbookdir)/rawgenbook.cpp
swtext.lo: $(textsdir)/swtext.cpp
rawtext.lo: $(rawtextdir)/rawtext.cpp
ztext.lo: $(ztextdir)/ztext.cpp
rawgbf.lo: $(rawgbfdir)/rawgbf.cpp
swcom.lo: $(commentsdir)/swcom.cpp
rawcom.lo: $(rawcomdir)/rawcom.cpp
rawfiles.lo: $(rawfilesdir)/rawfiles.cpp
zcom.lo: $(zcomdir)/zcom.cpp
hrefcom.lo: $(hrefcomdir)/hrefcom.cpp
swld.lo: $(lexdictdir)/swld.cpp
rawld.lo: $(rawlddir)/rawld.cpp
rawld4.lo: $(rawld4dir)/rawld4.cpp
zld.lo: $(zlddir)/zld.cpp
flatapi.lo: $(bindingsdir)/flatapi.cpp
regex.lo: $(utilfunsdir)/regex.c
utf8transliterator.lo: $(filtersdir)/utf8transliterator.cpp
utf8nfc.lo: $(filtersdir)/utf8nfc.cpp
utf8nfkd.lo: $(filtersdir)/utf8nfkd.cpp
utf8arshaping.lo: $(filtersdir)/utf8arshaping.cpp
utf8bidireorder.lo: $(filtersdir)/utf8bidireorder.cpp
libsword.la: $(libsword_la_OBJECTS) $(libsword_la_DEPENDENCIES) 
	$(CXXLINK) -rpath $(libdir) $(libsword_la_LDFLAGS) $(libsword_la_OBJECTS) $(libsword_la_LIBADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

include $(DEPDIR)/Greek2Greek.Plo
include $(DEPDIR)/cipherfil.Plo
include $(DEPDIR)/encfiltmgr.Plo
include $(DEPDIR)/entriesblk.Plo
include $(DEPDIR)/filemgr.Plo
include $(DEPDIR)/flatapi.Plo
include $(DEPDIR)/gbffootnotes.Plo
include $(DEPDIR)/gbfheadings.Plo
include $(DEPDIR)/gbfhtml.Plo
include $(DEPDIR)/gbfhtmlhref.Plo
include $(DEPDIR)/gbfmorph.Plo
include $(DEPDIR)/gbfplain.Plo
include $(DEPDIR)/gbfrtf.Plo
include $(DEPDIR)/gbfstrongs.Plo
include $(DEPDIR)/gbfthml.Plo
include $(DEPDIR)/hrefcom.Plo
include $(DEPDIR)/latin1utf16.Plo
include $(DEPDIR)/latin1utf8.Plo
include $(DEPDIR)/listkey.Plo
include $(DEPDIR)/localemgr.Plo
include $(DEPDIR)/lzsscomprs.Plo
include $(DEPDIR)/markupfiltmgr.Plo
include $(DEPDIR)/plainfootnotes.Plo
include $(DEPDIR)/plainhtml.Plo
include $(DEPDIR)/rawcom.Plo
include $(DEPDIR)/rawfiles.Plo
include $(DEPDIR)/rawgbf.Plo
include $(DEPDIR)/rawgenbook.Plo
include $(DEPDIR)/rawld.Plo
include $(DEPDIR)/rawld4.Plo
include $(DEPDIR)/rawstr.Plo
include $(DEPDIR)/rawstr4.Plo
include $(DEPDIR)/rawtext.Plo
include $(DEPDIR)/rawverse.Plo
include $(DEPDIR)/regex.Plo
include $(DEPDIR)/roman.Plo
include $(DEPDIR)/rtfhtml.Plo
include $(DEPDIR)/rwphtml.Plo
include $(DEPDIR)/rwprtf.Plo
include $(DEPDIR)/sapphire.Plo
include $(DEPDIR)/scsuutf8.Plo
include $(DEPDIR)/strkey.Plo
include $(DEPDIR)/swbasicfilter.Plo
include $(DEPDIR)/swcipher.Plo
include $(DEPDIR)/swcom.Plo
include $(DEPDIR)/swcomprs.Plo
include $(DEPDIR)/swconfig.Plo
include $(DEPDIR)/swdisp.Plo
include $(DEPDIR)/swfiltermgr.Plo
include $(DEPDIR)/swgenbook.Plo
include $(DEPDIR)/swkey.Plo
include $(DEPDIR)/swld.Plo
include $(DEPDIR)/swlocale.Plo
include $(DEPDIR)/swlog.Plo
include $(DEPDIR)/swmgr.Plo
include $(DEPDIR)/swmodule.Plo
include $(DEPDIR)/swtext.Plo
include $(DEPDIR)/swunicod.Plo
include $(DEPDIR)/swversion.Plo
include $(DEPDIR)/thmlfootnotes.Plo
include $(DEPDIR)/thmlgbf.Plo
include $(DEPDIR)/thmlheadings.Plo
include $(DEPDIR)/thmlhtml.Plo
include $(DEPDIR)/thmlhtmlhref.Plo
include $(DEPDIR)/thmllemma.Plo
include $(DEPDIR)/thmlmorph.Plo
include $(DEPDIR)/thmlolb.Plo
include $(DEPDIR)/thmlplain.Plo
include $(DEPDIR)/thmlrtf.Plo
include $(DEPDIR)/thmlscripref.Plo
include $(DEPDIR)/thmlstrongs.Plo
include $(DEPDIR)/thmlvariants.Plo
include $(DEPDIR)/treekey.Plo
include $(DEPDIR)/treekeyidx.Plo
include $(DEPDIR)/unicodertf.Plo
include $(DEPDIR)/unixstr.Plo
include $(DEPDIR)/utf16utf8.Plo
include $(DEPDIR)/utf8arshaping.Plo
include $(DEPDIR)/utf8bidireorder.Plo
include $(DEPDIR)/utf8cantillation.Plo
include $(DEPDIR)/utf8greekaccents.Plo
include $(DEPDIR)/utf8hebrewpoints.Plo
include $(DEPDIR)/utf8html.Plo
include $(DEPDIR)/utf8latin1.Plo
include $(DEPDIR)/utf8nfc.Plo
include $(DEPDIR)/utf8nfkd.Plo
include $(DEPDIR)/utf8transliterator.Plo
include $(DEPDIR)/utf8utf16.Plo
include $(DEPDIR)/utilstr.Plo
include $(DEPDIR)/versekey.Plo
include $(DEPDIR)/zcom.Plo
include $(DEPDIR)/zipcomprs.Plo
include $(DEPDIR)/zld.Plo
include $(DEPDIR)/zstr.Plo
include $(DEPDIR)/ztext.Plo
include $(DEPDIR)/zverse.Plo

distclean-depend:
	-rm -rf $(DEPDIR)

.c.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(COMPILE) -c `test -f $< || echo '$(srcdir)/'`$<

.c.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(COMPILE) -c `cygpath -w $<`

.c.lo:
	source='$<' object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CCDEPMODE) $(depcomp) \
	$(LTCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

roman.o: $(utilfunsdir)/roman.c
	source='$(utilfunsdir)/roman.c' object='roman.o' libtool=no \
	depfile='$(DEPDIR)/roman.Po' tmpdepfile='$(DEPDIR)/roman.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o roman.o `test -f $(utilfunsdir)/roman.c || echo '$(srcdir)/'`$(utilfunsdir)/roman.c

roman.obj: $(utilfunsdir)/roman.c
	source='$(utilfunsdir)/roman.c' object='roman.obj' libtool=no \
	depfile='$(DEPDIR)/roman.Po' tmpdepfile='$(DEPDIR)/roman.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o roman.obj `cygpath -w $(utilfunsdir)/roman.c`

roman.lo: $(utilfunsdir)/roman.c
	source='$(utilfunsdir)/roman.c' object='roman.lo' libtool=yes \
	depfile='$(DEPDIR)/roman.Plo' tmpdepfile='$(DEPDIR)/roman.TPlo' \
	$(CCDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o roman.lo `test -f $(utilfunsdir)/roman.c || echo '$(srcdir)/'`$(utilfunsdir)/roman.c

regex.o: $(utilfunsdir)/regex.c
	source='$(utilfunsdir)/regex.c' object='regex.o' libtool=no \
	depfile='$(DEPDIR)/regex.Po' tmpdepfile='$(DEPDIR)/regex.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o regex.o `test -f $(utilfunsdir)/regex.c || echo '$(srcdir)/'`$(utilfunsdir)/regex.c

regex.obj: $(utilfunsdir)/regex.c
	source='$(utilfunsdir)/regex.c' object='regex.obj' libtool=no \
	depfile='$(DEPDIR)/regex.Po' tmpdepfile='$(DEPDIR)/regex.TPo' \
	$(CCDEPMODE) $(depcomp) \
	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o regex.obj `cygpath -w $(utilfunsdir)/regex.c`

regex.lo: $(utilfunsdir)/regex.c
	source='$(utilfunsdir)/regex.c' object='regex.lo' libtool=yes \
	depfile='$(DEPDIR)/regex.Plo' tmpdepfile='$(DEPDIR)/regex.TPlo' \
	$(CCDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o regex.lo `test -f $(utilfunsdir)/regex.c || echo '$(srcdir)/'`$(utilfunsdir)/regex.c
CCDEPMODE = depmode=gcc3

.cpp.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

.cpp.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `cygpath -w $<`

.cpp.lo:
	source='$<' object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

swkey.o: $(keysdir)/swkey.cpp
	source='$(keysdir)/swkey.cpp' object='swkey.o' libtool=no \
	depfile='$(DEPDIR)/swkey.Po' tmpdepfile='$(DEPDIR)/swkey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swkey.o `test -f $(keysdir)/swkey.cpp || echo '$(srcdir)/'`$(keysdir)/swkey.cpp

swkey.obj: $(keysdir)/swkey.cpp
	source='$(keysdir)/swkey.cpp' object='swkey.obj' libtool=no \
	depfile='$(DEPDIR)/swkey.Po' tmpdepfile='$(DEPDIR)/swkey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swkey.obj `cygpath -w $(keysdir)/swkey.cpp`

swkey.lo: $(keysdir)/swkey.cpp
	source='$(keysdir)/swkey.cpp' object='swkey.lo' libtool=yes \
	depfile='$(DEPDIR)/swkey.Plo' tmpdepfile='$(DEPDIR)/swkey.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swkey.lo `test -f $(keysdir)/swkey.cpp || echo '$(srcdir)/'`$(keysdir)/swkey.cpp

listkey.o: $(keysdir)/listkey.cpp
	source='$(keysdir)/listkey.cpp' object='listkey.o' libtool=no \
	depfile='$(DEPDIR)/listkey.Po' tmpdepfile='$(DEPDIR)/listkey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o listkey.o `test -f $(keysdir)/listkey.cpp || echo '$(srcdir)/'`$(keysdir)/listkey.cpp

listkey.obj: $(keysdir)/listkey.cpp
	source='$(keysdir)/listkey.cpp' object='listkey.obj' libtool=no \
	depfile='$(DEPDIR)/listkey.Po' tmpdepfile='$(DEPDIR)/listkey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o listkey.obj `cygpath -w $(keysdir)/listkey.cpp`

listkey.lo: $(keysdir)/listkey.cpp
	source='$(keysdir)/listkey.cpp' object='listkey.lo' libtool=yes \
	depfile='$(DEPDIR)/listkey.Plo' tmpdepfile='$(DEPDIR)/listkey.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o listkey.lo `test -f $(keysdir)/listkey.cpp || echo '$(srcdir)/'`$(keysdir)/listkey.cpp

versekey.o: $(keysdir)/versekey.cpp
	source='$(keysdir)/versekey.cpp' object='versekey.o' libtool=no \
	depfile='$(DEPDIR)/versekey.Po' tmpdepfile='$(DEPDIR)/versekey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o versekey.o `test -f $(keysdir)/versekey.cpp || echo '$(srcdir)/'`$(keysdir)/versekey.cpp

versekey.obj: $(keysdir)/versekey.cpp
	source='$(keysdir)/versekey.cpp' object='versekey.obj' libtool=no \
	depfile='$(DEPDIR)/versekey.Po' tmpdepfile='$(DEPDIR)/versekey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o versekey.obj `cygpath -w $(keysdir)/versekey.cpp`

versekey.lo: $(keysdir)/versekey.cpp
	source='$(keysdir)/versekey.cpp' object='versekey.lo' libtool=yes \
	depfile='$(DEPDIR)/versekey.Plo' tmpdepfile='$(DEPDIR)/versekey.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o versekey.lo `test -f $(keysdir)/versekey.cpp || echo '$(srcdir)/'`$(keysdir)/versekey.cpp

strkey.o: $(keysdir)/strkey.cpp
	source='$(keysdir)/strkey.cpp' object='strkey.o' libtool=no \
	depfile='$(DEPDIR)/strkey.Po' tmpdepfile='$(DEPDIR)/strkey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o strkey.o `test -f $(keysdir)/strkey.cpp || echo '$(srcdir)/'`$(keysdir)/strkey.cpp

strkey.obj: $(keysdir)/strkey.cpp
	source='$(keysdir)/strkey.cpp' object='strkey.obj' libtool=no \
	depfile='$(DEPDIR)/strkey.Po' tmpdepfile='$(DEPDIR)/strkey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o strkey.obj `cygpath -w $(keysdir)/strkey.cpp`

strkey.lo: $(keysdir)/strkey.cpp
	source='$(keysdir)/strkey.cpp' object='strkey.lo' libtool=yes \
	depfile='$(DEPDIR)/strkey.Plo' tmpdepfile='$(DEPDIR)/strkey.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o strkey.lo `test -f $(keysdir)/strkey.cpp || echo '$(srcdir)/'`$(keysdir)/strkey.cpp

treekey.o: $(keysdir)/treekey.cpp
	source='$(keysdir)/treekey.cpp' object='treekey.o' libtool=no \
	depfile='$(DEPDIR)/treekey.Po' tmpdepfile='$(DEPDIR)/treekey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o treekey.o `test -f $(keysdir)/treekey.cpp || echo '$(srcdir)/'`$(keysdir)/treekey.cpp

treekey.obj: $(keysdir)/treekey.cpp
	source='$(keysdir)/treekey.cpp' object='treekey.obj' libtool=no \
	depfile='$(DEPDIR)/treekey.Po' tmpdepfile='$(DEPDIR)/treekey.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o treekey.obj `cygpath -w $(keysdir)/treekey.cpp`

treekey.lo: $(keysdir)/treekey.cpp
	source='$(keysdir)/treekey.cpp' object='treekey.lo' libtool=yes \
	depfile='$(DEPDIR)/treekey.Plo' tmpdepfile='$(DEPDIR)/treekey.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o treekey.lo `test -f $(keysdir)/treekey.cpp || echo '$(srcdir)/'`$(keysdir)/treekey.cpp

treekeyidx.o: $(keysdir)/treekeyidx.cpp
	source='$(keysdir)/treekeyidx.cpp' object='treekeyidx.o' libtool=no \
	depfile='$(DEPDIR)/treekeyidx.Po' tmpdepfile='$(DEPDIR)/treekeyidx.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o treekeyidx.o `test -f $(keysdir)/treekeyidx.cpp || echo '$(srcdir)/'`$(keysdir)/treekeyidx.cpp

treekeyidx.obj: $(keysdir)/treekeyidx.cpp
	source='$(keysdir)/treekeyidx.cpp' object='treekeyidx.obj' libtool=no \
	depfile='$(DEPDIR)/treekeyidx.Po' tmpdepfile='$(DEPDIR)/treekeyidx.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o treekeyidx.obj `cygpath -w $(keysdir)/treekeyidx.cpp`

treekeyidx.lo: $(keysdir)/treekeyidx.cpp
	source='$(keysdir)/treekeyidx.cpp' object='treekeyidx.lo' libtool=yes \
	depfile='$(DEPDIR)/treekeyidx.Plo' tmpdepfile='$(DEPDIR)/treekeyidx.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o treekeyidx.lo `test -f $(keysdir)/treekeyidx.cpp || echo '$(srcdir)/'`$(keysdir)/treekeyidx.cpp

Greek2Greek.o: $(utilfunsdir)/Greek2Greek.cpp
	source='$(utilfunsdir)/Greek2Greek.cpp' object='Greek2Greek.o' libtool=no \
	depfile='$(DEPDIR)/Greek2Greek.Po' tmpdepfile='$(DEPDIR)/Greek2Greek.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o Greek2Greek.o `test -f $(utilfunsdir)/Greek2Greek.cpp || echo '$(srcdir)/'`$(utilfunsdir)/Greek2Greek.cpp

Greek2Greek.obj: $(utilfunsdir)/Greek2Greek.cpp
	source='$(utilfunsdir)/Greek2Greek.cpp' object='Greek2Greek.obj' libtool=no \
	depfile='$(DEPDIR)/Greek2Greek.Po' tmpdepfile='$(DEPDIR)/Greek2Greek.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o Greek2Greek.obj `cygpath -w $(utilfunsdir)/Greek2Greek.cpp`

Greek2Greek.lo: $(utilfunsdir)/Greek2Greek.cpp
	source='$(utilfunsdir)/Greek2Greek.cpp' object='Greek2Greek.lo' libtool=yes \
	depfile='$(DEPDIR)/Greek2Greek.Plo' tmpdepfile='$(DEPDIR)/Greek2Greek.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o Greek2Greek.lo `test -f $(utilfunsdir)/Greek2Greek.cpp || echo '$(srcdir)/'`$(utilfunsdir)/Greek2Greek.cpp

utilstr.o: $(utilfunsdir)/utilstr.cpp
	source='$(utilfunsdir)/utilstr.cpp' object='utilstr.o' libtool=no \
	depfile='$(DEPDIR)/utilstr.Po' tmpdepfile='$(DEPDIR)/utilstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utilstr.o `test -f $(utilfunsdir)/utilstr.cpp || echo '$(srcdir)/'`$(utilfunsdir)/utilstr.cpp

utilstr.obj: $(utilfunsdir)/utilstr.cpp
	source='$(utilfunsdir)/utilstr.cpp' object='utilstr.obj' libtool=no \
	depfile='$(DEPDIR)/utilstr.Po' tmpdepfile='$(DEPDIR)/utilstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utilstr.obj `cygpath -w $(utilfunsdir)/utilstr.cpp`

utilstr.lo: $(utilfunsdir)/utilstr.cpp
	source='$(utilfunsdir)/utilstr.cpp' object='utilstr.lo' libtool=yes \
	depfile='$(DEPDIR)/utilstr.Plo' tmpdepfile='$(DEPDIR)/utilstr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utilstr.lo `test -f $(utilfunsdir)/utilstr.cpp || echo '$(srcdir)/'`$(utilfunsdir)/utilstr.cpp

unixstr.o: $(utilfunsdir)/unixstr.cpp
	source='$(utilfunsdir)/unixstr.cpp' object='unixstr.o' libtool=no \
	depfile='$(DEPDIR)/unixstr.Po' tmpdepfile='$(DEPDIR)/unixstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o unixstr.o `test -f $(utilfunsdir)/unixstr.cpp || echo '$(srcdir)/'`$(utilfunsdir)/unixstr.cpp

unixstr.obj: $(utilfunsdir)/unixstr.cpp
	source='$(utilfunsdir)/unixstr.cpp' object='unixstr.obj' libtool=no \
	depfile='$(DEPDIR)/unixstr.Po' tmpdepfile='$(DEPDIR)/unixstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o unixstr.obj `cygpath -w $(utilfunsdir)/unixstr.cpp`

unixstr.lo: $(utilfunsdir)/unixstr.cpp
	source='$(utilfunsdir)/unixstr.cpp' object='unixstr.lo' libtool=yes \
	depfile='$(DEPDIR)/unixstr.Plo' tmpdepfile='$(DEPDIR)/unixstr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o unixstr.lo `test -f $(utilfunsdir)/unixstr.cpp || echo '$(srcdir)/'`$(utilfunsdir)/unixstr.cpp

swunicod.o: $(utilfunsdir)/swunicod.cpp
	source='$(utilfunsdir)/swunicod.cpp' object='swunicod.o' libtool=no \
	depfile='$(DEPDIR)/swunicod.Po' tmpdepfile='$(DEPDIR)/swunicod.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swunicod.o `test -f $(utilfunsdir)/swunicod.cpp || echo '$(srcdir)/'`$(utilfunsdir)/swunicod.cpp

swunicod.obj: $(utilfunsdir)/swunicod.cpp
	source='$(utilfunsdir)/swunicod.cpp' object='swunicod.obj' libtool=no \
	depfile='$(DEPDIR)/swunicod.Po' tmpdepfile='$(DEPDIR)/swunicod.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swunicod.obj `cygpath -w $(utilfunsdir)/swunicod.cpp`

swunicod.lo: $(utilfunsdir)/swunicod.cpp
	source='$(utilfunsdir)/swunicod.cpp' object='swunicod.lo' libtool=yes \
	depfile='$(DEPDIR)/swunicod.Plo' tmpdepfile='$(DEPDIR)/swunicod.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swunicod.lo `test -f $(utilfunsdir)/swunicod.cpp || echo '$(srcdir)/'`$(utilfunsdir)/swunicod.cpp

swversion.o: $(utilfunsdir)/swversion.cpp
	source='$(utilfunsdir)/swversion.cpp' object='swversion.o' libtool=no \
	depfile='$(DEPDIR)/swversion.Po' tmpdepfile='$(DEPDIR)/swversion.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swversion.o `test -f $(utilfunsdir)/swversion.cpp || echo '$(srcdir)/'`$(utilfunsdir)/swversion.cpp

swversion.obj: $(utilfunsdir)/swversion.cpp
	source='$(utilfunsdir)/swversion.cpp' object='swversion.obj' libtool=no \
	depfile='$(DEPDIR)/swversion.Po' tmpdepfile='$(DEPDIR)/swversion.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swversion.obj `cygpath -w $(utilfunsdir)/swversion.cpp`

swversion.lo: $(utilfunsdir)/swversion.cpp
	source='$(utilfunsdir)/swversion.cpp' object='swversion.lo' libtool=yes \
	depfile='$(DEPDIR)/swversion.Plo' tmpdepfile='$(DEPDIR)/swversion.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swversion.lo `test -f $(utilfunsdir)/swversion.cpp || echo '$(srcdir)/'`$(utilfunsdir)/swversion.cpp

swconfig.o: $(mgrdir)/swconfig.cpp
	source='$(mgrdir)/swconfig.cpp' object='swconfig.o' libtool=no \
	depfile='$(DEPDIR)/swconfig.Po' tmpdepfile='$(DEPDIR)/swconfig.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swconfig.o `test -f $(mgrdir)/swconfig.cpp || echo '$(srcdir)/'`$(mgrdir)/swconfig.cpp

swconfig.obj: $(mgrdir)/swconfig.cpp
	source='$(mgrdir)/swconfig.cpp' object='swconfig.obj' libtool=no \
	depfile='$(DEPDIR)/swconfig.Po' tmpdepfile='$(DEPDIR)/swconfig.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swconfig.obj `cygpath -w $(mgrdir)/swconfig.cpp`

swconfig.lo: $(mgrdir)/swconfig.cpp
	source='$(mgrdir)/swconfig.cpp' object='swconfig.lo' libtool=yes \
	depfile='$(DEPDIR)/swconfig.Plo' tmpdepfile='$(DEPDIR)/swconfig.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swconfig.lo `test -f $(mgrdir)/swconfig.cpp || echo '$(srcdir)/'`$(mgrdir)/swconfig.cpp

swmgr.o: $(mgrdir)/swmgr.cpp
	source='$(mgrdir)/swmgr.cpp' object='swmgr.o' libtool=no \
	depfile='$(DEPDIR)/swmgr.Po' tmpdepfile='$(DEPDIR)/swmgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swmgr.o `test -f $(mgrdir)/swmgr.cpp || echo '$(srcdir)/'`$(mgrdir)/swmgr.cpp

swmgr.obj: $(mgrdir)/swmgr.cpp
	source='$(mgrdir)/swmgr.cpp' object='swmgr.obj' libtool=no \
	depfile='$(DEPDIR)/swmgr.Po' tmpdepfile='$(DEPDIR)/swmgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swmgr.obj `cygpath -w $(mgrdir)/swmgr.cpp`

swmgr.lo: $(mgrdir)/swmgr.cpp
	source='$(mgrdir)/swmgr.cpp' object='swmgr.lo' libtool=yes \
	depfile='$(DEPDIR)/swmgr.Plo' tmpdepfile='$(DEPDIR)/swmgr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swmgr.lo `test -f $(mgrdir)/swmgr.cpp || echo '$(srcdir)/'`$(mgrdir)/swmgr.cpp

swfiltermgr.o: $(mgrdir)/swfiltermgr.cpp
	source='$(mgrdir)/swfiltermgr.cpp' object='swfiltermgr.o' libtool=no \
	depfile='$(DEPDIR)/swfiltermgr.Po' tmpdepfile='$(DEPDIR)/swfiltermgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swfiltermgr.o `test -f $(mgrdir)/swfiltermgr.cpp || echo '$(srcdir)/'`$(mgrdir)/swfiltermgr.cpp

swfiltermgr.obj: $(mgrdir)/swfiltermgr.cpp
	source='$(mgrdir)/swfiltermgr.cpp' object='swfiltermgr.obj' libtool=no \
	depfile='$(DEPDIR)/swfiltermgr.Po' tmpdepfile='$(DEPDIR)/swfiltermgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swfiltermgr.obj `cygpath -w $(mgrdir)/swfiltermgr.cpp`

swfiltermgr.lo: $(mgrdir)/swfiltermgr.cpp
	source='$(mgrdir)/swfiltermgr.cpp' object='swfiltermgr.lo' libtool=yes \
	depfile='$(DEPDIR)/swfiltermgr.Plo' tmpdepfile='$(DEPDIR)/swfiltermgr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swfiltermgr.lo `test -f $(mgrdir)/swfiltermgr.cpp || echo '$(srcdir)/'`$(mgrdir)/swfiltermgr.cpp

encfiltmgr.o: $(mgrdir)/encfiltmgr.cpp
	source='$(mgrdir)/encfiltmgr.cpp' object='encfiltmgr.o' libtool=no \
	depfile='$(DEPDIR)/encfiltmgr.Po' tmpdepfile='$(DEPDIR)/encfiltmgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o encfiltmgr.o `test -f $(mgrdir)/encfiltmgr.cpp || echo '$(srcdir)/'`$(mgrdir)/encfiltmgr.cpp

encfiltmgr.obj: $(mgrdir)/encfiltmgr.cpp
	source='$(mgrdir)/encfiltmgr.cpp' object='encfiltmgr.obj' libtool=no \
	depfile='$(DEPDIR)/encfiltmgr.Po' tmpdepfile='$(DEPDIR)/encfiltmgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o encfiltmgr.obj `cygpath -w $(mgrdir)/encfiltmgr.cpp`

encfiltmgr.lo: $(mgrdir)/encfiltmgr.cpp
	source='$(mgrdir)/encfiltmgr.cpp' object='encfiltmgr.lo' libtool=yes \
	depfile='$(DEPDIR)/encfiltmgr.Plo' tmpdepfile='$(DEPDIR)/encfiltmgr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o encfiltmgr.lo `test -f $(mgrdir)/encfiltmgr.cpp || echo '$(srcdir)/'`$(mgrdir)/encfiltmgr.cpp

markupfiltmgr.o: $(mgrdir)/markupfiltmgr.cpp
	source='$(mgrdir)/markupfiltmgr.cpp' object='markupfiltmgr.o' libtool=no \
	depfile='$(DEPDIR)/markupfiltmgr.Po' tmpdepfile='$(DEPDIR)/markupfiltmgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o markupfiltmgr.o `test -f $(mgrdir)/markupfiltmgr.cpp || echo '$(srcdir)/'`$(mgrdir)/markupfiltmgr.cpp

markupfiltmgr.obj: $(mgrdir)/markupfiltmgr.cpp
	source='$(mgrdir)/markupfiltmgr.cpp' object='markupfiltmgr.obj' libtool=no \
	depfile='$(DEPDIR)/markupfiltmgr.Po' tmpdepfile='$(DEPDIR)/markupfiltmgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o markupfiltmgr.obj `cygpath -w $(mgrdir)/markupfiltmgr.cpp`

markupfiltmgr.lo: $(mgrdir)/markupfiltmgr.cpp
	source='$(mgrdir)/markupfiltmgr.cpp' object='markupfiltmgr.lo' libtool=yes \
	depfile='$(DEPDIR)/markupfiltmgr.Plo' tmpdepfile='$(DEPDIR)/markupfiltmgr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o markupfiltmgr.lo `test -f $(mgrdir)/markupfiltmgr.cpp || echo '$(srcdir)/'`$(mgrdir)/markupfiltmgr.cpp

filemgr.o: $(mgrdir)/filemgr.cpp
	source='$(mgrdir)/filemgr.cpp' object='filemgr.o' libtool=no \
	depfile='$(DEPDIR)/filemgr.Po' tmpdepfile='$(DEPDIR)/filemgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o filemgr.o `test -f $(mgrdir)/filemgr.cpp || echo '$(srcdir)/'`$(mgrdir)/filemgr.cpp

filemgr.obj: $(mgrdir)/filemgr.cpp
	source='$(mgrdir)/filemgr.cpp' object='filemgr.obj' libtool=no \
	depfile='$(DEPDIR)/filemgr.Po' tmpdepfile='$(DEPDIR)/filemgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o filemgr.obj `cygpath -w $(mgrdir)/filemgr.cpp`

filemgr.lo: $(mgrdir)/filemgr.cpp
	source='$(mgrdir)/filemgr.cpp' object='filemgr.lo' libtool=yes \
	depfile='$(DEPDIR)/filemgr.Plo' tmpdepfile='$(DEPDIR)/filemgr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o filemgr.lo `test -f $(mgrdir)/filemgr.cpp || echo '$(srcdir)/'`$(mgrdir)/filemgr.cpp

swlocale.o: $(mgrdir)/swlocale.cpp
	source='$(mgrdir)/swlocale.cpp' object='swlocale.o' libtool=no \
	depfile='$(DEPDIR)/swlocale.Po' tmpdepfile='$(DEPDIR)/swlocale.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swlocale.o `test -f $(mgrdir)/swlocale.cpp || echo '$(srcdir)/'`$(mgrdir)/swlocale.cpp

swlocale.obj: $(mgrdir)/swlocale.cpp
	source='$(mgrdir)/swlocale.cpp' object='swlocale.obj' libtool=no \
	depfile='$(DEPDIR)/swlocale.Po' tmpdepfile='$(DEPDIR)/swlocale.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swlocale.obj `cygpath -w $(mgrdir)/swlocale.cpp`

swlocale.lo: $(mgrdir)/swlocale.cpp
	source='$(mgrdir)/swlocale.cpp' object='swlocale.lo' libtool=yes \
	depfile='$(DEPDIR)/swlocale.Plo' tmpdepfile='$(DEPDIR)/swlocale.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swlocale.lo `test -f $(mgrdir)/swlocale.cpp || echo '$(srcdir)/'`$(mgrdir)/swlocale.cpp

localemgr.o: $(mgrdir)/localemgr.cpp
	source='$(mgrdir)/localemgr.cpp' object='localemgr.o' libtool=no \
	depfile='$(DEPDIR)/localemgr.Po' tmpdepfile='$(DEPDIR)/localemgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o localemgr.o `test -f $(mgrdir)/localemgr.cpp || echo '$(srcdir)/'`$(mgrdir)/localemgr.cpp

localemgr.obj: $(mgrdir)/localemgr.cpp
	source='$(mgrdir)/localemgr.cpp' object='localemgr.obj' libtool=no \
	depfile='$(DEPDIR)/localemgr.Po' tmpdepfile='$(DEPDIR)/localemgr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o localemgr.obj `cygpath -w $(mgrdir)/localemgr.cpp`

localemgr.lo: $(mgrdir)/localemgr.cpp
	source='$(mgrdir)/localemgr.cpp' object='localemgr.lo' libtool=yes \
	depfile='$(DEPDIR)/localemgr.Plo' tmpdepfile='$(DEPDIR)/localemgr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o localemgr.lo `test -f $(mgrdir)/localemgr.cpp || echo '$(srcdir)/'`$(mgrdir)/localemgr.cpp

swdisp.o: $(frontenddir)/swdisp.cpp
	source='$(frontenddir)/swdisp.cpp' object='swdisp.o' libtool=no \
	depfile='$(DEPDIR)/swdisp.Po' tmpdepfile='$(DEPDIR)/swdisp.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swdisp.o `test -f $(frontenddir)/swdisp.cpp || echo '$(srcdir)/'`$(frontenddir)/swdisp.cpp

swdisp.obj: $(frontenddir)/swdisp.cpp
	source='$(frontenddir)/swdisp.cpp' object='swdisp.obj' libtool=no \
	depfile='$(DEPDIR)/swdisp.Po' tmpdepfile='$(DEPDIR)/swdisp.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swdisp.obj `cygpath -w $(frontenddir)/swdisp.cpp`

swdisp.lo: $(frontenddir)/swdisp.cpp
	source='$(frontenddir)/swdisp.cpp' object='swdisp.lo' libtool=yes \
	depfile='$(DEPDIR)/swdisp.Plo' tmpdepfile='$(DEPDIR)/swdisp.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swdisp.lo `test -f $(frontenddir)/swdisp.cpp || echo '$(srcdir)/'`$(frontenddir)/swdisp.cpp

swlog.o: $(frontenddir)/swlog.cpp
	source='$(frontenddir)/swlog.cpp' object='swlog.o' libtool=no \
	depfile='$(DEPDIR)/swlog.Po' tmpdepfile='$(DEPDIR)/swlog.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swlog.o `test -f $(frontenddir)/swlog.cpp || echo '$(srcdir)/'`$(frontenddir)/swlog.cpp

swlog.obj: $(frontenddir)/swlog.cpp
	source='$(frontenddir)/swlog.cpp' object='swlog.obj' libtool=no \
	depfile='$(DEPDIR)/swlog.Po' tmpdepfile='$(DEPDIR)/swlog.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swlog.obj `cygpath -w $(frontenddir)/swlog.cpp`

swlog.lo: $(frontenddir)/swlog.cpp
	source='$(frontenddir)/swlog.cpp' object='swlog.lo' libtool=yes \
	depfile='$(DEPDIR)/swlog.Plo' tmpdepfile='$(DEPDIR)/swlog.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swlog.lo `test -f $(frontenddir)/swlog.cpp || echo '$(srcdir)/'`$(frontenddir)/swlog.cpp

swmodule.o: $(modulesdir)/swmodule.cpp
	source='$(modulesdir)/swmodule.cpp' object='swmodule.o' libtool=no \
	depfile='$(DEPDIR)/swmodule.Po' tmpdepfile='$(DEPDIR)/swmodule.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swmodule.o `test -f $(modulesdir)/swmodule.cpp || echo '$(srcdir)/'`$(modulesdir)/swmodule.cpp

swmodule.obj: $(modulesdir)/swmodule.cpp
	source='$(modulesdir)/swmodule.cpp' object='swmodule.obj' libtool=no \
	depfile='$(DEPDIR)/swmodule.Po' tmpdepfile='$(DEPDIR)/swmodule.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swmodule.obj `cygpath -w $(modulesdir)/swmodule.cpp`

swmodule.lo: $(modulesdir)/swmodule.cpp
	source='$(modulesdir)/swmodule.cpp' object='swmodule.lo' libtool=yes \
	depfile='$(DEPDIR)/swmodule.Plo' tmpdepfile='$(DEPDIR)/swmodule.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swmodule.lo `test -f $(modulesdir)/swmodule.cpp || echo '$(srcdir)/'`$(modulesdir)/swmodule.cpp

rawstr.o: $(commondir)/rawstr.cpp
	source='$(commondir)/rawstr.cpp' object='rawstr.o' libtool=no \
	depfile='$(DEPDIR)/rawstr.Po' tmpdepfile='$(DEPDIR)/rawstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawstr.o `test -f $(commondir)/rawstr.cpp || echo '$(srcdir)/'`$(commondir)/rawstr.cpp

rawstr.obj: $(commondir)/rawstr.cpp
	source='$(commondir)/rawstr.cpp' object='rawstr.obj' libtool=no \
	depfile='$(DEPDIR)/rawstr.Po' tmpdepfile='$(DEPDIR)/rawstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawstr.obj `cygpath -w $(commondir)/rawstr.cpp`

rawstr.lo: $(commondir)/rawstr.cpp
	source='$(commondir)/rawstr.cpp' object='rawstr.lo' libtool=yes \
	depfile='$(DEPDIR)/rawstr.Plo' tmpdepfile='$(DEPDIR)/rawstr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawstr.lo `test -f $(commondir)/rawstr.cpp || echo '$(srcdir)/'`$(commondir)/rawstr.cpp

rawstr4.o: $(commondir)/rawstr4.cpp
	source='$(commondir)/rawstr4.cpp' object='rawstr4.o' libtool=no \
	depfile='$(DEPDIR)/rawstr4.Po' tmpdepfile='$(DEPDIR)/rawstr4.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawstr4.o `test -f $(commondir)/rawstr4.cpp || echo '$(srcdir)/'`$(commondir)/rawstr4.cpp

rawstr4.obj: $(commondir)/rawstr4.cpp
	source='$(commondir)/rawstr4.cpp' object='rawstr4.obj' libtool=no \
	depfile='$(DEPDIR)/rawstr4.Po' tmpdepfile='$(DEPDIR)/rawstr4.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawstr4.obj `cygpath -w $(commondir)/rawstr4.cpp`

rawstr4.lo: $(commondir)/rawstr4.cpp
	source='$(commondir)/rawstr4.cpp' object='rawstr4.lo' libtool=yes \
	depfile='$(DEPDIR)/rawstr4.Plo' tmpdepfile='$(DEPDIR)/rawstr4.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawstr4.lo `test -f $(commondir)/rawstr4.cpp || echo '$(srcdir)/'`$(commondir)/rawstr4.cpp

swcomprs.o: $(commondir)/swcomprs.cpp
	source='$(commondir)/swcomprs.cpp' object='swcomprs.o' libtool=no \
	depfile='$(DEPDIR)/swcomprs.Po' tmpdepfile='$(DEPDIR)/swcomprs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcomprs.o `test -f $(commondir)/swcomprs.cpp || echo '$(srcdir)/'`$(commondir)/swcomprs.cpp

swcomprs.obj: $(commondir)/swcomprs.cpp
	source='$(commondir)/swcomprs.cpp' object='swcomprs.obj' libtool=no \
	depfile='$(DEPDIR)/swcomprs.Po' tmpdepfile='$(DEPDIR)/swcomprs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcomprs.obj `cygpath -w $(commondir)/swcomprs.cpp`

swcomprs.lo: $(commondir)/swcomprs.cpp
	source='$(commondir)/swcomprs.cpp' object='swcomprs.lo' libtool=yes \
	depfile='$(DEPDIR)/swcomprs.Plo' tmpdepfile='$(DEPDIR)/swcomprs.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcomprs.lo `test -f $(commondir)/swcomprs.cpp || echo '$(srcdir)/'`$(commondir)/swcomprs.cpp

lzsscomprs.o: $(commondir)/lzsscomprs.cpp
	source='$(commondir)/lzsscomprs.cpp' object='lzsscomprs.o' libtool=no \
	depfile='$(DEPDIR)/lzsscomprs.Po' tmpdepfile='$(DEPDIR)/lzsscomprs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o lzsscomprs.o `test -f $(commondir)/lzsscomprs.cpp || echo '$(srcdir)/'`$(commondir)/lzsscomprs.cpp

lzsscomprs.obj: $(commondir)/lzsscomprs.cpp
	source='$(commondir)/lzsscomprs.cpp' object='lzsscomprs.obj' libtool=no \
	depfile='$(DEPDIR)/lzsscomprs.Po' tmpdepfile='$(DEPDIR)/lzsscomprs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o lzsscomprs.obj `cygpath -w $(commondir)/lzsscomprs.cpp`

lzsscomprs.lo: $(commondir)/lzsscomprs.cpp
	source='$(commondir)/lzsscomprs.cpp' object='lzsscomprs.lo' libtool=yes \
	depfile='$(DEPDIR)/lzsscomprs.Plo' tmpdepfile='$(DEPDIR)/lzsscomprs.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o lzsscomprs.lo `test -f $(commondir)/lzsscomprs.cpp || echo '$(srcdir)/'`$(commondir)/lzsscomprs.cpp

zipcomprs.o: $(commondir)/zipcomprs.cpp
	source='$(commondir)/zipcomprs.cpp' object='zipcomprs.o' libtool=no \
	depfile='$(DEPDIR)/zipcomprs.Po' tmpdepfile='$(DEPDIR)/zipcomprs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zipcomprs.o `test -f $(commondir)/zipcomprs.cpp || echo '$(srcdir)/'`$(commondir)/zipcomprs.cpp

zipcomprs.obj: $(commondir)/zipcomprs.cpp
	source='$(commondir)/zipcomprs.cpp' object='zipcomprs.obj' libtool=no \
	depfile='$(DEPDIR)/zipcomprs.Po' tmpdepfile='$(DEPDIR)/zipcomprs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zipcomprs.obj `cygpath -w $(commondir)/zipcomprs.cpp`

zipcomprs.lo: $(commondir)/zipcomprs.cpp
	source='$(commondir)/zipcomprs.cpp' object='zipcomprs.lo' libtool=yes \
	depfile='$(DEPDIR)/zipcomprs.Plo' tmpdepfile='$(DEPDIR)/zipcomprs.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zipcomprs.lo `test -f $(commondir)/zipcomprs.cpp || echo '$(srcdir)/'`$(commondir)/zipcomprs.cpp

rawverse.o: $(commondir)/rawverse.cpp
	source='$(commondir)/rawverse.cpp' object='rawverse.o' libtool=no \
	depfile='$(DEPDIR)/rawverse.Po' tmpdepfile='$(DEPDIR)/rawverse.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawverse.o `test -f $(commondir)/rawverse.cpp || echo '$(srcdir)/'`$(commondir)/rawverse.cpp

rawverse.obj: $(commondir)/rawverse.cpp
	source='$(commondir)/rawverse.cpp' object='rawverse.obj' libtool=no \
	depfile='$(DEPDIR)/rawverse.Po' tmpdepfile='$(DEPDIR)/rawverse.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawverse.obj `cygpath -w $(commondir)/rawverse.cpp`

rawverse.lo: $(commondir)/rawverse.cpp
	source='$(commondir)/rawverse.cpp' object='rawverse.lo' libtool=yes \
	depfile='$(DEPDIR)/rawverse.Plo' tmpdepfile='$(DEPDIR)/rawverse.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawverse.lo `test -f $(commondir)/rawverse.cpp || echo '$(srcdir)/'`$(commondir)/rawverse.cpp

swcipher.o: $(commondir)/swcipher.cpp
	source='$(commondir)/swcipher.cpp' object='swcipher.o' libtool=no \
	depfile='$(DEPDIR)/swcipher.Po' tmpdepfile='$(DEPDIR)/swcipher.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcipher.o `test -f $(commondir)/swcipher.cpp || echo '$(srcdir)/'`$(commondir)/swcipher.cpp

swcipher.obj: $(commondir)/swcipher.cpp
	source='$(commondir)/swcipher.cpp' object='swcipher.obj' libtool=no \
	depfile='$(DEPDIR)/swcipher.Po' tmpdepfile='$(DEPDIR)/swcipher.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcipher.obj `cygpath -w $(commondir)/swcipher.cpp`

swcipher.lo: $(commondir)/swcipher.cpp
	source='$(commondir)/swcipher.cpp' object='swcipher.lo' libtool=yes \
	depfile='$(DEPDIR)/swcipher.Plo' tmpdepfile='$(DEPDIR)/swcipher.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcipher.lo `test -f $(commondir)/swcipher.cpp || echo '$(srcdir)/'`$(commondir)/swcipher.cpp

zverse.o: $(commondir)/zverse.cpp
	source='$(commondir)/zverse.cpp' object='zverse.o' libtool=no \
	depfile='$(DEPDIR)/zverse.Po' tmpdepfile='$(DEPDIR)/zverse.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zverse.o `test -f $(commondir)/zverse.cpp || echo '$(srcdir)/'`$(commondir)/zverse.cpp

zverse.obj: $(commondir)/zverse.cpp
	source='$(commondir)/zverse.cpp' object='zverse.obj' libtool=no \
	depfile='$(DEPDIR)/zverse.Po' tmpdepfile='$(DEPDIR)/zverse.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zverse.obj `cygpath -w $(commondir)/zverse.cpp`

zverse.lo: $(commondir)/zverse.cpp
	source='$(commondir)/zverse.cpp' object='zverse.lo' libtool=yes \
	depfile='$(DEPDIR)/zverse.Plo' tmpdepfile='$(DEPDIR)/zverse.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zverse.lo `test -f $(commondir)/zverse.cpp || echo '$(srcdir)/'`$(commondir)/zverse.cpp

zstr.o: $(commondir)/zstr.cpp
	source='$(commondir)/zstr.cpp' object='zstr.o' libtool=no \
	depfile='$(DEPDIR)/zstr.Po' tmpdepfile='$(DEPDIR)/zstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zstr.o `test -f $(commondir)/zstr.cpp || echo '$(srcdir)/'`$(commondir)/zstr.cpp

zstr.obj: $(commondir)/zstr.cpp
	source='$(commondir)/zstr.cpp' object='zstr.obj' libtool=no \
	depfile='$(DEPDIR)/zstr.Po' tmpdepfile='$(DEPDIR)/zstr.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zstr.obj `cygpath -w $(commondir)/zstr.cpp`

zstr.lo: $(commondir)/zstr.cpp
	source='$(commondir)/zstr.cpp' object='zstr.lo' libtool=yes \
	depfile='$(DEPDIR)/zstr.Plo' tmpdepfile='$(DEPDIR)/zstr.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zstr.lo `test -f $(commondir)/zstr.cpp || echo '$(srcdir)/'`$(commondir)/zstr.cpp

entriesblk.o: $(commondir)/entriesblk.cpp
	source='$(commondir)/entriesblk.cpp' object='entriesblk.o' libtool=no \
	depfile='$(DEPDIR)/entriesblk.Po' tmpdepfile='$(DEPDIR)/entriesblk.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o entriesblk.o `test -f $(commondir)/entriesblk.cpp || echo '$(srcdir)/'`$(commondir)/entriesblk.cpp

entriesblk.obj: $(commondir)/entriesblk.cpp
	source='$(commondir)/entriesblk.cpp' object='entriesblk.obj' libtool=no \
	depfile='$(DEPDIR)/entriesblk.Po' tmpdepfile='$(DEPDIR)/entriesblk.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o entriesblk.obj `cygpath -w $(commondir)/entriesblk.cpp`

entriesblk.lo: $(commondir)/entriesblk.cpp
	source='$(commondir)/entriesblk.cpp' object='entriesblk.lo' libtool=yes \
	depfile='$(DEPDIR)/entriesblk.Plo' tmpdepfile='$(DEPDIR)/entriesblk.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o entriesblk.lo `test -f $(commondir)/entriesblk.cpp || echo '$(srcdir)/'`$(commondir)/entriesblk.cpp

sapphire.o: $(commondir)/sapphire.cpp
	source='$(commondir)/sapphire.cpp' object='sapphire.o' libtool=no \
	depfile='$(DEPDIR)/sapphire.Po' tmpdepfile='$(DEPDIR)/sapphire.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o sapphire.o `test -f $(commondir)/sapphire.cpp || echo '$(srcdir)/'`$(commondir)/sapphire.cpp

sapphire.obj: $(commondir)/sapphire.cpp
	source='$(commondir)/sapphire.cpp' object='sapphire.obj' libtool=no \
	depfile='$(DEPDIR)/sapphire.Po' tmpdepfile='$(DEPDIR)/sapphire.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o sapphire.obj `cygpath -w $(commondir)/sapphire.cpp`

sapphire.lo: $(commondir)/sapphire.cpp
	source='$(commondir)/sapphire.cpp' object='sapphire.lo' libtool=yes \
	depfile='$(DEPDIR)/sapphire.Plo' tmpdepfile='$(DEPDIR)/sapphire.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o sapphire.lo `test -f $(commondir)/sapphire.cpp || echo '$(srcdir)/'`$(commondir)/sapphire.cpp

swbasicfilter.o: $(filtersdir)/swbasicfilter.cpp
	source='$(filtersdir)/swbasicfilter.cpp' object='swbasicfilter.o' libtool=no \
	depfile='$(DEPDIR)/swbasicfilter.Po' tmpdepfile='$(DEPDIR)/swbasicfilter.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swbasicfilter.o `test -f $(filtersdir)/swbasicfilter.cpp || echo '$(srcdir)/'`$(filtersdir)/swbasicfilter.cpp

swbasicfilter.obj: $(filtersdir)/swbasicfilter.cpp
	source='$(filtersdir)/swbasicfilter.cpp' object='swbasicfilter.obj' libtool=no \
	depfile='$(DEPDIR)/swbasicfilter.Po' tmpdepfile='$(DEPDIR)/swbasicfilter.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swbasicfilter.obj `cygpath -w $(filtersdir)/swbasicfilter.cpp`

swbasicfilter.lo: $(filtersdir)/swbasicfilter.cpp
	source='$(filtersdir)/swbasicfilter.cpp' object='swbasicfilter.lo' libtool=yes \
	depfile='$(DEPDIR)/swbasicfilter.Plo' tmpdepfile='$(DEPDIR)/swbasicfilter.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swbasicfilter.lo `test -f $(filtersdir)/swbasicfilter.cpp || echo '$(srcdir)/'`$(filtersdir)/swbasicfilter.cpp

gbfhtml.o: $(filtersdir)/gbfhtml.cpp
	source='$(filtersdir)/gbfhtml.cpp' object='gbfhtml.o' libtool=no \
	depfile='$(DEPDIR)/gbfhtml.Po' tmpdepfile='$(DEPDIR)/gbfhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfhtml.o `test -f $(filtersdir)/gbfhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfhtml.cpp

gbfhtml.obj: $(filtersdir)/gbfhtml.cpp
	source='$(filtersdir)/gbfhtml.cpp' object='gbfhtml.obj' libtool=no \
	depfile='$(DEPDIR)/gbfhtml.Po' tmpdepfile='$(DEPDIR)/gbfhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfhtml.obj `cygpath -w $(filtersdir)/gbfhtml.cpp`

gbfhtml.lo: $(filtersdir)/gbfhtml.cpp
	source='$(filtersdir)/gbfhtml.cpp' object='gbfhtml.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfhtml.Plo' tmpdepfile='$(DEPDIR)/gbfhtml.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfhtml.lo `test -f $(filtersdir)/gbfhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfhtml.cpp

gbfhtmlhref.o: $(filtersdir)/gbfhtmlhref.cpp
	source='$(filtersdir)/gbfhtmlhref.cpp' object='gbfhtmlhref.o' libtool=no \
	depfile='$(DEPDIR)/gbfhtmlhref.Po' tmpdepfile='$(DEPDIR)/gbfhtmlhref.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfhtmlhref.o `test -f $(filtersdir)/gbfhtmlhref.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfhtmlhref.cpp

gbfhtmlhref.obj: $(filtersdir)/gbfhtmlhref.cpp
	source='$(filtersdir)/gbfhtmlhref.cpp' object='gbfhtmlhref.obj' libtool=no \
	depfile='$(DEPDIR)/gbfhtmlhref.Po' tmpdepfile='$(DEPDIR)/gbfhtmlhref.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfhtmlhref.obj `cygpath -w $(filtersdir)/gbfhtmlhref.cpp`

gbfhtmlhref.lo: $(filtersdir)/gbfhtmlhref.cpp
	source='$(filtersdir)/gbfhtmlhref.cpp' object='gbfhtmlhref.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfhtmlhref.Plo' tmpdepfile='$(DEPDIR)/gbfhtmlhref.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfhtmlhref.lo `test -f $(filtersdir)/gbfhtmlhref.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfhtmlhref.cpp

gbfplain.o: $(filtersdir)/gbfplain.cpp
	source='$(filtersdir)/gbfplain.cpp' object='gbfplain.o' libtool=no \
	depfile='$(DEPDIR)/gbfplain.Po' tmpdepfile='$(DEPDIR)/gbfplain.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfplain.o `test -f $(filtersdir)/gbfplain.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfplain.cpp

gbfplain.obj: $(filtersdir)/gbfplain.cpp
	source='$(filtersdir)/gbfplain.cpp' object='gbfplain.obj' libtool=no \
	depfile='$(DEPDIR)/gbfplain.Po' tmpdepfile='$(DEPDIR)/gbfplain.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfplain.obj `cygpath -w $(filtersdir)/gbfplain.cpp`

gbfplain.lo: $(filtersdir)/gbfplain.cpp
	source='$(filtersdir)/gbfplain.cpp' object='gbfplain.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfplain.Plo' tmpdepfile='$(DEPDIR)/gbfplain.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfplain.lo `test -f $(filtersdir)/gbfplain.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfplain.cpp

gbfrtf.o: $(filtersdir)/gbfrtf.cpp
	source='$(filtersdir)/gbfrtf.cpp' object='gbfrtf.o' libtool=no \
	depfile='$(DEPDIR)/gbfrtf.Po' tmpdepfile='$(DEPDIR)/gbfrtf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfrtf.o `test -f $(filtersdir)/gbfrtf.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfrtf.cpp

gbfrtf.obj: $(filtersdir)/gbfrtf.cpp
	source='$(filtersdir)/gbfrtf.cpp' object='gbfrtf.obj' libtool=no \
	depfile='$(DEPDIR)/gbfrtf.Po' tmpdepfile='$(DEPDIR)/gbfrtf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfrtf.obj `cygpath -w $(filtersdir)/gbfrtf.cpp`

gbfrtf.lo: $(filtersdir)/gbfrtf.cpp
	source='$(filtersdir)/gbfrtf.cpp' object='gbfrtf.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfrtf.Plo' tmpdepfile='$(DEPDIR)/gbfrtf.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfrtf.lo `test -f $(filtersdir)/gbfrtf.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfrtf.cpp

plainhtml.o: $(filtersdir)/plainhtml.cpp
	source='$(filtersdir)/plainhtml.cpp' object='plainhtml.o' libtool=no \
	depfile='$(DEPDIR)/plainhtml.Po' tmpdepfile='$(DEPDIR)/plainhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o plainhtml.o `test -f $(filtersdir)/plainhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/plainhtml.cpp

plainhtml.obj: $(filtersdir)/plainhtml.cpp
	source='$(filtersdir)/plainhtml.cpp' object='plainhtml.obj' libtool=no \
	depfile='$(DEPDIR)/plainhtml.Po' tmpdepfile='$(DEPDIR)/plainhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o plainhtml.obj `cygpath -w $(filtersdir)/plainhtml.cpp`

plainhtml.lo: $(filtersdir)/plainhtml.cpp
	source='$(filtersdir)/plainhtml.cpp' object='plainhtml.lo' libtool=yes \
	depfile='$(DEPDIR)/plainhtml.Plo' tmpdepfile='$(DEPDIR)/plainhtml.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o plainhtml.lo `test -f $(filtersdir)/plainhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/plainhtml.cpp

rwphtml.o: $(filtersdir)/rwphtml.cpp
	source='$(filtersdir)/rwphtml.cpp' object='rwphtml.o' libtool=no \
	depfile='$(DEPDIR)/rwphtml.Po' tmpdepfile='$(DEPDIR)/rwphtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rwphtml.o `test -f $(filtersdir)/rwphtml.cpp || echo '$(srcdir)/'`$(filtersdir)/rwphtml.cpp

rwphtml.obj: $(filtersdir)/rwphtml.cpp
	source='$(filtersdir)/rwphtml.cpp' object='rwphtml.obj' libtool=no \
	depfile='$(DEPDIR)/rwphtml.Po' tmpdepfile='$(DEPDIR)/rwphtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rwphtml.obj `cygpath -w $(filtersdir)/rwphtml.cpp`

rwphtml.lo: $(filtersdir)/rwphtml.cpp
	source='$(filtersdir)/rwphtml.cpp' object='rwphtml.lo' libtool=yes \
	depfile='$(DEPDIR)/rwphtml.Plo' tmpdepfile='$(DEPDIR)/rwphtml.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rwphtml.lo `test -f $(filtersdir)/rwphtml.cpp || echo '$(srcdir)/'`$(filtersdir)/rwphtml.cpp

rwprtf.o: $(filtersdir)/rwprtf.cpp
	source='$(filtersdir)/rwprtf.cpp' object='rwprtf.o' libtool=no \
	depfile='$(DEPDIR)/rwprtf.Po' tmpdepfile='$(DEPDIR)/rwprtf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rwprtf.o `test -f $(filtersdir)/rwprtf.cpp || echo '$(srcdir)/'`$(filtersdir)/rwprtf.cpp

rwprtf.obj: $(filtersdir)/rwprtf.cpp
	source='$(filtersdir)/rwprtf.cpp' object='rwprtf.obj' libtool=no \
	depfile='$(DEPDIR)/rwprtf.Po' tmpdepfile='$(DEPDIR)/rwprtf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rwprtf.obj `cygpath -w $(filtersdir)/rwprtf.cpp`

rwprtf.lo: $(filtersdir)/rwprtf.cpp
	source='$(filtersdir)/rwprtf.cpp' object='rwprtf.lo' libtool=yes \
	depfile='$(DEPDIR)/rwprtf.Plo' tmpdepfile='$(DEPDIR)/rwprtf.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rwprtf.lo `test -f $(filtersdir)/rwprtf.cpp || echo '$(srcdir)/'`$(filtersdir)/rwprtf.cpp

cipherfil.o: $(filtersdir)/cipherfil.cpp
	source='$(filtersdir)/cipherfil.cpp' object='cipherfil.o' libtool=no \
	depfile='$(DEPDIR)/cipherfil.Po' tmpdepfile='$(DEPDIR)/cipherfil.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o cipherfil.o `test -f $(filtersdir)/cipherfil.cpp || echo '$(srcdir)/'`$(filtersdir)/cipherfil.cpp

cipherfil.obj: $(filtersdir)/cipherfil.cpp
	source='$(filtersdir)/cipherfil.cpp' object='cipherfil.obj' libtool=no \
	depfile='$(DEPDIR)/cipherfil.Po' tmpdepfile='$(DEPDIR)/cipherfil.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o cipherfil.obj `cygpath -w $(filtersdir)/cipherfil.cpp`

cipherfil.lo: $(filtersdir)/cipherfil.cpp
	source='$(filtersdir)/cipherfil.cpp' object='cipherfil.lo' libtool=yes \
	depfile='$(DEPDIR)/cipherfil.Plo' tmpdepfile='$(DEPDIR)/cipherfil.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o cipherfil.lo `test -f $(filtersdir)/cipherfil.cpp || echo '$(srcdir)/'`$(filtersdir)/cipherfil.cpp

rtfhtml.o: $(filtersdir)/rtfhtml.cpp
	source='$(filtersdir)/rtfhtml.cpp' object='rtfhtml.o' libtool=no \
	depfile='$(DEPDIR)/rtfhtml.Po' tmpdepfile='$(DEPDIR)/rtfhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rtfhtml.o `test -f $(filtersdir)/rtfhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/rtfhtml.cpp

rtfhtml.obj: $(filtersdir)/rtfhtml.cpp
	source='$(filtersdir)/rtfhtml.cpp' object='rtfhtml.obj' libtool=no \
	depfile='$(DEPDIR)/rtfhtml.Po' tmpdepfile='$(DEPDIR)/rtfhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rtfhtml.obj `cygpath -w $(filtersdir)/rtfhtml.cpp`

rtfhtml.lo: $(filtersdir)/rtfhtml.cpp
	source='$(filtersdir)/rtfhtml.cpp' object='rtfhtml.lo' libtool=yes \
	depfile='$(DEPDIR)/rtfhtml.Plo' tmpdepfile='$(DEPDIR)/rtfhtml.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rtfhtml.lo `test -f $(filtersdir)/rtfhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/rtfhtml.cpp

gbfstrongs.o: $(filtersdir)/gbfstrongs.cpp
	source='$(filtersdir)/gbfstrongs.cpp' object='gbfstrongs.o' libtool=no \
	depfile='$(DEPDIR)/gbfstrongs.Po' tmpdepfile='$(DEPDIR)/gbfstrongs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfstrongs.o `test -f $(filtersdir)/gbfstrongs.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfstrongs.cpp

gbfstrongs.obj: $(filtersdir)/gbfstrongs.cpp
	source='$(filtersdir)/gbfstrongs.cpp' object='gbfstrongs.obj' libtool=no \
	depfile='$(DEPDIR)/gbfstrongs.Po' tmpdepfile='$(DEPDIR)/gbfstrongs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfstrongs.obj `cygpath -w $(filtersdir)/gbfstrongs.cpp`

gbfstrongs.lo: $(filtersdir)/gbfstrongs.cpp
	source='$(filtersdir)/gbfstrongs.cpp' object='gbfstrongs.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfstrongs.Plo' tmpdepfile='$(DEPDIR)/gbfstrongs.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfstrongs.lo `test -f $(filtersdir)/gbfstrongs.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfstrongs.cpp

gbffootnotes.o: $(filtersdir)/gbffootnotes.cpp
	source='$(filtersdir)/gbffootnotes.cpp' object='gbffootnotes.o' libtool=no \
	depfile='$(DEPDIR)/gbffootnotes.Po' tmpdepfile='$(DEPDIR)/gbffootnotes.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbffootnotes.o `test -f $(filtersdir)/gbffootnotes.cpp || echo '$(srcdir)/'`$(filtersdir)/gbffootnotes.cpp

gbffootnotes.obj: $(filtersdir)/gbffootnotes.cpp
	source='$(filtersdir)/gbffootnotes.cpp' object='gbffootnotes.obj' libtool=no \
	depfile='$(DEPDIR)/gbffootnotes.Po' tmpdepfile='$(DEPDIR)/gbffootnotes.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbffootnotes.obj `cygpath -w $(filtersdir)/gbffootnotes.cpp`

gbffootnotes.lo: $(filtersdir)/gbffootnotes.cpp
	source='$(filtersdir)/gbffootnotes.cpp' object='gbffootnotes.lo' libtool=yes \
	depfile='$(DEPDIR)/gbffootnotes.Plo' tmpdepfile='$(DEPDIR)/gbffootnotes.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbffootnotes.lo `test -f $(filtersdir)/gbffootnotes.cpp || echo '$(srcdir)/'`$(filtersdir)/gbffootnotes.cpp

gbfheadings.o: $(filtersdir)/gbfheadings.cpp
	source='$(filtersdir)/gbfheadings.cpp' object='gbfheadings.o' libtool=no \
	depfile='$(DEPDIR)/gbfheadings.Po' tmpdepfile='$(DEPDIR)/gbfheadings.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfheadings.o `test -f $(filtersdir)/gbfheadings.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfheadings.cpp

gbfheadings.obj: $(filtersdir)/gbfheadings.cpp
	source='$(filtersdir)/gbfheadings.cpp' object='gbfheadings.obj' libtool=no \
	depfile='$(DEPDIR)/gbfheadings.Po' tmpdepfile='$(DEPDIR)/gbfheadings.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfheadings.obj `cygpath -w $(filtersdir)/gbfheadings.cpp`

gbfheadings.lo: $(filtersdir)/gbfheadings.cpp
	source='$(filtersdir)/gbfheadings.cpp' object='gbfheadings.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfheadings.Plo' tmpdepfile='$(DEPDIR)/gbfheadings.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfheadings.lo `test -f $(filtersdir)/gbfheadings.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfheadings.cpp

gbfmorph.o: $(filtersdir)/gbfmorph.cpp
	source='$(filtersdir)/gbfmorph.cpp' object='gbfmorph.o' libtool=no \
	depfile='$(DEPDIR)/gbfmorph.Po' tmpdepfile='$(DEPDIR)/gbfmorph.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfmorph.o `test -f $(filtersdir)/gbfmorph.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfmorph.cpp

gbfmorph.obj: $(filtersdir)/gbfmorph.cpp
	source='$(filtersdir)/gbfmorph.cpp' object='gbfmorph.obj' libtool=no \
	depfile='$(DEPDIR)/gbfmorph.Po' tmpdepfile='$(DEPDIR)/gbfmorph.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfmorph.obj `cygpath -w $(filtersdir)/gbfmorph.cpp`

gbfmorph.lo: $(filtersdir)/gbfmorph.cpp
	source='$(filtersdir)/gbfmorph.cpp' object='gbfmorph.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfmorph.Plo' tmpdepfile='$(DEPDIR)/gbfmorph.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfmorph.lo `test -f $(filtersdir)/gbfmorph.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfmorph.cpp

plainfootnotes.o: $(filtersdir)/plainfootnotes.cpp
	source='$(filtersdir)/plainfootnotes.cpp' object='plainfootnotes.o' libtool=no \
	depfile='$(DEPDIR)/plainfootnotes.Po' tmpdepfile='$(DEPDIR)/plainfootnotes.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o plainfootnotes.o `test -f $(filtersdir)/plainfootnotes.cpp || echo '$(srcdir)/'`$(filtersdir)/plainfootnotes.cpp

plainfootnotes.obj: $(filtersdir)/plainfootnotes.cpp
	source='$(filtersdir)/plainfootnotes.cpp' object='plainfootnotes.obj' libtool=no \
	depfile='$(DEPDIR)/plainfootnotes.Po' tmpdepfile='$(DEPDIR)/plainfootnotes.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o plainfootnotes.obj `cygpath -w $(filtersdir)/plainfootnotes.cpp`

plainfootnotes.lo: $(filtersdir)/plainfootnotes.cpp
	source='$(filtersdir)/plainfootnotes.cpp' object='plainfootnotes.lo' libtool=yes \
	depfile='$(DEPDIR)/plainfootnotes.Plo' tmpdepfile='$(DEPDIR)/plainfootnotes.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o plainfootnotes.lo `test -f $(filtersdir)/plainfootnotes.cpp || echo '$(srcdir)/'`$(filtersdir)/plainfootnotes.cpp

thmlstrongs.o: $(filtersdir)/thmlstrongs.cpp
	source='$(filtersdir)/thmlstrongs.cpp' object='thmlstrongs.o' libtool=no \
	depfile='$(DEPDIR)/thmlstrongs.Po' tmpdepfile='$(DEPDIR)/thmlstrongs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlstrongs.o `test -f $(filtersdir)/thmlstrongs.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlstrongs.cpp

thmlstrongs.obj: $(filtersdir)/thmlstrongs.cpp
	source='$(filtersdir)/thmlstrongs.cpp' object='thmlstrongs.obj' libtool=no \
	depfile='$(DEPDIR)/thmlstrongs.Po' tmpdepfile='$(DEPDIR)/thmlstrongs.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlstrongs.obj `cygpath -w $(filtersdir)/thmlstrongs.cpp`

thmlstrongs.lo: $(filtersdir)/thmlstrongs.cpp
	source='$(filtersdir)/thmlstrongs.cpp' object='thmlstrongs.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlstrongs.Plo' tmpdepfile='$(DEPDIR)/thmlstrongs.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlstrongs.lo `test -f $(filtersdir)/thmlstrongs.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlstrongs.cpp

thmlfootnotes.o: $(filtersdir)/thmlfootnotes.cpp
	source='$(filtersdir)/thmlfootnotes.cpp' object='thmlfootnotes.o' libtool=no \
	depfile='$(DEPDIR)/thmlfootnotes.Po' tmpdepfile='$(DEPDIR)/thmlfootnotes.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlfootnotes.o `test -f $(filtersdir)/thmlfootnotes.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlfootnotes.cpp

thmlfootnotes.obj: $(filtersdir)/thmlfootnotes.cpp
	source='$(filtersdir)/thmlfootnotes.cpp' object='thmlfootnotes.obj' libtool=no \
	depfile='$(DEPDIR)/thmlfootnotes.Po' tmpdepfile='$(DEPDIR)/thmlfootnotes.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlfootnotes.obj `cygpath -w $(filtersdir)/thmlfootnotes.cpp`

thmlfootnotes.lo: $(filtersdir)/thmlfootnotes.cpp
	source='$(filtersdir)/thmlfootnotes.cpp' object='thmlfootnotes.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlfootnotes.Plo' tmpdepfile='$(DEPDIR)/thmlfootnotes.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlfootnotes.lo `test -f $(filtersdir)/thmlfootnotes.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlfootnotes.cpp

thmlheadings.o: $(filtersdir)/thmlheadings.cpp
	source='$(filtersdir)/thmlheadings.cpp' object='thmlheadings.o' libtool=no \
	depfile='$(DEPDIR)/thmlheadings.Po' tmpdepfile='$(DEPDIR)/thmlheadings.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlheadings.o `test -f $(filtersdir)/thmlheadings.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlheadings.cpp

thmlheadings.obj: $(filtersdir)/thmlheadings.cpp
	source='$(filtersdir)/thmlheadings.cpp' object='thmlheadings.obj' libtool=no \
	depfile='$(DEPDIR)/thmlheadings.Po' tmpdepfile='$(DEPDIR)/thmlheadings.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlheadings.obj `cygpath -w $(filtersdir)/thmlheadings.cpp`

thmlheadings.lo: $(filtersdir)/thmlheadings.cpp
	source='$(filtersdir)/thmlheadings.cpp' object='thmlheadings.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlheadings.Plo' tmpdepfile='$(DEPDIR)/thmlheadings.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlheadings.lo `test -f $(filtersdir)/thmlheadings.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlheadings.cpp

thmlmorph.o: $(filtersdir)/thmlmorph.cpp
	source='$(filtersdir)/thmlmorph.cpp' object='thmlmorph.o' libtool=no \
	depfile='$(DEPDIR)/thmlmorph.Po' tmpdepfile='$(DEPDIR)/thmlmorph.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlmorph.o `test -f $(filtersdir)/thmlmorph.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlmorph.cpp

thmlmorph.obj: $(filtersdir)/thmlmorph.cpp
	source='$(filtersdir)/thmlmorph.cpp' object='thmlmorph.obj' libtool=no \
	depfile='$(DEPDIR)/thmlmorph.Po' tmpdepfile='$(DEPDIR)/thmlmorph.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlmorph.obj `cygpath -w $(filtersdir)/thmlmorph.cpp`

thmlmorph.lo: $(filtersdir)/thmlmorph.cpp
	source='$(filtersdir)/thmlmorph.cpp' object='thmlmorph.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlmorph.Plo' tmpdepfile='$(DEPDIR)/thmlmorph.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlmorph.lo `test -f $(filtersdir)/thmlmorph.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlmorph.cpp

thmllemma.o: $(filtersdir)/thmllemma.cpp
	source='$(filtersdir)/thmllemma.cpp' object='thmllemma.o' libtool=no \
	depfile='$(DEPDIR)/thmllemma.Po' tmpdepfile='$(DEPDIR)/thmllemma.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmllemma.o `test -f $(filtersdir)/thmllemma.cpp || echo '$(srcdir)/'`$(filtersdir)/thmllemma.cpp

thmllemma.obj: $(filtersdir)/thmllemma.cpp
	source='$(filtersdir)/thmllemma.cpp' object='thmllemma.obj' libtool=no \
	depfile='$(DEPDIR)/thmllemma.Po' tmpdepfile='$(DEPDIR)/thmllemma.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmllemma.obj `cygpath -w $(filtersdir)/thmllemma.cpp`

thmllemma.lo: $(filtersdir)/thmllemma.cpp
	source='$(filtersdir)/thmllemma.cpp' object='thmllemma.lo' libtool=yes \
	depfile='$(DEPDIR)/thmllemma.Plo' tmpdepfile='$(DEPDIR)/thmllemma.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmllemma.lo `test -f $(filtersdir)/thmllemma.cpp || echo '$(srcdir)/'`$(filtersdir)/thmllemma.cpp

thmlscripref.o: $(filtersdir)/thmlscripref.cpp
	source='$(filtersdir)/thmlscripref.cpp' object='thmlscripref.o' libtool=no \
	depfile='$(DEPDIR)/thmlscripref.Po' tmpdepfile='$(DEPDIR)/thmlscripref.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlscripref.o `test -f $(filtersdir)/thmlscripref.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlscripref.cpp

thmlscripref.obj: $(filtersdir)/thmlscripref.cpp
	source='$(filtersdir)/thmlscripref.cpp' object='thmlscripref.obj' libtool=no \
	depfile='$(DEPDIR)/thmlscripref.Po' tmpdepfile='$(DEPDIR)/thmlscripref.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlscripref.obj `cygpath -w $(filtersdir)/thmlscripref.cpp`

thmlscripref.lo: $(filtersdir)/thmlscripref.cpp
	source='$(filtersdir)/thmlscripref.cpp' object='thmlscripref.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlscripref.Plo' tmpdepfile='$(DEPDIR)/thmlscripref.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlscripref.lo `test -f $(filtersdir)/thmlscripref.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlscripref.cpp

thmlvariants.o: $(filtersdir)/thmlvariants.cpp
	source='$(filtersdir)/thmlvariants.cpp' object='thmlvariants.o' libtool=no \
	depfile='$(DEPDIR)/thmlvariants.Po' tmpdepfile='$(DEPDIR)/thmlvariants.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlvariants.o `test -f $(filtersdir)/thmlvariants.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlvariants.cpp

thmlvariants.obj: $(filtersdir)/thmlvariants.cpp
	source='$(filtersdir)/thmlvariants.cpp' object='thmlvariants.obj' libtool=no \
	depfile='$(DEPDIR)/thmlvariants.Po' tmpdepfile='$(DEPDIR)/thmlvariants.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlvariants.obj `cygpath -w $(filtersdir)/thmlvariants.cpp`

thmlvariants.lo: $(filtersdir)/thmlvariants.cpp
	source='$(filtersdir)/thmlvariants.cpp' object='thmlvariants.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlvariants.Plo' tmpdepfile='$(DEPDIR)/thmlvariants.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlvariants.lo `test -f $(filtersdir)/thmlvariants.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlvariants.cpp

gbfthml.o: $(filtersdir)/gbfthml.cpp
	source='$(filtersdir)/gbfthml.cpp' object='gbfthml.o' libtool=no \
	depfile='$(DEPDIR)/gbfthml.Po' tmpdepfile='$(DEPDIR)/gbfthml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfthml.o `test -f $(filtersdir)/gbfthml.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfthml.cpp

gbfthml.obj: $(filtersdir)/gbfthml.cpp
	source='$(filtersdir)/gbfthml.cpp' object='gbfthml.obj' libtool=no \
	depfile='$(DEPDIR)/gbfthml.Po' tmpdepfile='$(DEPDIR)/gbfthml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfthml.obj `cygpath -w $(filtersdir)/gbfthml.cpp`

gbfthml.lo: $(filtersdir)/gbfthml.cpp
	source='$(filtersdir)/gbfthml.cpp' object='gbfthml.lo' libtool=yes \
	depfile='$(DEPDIR)/gbfthml.Plo' tmpdepfile='$(DEPDIR)/gbfthml.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o gbfthml.lo `test -f $(filtersdir)/gbfthml.cpp || echo '$(srcdir)/'`$(filtersdir)/gbfthml.cpp

thmlgbf.o: $(filtersdir)/thmlgbf.cpp
	source='$(filtersdir)/thmlgbf.cpp' object='thmlgbf.o' libtool=no \
	depfile='$(DEPDIR)/thmlgbf.Po' tmpdepfile='$(DEPDIR)/thmlgbf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlgbf.o `test -f $(filtersdir)/thmlgbf.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlgbf.cpp

thmlgbf.obj: $(filtersdir)/thmlgbf.cpp
	source='$(filtersdir)/thmlgbf.cpp' object='thmlgbf.obj' libtool=no \
	depfile='$(DEPDIR)/thmlgbf.Po' tmpdepfile='$(DEPDIR)/thmlgbf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlgbf.obj `cygpath -w $(filtersdir)/thmlgbf.cpp`

thmlgbf.lo: $(filtersdir)/thmlgbf.cpp
	source='$(filtersdir)/thmlgbf.cpp' object='thmlgbf.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlgbf.Plo' tmpdepfile='$(DEPDIR)/thmlgbf.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlgbf.lo `test -f $(filtersdir)/thmlgbf.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlgbf.cpp

thmlrtf.o: $(filtersdir)/thmlrtf.cpp
	source='$(filtersdir)/thmlrtf.cpp' object='thmlrtf.o' libtool=no \
	depfile='$(DEPDIR)/thmlrtf.Po' tmpdepfile='$(DEPDIR)/thmlrtf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlrtf.o `test -f $(filtersdir)/thmlrtf.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlrtf.cpp

thmlrtf.obj: $(filtersdir)/thmlrtf.cpp
	source='$(filtersdir)/thmlrtf.cpp' object='thmlrtf.obj' libtool=no \
	depfile='$(DEPDIR)/thmlrtf.Po' tmpdepfile='$(DEPDIR)/thmlrtf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlrtf.obj `cygpath -w $(filtersdir)/thmlrtf.cpp`

thmlrtf.lo: $(filtersdir)/thmlrtf.cpp
	source='$(filtersdir)/thmlrtf.cpp' object='thmlrtf.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlrtf.Plo' tmpdepfile='$(DEPDIR)/thmlrtf.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlrtf.lo `test -f $(filtersdir)/thmlrtf.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlrtf.cpp

thmlhtml.o: $(filtersdir)/thmlhtml.cpp
	source='$(filtersdir)/thmlhtml.cpp' object='thmlhtml.o' libtool=no \
	depfile='$(DEPDIR)/thmlhtml.Po' tmpdepfile='$(DEPDIR)/thmlhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlhtml.o `test -f $(filtersdir)/thmlhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlhtml.cpp

thmlhtml.obj: $(filtersdir)/thmlhtml.cpp
	source='$(filtersdir)/thmlhtml.cpp' object='thmlhtml.obj' libtool=no \
	depfile='$(DEPDIR)/thmlhtml.Po' tmpdepfile='$(DEPDIR)/thmlhtml.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlhtml.obj `cygpath -w $(filtersdir)/thmlhtml.cpp`

thmlhtml.lo: $(filtersdir)/thmlhtml.cpp
	source='$(filtersdir)/thmlhtml.cpp' object='thmlhtml.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlhtml.Plo' tmpdepfile='$(DEPDIR)/thmlhtml.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlhtml.lo `test -f $(filtersdir)/thmlhtml.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlhtml.cpp

thmlhtmlhref.o: $(filtersdir)/thmlhtmlhref.cpp
	source='$(filtersdir)/thmlhtmlhref.cpp' object='thmlhtmlhref.o' libtool=no \
	depfile='$(DEPDIR)/thmlhtmlhref.Po' tmpdepfile='$(DEPDIR)/thmlhtmlhref.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlhtmlhref.o `test -f $(filtersdir)/thmlhtmlhref.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlhtmlhref.cpp

thmlhtmlhref.obj: $(filtersdir)/thmlhtmlhref.cpp
	source='$(filtersdir)/thmlhtmlhref.cpp' object='thmlhtmlhref.obj' libtool=no \
	depfile='$(DEPDIR)/thmlhtmlhref.Po' tmpdepfile='$(DEPDIR)/thmlhtmlhref.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlhtmlhref.obj `cygpath -w $(filtersdir)/thmlhtmlhref.cpp`

thmlhtmlhref.lo: $(filtersdir)/thmlhtmlhref.cpp
	source='$(filtersdir)/thmlhtmlhref.cpp' object='thmlhtmlhref.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlhtmlhref.Plo' tmpdepfile='$(DEPDIR)/thmlhtmlhref.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlhtmlhref.lo `test -f $(filtersdir)/thmlhtmlhref.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlhtmlhref.cpp

thmlplain.o: $(filtersdir)/thmlplain.cpp
	source='$(filtersdir)/thmlplain.cpp' object='thmlplain.o' libtool=no \
	depfile='$(DEPDIR)/thmlplain.Po' tmpdepfile='$(DEPDIR)/thmlplain.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlplain.o `test -f $(filtersdir)/thmlplain.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlplain.cpp

thmlplain.obj: $(filtersdir)/thmlplain.cpp
	source='$(filtersdir)/thmlplain.cpp' object='thmlplain.obj' libtool=no \
	depfile='$(DEPDIR)/thmlplain.Po' tmpdepfile='$(DEPDIR)/thmlplain.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlplain.obj `cygpath -w $(filtersdir)/thmlplain.cpp`

thmlplain.lo: $(filtersdir)/thmlplain.cpp
	source='$(filtersdir)/thmlplain.cpp' object='thmlplain.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlplain.Plo' tmpdepfile='$(DEPDIR)/thmlplain.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlplain.lo `test -f $(filtersdir)/thmlplain.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlplain.cpp

unicodertf.o: $(filtersdir)/unicodertf.cpp
	source='$(filtersdir)/unicodertf.cpp' object='unicodertf.o' libtool=no \
	depfile='$(DEPDIR)/unicodertf.Po' tmpdepfile='$(DEPDIR)/unicodertf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o unicodertf.o `test -f $(filtersdir)/unicodertf.cpp || echo '$(srcdir)/'`$(filtersdir)/unicodertf.cpp

unicodertf.obj: $(filtersdir)/unicodertf.cpp
	source='$(filtersdir)/unicodertf.cpp' object='unicodertf.obj' libtool=no \
	depfile='$(DEPDIR)/unicodertf.Po' tmpdepfile='$(DEPDIR)/unicodertf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o unicodertf.obj `cygpath -w $(filtersdir)/unicodertf.cpp`

unicodertf.lo: $(filtersdir)/unicodertf.cpp
	source='$(filtersdir)/unicodertf.cpp' object='unicodertf.lo' libtool=yes \
	depfile='$(DEPDIR)/unicodertf.Plo' tmpdepfile='$(DEPDIR)/unicodertf.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o unicodertf.lo `test -f $(filtersdir)/unicodertf.cpp || echo '$(srcdir)/'`$(filtersdir)/unicodertf.cpp

latin1utf8.o: $(filtersdir)/latin1utf8.cpp
	source='$(filtersdir)/latin1utf8.cpp' object='latin1utf8.o' libtool=no \
	depfile='$(DEPDIR)/latin1utf8.Po' tmpdepfile='$(DEPDIR)/latin1utf8.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o latin1utf8.o `test -f $(filtersdir)/latin1utf8.cpp || echo '$(srcdir)/'`$(filtersdir)/latin1utf8.cpp

latin1utf8.obj: $(filtersdir)/latin1utf8.cpp
	source='$(filtersdir)/latin1utf8.cpp' object='latin1utf8.obj' libtool=no \
	depfile='$(DEPDIR)/latin1utf8.Po' tmpdepfile='$(DEPDIR)/latin1utf8.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o latin1utf8.obj `cygpath -w $(filtersdir)/latin1utf8.cpp`

latin1utf8.lo: $(filtersdir)/latin1utf8.cpp
	source='$(filtersdir)/latin1utf8.cpp' object='latin1utf8.lo' libtool=yes \
	depfile='$(DEPDIR)/latin1utf8.Plo' tmpdepfile='$(DEPDIR)/latin1utf8.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o latin1utf8.lo `test -f $(filtersdir)/latin1utf8.cpp || echo '$(srcdir)/'`$(filtersdir)/latin1utf8.cpp

latin1utf16.o: $(filtersdir)/latin1utf16.cpp
	source='$(filtersdir)/latin1utf16.cpp' object='latin1utf16.o' libtool=no \
	depfile='$(DEPDIR)/latin1utf16.Po' tmpdepfile='$(DEPDIR)/latin1utf16.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o latin1utf16.o `test -f $(filtersdir)/latin1utf16.cpp || echo '$(srcdir)/'`$(filtersdir)/latin1utf16.cpp

latin1utf16.obj: $(filtersdir)/latin1utf16.cpp
	source='$(filtersdir)/latin1utf16.cpp' object='latin1utf16.obj' libtool=no \
	depfile='$(DEPDIR)/latin1utf16.Po' tmpdepfile='$(DEPDIR)/latin1utf16.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o latin1utf16.obj `cygpath -w $(filtersdir)/latin1utf16.cpp`

latin1utf16.lo: $(filtersdir)/latin1utf16.cpp
	source='$(filtersdir)/latin1utf16.cpp' object='latin1utf16.lo' libtool=yes \
	depfile='$(DEPDIR)/latin1utf16.Plo' tmpdepfile='$(DEPDIR)/latin1utf16.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o latin1utf16.lo `test -f $(filtersdir)/latin1utf16.cpp || echo '$(srcdir)/'`$(filtersdir)/latin1utf16.cpp

utf8utf16.o: $(filtersdir)/utf8utf16.cpp
	source='$(filtersdir)/utf8utf16.cpp' object='utf8utf16.o' libtool=no \
	depfile='$(DEPDIR)/utf8utf16.Po' tmpdepfile='$(DEPDIR)/utf8utf16.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8utf16.o `test -f $(filtersdir)/utf8utf16.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8utf16.cpp

utf8utf16.obj: $(filtersdir)/utf8utf16.cpp
	source='$(filtersdir)/utf8utf16.cpp' object='utf8utf16.obj' libtool=no \
	depfile='$(DEPDIR)/utf8utf16.Po' tmpdepfile='$(DEPDIR)/utf8utf16.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8utf16.obj `cygpath -w $(filtersdir)/utf8utf16.cpp`

utf8utf16.lo: $(filtersdir)/utf8utf16.cpp
	source='$(filtersdir)/utf8utf16.cpp' object='utf8utf16.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8utf16.Plo' tmpdepfile='$(DEPDIR)/utf8utf16.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8utf16.lo `test -f $(filtersdir)/utf8utf16.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8utf16.cpp

utf16utf8.o: $(filtersdir)/utf16utf8.cpp
	source='$(filtersdir)/utf16utf8.cpp' object='utf16utf8.o' libtool=no \
	depfile='$(DEPDIR)/utf16utf8.Po' tmpdepfile='$(DEPDIR)/utf16utf8.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf16utf8.o `test -f $(filtersdir)/utf16utf8.cpp || echo '$(srcdir)/'`$(filtersdir)/utf16utf8.cpp

utf16utf8.obj: $(filtersdir)/utf16utf8.cpp
	source='$(filtersdir)/utf16utf8.cpp' object='utf16utf8.obj' libtool=no \
	depfile='$(DEPDIR)/utf16utf8.Po' tmpdepfile='$(DEPDIR)/utf16utf8.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf16utf8.obj `cygpath -w $(filtersdir)/utf16utf8.cpp`

utf16utf8.lo: $(filtersdir)/utf16utf8.cpp
	source='$(filtersdir)/utf16utf8.cpp' object='utf16utf8.lo' libtool=yes \
	depfile='$(DEPDIR)/utf16utf8.Plo' tmpdepfile='$(DEPDIR)/utf16utf8.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf16utf8.lo `test -f $(filtersdir)/utf16utf8.cpp || echo '$(srcdir)/'`$(filtersdir)/utf16utf8.cpp

scsuutf8.o: $(filtersdir)/scsuutf8.cpp
	source='$(filtersdir)/scsuutf8.cpp' object='scsuutf8.o' libtool=no \
	depfile='$(DEPDIR)/scsuutf8.Po' tmpdepfile='$(DEPDIR)/scsuutf8.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o scsuutf8.o `test -f $(filtersdir)/scsuutf8.cpp || echo '$(srcdir)/'`$(filtersdir)/scsuutf8.cpp

scsuutf8.obj: $(filtersdir)/scsuutf8.cpp
	source='$(filtersdir)/scsuutf8.cpp' object='scsuutf8.obj' libtool=no \
	depfile='$(DEPDIR)/scsuutf8.Po' tmpdepfile='$(DEPDIR)/scsuutf8.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o scsuutf8.obj `cygpath -w $(filtersdir)/scsuutf8.cpp`

scsuutf8.lo: $(filtersdir)/scsuutf8.cpp
	source='$(filtersdir)/scsuutf8.cpp' object='scsuutf8.lo' libtool=yes \
	depfile='$(DEPDIR)/scsuutf8.Plo' tmpdepfile='$(DEPDIR)/scsuutf8.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o scsuutf8.lo `test -f $(filtersdir)/scsuutf8.cpp || echo '$(srcdir)/'`$(filtersdir)/scsuutf8.cpp

utf8html.o: $(filtersdir)/utf8html.cpp
	source='$(filtersdir)/utf8html.cpp' object='utf8html.o' libtool=no \
	depfile='$(DEPDIR)/utf8html.Po' tmpdepfile='$(DEPDIR)/utf8html.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8html.o `test -f $(filtersdir)/utf8html.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8html.cpp

utf8html.obj: $(filtersdir)/utf8html.cpp
	source='$(filtersdir)/utf8html.cpp' object='utf8html.obj' libtool=no \
	depfile='$(DEPDIR)/utf8html.Po' tmpdepfile='$(DEPDIR)/utf8html.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8html.obj `cygpath -w $(filtersdir)/utf8html.cpp`

utf8html.lo: $(filtersdir)/utf8html.cpp
	source='$(filtersdir)/utf8html.cpp' object='utf8html.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8html.Plo' tmpdepfile='$(DEPDIR)/utf8html.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8html.lo `test -f $(filtersdir)/utf8html.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8html.cpp

utf8latin1.o: $(filtersdir)/utf8latin1.cpp
	source='$(filtersdir)/utf8latin1.cpp' object='utf8latin1.o' libtool=no \
	depfile='$(DEPDIR)/utf8latin1.Po' tmpdepfile='$(DEPDIR)/utf8latin1.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8latin1.o `test -f $(filtersdir)/utf8latin1.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8latin1.cpp

utf8latin1.obj: $(filtersdir)/utf8latin1.cpp
	source='$(filtersdir)/utf8latin1.cpp' object='utf8latin1.obj' libtool=no \
	depfile='$(DEPDIR)/utf8latin1.Po' tmpdepfile='$(DEPDIR)/utf8latin1.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8latin1.obj `cygpath -w $(filtersdir)/utf8latin1.cpp`

utf8latin1.lo: $(filtersdir)/utf8latin1.cpp
	source='$(filtersdir)/utf8latin1.cpp' object='utf8latin1.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8latin1.Plo' tmpdepfile='$(DEPDIR)/utf8latin1.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8latin1.lo `test -f $(filtersdir)/utf8latin1.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8latin1.cpp

thmlolb.o: $(filtersdir)/thmlolb.cpp
	source='$(filtersdir)/thmlolb.cpp' object='thmlolb.o' libtool=no \
	depfile='$(DEPDIR)/thmlolb.Po' tmpdepfile='$(DEPDIR)/thmlolb.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlolb.o `test -f $(filtersdir)/thmlolb.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlolb.cpp

thmlolb.obj: $(filtersdir)/thmlolb.cpp
	source='$(filtersdir)/thmlolb.cpp' object='thmlolb.obj' libtool=no \
	depfile='$(DEPDIR)/thmlolb.Po' tmpdepfile='$(DEPDIR)/thmlolb.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlolb.obj `cygpath -w $(filtersdir)/thmlolb.cpp`

thmlolb.lo: $(filtersdir)/thmlolb.cpp
	source='$(filtersdir)/thmlolb.cpp' object='thmlolb.lo' libtool=yes \
	depfile='$(DEPDIR)/thmlolb.Plo' tmpdepfile='$(DEPDIR)/thmlolb.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o thmlolb.lo `test -f $(filtersdir)/thmlolb.cpp || echo '$(srcdir)/'`$(filtersdir)/thmlolb.cpp

utf8cantillation.o: $(filtersdir)/utf8cantillation.cpp
	source='$(filtersdir)/utf8cantillation.cpp' object='utf8cantillation.o' libtool=no \
	depfile='$(DEPDIR)/utf8cantillation.Po' tmpdepfile='$(DEPDIR)/utf8cantillation.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8cantillation.o `test -f $(filtersdir)/utf8cantillation.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8cantillation.cpp

utf8cantillation.obj: $(filtersdir)/utf8cantillation.cpp
	source='$(filtersdir)/utf8cantillation.cpp' object='utf8cantillation.obj' libtool=no \
	depfile='$(DEPDIR)/utf8cantillation.Po' tmpdepfile='$(DEPDIR)/utf8cantillation.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8cantillation.obj `cygpath -w $(filtersdir)/utf8cantillation.cpp`

utf8cantillation.lo: $(filtersdir)/utf8cantillation.cpp
	source='$(filtersdir)/utf8cantillation.cpp' object='utf8cantillation.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8cantillation.Plo' tmpdepfile='$(DEPDIR)/utf8cantillation.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8cantillation.lo `test -f $(filtersdir)/utf8cantillation.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8cantillation.cpp

utf8hebrewpoints.o: $(filtersdir)/utf8hebrewpoints.cpp
	source='$(filtersdir)/utf8hebrewpoints.cpp' object='utf8hebrewpoints.o' libtool=no \
	depfile='$(DEPDIR)/utf8hebrewpoints.Po' tmpdepfile='$(DEPDIR)/utf8hebrewpoints.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8hebrewpoints.o `test -f $(filtersdir)/utf8hebrewpoints.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8hebrewpoints.cpp

utf8hebrewpoints.obj: $(filtersdir)/utf8hebrewpoints.cpp
	source='$(filtersdir)/utf8hebrewpoints.cpp' object='utf8hebrewpoints.obj' libtool=no \
	depfile='$(DEPDIR)/utf8hebrewpoints.Po' tmpdepfile='$(DEPDIR)/utf8hebrewpoints.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8hebrewpoints.obj `cygpath -w $(filtersdir)/utf8hebrewpoints.cpp`

utf8hebrewpoints.lo: $(filtersdir)/utf8hebrewpoints.cpp
	source='$(filtersdir)/utf8hebrewpoints.cpp' object='utf8hebrewpoints.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8hebrewpoints.Plo' tmpdepfile='$(DEPDIR)/utf8hebrewpoints.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8hebrewpoints.lo `test -f $(filtersdir)/utf8hebrewpoints.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8hebrewpoints.cpp

utf8greekaccents.o: $(filtersdir)/utf8greekaccents.cpp
	source='$(filtersdir)/utf8greekaccents.cpp' object='utf8greekaccents.o' libtool=no \
	depfile='$(DEPDIR)/utf8greekaccents.Po' tmpdepfile='$(DEPDIR)/utf8greekaccents.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8greekaccents.o `test -f $(filtersdir)/utf8greekaccents.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8greekaccents.cpp

utf8greekaccents.obj: $(filtersdir)/utf8greekaccents.cpp
	source='$(filtersdir)/utf8greekaccents.cpp' object='utf8greekaccents.obj' libtool=no \
	depfile='$(DEPDIR)/utf8greekaccents.Po' tmpdepfile='$(DEPDIR)/utf8greekaccents.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8greekaccents.obj `cygpath -w $(filtersdir)/utf8greekaccents.cpp`

utf8greekaccents.lo: $(filtersdir)/utf8greekaccents.cpp
	source='$(filtersdir)/utf8greekaccents.cpp' object='utf8greekaccents.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8greekaccents.Plo' tmpdepfile='$(DEPDIR)/utf8greekaccents.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8greekaccents.lo `test -f $(filtersdir)/utf8greekaccents.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8greekaccents.cpp

swgenbook.o: $(genbookdir)/swgenbook.cpp
	source='$(genbookdir)/swgenbook.cpp' object='swgenbook.o' libtool=no \
	depfile='$(DEPDIR)/swgenbook.Po' tmpdepfile='$(DEPDIR)/swgenbook.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swgenbook.o `test -f $(genbookdir)/swgenbook.cpp || echo '$(srcdir)/'`$(genbookdir)/swgenbook.cpp

swgenbook.obj: $(genbookdir)/swgenbook.cpp
	source='$(genbookdir)/swgenbook.cpp' object='swgenbook.obj' libtool=no \
	depfile='$(DEPDIR)/swgenbook.Po' tmpdepfile='$(DEPDIR)/swgenbook.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swgenbook.obj `cygpath -w $(genbookdir)/swgenbook.cpp`

swgenbook.lo: $(genbookdir)/swgenbook.cpp
	source='$(genbookdir)/swgenbook.cpp' object='swgenbook.lo' libtool=yes \
	depfile='$(DEPDIR)/swgenbook.Plo' tmpdepfile='$(DEPDIR)/swgenbook.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swgenbook.lo `test -f $(genbookdir)/swgenbook.cpp || echo '$(srcdir)/'`$(genbookdir)/swgenbook.cpp

rawgenbook.o: $(rawgenbookdir)/rawgenbook.cpp
	source='$(rawgenbookdir)/rawgenbook.cpp' object='rawgenbook.o' libtool=no \
	depfile='$(DEPDIR)/rawgenbook.Po' tmpdepfile='$(DEPDIR)/rawgenbook.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawgenbook.o `test -f $(rawgenbookdir)/rawgenbook.cpp || echo '$(srcdir)/'`$(rawgenbookdir)/rawgenbook.cpp

rawgenbook.obj: $(rawgenbookdir)/rawgenbook.cpp
	source='$(rawgenbookdir)/rawgenbook.cpp' object='rawgenbook.obj' libtool=no \
	depfile='$(DEPDIR)/rawgenbook.Po' tmpdepfile='$(DEPDIR)/rawgenbook.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawgenbook.obj `cygpath -w $(rawgenbookdir)/rawgenbook.cpp`

rawgenbook.lo: $(rawgenbookdir)/rawgenbook.cpp
	source='$(rawgenbookdir)/rawgenbook.cpp' object='rawgenbook.lo' libtool=yes \
	depfile='$(DEPDIR)/rawgenbook.Plo' tmpdepfile='$(DEPDIR)/rawgenbook.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawgenbook.lo `test -f $(rawgenbookdir)/rawgenbook.cpp || echo '$(srcdir)/'`$(rawgenbookdir)/rawgenbook.cpp

swtext.o: $(textsdir)/swtext.cpp
	source='$(textsdir)/swtext.cpp' object='swtext.o' libtool=no \
	depfile='$(DEPDIR)/swtext.Po' tmpdepfile='$(DEPDIR)/swtext.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swtext.o `test -f $(textsdir)/swtext.cpp || echo '$(srcdir)/'`$(textsdir)/swtext.cpp

swtext.obj: $(textsdir)/swtext.cpp
	source='$(textsdir)/swtext.cpp' object='swtext.obj' libtool=no \
	depfile='$(DEPDIR)/swtext.Po' tmpdepfile='$(DEPDIR)/swtext.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swtext.obj `cygpath -w $(textsdir)/swtext.cpp`

swtext.lo: $(textsdir)/swtext.cpp
	source='$(textsdir)/swtext.cpp' object='swtext.lo' libtool=yes \
	depfile='$(DEPDIR)/swtext.Plo' tmpdepfile='$(DEPDIR)/swtext.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swtext.lo `test -f $(textsdir)/swtext.cpp || echo '$(srcdir)/'`$(textsdir)/swtext.cpp

rawtext.o: $(rawtextdir)/rawtext.cpp
	source='$(rawtextdir)/rawtext.cpp' object='rawtext.o' libtool=no \
	depfile='$(DEPDIR)/rawtext.Po' tmpdepfile='$(DEPDIR)/rawtext.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawtext.o `test -f $(rawtextdir)/rawtext.cpp || echo '$(srcdir)/'`$(rawtextdir)/rawtext.cpp

rawtext.obj: $(rawtextdir)/rawtext.cpp
	source='$(rawtextdir)/rawtext.cpp' object='rawtext.obj' libtool=no \
	depfile='$(DEPDIR)/rawtext.Po' tmpdepfile='$(DEPDIR)/rawtext.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawtext.obj `cygpath -w $(rawtextdir)/rawtext.cpp`

rawtext.lo: $(rawtextdir)/rawtext.cpp
	source='$(rawtextdir)/rawtext.cpp' object='rawtext.lo' libtool=yes \
	depfile='$(DEPDIR)/rawtext.Plo' tmpdepfile='$(DEPDIR)/rawtext.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawtext.lo `test -f $(rawtextdir)/rawtext.cpp || echo '$(srcdir)/'`$(rawtextdir)/rawtext.cpp

ztext.o: $(ztextdir)/ztext.cpp
	source='$(ztextdir)/ztext.cpp' object='ztext.o' libtool=no \
	depfile='$(DEPDIR)/ztext.Po' tmpdepfile='$(DEPDIR)/ztext.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o ztext.o `test -f $(ztextdir)/ztext.cpp || echo '$(srcdir)/'`$(ztextdir)/ztext.cpp

ztext.obj: $(ztextdir)/ztext.cpp
	source='$(ztextdir)/ztext.cpp' object='ztext.obj' libtool=no \
	depfile='$(DEPDIR)/ztext.Po' tmpdepfile='$(DEPDIR)/ztext.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o ztext.obj `cygpath -w $(ztextdir)/ztext.cpp`

ztext.lo: $(ztextdir)/ztext.cpp
	source='$(ztextdir)/ztext.cpp' object='ztext.lo' libtool=yes \
	depfile='$(DEPDIR)/ztext.Plo' tmpdepfile='$(DEPDIR)/ztext.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o ztext.lo `test -f $(ztextdir)/ztext.cpp || echo '$(srcdir)/'`$(ztextdir)/ztext.cpp

rawgbf.o: $(rawgbfdir)/rawgbf.cpp
	source='$(rawgbfdir)/rawgbf.cpp' object='rawgbf.o' libtool=no \
	depfile='$(DEPDIR)/rawgbf.Po' tmpdepfile='$(DEPDIR)/rawgbf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawgbf.o `test -f $(rawgbfdir)/rawgbf.cpp || echo '$(srcdir)/'`$(rawgbfdir)/rawgbf.cpp

rawgbf.obj: $(rawgbfdir)/rawgbf.cpp
	source='$(rawgbfdir)/rawgbf.cpp' object='rawgbf.obj' libtool=no \
	depfile='$(DEPDIR)/rawgbf.Po' tmpdepfile='$(DEPDIR)/rawgbf.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawgbf.obj `cygpath -w $(rawgbfdir)/rawgbf.cpp`

rawgbf.lo: $(rawgbfdir)/rawgbf.cpp
	source='$(rawgbfdir)/rawgbf.cpp' object='rawgbf.lo' libtool=yes \
	depfile='$(DEPDIR)/rawgbf.Plo' tmpdepfile='$(DEPDIR)/rawgbf.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawgbf.lo `test -f $(rawgbfdir)/rawgbf.cpp || echo '$(srcdir)/'`$(rawgbfdir)/rawgbf.cpp

swcom.o: $(commentsdir)/swcom.cpp
	source='$(commentsdir)/swcom.cpp' object='swcom.o' libtool=no \
	depfile='$(DEPDIR)/swcom.Po' tmpdepfile='$(DEPDIR)/swcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcom.o `test -f $(commentsdir)/swcom.cpp || echo '$(srcdir)/'`$(commentsdir)/swcom.cpp

swcom.obj: $(commentsdir)/swcom.cpp
	source='$(commentsdir)/swcom.cpp' object='swcom.obj' libtool=no \
	depfile='$(DEPDIR)/swcom.Po' tmpdepfile='$(DEPDIR)/swcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcom.obj `cygpath -w $(commentsdir)/swcom.cpp`

swcom.lo: $(commentsdir)/swcom.cpp
	source='$(commentsdir)/swcom.cpp' object='swcom.lo' libtool=yes \
	depfile='$(DEPDIR)/swcom.Plo' tmpdepfile='$(DEPDIR)/swcom.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swcom.lo `test -f $(commentsdir)/swcom.cpp || echo '$(srcdir)/'`$(commentsdir)/swcom.cpp

rawcom.o: $(rawcomdir)/rawcom.cpp
	source='$(rawcomdir)/rawcom.cpp' object='rawcom.o' libtool=no \
	depfile='$(DEPDIR)/rawcom.Po' tmpdepfile='$(DEPDIR)/rawcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawcom.o `test -f $(rawcomdir)/rawcom.cpp || echo '$(srcdir)/'`$(rawcomdir)/rawcom.cpp

rawcom.obj: $(rawcomdir)/rawcom.cpp
	source='$(rawcomdir)/rawcom.cpp' object='rawcom.obj' libtool=no \
	depfile='$(DEPDIR)/rawcom.Po' tmpdepfile='$(DEPDIR)/rawcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawcom.obj `cygpath -w $(rawcomdir)/rawcom.cpp`

rawcom.lo: $(rawcomdir)/rawcom.cpp
	source='$(rawcomdir)/rawcom.cpp' object='rawcom.lo' libtool=yes \
	depfile='$(DEPDIR)/rawcom.Plo' tmpdepfile='$(DEPDIR)/rawcom.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawcom.lo `test -f $(rawcomdir)/rawcom.cpp || echo '$(srcdir)/'`$(rawcomdir)/rawcom.cpp

rawfiles.o: $(rawfilesdir)/rawfiles.cpp
	source='$(rawfilesdir)/rawfiles.cpp' object='rawfiles.o' libtool=no \
	depfile='$(DEPDIR)/rawfiles.Po' tmpdepfile='$(DEPDIR)/rawfiles.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawfiles.o `test -f $(rawfilesdir)/rawfiles.cpp || echo '$(srcdir)/'`$(rawfilesdir)/rawfiles.cpp

rawfiles.obj: $(rawfilesdir)/rawfiles.cpp
	source='$(rawfilesdir)/rawfiles.cpp' object='rawfiles.obj' libtool=no \
	depfile='$(DEPDIR)/rawfiles.Po' tmpdepfile='$(DEPDIR)/rawfiles.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawfiles.obj `cygpath -w $(rawfilesdir)/rawfiles.cpp`

rawfiles.lo: $(rawfilesdir)/rawfiles.cpp
	source='$(rawfilesdir)/rawfiles.cpp' object='rawfiles.lo' libtool=yes \
	depfile='$(DEPDIR)/rawfiles.Plo' tmpdepfile='$(DEPDIR)/rawfiles.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawfiles.lo `test -f $(rawfilesdir)/rawfiles.cpp || echo '$(srcdir)/'`$(rawfilesdir)/rawfiles.cpp

zcom.o: $(zcomdir)/zcom.cpp
	source='$(zcomdir)/zcom.cpp' object='zcom.o' libtool=no \
	depfile='$(DEPDIR)/zcom.Po' tmpdepfile='$(DEPDIR)/zcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zcom.o `test -f $(zcomdir)/zcom.cpp || echo '$(srcdir)/'`$(zcomdir)/zcom.cpp

zcom.obj: $(zcomdir)/zcom.cpp
	source='$(zcomdir)/zcom.cpp' object='zcom.obj' libtool=no \
	depfile='$(DEPDIR)/zcom.Po' tmpdepfile='$(DEPDIR)/zcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zcom.obj `cygpath -w $(zcomdir)/zcom.cpp`

zcom.lo: $(zcomdir)/zcom.cpp
	source='$(zcomdir)/zcom.cpp' object='zcom.lo' libtool=yes \
	depfile='$(DEPDIR)/zcom.Plo' tmpdepfile='$(DEPDIR)/zcom.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zcom.lo `test -f $(zcomdir)/zcom.cpp || echo '$(srcdir)/'`$(zcomdir)/zcom.cpp

hrefcom.o: $(hrefcomdir)/hrefcom.cpp
	source='$(hrefcomdir)/hrefcom.cpp' object='hrefcom.o' libtool=no \
	depfile='$(DEPDIR)/hrefcom.Po' tmpdepfile='$(DEPDIR)/hrefcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o hrefcom.o `test -f $(hrefcomdir)/hrefcom.cpp || echo '$(srcdir)/'`$(hrefcomdir)/hrefcom.cpp

hrefcom.obj: $(hrefcomdir)/hrefcom.cpp
	source='$(hrefcomdir)/hrefcom.cpp' object='hrefcom.obj' libtool=no \
	depfile='$(DEPDIR)/hrefcom.Po' tmpdepfile='$(DEPDIR)/hrefcom.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o hrefcom.obj `cygpath -w $(hrefcomdir)/hrefcom.cpp`

hrefcom.lo: $(hrefcomdir)/hrefcom.cpp
	source='$(hrefcomdir)/hrefcom.cpp' object='hrefcom.lo' libtool=yes \
	depfile='$(DEPDIR)/hrefcom.Plo' tmpdepfile='$(DEPDIR)/hrefcom.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o hrefcom.lo `test -f $(hrefcomdir)/hrefcom.cpp || echo '$(srcdir)/'`$(hrefcomdir)/hrefcom.cpp

swld.o: $(lexdictdir)/swld.cpp
	source='$(lexdictdir)/swld.cpp' object='swld.o' libtool=no \
	depfile='$(DEPDIR)/swld.Po' tmpdepfile='$(DEPDIR)/swld.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swld.o `test -f $(lexdictdir)/swld.cpp || echo '$(srcdir)/'`$(lexdictdir)/swld.cpp

swld.obj: $(lexdictdir)/swld.cpp
	source='$(lexdictdir)/swld.cpp' object='swld.obj' libtool=no \
	depfile='$(DEPDIR)/swld.Po' tmpdepfile='$(DEPDIR)/swld.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swld.obj `cygpath -w $(lexdictdir)/swld.cpp`

swld.lo: $(lexdictdir)/swld.cpp
	source='$(lexdictdir)/swld.cpp' object='swld.lo' libtool=yes \
	depfile='$(DEPDIR)/swld.Plo' tmpdepfile='$(DEPDIR)/swld.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o swld.lo `test -f $(lexdictdir)/swld.cpp || echo '$(srcdir)/'`$(lexdictdir)/swld.cpp

rawld.o: $(rawlddir)/rawld.cpp
	source='$(rawlddir)/rawld.cpp' object='rawld.o' libtool=no \
	depfile='$(DEPDIR)/rawld.Po' tmpdepfile='$(DEPDIR)/rawld.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawld.o `test -f $(rawlddir)/rawld.cpp || echo '$(srcdir)/'`$(rawlddir)/rawld.cpp

rawld.obj: $(rawlddir)/rawld.cpp
	source='$(rawlddir)/rawld.cpp' object='rawld.obj' libtool=no \
	depfile='$(DEPDIR)/rawld.Po' tmpdepfile='$(DEPDIR)/rawld.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawld.obj `cygpath -w $(rawlddir)/rawld.cpp`

rawld.lo: $(rawlddir)/rawld.cpp
	source='$(rawlddir)/rawld.cpp' object='rawld.lo' libtool=yes \
	depfile='$(DEPDIR)/rawld.Plo' tmpdepfile='$(DEPDIR)/rawld.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawld.lo `test -f $(rawlddir)/rawld.cpp || echo '$(srcdir)/'`$(rawlddir)/rawld.cpp

rawld4.o: $(rawld4dir)/rawld4.cpp
	source='$(rawld4dir)/rawld4.cpp' object='rawld4.o' libtool=no \
	depfile='$(DEPDIR)/rawld4.Po' tmpdepfile='$(DEPDIR)/rawld4.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawld4.o `test -f $(rawld4dir)/rawld4.cpp || echo '$(srcdir)/'`$(rawld4dir)/rawld4.cpp

rawld4.obj: $(rawld4dir)/rawld4.cpp
	source='$(rawld4dir)/rawld4.cpp' object='rawld4.obj' libtool=no \
	depfile='$(DEPDIR)/rawld4.Po' tmpdepfile='$(DEPDIR)/rawld4.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawld4.obj `cygpath -w $(rawld4dir)/rawld4.cpp`

rawld4.lo: $(rawld4dir)/rawld4.cpp
	source='$(rawld4dir)/rawld4.cpp' object='rawld4.lo' libtool=yes \
	depfile='$(DEPDIR)/rawld4.Plo' tmpdepfile='$(DEPDIR)/rawld4.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o rawld4.lo `test -f $(rawld4dir)/rawld4.cpp || echo '$(srcdir)/'`$(rawld4dir)/rawld4.cpp

zld.o: $(zlddir)/zld.cpp
	source='$(zlddir)/zld.cpp' object='zld.o' libtool=no \
	depfile='$(DEPDIR)/zld.Po' tmpdepfile='$(DEPDIR)/zld.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zld.o `test -f $(zlddir)/zld.cpp || echo '$(srcdir)/'`$(zlddir)/zld.cpp

zld.obj: $(zlddir)/zld.cpp
	source='$(zlddir)/zld.cpp' object='zld.obj' libtool=no \
	depfile='$(DEPDIR)/zld.Po' tmpdepfile='$(DEPDIR)/zld.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zld.obj `cygpath -w $(zlddir)/zld.cpp`

zld.lo: $(zlddir)/zld.cpp
	source='$(zlddir)/zld.cpp' object='zld.lo' libtool=yes \
	depfile='$(DEPDIR)/zld.Plo' tmpdepfile='$(DEPDIR)/zld.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o zld.lo `test -f $(zlddir)/zld.cpp || echo '$(srcdir)/'`$(zlddir)/zld.cpp

flatapi.o: $(bindingsdir)/flatapi.cpp
	source='$(bindingsdir)/flatapi.cpp' object='flatapi.o' libtool=no \
	depfile='$(DEPDIR)/flatapi.Po' tmpdepfile='$(DEPDIR)/flatapi.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o flatapi.o `test -f $(bindingsdir)/flatapi.cpp || echo '$(srcdir)/'`$(bindingsdir)/flatapi.cpp

flatapi.obj: $(bindingsdir)/flatapi.cpp
	source='$(bindingsdir)/flatapi.cpp' object='flatapi.obj' libtool=no \
	depfile='$(DEPDIR)/flatapi.Po' tmpdepfile='$(DEPDIR)/flatapi.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o flatapi.obj `cygpath -w $(bindingsdir)/flatapi.cpp`

flatapi.lo: $(bindingsdir)/flatapi.cpp
	source='$(bindingsdir)/flatapi.cpp' object='flatapi.lo' libtool=yes \
	depfile='$(DEPDIR)/flatapi.Plo' tmpdepfile='$(DEPDIR)/flatapi.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o flatapi.lo `test -f $(bindingsdir)/flatapi.cpp || echo '$(srcdir)/'`$(bindingsdir)/flatapi.cpp

utf8transliterator.o: $(filtersdir)/utf8transliterator.cpp
	source='$(filtersdir)/utf8transliterator.cpp' object='utf8transliterator.o' libtool=no \
	depfile='$(DEPDIR)/utf8transliterator.Po' tmpdepfile='$(DEPDIR)/utf8transliterator.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8transliterator.o `test -f $(filtersdir)/utf8transliterator.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8transliterator.cpp

utf8transliterator.obj: $(filtersdir)/utf8transliterator.cpp
	source='$(filtersdir)/utf8transliterator.cpp' object='utf8transliterator.obj' libtool=no \
	depfile='$(DEPDIR)/utf8transliterator.Po' tmpdepfile='$(DEPDIR)/utf8transliterator.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8transliterator.obj `cygpath -w $(filtersdir)/utf8transliterator.cpp`

utf8transliterator.lo: $(filtersdir)/utf8transliterator.cpp
	source='$(filtersdir)/utf8transliterator.cpp' object='utf8transliterator.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8transliterator.Plo' tmpdepfile='$(DEPDIR)/utf8transliterator.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8transliterator.lo `test -f $(filtersdir)/utf8transliterator.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8transliterator.cpp

utf8nfc.o: $(filtersdir)/utf8nfc.cpp
	source='$(filtersdir)/utf8nfc.cpp' object='utf8nfc.o' libtool=no \
	depfile='$(DEPDIR)/utf8nfc.Po' tmpdepfile='$(DEPDIR)/utf8nfc.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8nfc.o `test -f $(filtersdir)/utf8nfc.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8nfc.cpp

utf8nfc.obj: $(filtersdir)/utf8nfc.cpp
	source='$(filtersdir)/utf8nfc.cpp' object='utf8nfc.obj' libtool=no \
	depfile='$(DEPDIR)/utf8nfc.Po' tmpdepfile='$(DEPDIR)/utf8nfc.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8nfc.obj `cygpath -w $(filtersdir)/utf8nfc.cpp`

utf8nfc.lo: $(filtersdir)/utf8nfc.cpp
	source='$(filtersdir)/utf8nfc.cpp' object='utf8nfc.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8nfc.Plo' tmpdepfile='$(DEPDIR)/utf8nfc.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8nfc.lo `test -f $(filtersdir)/utf8nfc.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8nfc.cpp

utf8nfkd.o: $(filtersdir)/utf8nfkd.cpp
	source='$(filtersdir)/utf8nfkd.cpp' object='utf8nfkd.o' libtool=no \
	depfile='$(DEPDIR)/utf8nfkd.Po' tmpdepfile='$(DEPDIR)/utf8nfkd.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8nfkd.o `test -f $(filtersdir)/utf8nfkd.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8nfkd.cpp

utf8nfkd.obj: $(filtersdir)/utf8nfkd.cpp
	source='$(filtersdir)/utf8nfkd.cpp' object='utf8nfkd.obj' libtool=no \
	depfile='$(DEPDIR)/utf8nfkd.Po' tmpdepfile='$(DEPDIR)/utf8nfkd.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8nfkd.obj `cygpath -w $(filtersdir)/utf8nfkd.cpp`

utf8nfkd.lo: $(filtersdir)/utf8nfkd.cpp
	source='$(filtersdir)/utf8nfkd.cpp' object='utf8nfkd.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8nfkd.Plo' tmpdepfile='$(DEPDIR)/utf8nfkd.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8nfkd.lo `test -f $(filtersdir)/utf8nfkd.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8nfkd.cpp

utf8arshaping.o: $(filtersdir)/utf8arshaping.cpp
	source='$(filtersdir)/utf8arshaping.cpp' object='utf8arshaping.o' libtool=no \
	depfile='$(DEPDIR)/utf8arshaping.Po' tmpdepfile='$(DEPDIR)/utf8arshaping.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8arshaping.o `test -f $(filtersdir)/utf8arshaping.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8arshaping.cpp

utf8arshaping.obj: $(filtersdir)/utf8arshaping.cpp
	source='$(filtersdir)/utf8arshaping.cpp' object='utf8arshaping.obj' libtool=no \
	depfile='$(DEPDIR)/utf8arshaping.Po' tmpdepfile='$(DEPDIR)/utf8arshaping.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8arshaping.obj `cygpath -w $(filtersdir)/utf8arshaping.cpp`

utf8arshaping.lo: $(filtersdir)/utf8arshaping.cpp
	source='$(filtersdir)/utf8arshaping.cpp' object='utf8arshaping.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8arshaping.Plo' tmpdepfile='$(DEPDIR)/utf8arshaping.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8arshaping.lo `test -f $(filtersdir)/utf8arshaping.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8arshaping.cpp

utf8bidireorder.o: $(filtersdir)/utf8bidireorder.cpp
	source='$(filtersdir)/utf8bidireorder.cpp' object='utf8bidireorder.o' libtool=no \
	depfile='$(DEPDIR)/utf8bidireorder.Po' tmpdepfile='$(DEPDIR)/utf8bidireorder.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8bidireorder.o `test -f $(filtersdir)/utf8bidireorder.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8bidireorder.cpp

utf8bidireorder.obj: $(filtersdir)/utf8bidireorder.cpp
	source='$(filtersdir)/utf8bidireorder.cpp' object='utf8bidireorder.obj' libtool=no \
	depfile='$(DEPDIR)/utf8bidireorder.Po' tmpdepfile='$(DEPDIR)/utf8bidireorder.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8bidireorder.obj `cygpath -w $(filtersdir)/utf8bidireorder.cpp`

utf8bidireorder.lo: $(filtersdir)/utf8bidireorder.cpp
	source='$(filtersdir)/utf8bidireorder.cpp' object='utf8bidireorder.lo' libtool=yes \
	depfile='$(DEPDIR)/utf8bidireorder.Plo' tmpdepfile='$(DEPDIR)/utf8bidireorder.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o utf8bidireorder.lo `test -f $(filtersdir)/utf8bidireorder.cpp || echo '$(srcdir)/'`$(filtersdir)/utf8bidireorder.cpp
CXXDEPMODE = depmode=gcc3
uninstall-info-am:
install-pkgincludeHEADERS: $(pkginclude_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	@list='$(pkginclude_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_HEADER) $$d$$p $(DESTDIR)$(pkgincludedir)/$$f"; \
	  $(INSTALL_HEADER) $$d$$p $(DESTDIR)$(pkgincludedir)/$$f; \
	done

uninstall-pkgincludeHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(pkginclude_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(pkgincludedir)/$$f"; \
	  rm -f $(DESTDIR)$(pkgincludedir)/$$f; \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags  $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = ..
distdir = $(top_distdir)/$(PACKAGE)-$(VERSION)

distdir: $(DISTFILES)
	$(mkinstalldirs) $(distdir)/$(swincludedir)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(LTLIBRARIES) $(HEADERS)

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(libdir) $(DESTDIR)$(pkgincludedir)

install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libLTLIBRARIES clean-libtool \
	mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-compile distclean-depend \
	distclean-generic distclean-libtool distclean-tags

dvi: dvi-am

dvi-am:

info: info-am

info-am:

install-data-am: install-pkgincludeHEADERS

install-exec-am: install-libLTLIBRARIES

install-info: install-info-am

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

uninstall-am: uninstall-info-am uninstall-libLTLIBRARIES \
	uninstall-pkgincludeHEADERS

.PHONY: GTAGS all all-am check check-am clean clean-generic \
	clean-libLTLIBRARIES clean-libtool distclean distclean-compile \
	distclean-depend distclean-generic distclean-libtool \
	distclean-tags distdir dvi dvi-am info info-am install \
	install-am install-data install-data-am install-exec \
	install-exec-am install-info install-info-am \
	install-libLTLIBRARIES install-man install-pkgincludeHEADERS \
	install-strip installcheck installcheck-am installdirs \
	maintainer-clean maintainer-clean-generic mostlyclean \
	mostlyclean-compile mostlyclean-generic mostlyclean-libtool \
	tags uninstall uninstall-am uninstall-info-am \
	uninstall-libLTLIBRARIES uninstall-pkgincludeHEADERS

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
