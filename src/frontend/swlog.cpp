//---------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>
#include <iostream.h>
#include "swlog.h"
//---------------------------------------------------------------------------

SWLog *SWLog::systemlog = new SWLog();

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

		cerr << msg;
		cerr << "\n";
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

		cerr << msg;
		cerr << "\n";
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

		cout << msg;
		cout << "\n";
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

		cout << msg;
		cout << "\n";
	}
} 
