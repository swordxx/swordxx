//---------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <iostream>
#endif
#include "swlog.h"
//---------------------------------------------------------------------------


SWLog *SWLog::systemlog = 0;


class __staticsystemlog {
public:
	__staticsystemlog() {
		SWLog::systemlog = new SWLog();
	}
	~__staticsystemlog() {
		delete SWLog::systemlog;
	}
} _staticsystemlog;


void SWLog::LogWarning(char *fmt, ...)
{
	char msg[2048];
	va_list argptr;

	if (logLevel >= 2) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _MSC_VER
		std::cerr << msg;
		std::cerr << std::endl;
#endif
	}
}


void SWLog::LogError(char *fmt, ...)
{
	char msg[2048];
	va_list argptr;

	if (logLevel) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _MSC_VER
		std::cerr << msg;
		std::cerr << std::endl;
#endif
	}
}


void SWLog::LogTimedInformation(char *fmt, ...)
{
	char msg[2048];
	va_list argptr;

	if (logLevel >= 4) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _MSC_VER
		std::cout << msg;
		std::cout << std::endl;
#endif
	}
}


void SWLog::LogInformation(char *fmt, ...)
{
	char msg[2048];
	va_list argptr;

	if (logLevel >= 3) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef _MSC_VER
		std::cout << msg;
		std::cout << std::endl;
#endif
	}
} 
