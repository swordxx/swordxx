/******************************************************************************
 *  listkey.cpp - code for base class 'ListKey'.  ListKey is the basis for all
 *		  types of keys that have lists of specified indexes
 *		  (e.g. a list of verses, place, etc.)
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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


#include <utilstr.h>
#include <stdlib.h>
#include <swkey.h>
#include <listkey.h>
#include <string.h>

SWORD_NAMESPACE_START

static const char *classes[] = {"ListKey", "SWKey", "SWObject", 0};
SWClass ListKey::classdef(classes);

/******************************************************************************
 * ListKey Constructor - initializes instance of ListKey
 *
 * ENT:	ikey - text key
 */

ListKey::ListKey(const char *ikey): SWKey(ikey) {
	arraymax = 0;
	ClearList();
	init();
}


ListKey::ListKey(ListKey const &k) : SWKey(k.keytext) {
	arraymax = k.arraymax;
	arraypos = k.arraypos;
	arraycnt = k.arraycnt;
	array = (arraymax)?(SWKey **)malloc(k.arraymax * sizeof(SWKey *)):0;
	for (int i = 0; i < arraycnt; i++)
		array[i] = k.array[i]->clone();
	init();
}


void ListKey::init() {
	myclass = &classdef;
	// this is a listkey, bound is always set
	boundSet = true;
}


SWKey *ListKey::clone() const
{
	return new ListKey(*this);
}

/******************************************************************************
 * ListKey Destructor - cleans up instance of ListKey
 */

ListKey::~ListKey()
{
	ClearList();
}


/******************************************************************************
 * ListKey::ClearList	- Clears out elements of list
 */

void ListKey::clear()
{
	int loop;

	if (arraymax) {
		for (loop = 0; loop < arraycnt; loop++)
			delete array[loop];

		free(array);
		arraymax  = 0;
	}
	arraycnt  = 0;
	arraypos  = 0;
	array     = 0;
}


/******************************************************************************
 * ListKey::copyFrom Equates this ListKey to another ListKey object
 *
 * ENT:	ikey - other ListKey object
 */

void ListKey::copyFrom(const ListKey &ikey) {
	ClearList();

	arraymax = ikey.arraymax;
	arraypos = ikey.arraypos;
	arraycnt = ikey.arraycnt;
	array = (arraymax)?(SWKey **)malloc(ikey.arraymax * sizeof(SWKey *)):0;
	for (int i = 0; i < arraycnt; i++)
		array[i] = ikey.array[i]->clone();

	SetToElement(0);
}


/******************************************************************************
 * ListKey::add - Adds an element to the list
 */

void ListKey::add(const SWKey &ikey) {
	if (++arraycnt > arraymax) {
		array = (SWKey **) ((array) ? realloc(array, (arraycnt + 32) * sizeof(SWKey *)) : calloc(arraycnt + 32, sizeof(SWKey *)));
		arraymax = arraycnt + 32;
	}
	array[arraycnt-1] = ikey.clone();
	SetToElement(arraycnt-1);
}



/******************************************************************************
 * ListKey::setPosition(SW_POSITION)	- Positions this key
 *
 * ENT:	p	- position
 *
 * RET:	*this
 */

void ListKey::setPosition(SW_POSITION p) {
	switch (p) {
	case 1:	// GCC won't compile P_TOP
		SetToElement(0, p);
		break;
	case 2:	// GCC won't compile P_BOTTOM
		SetToElement(arraycnt-1, p);
		break;
	}
}


/******************************************************************************
 * ListKey::increment - Increments a number of elements
 */

void ListKey::increment(int step) {
	if (step < 0) {
		decrement(step*-1);
		return;
	}
	Error();		// clear error
	for(; step && !Error(); step--) {
		if (arraypos < arraycnt) {
			if (array[arraypos]->isBoundSet())
				(*(array[arraypos]))++;
			if ((array[arraypos]->Error()) || (!array[arraypos]->isBoundSet())) {
				SetToElement(arraypos+1);
			}
			else SWKey::setText((const char *)(*array[arraypos]));
		}
		else error = KEYERR_OUTOFBOUNDS;
	}
}


/******************************************************************************
 * ListKey::decrement - Decrements a number of elements
 */

