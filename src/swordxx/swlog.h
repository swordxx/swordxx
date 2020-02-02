/******************************************************************************
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

#ifndef SWORDXX_SWLOG_H
#define SWORDXX_SWLOG_H

#include <memory>
#include <utility>
#include "defs.h"


namespace swordxx {

class SWDLLEXPORT SWLog {

public: /* Types: */

    enum LogLevel {
        LOG_ERROR     = 1,
        LOG_WARN      = 2,
        LOG_INFO      = 3,
        LOG_TIMEDINFO = 4,
        LOG_DEBUG     = 5
    };

public: /* Methods: */

    SWLog() noexcept;
    virtual ~SWLog() noexcept;

    void setLogLevel(LogLevel level) noexcept { m_logLevel = level; }
    char getLogLevel() const noexcept { return m_logLevel; }

    template <typename ... Args>
    void logError(Args && ... args) const
    { return logMessage<LOG_ERROR>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void logWarning(Args && ... args) const
    { return logMessage<LOG_WARN>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void logInformation(Args && ... args) const
    { return logMessage<LOG_INFO>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void logTimedInformation(Args && ... args) const
    { return logMessage<LOG_TIMEDINFO>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void logDebug(Args && ... args) const
    { return logMessage<LOG_DEBUG>(std::forward<Args>(args)...); }

    static std::shared_ptr<SWLog> const & getSystemLog();
    static void setSystemLog(std::shared_ptr<SWLog> newLogger) noexcept;

private: /* Methods: */

    template <LogLevel LOGLEVEL>
    void logMessage(char const * fmt, ...) const;

private: /* Fields: */

    LogLevel m_logLevel = LOG_ERROR;
    static std::shared_ptr<SWLog> m_systemLog;

};

extern template
void SWLog::logMessage<SWLog::LOG_ERROR>(char const * fmt, ...) const;
extern template
void SWLog::logMessage<SWLog::LOG_WARN>(char const * fmt, ...) const;
extern template
void SWLog::logMessage<SWLog::LOG_INFO>(char const * fmt, ...) const;
extern template
void SWLog::logMessage<SWLog::LOG_TIMEDINFO>(char const * fmt, ...) const;
extern template
void SWLog::logMessage<SWLog::LOG_DEBUG>(char const * fmt, ...) const;

} /* namespace swordxx */

#endif /* SWORDXX_SWLOG_H */
