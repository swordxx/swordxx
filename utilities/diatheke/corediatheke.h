// Diatheke 4.0 by Chris Little <chrislit@crosswire.org>
// Copyright 1999, 2000, 2001 by CrossWire Bible Society http://www.crosswire.org
// Licensed under GNU General Public License (GPL)
// see accompanying LICENSE file for license details

#include <stdio.h>

#include <rawtext.h>
#include <swmgr.h>
#include <gbfthml.h>
#include <thmlgbf.h>
#include <thmlplain.h>
#include <thmlhtml.h>
#include <thmlrtf.h>
#include <localemgr.h>

#define FMT_PLAIN 0
#define FMT_THML 1
#define FMT_GBF 2
#define FMT_RTF 3
#define FMT_HTML 4

#define QT_BIBLE 1
#define QT_COMM 2
#define QT_LD 3
#define QT_SEARCH 4
#define QT_SYSTEM 5
#define QT_INFO 6

#define OP_NONE 0
#define OP_STRONGS 1
#define OP_FOOTNOTES 2
#define OP_HEADINGS 4
#define OP_MORPH 8

#define ST_NONE 0
#define ST_REGEX 1 //0
#define ST_MULTIWORD 2  // -1
#define ST_PHRASE 3 // -2

int hasalpha (char * string);
char* doquery(int maxverses, char outputformat, char optionfilters, char searchtype, const char *text, const char *locale, const char *ref);
