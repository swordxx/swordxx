//---------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>
#ifndef __VISUALC__
#include <iostream.h>
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

	if (log) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef __VISUALC__
		cerr << msg;
		cerr << "\n";
#endif
	}
}


void SWLog::LogError(char *fmt, ...)
{
	char msg[2048];
	va_list argptr;

	if (log) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef __VISUALC__
		cerr << msg;
		cerr << "\n";
#endif
	}
}


void SWLog::LogTimedInformation(char *fmt, ...)
{
	char msg[2048];
	va_list argptr;

	if (log) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef __VISUALC__
		cout << msg;
		cout << "\n";
#endif
	}
}


void SWLog::LogInformation(char *fmt, ...)
{
	char msg[2048];
	va_list argptr;

	if (log) {
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

#ifndef __VISUALC__
		cout << msg;
		cout << "\n";
#endif
	}
} 
