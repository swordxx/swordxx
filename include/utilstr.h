/******************************************************************************
 *  utilstr.h	- prototypes for string utility functions
 *
 * $Id: utilstr.h,v 1.6 2001/09/02 08:10:41 scribe Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef UTILSTR_H
#define UTILSTR_H

/** Use stdstr to copy a const char* to a char*
*/
char *stdstr (char **iistr, const char *istr);
char *strstrip (char *istr);
const char *stristr (const char *s1, const char *s2);
const char strnicmp(const char *s1, const char *s2, int len);
unsigned int strlenw(const char *s1);
char *toupperstr(char *buf);

/*
extern "C" {
int stricmp(const char *s1, const char *s2);
}
*/
#endif
