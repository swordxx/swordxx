/******************************************************************************
*  swbuf.cpp  - code for SWBuf used as a transport and utility for data buffers
*
* $Id: swbuf.cpp,v 1.7 2003/02/27 23:34:22 mgruner Exp $
*
* Copyright 2003 CrossWire Bible Society (http://www.crosswire.org)
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

#include <swbuf.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

SWORD_NAMESPACE_START

char *SWBuf::nullStr = "";
char SWBuf::junkBuf[JUNKBUFSIZE];

SWBuf::SWBuf(const char *initVal) {
	if (initVal) {
		allocSize = strlen(initVal)+1;
		buf = (char *)calloc(allocSize + 5, 1);
		memcpy(buf, initVal, allocSize);
		end = buf + allocSize - 1;
		allocSize += 5;
	}
	else {
		allocSize = 15;
		buf = (char *)calloc(allocSize, 1);
		end = buf;
	}
	init();
}


SWBuf::SWBuf(const SWBuf &other) {
	allocSize = other.length()+1;
	buf = (char *)calloc(allocSize + 5, 1);
	memcpy(buf, other.buf, allocSize);
	end = buf + allocSize - 1;
	allocSize += 5;
	init();
}


SWBuf::SWBuf(char initVal) {
	allocSize = 15;
	buf = (char *)calloc(allocSize, 1);
	*buf = initVal;
	end = buf+1;
	init();
}

void SWBuf::init() {
	fillByte = ' ';
}

SWBuf::~SWBuf() {
	free(buf);
}

void SWBuf::set(const char *newVal) {
	unsigned int len = strlen(newVal) + 1;
	assureSize(len);
	memcpy(buf, newVal, len);
	end = buf + (len-1);
}


void SWBuf::set(const SWBuf &newVal) {
	unsigned int len = newVal.length() + 1;
	assureSize(len);
	memcpy(buf, newVal.c_str(), len);
	end = buf + (len-1);
}


void SWBuf::append(const char *str) {
	unsigned int len = strlen(str) + 1;
	assureSize((end-buf)+len);
	memcpy(end, str, len);
	end += (len-1);
}

void SWBuf::append(const SWBuf &str) {
	unsigned int len = str.length() + 1;
	assureSize((end-buf)+len);
	memcpy(end, str.c_str(), len);
	end += (len-1);
}

void SWBuf::setSize(unsigned int len) {
	assureSize(len+1);
	if ((end - buf) < len)
		memset(end, fillByte, len - (end-buf));
	end = buf + len;
	*end = 0;
}

// WARNING: This function can only write at most
// JUNKBUFSIZE to the string per call.
void SWBuf::appendFormatted(const char *format, ...) {
	va_list argptr;

	va_start(argptr, format);
	int len = vsprintf(junkBuf, format, argptr)+1;
	assureSize((end-buf)+len);
	end += vsprintf(end, format, argptr);
	va_end(argptr);
}

SWORD_NAMESPACE_END
