//---------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>
#ifndef	_WIN32_WCE
#include <iostream>
#endif
#if defined(_ICU_) && !defined(_ICUSWORD_)
#define _USTDIO_
#include <unicode/ustdio.h>
#include <unicode/ustream.h>
#endif
#include "swlog.h"
//---------------------------------------------------------------------------

SWORD_NAMESPACE_START


SWLog *SWLog::systemLog = 0;


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

	if (logLevel >= 2) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, 2);
	}
}


void SWLog::logError(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, 1);
	}
}


void SWLog::logTimedInformation(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel >= 4) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, 4);
	}
}


void SWLog::logInformation(const char *fmt, ...) const {
	char msg[2048];
	va_list argptr;

	if (logLevel >= 3) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);
		logMessage(msg, 3);
	}
}

void SWLog::logMessage(const char *message, int level) const {
	std::cerr << message;
	std::cerr << std::endl;
}
SWORD_NAMESPACE_END
