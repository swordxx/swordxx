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

class __staticsystemLog {
public:
	__staticsystemLog() { }
	~__staticsystemLog() { delete SWLog::systemLog; }
} _staticsystemLog;


SWLog *SWLog::getSystemLog() {
	if (!systemLog)
		systemLog = new SWLog();

	return systemLog;
}


void SWLog::setSystemLog(SWLog *newLog) {
	if (systemLog)
		delete systemLog;
	systemLog = newLog;
}


void SWLog::logWarning(char *fmt, ...) {
	char msg[2048];
	va_list argptr;

	if (logLevel >= 2) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _WIN32_WCE
		std::cerr << msg;
		std::cerr << std::endl;
#endif
	}
}


void SWLog::logError(char *fmt, ...) {
	char msg[2048];
	va_list argptr;

	if (logLevel) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _WIN32_WCE
		std::cerr << msg;
		std::cerr << std::endl;
#endif
	}
}


void SWLog::logTimedInformation(char *fmt, ...) {
	char msg[2048];
	va_list argptr;

	if (logLevel >= 4) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _WIN32_WCE
		std::cout << msg;
		std::cout << std::endl;
#endif
	}
}


void SWLog::logInformation(char *fmt, ...) {
	char msg[2048];
	va_list argptr;

	if (logLevel >= 3) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _WIN32_WCE
		std::cout << msg;
		std::cout << std::endl;
#endif
	}
}

SWORD_NAMESPACE_END
