/******************************************************************************
 *  swtext.cpp  - code for base class 'SWText'- The basis for all text modules
 */

#include <swtext.h>
#include <listkey.h>


/******************************************************************************
 * SWText Constructor - Initializes data for instance of SWText
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWText::SWText(const char *imodname, const char *imoddesc, SWDisplay *idisp, bool unicode): SWModule(imodname, imoddesc, idisp, "Biblical Texts", unicode)
{
	delete key;
	key = CreateKey();
}


/******************************************************************************
 * SWText Destructor - Cleans up instance of SWText
 */

SWText::~SWText() {
}


/******************************************************************************
 * SWText CreateKey - Create the correct key (VerseKey) for use with SWText
 */

SWKey *SWText::CreateKey()
{
	return new VerseKey();
}
