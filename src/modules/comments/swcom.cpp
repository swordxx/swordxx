/******************************************************************************
 *  swcom.cpp  - code for base class 'SWCom'- The basis for all commentary
 *			modules
 */

#include <swcom.h>


/******************************************************************************
 * SWCom Constructor - Initializes data for instance of SWCom
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWCom::SWCom(const char *imodname, const char *imoddesc, SWDisplay *idisp, bool unicode, char dir): SWModule(imodname, imoddesc, idisp, "Commentaries", unicode, dir)
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
