/******************************************************************************
 *  VerseTreeKey.cpp - code for class 'VerseTreeKey'- versekey using treekey
 *		for data retrieval
 */

#include <versetreekey.h>

SWORD_NAMESPACE_START

static const char *classes[] = {"VerseTreeKey", "VerseKey", "SWKey", "SWObject", 0};
SWClass VerseTreeKey::classdef(classes);


/******************************************************************************
 * VerseTreeKey Constructor - initializes instance of VerseTreeKey
 *
 * ENT:	ikey - base key (will take various forms of 'BOOK CH:VS'.  See
 *		VerseTreeKey::parse for more detailed information)
 */

VerseTreeKey::VerseTreeKey(TreeKey *treeKey, const SWKey *ikey) : VerseKey(ikey)
{
	this->treeKey = treeKey;
	if (ikey)
		parse();
}


/******************************************************************************
 * VerseTreeKey Constructor - initializes instance of VerseTreeKey
 *
 * ENT:	ikey - text key (will take various forms of 'BOOK CH:VS'.  See
 *		VerseTreeKey::parse for more detailed information)
 */

VerseTreeKey::VerseTreeKey(TreeKey *treeKey, const char *ikey) : VerseKey(ikey)
{
	this->treeKey = treeKey;
	if (ikey)
		parse();
}


VerseTreeKey::VerseTreeKey(VerseTreeKey const &k) : VerseKey(k)
{
	treeKey = k.treeKey;
}


VerseTreeKey::VerseTreeKey(TreeKey *treeKey, const char *min, const char *max) : VerseKey(min, max)
{
	this->treeKey = treeKey;
}


SWKey *VerseTreeKey::clone() const
{
	return new VerseTreeKey(*this);
}


/******************************************************************************
 * VerseTreeKey Destructor - cleans up instance of VerseTreeKey
 *
 * ENT:	ikey - text key
 */

VerseTreeKey::~VerseTreeKey() {
}


SWORD_NAMESPACE_END
