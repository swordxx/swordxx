/******************************************************************************
 *  swcom.cpp  - code for base class 'SWCom'- The basis for all commentary
 *			modules
 */

#include <swcom.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWCom Constructor - Initializes data for instance of SWCom
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWCom::SWCom(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang): SWModule(imodname, imoddesc, idisp, "Commentaries", enc, dir, mark, ilang)
{
	delete key;
	key = CreateKey();
}


/******************************************************************************
 * SWCom Destructor - Cleans up instance of SWCom
 */

SWCom::~SWCom()
{
}


long SWCom::Index() const {
	VerseKey *key = 0;
	try {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	entryIndex = key->NewIndex();

	if (key != this->key)
		delete key;

	return entryIndex;
}

long SWCom::Index(long iindex) {
	VerseKey *key = 0;
	try {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	key->Testament(1);
	key->Index(iindex);

	if (key != this->key) {
		this->key->copyFrom(*key);
		delete key;
	}

	return Index();
}

SWORD_NAMESPACE_END
