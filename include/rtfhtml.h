/***************************************************************************
                          rtfhtml.h  -  description                              
                             -------------------                                         
    begin                : Wed Oct 13 1999                                           
    copyright            : (C) 1999 by The team of BibleTime                         
    email                : info@bibletime.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

/* $Header: /space/oldserver/cvsroot/core/sword/include/rtfhtml.h,v 1.1 2000/02/12 20:59:29 tuhlmann Exp $ */
/* $Revision: 1.1 $ */

#ifndef RTFHTML_H
#define RTFHTML_H

#include <swfilter.h>

class RTFHTML : public SWFilter {
public:
	RTFHTML();
	virtual char ProcessText(char *text, int maxlen, const SWKey *key);
};

#endif
