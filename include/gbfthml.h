/***************************************************************************
                          gbfthml.h  -  description
                             -------------------
    begin                : 1999-10-28
    copyright            : 1999 by Chris Little
    email                : chrislit@chiasma.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GBFTHML_H
#define GBFTHML_H

#include <swfilter.h>

#include <defs.h>

SWORD_NAMESPACE_START

  /** this filter converts GBF text into ThML text
  */
class SWDLLEXPORT GBFThML:public SWFilter
{
public:
  GBFThML ();
  virtual char ProcessText (char *text, int maxlen = -1);
};

SWORD_NAMESPACE_END
#endif
