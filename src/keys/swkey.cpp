/******************************************************************************
 *  swkey.cpp - code for base class 'SWKey'.  SWKey is the basis for all
 *	        types of keys for indexing into modules (e.g. verse, word,
 *		place, etc.)
 */

#include <swkey.h>
#include <utilfuns.h>
#include <string.h>

static const char *classes[] = {"SWKey", "SWObject", 0};
SWClass SWKey::classdef(classes);

/******************************************************************************
 * SWKey Constructor - initializes instance of SWKey
 *
 * ENT:	ikey - text key
 */

SWKey::SWKey(const char *ikey)
{
	index   = 0;
	persist = 0;
	keytext = 0;
	error   = 0;
	stdstr(&keytext, ikey);
	init();
}

SWKey::SWKey(SWKey const &k)
{
	index   = k.index;
	persist = k.persist;
	keytext = 0;
	error = k.error;
	stdstr(&keytext, k.keytext);
	init();
}

void SWKey::init() {
	myclass = &classdef;
}

SWKey *SWKey::clone() const
{
	return new SWKey(*this);
}

/******************************************************************************
 * SWKey Destructor - cleans up instance of SWKey
 */

SWKey::~SWKey() {
	if (keytext)
		delete [] keytext;
}


/******************************************************************************
 * SWKey::Persist - Gets whether this object itself persists within a
 *			module that it was used to SetKey or just a copy.
 *			(1 - persists in module; 0 - a copy is attempted
 *
 * RET:	value of persist
 */

char SWKey::Persist() const
{
	return persist;
}


/******************************************************************************
 * SWKey::Persist - Set/gets whether this object itself persists within a
 *			module that it was used to SetKey or just a copy.
 *			(1 - persists in module; 0 - a copy is attempted
 *
 * ENT:	ipersist - value which to set persist
 *		[-1] - only get
 *
 * RET:	value of persist
 */

char SWKey::Persist(char ipersist)
{
	if (ipersist != -1)
		persist = ipersist;

	return persist;
}


/******************************************************************************
 * SWKey::Error - Gets and clears error status
 *
 * RET:	error status
 */

char SWKey::Error()
{
	char retval = error;

	error = 0;
	return retval;
}


/******************************************************************************
 * SWKey::operator = Equates this SWKey to a character string
 *
 * ENT:	ikey - other swkey object
 */

SWKey &SWKey::operator =(const char *ikey)
{
	stdstr(&keytext, ikey);

	return *this;
}


/******************************************************************************
 * SWKey::operator = Equates this SWKey to another SWKey object
 *
 * ENT:	ikey - other swkey object
 */

SWKey &SWKey::operator =(const SWKey &ikey)
{
// not desirable	Persist(ikey.Persist());
	stdstr(&keytext, (const char *)ikey);

	return *this;
}


/******************************************************************************
 * SWKey::operator char * - returns text key if (char *) cast is requested
 */

SWKey::operator const char *() const
{
	return keytext;
}


/******************************************************************************
 * SWKey::compare	- Compares another VerseKey object
 *
 * ENT:	ikey - key to compare with this one
 *
 * RET:	> 0 if this key is greater than compare key
 *	< 0
 *	  0
 */

int SWKey::compare(const SWKey &ikey)
{
	return strcmp((const char *)*this, (const char *)ikey);
}


/******************************************************************************
 * SWKey::operator =(SW_POSITION)	- Positions this key if applicable
 */

SWKey &SWKey::operator =(SW_POSITION p)
{
	switch (p) {
	case POS_TOP:
//		*this = "";
		break;
	case POS_BOTTOM:
//		*this = "zzzzzzzzz";
		break;
	} 
	return *this;
}


/******************************************************************************
 * SWKey::operator +=	- Increments key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

SWKey &SWKey::operator += (int)
{
	error = KEYERR_OUTOFBOUNDS;
	return *this;
}


/******************************************************************************
 * SWKey::operator -=	- Decrements key a number of entries
 *
 * ENT:	decrement	- Number of entries to jump backward
 *
 * RET: *this
 */

SWKey &SWKey::operator -= (int)
{
	error = KEYERR_OUTOFBOUNDS;
	return *this;
}
