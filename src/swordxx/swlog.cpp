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

#include "swlog.h"

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <new>
#include "max_v.h"


namespace swordxx {

std::shared_ptr<SWLog> SWLog::m_systemLog;

SWLog::SWLog() noexcept = default;
SWLog::~SWLog() noexcept = default;

std::shared_ptr<SWLog> const & SWLog::getSystemLog() {
    if (!m_systemLog)
        m_systemLog = std::make_shared<SWLog>();

    return m_systemLog;
}

void SWLog::setSystemLog(std::shared_ptr<SWLog> newLogger) noexcept
{ m_systemLog = std::move(newLogger); }

template <SWLog::LogLevel LOGLEVEL>
void SWLog::logMessage(char const * fmt, ...) const {
    using U = std::underlying_type<LogLevel>::type;
    if (static_cast<U>(m_logLevel) >= static_cast<U>(LOGLEVEL)) {
        va_list argptr;
        va_start(argptr, fmt);
        auto size_(std::vsnprintf(nullptr, 0u, fmt, argptr));
        va_end(argptr);
        assert(size_ >= 0);
        static_assert(max_v<decltype(size_)> <= max_v<std::size_t>, "");
        auto size(static_cast<std::size_t>(size_));
        if (!++size)
            throw std::bad_array_new_length();
        auto strData(std::make_unique<char[]>(size));

        va_start(argptr, fmt);
        std::vsnprintf(strData.get(), size, fmt, argptr);
        va_end(argptr);
    }
}

template void SWLog::logMessage<SWLog::LOG_ERROR>(char const * fmt, ...) const;
template void SWLog::logMessage<SWLog::LOG_WARN>(char const * fmt, ...) const;
template void SWLog::logMessage<SWLog::LOG_INFO>(char const * fmt, ...) const;
template void SWLog::logMessage<SWLog::LOG_TIMEDINFO>(char const * fmt, ...)
        const;
template void SWLog::logMessage<SWLog::LOG_DEBUG>(char const * fmt, ...) const;


} /* namespace swordxx */
