/******************************************************************************
 *  swld.cpp   - code for base class 'SWLD'.  SWLD is the basis for all
 *				types of Lexicon and Dictionary modules (hence the 'LD').
 */

#include <swgenbook.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWLD Constructor - Initializes data for instance of SWLD
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWGenBook::SWGenBook(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : SWModule(imodname, imoddesc, idisp, (char *)"Generic Books", enc, dir, mark, ilang) {
}


/******************************************************************************
 * SWLD Destructor - Cleans up instance of SWLD
 */

SWGenBook::~SWGenBook() {
}

SWORD_NAMESPACE_END
