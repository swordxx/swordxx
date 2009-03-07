/******************************************************************************
 *  swld.cpp   - code for base class 'SWLD'.  SWLD is the basis for all
 *		 types of Lexicon and Dictionary modules (hence the 'LD').
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


#include <swgenbook.h>
#include <versetreekey.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWLD Constructor - Initializes data for instance of SWLD
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWGenBook::SWGenBook(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : SWModule(imodname, imoddesc, idisp, (char *)"Generic Books", enc, dir, mark, ilang) {
	tmpTreeKey = 0;
}


/******************************************************************************
 * SWLD Destructor - Cleans up instance of SWGenBook
 */

SWGenBook::~SWGenBook() {
	delete tmpTreeKey;
}


TreeKey &SWGenBook::getTreeKey() const {

	TreeKey *key = 0;

	SWTRY {
		key = SWDYNAMIC_CAST(TreeKey, (this->key));
	}
	SWCATCH ( ... ) {}

	if (!key) {
		ListKey *lkTest = 0;
		SWTRY {
			lkTest = SWDYNAMIC_CAST(ListKey, this->key);
		}
		SWCATCH ( ... ) {	}
		if (lkTest) {
			SWTRY {
				key = SWDYNAMIC_CAST(TreeKey, lkTest->GetElement());
				if (!key) {
					VerseTreeKey *tkey = 0;
					SWTRY {
						tkey = SWDYNAMIC_CAST(VerseTreeKey, lkTest->GetElement());
					}
					SWCATCH ( ... ) {}
					if (tkey) key = tkey->getTreeKey();
				}
			}
			SWCATCH ( ... ) {	}
		}
	}
	if (!key) {
		VerseTreeKey *tkey = 0;
		SWTRY {
			tkey = SWDYNAMIC_CAST(VerseTreeKey, (this->key));
		}
		SWCATCH ( ... ) {}
		if (tkey) key = tkey->getTreeKey();
	}

	if (!key) {
		delete tmpTreeKey;
		tmpTreeKey = (TreeKey *)CreateKey();
		(*tmpTreeKey) = *(this->key);
		return (*tmpTreeKey);
	}
	else	return *key;
}

SWORD_NAMESPACE_END
