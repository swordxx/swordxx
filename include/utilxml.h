/******************************************************************************
 *  utilxml.h   - definition of class that deal with xml constructs 
 *
 * $Id: utilxml.h,v 1.3 2003/05/31 17:34:43 mgruner Exp $
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

#ifndef UTILXML_H
#define UTILXML_H

#include <defs.h>
#include <swbuf.h>
#include <list>
#include <map>

SWORD_NAMESPACE_START

typedef std::map<SWBuf, SWBuf> MapStringPair;
typedef std::list<SWBuf> ListString;

/** Simple XML helper class.
*/
class SWDLLEXPORT XMLTag {
private:
	mutable char *buf;
	char *name;
	mutable bool parsed;
	mutable bool empty;
	mutable bool endTag;
	mutable MapStringPair attributes;

	void parse() const;

	
public:
	XMLTag(const char *tagString);
	~XMLTag();

	inline const char *getName() const { return name; }

	inline bool isEmpty() const {
		if (!parsed)
			parse();

		return empty;
	}

	inline bool isEndTag() const { return endTag; }

	const ListString getAttributeNames() const;
	const char *getAttribute(const char *attribName) const;
	const char *setAttribute(const char *attribName, const char *attribValue);
	const char *toString() const;
	inline operator const char *() const { return toString(); }
};

SWORD_NAMESPACE_END
#endif

