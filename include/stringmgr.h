/******************************************************************************
 *	stringmgr.h - A class which provides string andling functions which can 
 			reimplemented by frontends
 *
 * $Id: stringmgr.h,v 1.1 2004/04/17 17:16:16 joachim Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ	85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * General Public License for more details.
 *
 */


#ifndef STRINGMGR_H
#define STRINGMGR_H

#include <swbuf.h>

SWORD_NAMESPACE_START

class StringMgr {
public:
	/** Sets the global StringMgr handle
	* @param newStringMgr The new global StringMgr. This pointer will be deleted by this StringMgr
	*/	
	static void setSystemStringMgr( StringMgr* newStringMgr );
	/** Returns the global StringMgr handle
	* @return The global string handle
	*/
	static StringMgr* getSystemStringMgr();
	/*
	*/
	static const bool hasUtf8Support() {
		return getSystemStringMgr()->supportsUnicode();
	};
	
	/** Converts the param to an upper case Utf8 string
	* @param The text encoded in utf8 which should be turned into an upper case string
	*/	
	virtual char* upperUtf8(char*, const unsigned int max = 0);
	/** Converts the param to an uppercase latin1 string
	* @param The text encoded in latin1 which should be turned into an upper case string
	*/	
	virtual char* upperLatin1(char*);
	

protected:
	friend class __staticsystemStringMgr;
	
	/** Default constructor
	*/		
	StringMgr();
	/** Copy constructor
	*/	
	StringMgr( const StringMgr& );
	/** Destructor
	*/	
	virtual ~StringMgr();
	
	virtual const bool supportsUnicode() const;

private:
	static StringMgr* m_systemStringMgr;
};

namespace utilstr {
	inline char* toupperstr( char* t ) {
		return StringMgr::getSystemStringMgr()->upperLatin1( t );
	};
	
	inline char* toupperstr_utf8( char* t, const unsigned int max = 0 ) {
		return StringMgr::getSystemStringMgr()->upperUtf8( t, max );
	};
};
using namespace utilstr;

SWORD_NAMESPACE_END


#endif //STRINGMGR_H
