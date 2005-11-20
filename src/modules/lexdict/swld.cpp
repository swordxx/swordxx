/******************************************************************************
 *  swld.cpp   - code for base class 'SWLD'.  SWLD is the basis for all
 *				types of Lexicon and Dictionary modules (hence the 'LD').
 */

#include <swld.h>
#include <strkey.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWLD Constructor - Initializes data for instance of SWLD
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWLD::SWLD(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : SWModule(imodname, imoddesc, idisp, "Lexicons / Dictionaries", enc, dir, mark, ilang)
{
	delete key;
	key = CreateKey();
	entkeytxt = new char [1];
	*entkeytxt = 0;
}


/******************************************************************************
 * SWLD Destructor - Cleans up instance of SWLD
 */

SWLD::~SWLD()
{
	if (entkeytxt)
		delete [] entkeytxt;
}


SWKey *SWLD::CreateKey() { return new StrKey(); }


/******************************************************************************
 * SWLD::KeyText - Sets/gets module KeyText, getting from saved text if key is
 *				persistent
 *
 * ENT:	ikeytext - value which to set keytext
 *		[0] - only get
 *
 * RET:	pointer to keytext
 */

const char *SWLD::KeyText(const char *ikeytext)
{
	if (key->Persist() && !ikeytext) {
		getRawEntryBuf();	// force module key to snap to entry
		return entkeytxt;
	}
	else return SWModule::KeyText(ikeytext);
}


/******************************************************************************
 * SWLD::setPosition(SW_POSITION)	- Positions this key if applicable
 */

void SWLD::setPosition(SW_POSITION p) {
	if (!key->isTraversable()) {
		switch (p) {
		case POS_TOP:
			*key = "";
			break;
		case POS_BOTTOM:
			*key = "zzzzzzzzz";
			break;
		} 
	}
	else	*key = p;
	getRawEntryBuf();
}


SWORD_NAMESPACE_END
