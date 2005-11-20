/***************************************************************************
                   thmlcgi.h  -  OSIS to Diatheke/CGI format
                             -------------------
    begin                : 2001-11-12
    copyright            : 2001 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef THMLCGI_H
#define THMLCGI_H

#include <swbasicfilter.h>
#include <map>

#include <defs.h>

SWORD_NAMESPACE_START

typedef std::map<SWBuf, SWBuf> DualStringMap;
/** this filter converts ThML text to Diatheke/CGI format
 */
class SWDLLEXPORT ThMLCGI : public SWBasicFilter {
protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	ThMLCGI();
};

SWORD_NAMESPACE_END
#endif
