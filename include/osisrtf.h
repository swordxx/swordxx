/******************************************************************************
 *
 * $Id: osisrtf.h,v 1.7 2003/07/30 02:03:45 scribe Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifndef OSISRTF_H
#define OSISRTF_H

#include <swbasicfilter.h>

SWORD_NAMESPACE_START

/** this filter converts OSIS text to RTF text
 */
class SWDLLEXPORT OSISRTF : public SWBasicFilter {
private:

protected:
	class MyUserData : public UserData {
	public:
		bool osisQToTick;
		SWBuf w;
		MyUserData(const SWModule *module, const SWKey *key);
	};
	virtual UserData *createUserData(const SWModule *module, const SWKey *key) {
		return new MyUserData(module, key);
	}
	virtual bool handleToken(SWBuf &buf, const char *token, UserData *userData);
	virtual bool processStage(char stage, SWBuf &text, char *&from, UserData *userData);
public:
	OSISRTF();
};

SWORD_NAMESPACE_END
#endif
