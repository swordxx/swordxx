/*-*************************************************************************
                          gbfhtml.h  -  description
                             -------------------
    begin                    : 2001-09-03
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

#ifndef GBFHTML_H
#define GBFHTML_H

#include <swbasicfilter.h>

#include <defs.h>

SWORD_NAMESPACE_START

/** this filter converts GBF  text to HTML text 
 */
class SWDLLEXPORT GBFHTML : public SWBasicFilter {
protected:
	virtual bool handleToken(char **buf, const char *token, DualStringMap &userData);
public:
  GBFHTML ();
};


SWORD_NAMESPACE_END
#endif
