/******************************************************************************
 *  swlog.h	- definition of class SWLog used for logging messages
 *
 * $Id$
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

SWORD_NAMESPACE_START

class SWDLLEXPORT SWLog {
	friend class __staticsystemLog;
protected:
	char logLevel;
	static SWLog *systemLog;

public:

	static SWLog *getSystemLog();
	static void setSystemLog(SWLog *newLogger);

	SWLog () { logLevel = 1;	/*default to show only errors*/}
	virtual ~SWLog() {};

	virtual void setLogLevel(char level) { logLevel = level; }
	virtual char getLogLevel() { return logLevel; }
	virtual void logWarning (char *fmt, ...);
	virtual void logError (char *fmt, ...);
	virtual void logTimedInformation (char *fmt, ...);
	virtual void logInformation (char *fmt, ...);
};

SWORD_NAMESPACE_END
#endif
