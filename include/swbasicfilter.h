/******************************************************************************
 *  swbasicfilter.h	- definition of class SWBasicFilter.  An SWFilter
 *  				impl that provides some basic methods that
 *  				many filter will need and can use as a starting
 *  				point. 
 *
 * $Id$
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
#include <map>

SWORD_NAMESPACE_START


// not a protected inner class because MSVC++ sucks and can't handle it
class BasicFilterUserData {
public:
	BasicFilterUserData(const SWModule *module, const SWKey *key) { this->module = module; this->key = key; suspendTextPassThru = false; supressAdjacentWhitespace = false; }
	virtual ~BasicFilterUserData() {}
	const SWModule *module;
	const SWKey *key;
	SWBuf lastTextNode;
	bool suspendTextPassThru;
	bool supressAdjacentWhitespace;
};

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
	char escStartLen;
	char escEndLen;
	char tokenStartLen;
	char tokenEndLen;
	bool escStringCaseSensitive;
	bool tokenCaseSensitive;
	bool passThruUnknownToken;
	bool passThruUnknownEsc;
	char processStages;

public:

	SWBasicFilter();
	virtual char processText(SWBuf &text, const SWKey *key = 0, const SWModule *module = 0);
	virtual ~SWBasicFilter();

protected:

	virtual BasicFilterUserData *createUserData(const SWModule *module, const SWKey *key) {
		return new BasicFilterUserData(module, key);
	}

	// STAGEs
	static const char INITIALIZE;	// flag for indicating processing before char loop
	static const char PRECHAR;	// flag for indicating processing at top in char loop
	static const char POSTCHAR;	// flag for indicating processing at bottom in char loop
	static const char FINALIZE;	// flag for indicating processing after char loop


	typedef std::map<SWBuf, SWBuf> DualStringMap;
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
	
	void replaceTokenSubstitute(const char *findString, const char *replaceString);
	void replaceEscapeStringSubstitute(const char *findString, const char *replaceString);
	
	bool substituteToken(SWBuf &buf, const char *token);
	bool substituteEscapeString(SWBuf &buf, const char *escString);

	/** This function is called for every token encountered in the input text.
	* @param buf the output buffer (FIXME: what is its size?)
	* @param token the token (e.g. <code>"p align='left'"</code>
	* @param userData FIXME: document this
	* @return <code>false</code> if was not handled and should be handled in
	* the default way (by just substituting).*/
	virtual bool handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData);
	virtual bool processStage(char stage, SWBuf &text, char *&from, BasicFilterUserData *userData) { return false; }
	virtual void setStageProcessing(char stages) { processStages = stages; }	// see STATICs up above

	/** This function is called for every escape sequence encountered in the input text.
	* @param buf the output buffer (FIXME: what is its size?)
	* @param escString the escape sequence (e.g. <code>"amp"</code> for &amp;amp;)
	* @param userData FIXME: document this
	* @return <code>false</code> if was not handled and should be handled in
	* the default way (by just substituting).*/
	virtual bool handleEscapeString(SWBuf &buf, const char *escString, BasicFilterUserData *userData);
};

SWORD_NAMESPACE_END
#endif
