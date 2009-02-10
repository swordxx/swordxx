/******************************************************************************
 *  swcom.cpp  - code for base class 'SWCom'- The basis for all commentary
 *			modules
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

SWCom::SWCom(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char *ilang, const char *versification): SWModule(imodname, imoddesc, idisp, "Commentaries", enc, dir, mark, ilang) {
	this->versification = 0;
	stdstr(&(this->versification), versification);
	delete key;
	key = (VerseKey *)CreateKey();
	tmpVK = (VerseKey *)CreateKey();
}


/******************************************************************************
 * SWCom Destructor - Cleans up instance of SWCom
 */

SWCom::~SWCom() {
	delete tmpVK;
	delete [] versification;
}


SWKey *SWCom::CreateKey() {
	VerseKey *vk = new VerseKey();

	vk->setVersificationSystem(versification);

	return vk;
}


long SWCom::Index() const {
	VerseKey *key = 0;
	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	entryIndex = key->Index();

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
