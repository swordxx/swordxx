/***************************************************************************
                   osiscgi.h  -  OSIS to Diatheke/CGI format
                             -------------------
    begin                : 2003-10-21
    copyright            : 2003 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef OSISCGI_H
#define OSISCGI_H

#include <swbasicfilter.h>

#include <defs.h>

SWORD_NAMESPACE_START

/** this filter converts OSIS text to Diatheke/CGI format
 */
class SWDLLEXPORT OSISCGI : public SWBasicFilter {
private:
protected:
	class MyUserData : public BasicFilterUserData {
	public:
		bool osisQToTick;
		bool inBold;
		SWBuf w;
		SWBuf fn;
		MyUserData(const SWModule *module, const SWKey *key);
	};
	virtual BasicFilterUserData *createUserData(const SWModule *module, const SWKey *key) {
		return new MyUserData(module, key);
	}
	virtual bool handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData);
public:
	OSISCGI();
};

SWORD_NAMESPACE_END
#endif
