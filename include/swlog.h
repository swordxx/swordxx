/******************************************************************************
 *  swlog.h	- definition of class SWLog used for logging messages
 *
 * $Id: swlog.h,v 1.1 1999/05/04 22:03:36 scribe Exp $
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

class SWLog {
public:
	static SWLog *systemlog;
	
	bool log;
	SWLog() { log = true; }
	virtual void LogWarning(char *fmt, ...);
	virtual void LogError(char *fmt, ...);
	virtual void LogTimedInformation(char *fmt, ...);
	virtual void LogInformation(char *fmt, ...);
};


#endif
 
