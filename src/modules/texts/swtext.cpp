/******************************************************************************
 *  swtext.cpp  - code for base class 'SWText'- The basis for all text modules
 */

#include <swtext.h>
#include <listkey.h>
#include <localemgr.h>
#include <versekey.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWText Constructor - Initializes data for instance of SWText
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWText::SWText(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang): SWModule(imodname, imoddesc, idisp, (char *)"Biblical Texts", enc, dir, mark, ilang) {
	tmpVK = new VerseKey();
	delete key;
	key = CreateKey();
	skipConsecutiveLinks = false;
}


/******************************************************************************
 * SWText Destructor - Cleans up instance of SWText
 */

SWText::~SWText() {
	delete tmpVK;
}


/******************************************************************************
 * SWText CreateKey - Create the correct key (VerseKey) for use with SWText
 */

SWKey *SWText::CreateKey() {
	return new VerseKey();
}


long SWText::Index() const {
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

long SWText::Index(long iindex) {
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


VerseKey &SWText::getVerseKey() const {
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
