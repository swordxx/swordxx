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
	init(treeKey);
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
	init(treeKey);
	if (ikey)
		parse();
}


VerseTreeKey::VerseTreeKey(VerseTreeKey const &k) : VerseKey(k)
{
	init(k.treeKey);
}


VerseTreeKey::VerseTreeKey(TreeKey *treeKey, const char *min, const char *max) : VerseKey(min, max)
{
	init(treeKey);
}


void VerseTreeKey::init(TreeKey *treeKey)
{
	myclass = &classdef;
	this->treeKey = treeKey;
	this->treeKey->setPositionChangeListener(this);
	internalPosChange = false;
}


SWKey *VerseTreeKey::clone() const
{
	return new VerseTreeKey(*this);
}


int VerseTreeKey::getBookAbbrev(const char *iabbr)
{
	int bookno = VerseKey::getBookAbbrev(iabbr);
	if (bookno < 0) {
/*
		vector<struct sbook>::iterator it = find(books, iabbr);
		if (it == books.end()) {
			TreeKey *tkey = this->treeKey;
			int saveError = tkey->Error();
			long bookmark = tkey->getOffset();
			SWBuf segment;
			internalPosChange = true;
			do {
				segment = (SWBuf)tkey->getLocalName();
			} while (tkey->parent());
			segment << 1;
			if (saveError) {
				error = saveError;
			}
			tkey->setOffset(bookmark);
		}
		books.push_back(sbook(iabbr));
*/
	}
	return bookno;
}

/******************************************************************************
 * VerseTreeKey Destructor - cleans up instance of VerseTreeKey
 *
 * ENT:	ikey - text key
 */

VerseTreeKey::~VerseTreeKey() {
}


void VerseTreeKey::decrement(int steps) {
	int treeError = 0;
	if (!error) lastGoodOffset = treeKey->getOffset();
	do {
		treeKey->decrement();
		treeError = treeKey->Error();
	// iterate until 3 levels and no versekey parse errors
	} while (!treeError && ((treeKey->getLevel() < 3) || error));
	if (error && !treeError) {
		int saveError = error;
		increment();
		error = saveError;
	}
	if (treeError) {
		treeKey->setOffset(lastGoodOffset);
		error = treeError;
	}
}
void VerseTreeKey::increment(int steps) {
	int treeError = 0;
	if (!error) lastGoodOffset = treeKey->getOffset();
	do {
		treeKey->increment();
		treeError = treeKey->Error();
	// iterate until 3 levels and no versekey parse errors
	} while (!treeError && ((treeKey->getLevel() < 3) || error));
	if (error && !treeError) {
		int saveError = error;
		decrement();
		error = saveError;
	}
	if (treeError) {
		treeKey->setOffset(lastGoodOffset);
		error = treeError;
	}
}


void VerseTreeKey::positionChanged() {
	if (!internalPosChange) {
		TreeKey *tkey = this->TreeKey::PositionChangeListener::getTreeKey();
		int saveError = tkey->Error();
		long bookmark = tkey->getOffset();
		SWBuf path;
		internalPosChange = true;
		int legs = 0;
		do {
			path = (SWBuf)tkey->getLocalName() + "." + path;
			legs++;
		} while (tkey->parent());
		path--;
		path << 1;
		setText(path);
		if (saveError) {
			error = saveError;
		}
		tkey->setOffset(bookmark);
		tkey->setError(saveError);
		internalPosChange = false;
	}
}


void VerseTreeKey::syncVerseToTree() {
	internalPosChange = true;
	SWBuf path;
	path.setFormatted("/%s/%d/%d", getOSISBookName(), Chapter(), Verse());
	if (getSuffix()) path += getSuffix();
	long bookmark = treeKey->getOffset();
	treeKey->setText(path);

	// if our module has jacked inconsistencies, then let's put our tree back to where it was
	if (treeKey->Error()) {
		treeKey->setOffset(bookmark);
	}

	internalPosChange = false;
}


TreeKey *VerseTreeKey::getTreeKey() {
	syncVerseToTree();
	return treeKey;
}

// can autonormalize yet (ever?)
void VerseTreeKey::Normalize(char autocheck) {
	error = 0;
}

long VerseTreeKey::NewIndex() const {
	return treeKey->getOffset();
}


void VerseTreeKey::setPosition(SW_POSITION p) {

/*
	if (boundSet) {
		return VerseKey::setPosition(p);
	}
*/

	switch (p) {
	case POS_TOP:
		Error();
		treeKey->setPosition(p);
		increment();
		decrement();
		Error();
		break;
	case POS_BOTTOM:
		Error();
		treeKey->setPosition(p);
		decrement();
		increment();
		Error();
		break;
	case POS_MAXVERSE:
	case POS_MAXCHAPTER:
		VerseKey::setPosition(p);
		break;
	}
}

SWORD_NAMESPACE_END
