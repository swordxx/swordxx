/******************************************************************************
 *  versekey.h - code for class 'versekey'- a standard Biblical verse key
 *
 * $Id: treekey.h,v 1.1 2002/01/24 08:55:27 scribe Exp $
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


#ifndef TREEKEY_H
#define TREEKEY_H

#include <swkey.h>
#include <swmacs.h>

#include <defs.h>


/**
 * Class VerseKey
 * The SWKey implementation used for verse based modules like Bibles or commentaries.
 */
class SWDLLEXPORT TreeKey : public SWKey {
	static SWClass classdef;

public:
//	TreeKey (const char *ikey = 0);
//	TreeKey (const SWKey * ikey);
//	TreeKey (TreeKey const &k);
	TreeKey () {};
	~TreeKey () {};


	virtual const char *getLocalName() = 0;
	virtual const char *setLocalName(const char *) = 0;

	virtual const char *getFullName() const = 0;

	virtual void root() = 0;
	virtual bool parent() = 0;

	virtual bool firstChild() = 0;
	virtual bool nextSibling() = 0;
	virtual bool previousSibling() = 0;

	virtual bool hasChildren() = 0;

	virtual void append() = 0;
	virtual void appendChild() = 0;
	virtual void insertBefore() = 0;

	virtual void remove() = 0;


	SWKEY_OPERATORS

	};


#endif
