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

/* $Header: /space/oldserver/cvsroot/core/sword/include/plainhtml.h,v 1.4 2001/02/08 09:20:48 chrislit Exp $ */
/* $Revision: 1.4 $ */

#ifndef PLAINHTML_H
#define PLAINHTML_H

#include <swfilter.h>

#include <defs.h>

class SWDLLEXPORT PLAINHTML : public SWFilter {
public:
	PLAINHTML();
	virtual char ProcessText(char *text, int maxlen, const SWKey *key);
};

#endif
