/******************************************************************************
 *  swbasicfilter.h	- definition of class SWBasicFilter.  An SWFilter
 *  				impl that provides some basic methods that
 *  				many filter will need and can use as a starting
 *  				point. 
 *
 * $Id: swbasicfilter.h,v 1.11 2002/10/01 19:52:40 dglassey Exp $
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

#ifndef SWBASICFILTER_H
#define SWBASICFILTER_H

#include <swfilter.h>

#include <defs.h>
#include <map>

SWORD_NAMESPACE_START

/** A filter providing commonly used functionality.
* This filter has facilities for handling SGML/HTML/XML like tokens and
* escape strings (like SGML entities). It has the facility for just
* substituting the given tokens and escape strings to other strings and for
* "manual" custom token handling.
*
* In this class the functions with arguments looking as <code>char
* **buf</code> write a character sequnce at address specified by
* <code>*buf</code> address and change <code>*buf</code> to point past
* the last char of the written sequence.
*/
class SWDLLEXPORT SWBasicFilter : public SWFilter {

	char *tokenStart;
	char *tokenEnd;
	char *escStart;
	char *escEnd;
	bool escStringCaseSensitive;
	bool tokenCaseSensitive;
	bool passThruUnknownToken;
	bool passThruUnknownEsc;

public:
	SWBasicFilter();
	virtual char ProcessText(char *text, int maxlen, const SWKey *, const SWModule * = 0);
	virtual ~SWBasicFilter();

protected:
	const SWModule *module;
	const SWKey *key;
	char *resultBuffer;
	typedef map<string, string> DualStringMap;
	DualStringMap tokenSubMap;
	DualStringMap escSubMap;

	/** Sets the beginning of escape sequence (by default "&").*/
	void setEscapeStart(const char *escStart);

	/** Sets the end of escape sequence (by default ";").*/
	void setEscapeEnd(const char *escEnd);

	/** Sets the beginning of token start sequence (by default "<").*/
	void setTokenStart(const char *tokenStart);

	/** Sets the end of token start sequence (by default ">").*/
	void setTokenEnd(const char *tokenEnd);

	/** Sets whether pass thru unknown tokens unchanged or just ignore (remove) them.
	 * Default is false.*/
	void setPassThruUnknownToken(bool val);

	/** Sets whether pass thru unknown escape sequences unchanged or just ignore (remove) them.
	 * Default is false.*/
	void setPassThruUnknownEscapeString(bool val);

	void setTokenCaseSensitive(bool val);
	void setEscapeStringCaseSensitive(bool val);

	void addTokenSubstitute(const char *findString, const char *replaceString);
	void addEscapeStringSubstitute(const char *findString, const char *replaceString);
	bool substituteToken(char **buf, const char *token);
	bool substituteEscapeString(char **buf, const char *escString);

	/** Like sprintf*/
	void pushString(char **buf, const char *format, ...);

	/** This function is called for every token encountered in the input text.
	* @param buf the output buffer (FIXME: what is its size?)
	* @param token the token (e.g. <code>"p align='left'"</code>
	* @param userData FIXME: document this
	* @return <code>false</code> if was not handled and should be handled in
	* the default way (by just substituting).*/
	virtual bool handleToken(char **buf, const char *token, DualStringMap &userData);

	/** This function is called for every escape sequence encountered in the input text.
	* @param buf the output buffer (FIXME: what is its size?)
	* @param escString the escape sequence (e.g. <code>"amp"</code> for &amp;amp;)
	* @param userData FIXME: document this
	* @return <code>false</code> if was not handled and should be handled in
	* the default way (by just substituting).*/
	virtual bool handleEscapeString(char **buf, const char *escString, DualStringMap &userData);
};

SWORD_NAMESPACE_END
#endif
