/***************************************************************************
                          rwphtml.h  -  description
                             -------------------
    begin                : Tue Jun 15 15:34:26 CEST 1999

    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* $Header: /space/oldserver/cvsroot/core/sword/include/Attic/rwphtml.h,v 1.1 1999/08/02 06:40:35 scribe Exp $ */
/* $Revision: 1.1 $ */

#ifndef RWPHTML_H
#define RWPHTML_H

#include <swfilter.h>

class RWPHTML : public SWFilter {
public:
	RWPHTML();
	virtual char ProcessText(char *text, int maxlen = -1);
};

#endif
