/******************************************************************************
 *  stringmgr.cpp - implementation of class StringMgr
 *
 * $Id: stringmgr.cpp,v 1.6 2004/07/20 11:21:00 joachim Exp $
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
		virtual char* upperUTF8(char*, const unsigned int /*maxlen*/ = 0);
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
	char* ret = t;
 	while (*t) {
 		*t = SW_toupper(*t);
		t++;
 	}
	
	return ret;
}

const bool StringMgr::supportsUnicode() const {
	return false; //default impl has no UTF8 support
}


#ifdef _ICU_

	char* ICUStringMgr::upperUTF8(char* buf, const unsigned int maxlen) {
		char *ret = buf;		
		const int max = (maxlen > 0) ? maxlen : strlen(ret);

		UErrorCode err = U_ZERO_ERROR;
		UConverter *conv = ucnv_open("utf-8", &err);

		//UnicodeString from(buf, -1, conv, err);
		UnicodeString from(buf);
		SWLog::getSystemLog()->logError("from: %s", u_errorName(err));
		
		UChar upperStr[max+10];
		u_strToUpper(
			upperStr,
			max+10, 
			from.getBuffer(), 
			from.length(),
			"utf-8", 
			&err
		);
		SWLog::getSystemLog()->logError("upper: %s", u_errorName(err));
		
		u_strToUTF8 ( 
			ret, 
			max, 
			0, 
			upperStr, 
			u_strlen( upperStr ),
			&err
		);
		SWLog::getSystemLog()->logError("as utf8: %s", u_errorName(err));
		
		ucnv_close(conv);
		
		
		SWLog::getSystemLog()->logError("uppercase name: %s", ret);
		return ret;
	}
	
	char* ICUStringMgr::upperLatin1(char* t) {
 		return StringMgr::upperLatin1(t); //use the default implementation
	}
	
#endif

SWORD_NAMESPACE_END
