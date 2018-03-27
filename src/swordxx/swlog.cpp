/******************************************************************************
 *
 *  swlog.cpp -
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#include "swlog.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>
#if SWORDXX_HAS_ICU
#include <unicode/ustdio.h>
#include <unicode/ustream.h>
#endif


namespace swordxx {


std::shared_ptr<SWLog> SWLog::systemLog;

const int SWLog::LOG_ERROR     = 1;
const int SWLog::LOG_WARN      = 2;
const int SWLog::LOG_INFO      = 3;
const int SWLog::LOG_TIMEDINFO = 4;
const int SWLog::LOG_DEBUG     = 5;

std::shared_ptr<SWLog> const & SWLog::getSystemLog() {
    if (!systemLog)
        systemLog = std::make_shared<SWLog>();

    return systemLog;
}


void SWLog::setSystemLog(std::shared_ptr<SWLog> newLogger) {
    systemLog = std::move(newLogger);
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

void SWLog::logMessage(const char *message, int /* level */) const {
    std::cerr << message;
    std::cerr << std::endl;
}
} /* namespace swordxx */
