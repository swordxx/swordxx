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

/* $Header: /space/oldserver/cvsroot/core/sword/include/plainhtml.h,v 1.3 1999/10/25 04:31:24 scribe Exp $ */
/* $Revision: 1.3 $ */

#ifndef PLAINHTML_H
#define PLAINHTML_H

#include <swfilter.h>

class PLAINHTML : public SWFilter {
public:
	PLAINHTML();
	virtual char ProcessText(char *text, int maxlen, const SWKey *key);
};

#endif
