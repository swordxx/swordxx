/***************************************************************************
                          gbfosis.h  -  description
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

#ifndef GBFOSIS_H
#define GBFOSIS_H

#include <defs.h>
#include <swfilter.h>

/** this filter converts GBF text to HTML text with hrefs
 */
class SWDLLEXPORT GBFOSIS : public SWFilter {
protected:
	virtual const char *convertToOSIS(const char *, const SWKey *key);
	void pushString(char **buf, const char *format, ...);
public:
	GBFOSIS();
	virtual ~GBFOSIS();
	char ProcessText(char *text, int maxlen, const SWKey *, const SWModule * = 0);
};

#endif /* THMLOSIS_H */
