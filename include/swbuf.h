/******************************************************************************
*  swbuf.h  - code for SWBuf used as a transport and utility for data buffers
*
* $Id: swbuf.h,v 1.4 2003/02/25 04:12:47 scribe Exp $
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

#ifndef SWBUF_H
#define SWBUF_H

#include <defs.h>

SWORD_NAMESPACE_START


#define JUNKBUFSIZE 8191

class SWDLLEXPORT SWBuf {
	char *buf;
	char *end;
	char fillByte;
	unsigned int allocSize;
	static char *nullStr;
	static char junkBuf[JUNKBUFSIZE];

	void assureSize(unsigned int size);
	void init();

public:
	SWBuf(const char *initVal = 0);
	SWBuf(char initVal);
	SWBuf(const SWBuf &other);

	void setFillByte(char ch) { fillByte = ch; }
	char getFillByte() { return fillByte; }

	virtual ~SWBuf();

	const char *c_str() const { return buf; }
	char &charAt(unsigned int pos) { return ((pos <= (end - buf)) ? buf[pos] : nullStr[0]); }

	unsigned int size() const { return length(); }
	unsigned int length() const { return end - buf; }
	void set(const char *newVal);
	void set(const SWBuf &newVal);
	void setSize(unsigned int len);
	void append(const char *str);
	void append(const SWBuf &str);
	void append(char ch);
	void appendFormatted(const char *format, ...);
	
	char *getRawData() { return buf; }	// be careful!  Probably setSize needs to be called in conjunction before and maybe after

	operator const char *() const { return c_str(); }
	char &operator[](unsigned int pos) { return charAt(pos); }
	char &operator[](int pos) { return charAt((unsigned int)pos); }
	SWBuf &operator =(const char *newVal) { set(newVal); return *this; }
	SWBuf &operator =(const SWBuf &other) { set(other); return *this; }
	SWBuf &operator +=(const char *str) { append(str); return *this; }
	SWBuf &operator +=(char ch) { append(ch); return *this; }
	SWBuf &operator -=(unsigned int len) { setSize(length()-len); return *this; }
	SWBuf &operator --() { operator -=(1); return *this; }
	SWBuf operator +(const SWBuf &other) const;
	SWBuf operator +(char ch) const { return (*this) + SWBuf(ch); }
};



SWORD_NAMESPACE_END
#endif
