/******************************************************************************
*  swbuf.h  - code for SWBuf used as a transport and utility for data buffers
*
* $Id: swbuf.h,v 1.7 2003/02/27 19:14:32 mgruner Exp $
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
#include <stdlib.h>

SWORD_NAMESPACE_START


#define JUNKBUFSIZE 8191

class SWDLLEXPORT SWBuf {
	char *buf;
	char *end;
	char fillByte;
	unsigned int allocSize;
	static char *nullStr;
	static char junkBuf[JUNKBUFSIZE];

	inline void assureSize(unsigned int newsize) {
		if (newsize > allocSize) {
			allocSize = newsize + 5;
			end = (char *)(end - buf);
			buf = (char *)realloc(buf, allocSize);
			end = (buf + (unsigned int)end);
		}
	}
	void init();

	inline const char *raw_buf() const{		return buf;	}


public:
	SWBuf(const char *initVal = 0);
	SWBuf(char initVal);
	SWBuf(const SWBuf &other);

	inline void setFillByte(char ch) { fillByte = ch; }
	inline char getFillByte() { return fillByte; }

	virtual ~SWBuf();

	//return null-terminated strings only!
	inline const char *c_str(){
		unsigned int size_cache = size();		
		if (buf[size_cache - 1] != 0){
			assureSize(size_cache + 2);
			buf[size_cache] = 0;
		}
		return buf;
	}
	inline char &charAt(unsigned int pos) { return ((pos <= (unsigned int)(end - buf)) ? buf[pos] : nullStr[0]); }

	inline unsigned int size() const { return length(); }
	inline unsigned int length() const { return end - buf; }
	void set(const char *newVal);
	void set(const SWBuf &newVal);
	void setSize(unsigned int len);
	void append(const char *str);
	void append(const SWBuf &str);
	inline void append(char ch) {
		assureSize((end-buf)+2);
		*end = ch;
		end++;
		*end = 0;
	}
	void appendFormatted(const char *format, ...);
	
	inline char *getRawData() { return buf; }	// be careful!  Probably setSize needs to be called in conjunction before and maybe after

	inline operator const char *() { return c_str(); }
	inline char &operator[](unsigned int pos) { return charAt(pos); }
	inline char &operator[](int pos) { return charAt((unsigned int)pos); }
	inline SWBuf &operator =(const char *newVal) { set(newVal); return *this; }
	inline SWBuf &operator =(const SWBuf &other) { set(other); return *this; }
	inline SWBuf &operator +=(const char *str) { append(str); return *this; }
	inline SWBuf &operator +=(char ch) { append(ch); return *this; }
	inline SWBuf &operator +=(const SWBuf &str) { append(str); return *this; }
	inline SWBuf &operator -=(unsigned int len) { setSize(length()-len); return *this; }
	inline SWBuf &operator --(int) { operator -=(1); return *this; }
	inline SWBuf operator +(const SWBuf &other) const {
		SWBuf retVal = buf;
		retVal += other;
		return retVal;
	}
	inline SWBuf operator +(char ch) const { return (*this) + SWBuf(ch); }
};



SWORD_NAMESPACE_END
#endif
