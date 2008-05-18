/******************************************************************************
 *  swcom.cpp  - code for base class 'SWCom'- The basis for all commentary
 *			modules
 */

#include <swcom.h>
#include <localemgr.h>
#include <versekey.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWCom Constructor - Initializes data for instance of SWCom
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWCom::SWCom(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang): SWModule(imodname, imoddesc, idisp, (char *)"Commentaries", enc, dir, mark, ilang) {
	delete key;
	key = CreateKey();
	tmpVK = new VerseKey();
}


/******************************************************************************
 * SWCom Destructor - Cleans up instance of SWCom
 */

SWCom::~SWCom() {
	delete tmpVK;
}


SWKey *SWCom::CreateKey() { return new VerseKey(); }


long SWCom::Index() const {
	VerseKey *key = 0;
	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	entryIndex = key->NewIndex();

	if (key != this->key)
		delete key;

	return entryIndex;
}

long SWCom::Index(long iindex) {
	VerseKey *key = 0;
	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {}
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


VerseKey &SWCom::getVerseKey() const {
	VerseKey *key = NULL;
	// see if we have a VerseKey * or decendant
	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {	}
	if (!key) {
		ListKey *lkTest = 0;
		SWTRY {
			lkTest = SWDYNAMIC_CAST(ListKey, this->key);
		}
		SWCATCH ( ... ) {	}
		if (lkTest) {
			SWTRY {
				key = SWDYNAMIC_CAST(VerseKey, lkTest->GetElement());
			}
			SWCATCH ( ... ) {	}
		}
	}
	if (!key) {
		tmpVK->setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
		(*tmpVK) = *(this->key);
		return (*tmpVK);
	}
	else	return *key;
}

SWORD_NAMESPACE_END
