/******************************************************************************
 *  swlog.h	- definition of class SWLog used for logging messages
 *
 * $Id: swlog.h,v 1.4 2002/07/19 23:00:43 scribe Exp $
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

//---------------------------------------------------------------------------
#ifndef swlogH
#define swlogH
//---------------------------------------------------------------------------

#include <defs.h>

class SWDLLEXPORT SWLog {
protected:
	char logLevel;

public:
	static SWLog *systemlog;

	SWLog () { logLevel = 1;	/*default to show only errors*/}
	virtual void setLogLevel(char level) { logLevel = level; }
	virtual char getLogLevel() { return logLevel; }
	virtual void LogWarning (char *fmt, ...);
	virtual void LogError (char *fmt, ...);
	virtual void LogTimedInformation (char *fmt, ...);
	virtual void LogInformation (char *fmt, ...);
};


#endif
