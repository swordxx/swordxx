// Diatheke 4.2 by Chris Little <chrislit@crosswire.org>
// Copyright 1999-2009 by CrossWire Bible Society
// http://www.crosswire.org/sword/diatheke

/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <stdio.h>
#include <iostream>

#include "diathekemgr.h"
#include <localemgr.h>

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
#define OP_CANTILLATION 16
#define OP_HEBREWPOINTS 32
#define OP_GREEKACCENTS 64
#define OP_TRANSLITERATOR 128
#define OP_LEMMAS 256
#define OP_SCRIPREF 512
#define OP_ARSHAPE 1024
#define OP_BIDI 2048
#define OP_VARIANTS 4096
#define OP_RED 8192
#define OP_ARABICPOINTS 16384
#define ST_NONE 0
#define ST_REGEX 1 //0
#define ST_PHRASE 2  // -1
#define ST_MULTIWORD 3 // -2

using namespace std;

int hasalpha (char * string);
void doquery(unsigned long maxverses, unsigned char outputformat, unsigned char outputencoding, unsigned long optionfilters, unsigned char searchtype, const char *range, const char *text, const char *locale, const char *ref, ostream* output, const char* script, signed short variants); 
