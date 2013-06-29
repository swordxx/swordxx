/******************************************************************************
 *
 *  swlog.cpp -	
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#if defined(_ICU_) && !defined(_ICUSWORD_)
#define _USTDIO_
#include <unicode/ustdio.h>
#include <unicode/ustream.h>
#endif
#include "swlog.h"


SWORD_NAMESPACE_START


SWLog *SWLog::systemLog = 0;

const int SWLog::LOG_ERROR     = 1;
const int SWLog::LOG_WARN      = 2;
const int SWLog::LOG_INFO      = 3;
const int SWLog::LOG_TIMEDINFO = 4;
const int SWLog::LOG_DEBUG     = 5;

SWLog *SWLog::getSystemLog() {
	static class __staticSystemLog {
	SWLog **clear;
	public:
		__staticSystemLog(SWLog **clear) { this->clear = clear; }
		~__staticSystemLog() { delete *clear; *clear = 0; }
	} _staticSystemLog(&SWLog::systemLog);

	if (!systemLog)
		systemLog = new SWLog();

	return systemLog;
}


void SWLog::setSystemLog(SWLog *newLog) {
	delete getSystemLog();
	systemLog = newLog;
}


void SWLog::logWarning(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel >= LOG_WARN) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, LOG_WARN);
	}
}


void SWLog::logError(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, LOG_ERROR);
	}
}


void SWLog::logInformation(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel >= LOG_INFO) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, LOG_INFO);
	}
}


void SWLog::logTimedInformation(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel >= LOG_TIMEDINFO) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, LOG_TIMEDINFO);
	}
}


void SWLog::logDebug(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel >= LOG_DEBUG) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, LOG_DEBUG);
	}
}

void SWLog::logMessage(const char *message, int level) const {
	std::cerr << message;
	std::cerr << std::endl;
}
SWORD_NAMESPACE_END
