/***************************************************************************
                          thmlosis.h  -  description
                             -------------------
    begin                : 2001-09-03
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

#ifndef THMLOSIS_H
#define THMLOSIS_H

#include <defs.h>
#include <swfilter.h>

SWORD_NAMESPACE_START

/** this filter converts ThML text to OSIS text
 */
class SWDLLEXPORT ThMLOSIS : public SWFilter {
protected:
	virtual const char *convertToOSIS(const char *, const SWKey *key);
	void pushString(char **buf, const char *format, ...);
public:
	ThMLOSIS();
	virtual ~ThMLOSIS();
	char ProcessText(char *text, int maxlen, const SWKey *, const SWModule * = 0);
};
SWORD_NAMESPACE_END
#endif /* THMLOSIS_H */
