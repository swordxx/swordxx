/***************************************************************************
                          plainfootnotes.h  -  description                              
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

/* $Header: /space/oldserver/cvsroot/core/sword/include/plainfootnotes.h,v 1.1 2000/02/17 09:54:40 tuhlmann Exp $ */
/* $Revision: 1.1 $ */

#ifndef PLAINFOOTNOTES_H
#define PLAINFOOTNOTES_H

class QObject;
#include <swfilter.h>
class SWKey;

/**Shows or hides footnotes in plain text.
  *@author The team of BibleTime
  */
class PLAINFootnotes : public SWFilter  {
	bool option;
	static const char on[];
	static const char off[];
	static const char optName[];
	static const char optTip[];
	OptionsList options;

public:
	PLAINFootnotes();
	virtual ~PLAINFootnotes();
	virtual char ProcessText(char *text, int maxlen = -1, const SWKey *key = 0);
	virtual const char *getOptionName() { return optName; }
	virtual const char *getOptionTip() { return optTip; }
	virtual void setOptionValue(const char *ival);
	virtual const char *getOptionValue();
	virtual OptionsList getOptionValues() { return options; }
};

#endif




