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

/* $Header: /space/oldserver/cvsroot/core/sword/include/rtfhtml.h,v 1.5 2001/12/11 22:35:10 scribe Exp $ */
/* $Revision: 1.5 $ */

#ifndef RTFHTML_H
#define RTFHTML_H

#include <swfilter.h>

#include <defs.h>

  /** this filter converts RTF text into HTML text
  */
class SWDLLEXPORT RTFHTML:public SWFilter
{
public:
  RTFHTML ();
  virtual char ProcessText (char *text, int maxlen, const SWKey * key, const SWModule * = 0);
};

#endif
