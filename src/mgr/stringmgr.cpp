/******************************************************************************
 *  stringmgr.cpp - implementation of class StringMgr
 *
 * $Id: stringmgr.cpp,v 1.4 2004/07/03 18:10:50 scribe Exp $
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

#include <stringmgr.h>
#include <swlog.h>
#include <localemgr.h>
#include <utilstr.h>

#ifdef _ICU_

#include <unicode/utypes.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <unicode/uchar.h>

#include <unicode/unistr.h>
#include <unicode/translit.h>

#include <unicode/locid.h>

#endif

SWORD_NAMESPACE_START

StringMgr* StringMgr::m_systemStringMgr = 0;

class __staticsystemStringMgr {
public:
 	__staticsystemStringMgr() { }
 	~__staticsystemStringMgr() { delete StringMgr::m_systemStringMgr; StringMgr::m_systemStringMgr = 0; }
} _staticsystemStringMgr;


#ifdef _ICU_

	//here comes our IcuStringMgr reimplementation
	class ICUStringMgr : public StringMgr {
	public:
		virtual char* upperUTF8(char*, const unsigned int maxlen = 0);
		virtual char* upperLatin1(char*);
	
	protected:
		virtual const bool supportsUnicode() const {
			return true;
		};
	};

#endif


/** Default constructor
*/		
StringMgr::StringMgr() {
}

/** Copy constructor
*/	
StringMgr::StringMgr( const StringMgr& m ) {
}

/** Destructor
*/	
StringMgr::~StringMgr() {
}

/** Sets the global StringMgr handle
* @param newStringMgr The new global StringMgr. This pointer will be deleted by this StringMgr
*/	
void StringMgr::setSystemStringMgr( StringMgr* newStringMgr ) {
	if (m_systemStringMgr) 
		delete m_systemStringMgr;
	
	m_systemStringMgr = newStringMgr;
	LocaleMgr::getSystemLocaleMgr()->setSystemLocaleMgr( new LocaleMgr() );
}

/** Returns the global StringMgr handle
* @return The global string handle
*/
StringMgr* StringMgr::getSystemStringMgr() {
	if (!m_systemStringMgr) {
#ifndef _ICU_
		m_systemStringMgr = new StringMgr();
		SWLog::getSystemLog()->logInformation("created default StringMgr");
#else
		m_systemStringMgr = new ICUStringMgr();
		SWLog::getSystemLog()->logInformation("created default IcuStringMgr");
#endif	
	}
	
	return m_systemStringMgr;
}


/** Converts the param to an upper case Utf8 string
* @param The text encoded in utf8 which should be turned into an upper case string
*/	
char* StringMgr::upperUTF8(char* t, const unsigned int maxlen) {
        // try to decide if it's worth trying to toupper.  Do we have more
        // characters that are probably lower latin than not?
        long performOp = 0;
        for (const char *ch = t; *ch; ch++)
                performOp += (*ch > 0) ? 1 : -1;

        if (performOp) {
		return upperLatin1(t);
        }

	return t;
}

/** Converts the param to an uppercase latin1 string
* @param The text encoded in latin1 which should be turned into an upper case string
*/	
char* StringMgr::upperLatin1(char* t) {
// 	SWLog::getSystemLog()->logError("StringMgr::upperLatin1 with %s", t);
	char* ret = t;
 	while (*t) {
 		*t++ = SW_toupper(*t);
 	}
	
	return ret;
}

const bool StringMgr::supportsUnicode() const {
// 	SWLog::getSystemLog()->logError("StringMgr::supportsUtf8");
	return false; //default impl has no UTF8 support
}


#ifdef _ICU_

	char* ICUStringMgr::upperUTF8(char* buf, const unsigned int maxlen) {
		char *ret = buf;
		
		int max = maxlen;
		if (!max)
			max = strlen(ret);

		UErrorCode err = U_ZERO_ERROR;
		UConverter *conv = ucnv_open("UTF-8", &err);
		UnicodeString str(buf, -1, conv, err);
		UnicodeString ustr = str.toUpper();
		ustr.extract(ret, max+1, conv, err);
		
		SWLog::getSystemLog()->logError("%s", u_errorName(err));
		
		ucnv_close(conv);
		
		SWLog::getSystemLog()->logError("%s", ret);
		return ret;
	}
	
	char* ICUStringMgr::upperLatin1(char* t) {
 		StringMgr::upperLatin1(t); //use the default implementation
	}
	
#endif

SWORD_NAMESPACE_END