void ListKey::decrement(int step) {
	if (step < 0) {
		increment(step*-1);
		return;
	}
	Error();		// clear error
	for(; step && !Error(); step--) {
		if (arraypos > -1) {
			if (array[arraypos]->isBoundSet())
				(*(array[arraypos]))--;
			if ((array[arraypos]->Error()) || (!array[arraypos]->isBoundSet())) {
				SetToElement(arraypos-1, BOTTOM);
			}
			else SWKey::setText((const char *)(*array[arraypos]));
		}
		else error = KEYERR_OUTOFBOUNDS;
	}
}


/******************************************************************************
 * ListKey::Count	- Returns number of elements in list
 */

int ListKey::Count() {
	return arraycnt;
}


/******************************************************************************
 * ListKey::SetToElement	- Sets key to element number
 *
 * ENT:	ielement	- element number to set to
 *
 * RET:	error status
 */

char ListKey::SetToElement(int ielement, SW_POSITION pos) {
	arraypos = ielement;
	if (arraypos >= arraycnt) {
		arraypos = (arraycnt>0)?arraycnt - 1:0;
		error = KEYERR_OUTOFBOUNDS;
	}
	else {
		if (arraypos < 0) {
			arraypos = 0;
			error = KEYERR_OUTOFBOUNDS;
		}
		else {
			error = 0;
		}
	}
	
	if (arraycnt) {
		if (array[arraypos]->isBoundSet())
			(*array[arraypos]) = pos;
		SWKey::setText((const char *)(*array[arraypos]));
	}
	else SWKey::setText("");
	
	return error;
}


/******************************************************************************
 * ListKey::GetElement	- Gets a key element number
 *
 * ENT:	pos	- element number to get (or default current)
 *
 * RET:	Key or null on error
 */

SWKey *ListKey::getElement(int pos) {
	if (pos < 0)
		pos = arraypos;
		
	if (pos >=arraycnt)
		error = KEYERR_OUTOFBOUNDS;

	return (error) ? 0:array[pos];
}
	

/******************************************************************************
 * ListKey::Remove	- Removes current element from list
 */

void ListKey::Remove() {
	if ((arraypos > -1) && (arraypos < arraycnt)) {
		delete array[arraypos];
		if (arraypos < arraycnt - 1)
			memmove(&array[arraypos], &array[arraypos+1], (arraycnt - arraypos - 1) * sizeof(SWKey *));
		arraycnt--;
		
		SetToElement((arraypos)?arraypos-1:0);
	}
}


/******************************************************************************
 * ListKey::getRangeText - returns parsable range text for this key
 */

const char *ListKey::getRangeText() const {
	char *buf = new char[(arraycnt + 1) * 255];
	buf[0] = 0;
	for (int i = 0; i < arraycnt; i++) {
		strcat(buf, array[i]->getRangeText());
		if (i < arraycnt-1)
			strcat(buf, "; ");
	}
	stdstr(&rangeText, buf);
	delete [] buf;
	return rangeText;
}


/******************************************************************************
 * ListKey::getOSISRefRangeText - returns parsable range text for this key
 */

const char *ListKey::getOSISRefRangeText() const {
	char *buf = new char[(arraycnt + 1) * 255];
	buf[0] = 0;
	for (int i = 0; i < arraycnt; i++) {
		strcat(buf, array[i]->getOSISRefRangeText());
		if (i < arraycnt-1)
			strcat(buf, ";");
	}
	stdstr(&rangeText, buf);
	delete [] buf;
	return rangeText;
}


/******************************************************************************
 * ListKey::getText - returns text key if (const char *) cast is requested
 */

const char *ListKey::getText() const {
	int pos = arraypos;
	SWKey *key = (pos >= arraycnt) ? 0:array[pos];
	return (key) ? key->getText() : keytext;
}


void ListKey::setText(const char *ikey) {
	// at least try to set the current element to this text
	for (arraypos = 0; arraypos < arraycnt; arraypos++) {
		SWKey *key = array[arraypos];
		if (key) {
			if (key->isTraversable() && key->isBoundSet()) {
				key->setText(ikey);
				if (!key->Error())
					break;
			}
			else {
				if (!strcmp(key->getText(), ikey))
					break;
			}
		}
	}
	if (arraypos >= arraycnt) {
		error = 1;
		arraypos = arraycnt-1;
	}
	
	SWKey::setText(ikey);
}

// This sort impl sucks.  Let's change it to a quicksort or some other efficient algol
void ListKey::sort() {
	for (int i = 0; i < arraycnt; i++) {
		for (int j = i; j < arraycnt; j++) {
			if (*array[j] < *array[i]) {
				SWKey *tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}
}

SWORD_NAMESPACE_END

