/******************************************************************************
 *
 *  swbasicfilter.cpp -    definition of class SWBasicFilter.  An SWFilter
 *              impl that provides some basic methods that
 *              many filters will need and can use as a starting
 *              point.
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swbasicfilter.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <set>
#include "stringmgr.h"
#include "utilstr.h"


namespace swordxx {

BasicFilterUserData::~BasicFilterUserData() noexcept = default;

void SWBasicFilter::addTokenSubstitute(char const * findString,
                                       char const * replaceString)
{
    if (!m_tokenCaseSensitive) {
        std::string buf(findString);
        toupperstr(buf);
        m_tokenSubMap[buf] = replaceString;
    } else {
        m_tokenSubMap[findString] = replaceString;
    }
}

void SWBasicFilter::addAllowedEscapeString(char const * findString) {
    std::string buf(findString);
    if (!m_escStringCaseSensitive)
        toupperstr(buf);
    m_escPassSet.emplace(std::move(buf));
}

void SWBasicFilter::addEscapeStringSubstitute(char const * findString,
                                              char const * replaceString)
{
    std::string buf(findString);
    if (!m_escStringCaseSensitive)
        toupperstr(buf);
    m_escSubMap.emplace(std::move(buf), replaceString);
}

bool SWBasicFilter::substituteToken(std::string & buf, char const * token) {
    decltype(m_tokenSubMap)::iterator it;
    if (!m_tokenCaseSensitive) {
        std::string tmp(token);
        toupperstr(tmp);
        it = m_tokenSubMap.find(std::move(tmp));
    } else {
        it = m_tokenSubMap.find(token);
    }

    if (it != m_tokenSubMap.end()) {
        buf += it->second;
        return true;
    }
    return false;
}

bool SWBasicFilter::passAllowedEscapeString(std::string & buf,
                                            char const * escString)
{
    decltype(m_escPassSet)::iterator it;
    if (!m_escStringCaseSensitive) {
        std::string tmp(escString);
        toupperstr(tmp);
        it = m_escPassSet.find(std::move(tmp));
    } else {
        it = m_escPassSet.find(escString);
    }

    if (it != m_escPassSet.end()) {
        appendEscapeString(buf, escString);
        return true;
    }

    return false;
}

bool SWBasicFilter::handleNumericEscapeString(std::string & buf,
                                              char const * escString)
{
    if (m_passThruNumericEsc) {
        appendEscapeString(buf, escString);
        return true;
    }
    return false;
}

bool SWBasicFilter::substituteEscapeString(std::string & buf,
                                           char const * escString)
{
    if (*escString == '#')
        return handleNumericEscapeString(buf, escString);
    if (passAllowedEscapeString(buf, escString))
        return true;

    decltype(m_escSubMap)::iterator it;
    if (!m_escStringCaseSensitive) {
        std::string tmp(escString);
        toupperstr(tmp);
        it = m_escSubMap.find(std::move(tmp));
    } else {
        it = m_escSubMap.find(escString);
    }

    if (it != m_escSubMap.end()) {
        buf += it->second;
        return true;
    }
    return false;
}

bool SWBasicFilter::handleToken(std::string & buf,
                                char const * token,
                                BasicFilterUserData * /* userData */)
{ return substituteToken(buf, token); }

bool SWBasicFilter::handleEscapeString(std::string & buf,
                                       char const * escString,
                                       BasicFilterUserData * /* userData */)
{ return substituteEscapeString(buf, escString); }

char SWBasicFilter::processText(std::string & text,
                                SWKey const * key,
                                SWModule const * module)
{
    char * from;
    char token[4096u];
    std::size_t tokpos = 0u;
    bool intoken = false;
    bool inEsc = false;
    std::size_t escStartPos = 0u;
    std::size_t escEndPos = 0u;
    std::size_t tokenStartPos = 0u;
    std::size_t tokenEndPos = 0u;
    std::string lastTextNode;
    auto const userData(createUserData(module, key));

    std::string orig = text;
    from = &orig[0u];
    text = "";

    // Check if processStage handled it all:
    if ((m_processStages & INITIALIZE)
        && processStage(INITIALIZE, text, from, userData.get()))
        return 0;

    for (;*from; from++) {
        // If processStage handled this char:
        if ((m_processStages & PRECHAR)
            && processStage(PRECHAR, text, from, userData.get()))
            continue;

        if (*from == m_tokenStart[tokenStartPos]) {
            if (tokenStartPos == (m_tokenStart.size() - 1u)) {
                intoken = true;
                tokpos = 0;
                token[0] = 0;
                token[1] = 0;
                token[2] = 0;
                inEsc = false;
            }
            else tokenStartPos++;
            continue;
        }

        if (*from == m_escStart[escStartPos]) {
            if (escStartPos == (m_escStart.size() - 1u)) {
                intoken = true;
                tokpos = 0;
                token[0] = 0;
                token[1] = 0;
                token[2] = 0;
                inEsc = true;
            }
            else escStartPos++;
            continue;
        }

        if (inEsc) {
            if (*from == m_escEnd[escEndPos]) {
                if (escEndPos == (m_escEnd.size() - 1u)) {
                    intoken = inEsc = false;
                    userData->lastTextNode = lastTextNode;

                    // If text through is disabled no tokens should pass, too:
                    if (!userData->suspendTextPassThru
                        && !handleEscapeString(text, token, userData.get())
                        && m_passThruUnknownEsc)
                        appendEscapeString(text, token);
                    escEndPos = escStartPos = tokenEndPos = tokenStartPos = 0;
                    lastTextNode = "";
                    continue;
                }
            }
        }

        if (!inEsc) {
            if (*from == m_tokenEnd[tokenEndPos]) {
                if (tokenEndPos == (m_tokenEnd.size() - 1u)) {
                    intoken = false;
                    userData->lastTextNode = lastTextNode;
                    if (!handleToken(text, token, userData.get())
                        && m_passThruUnknownToken)
                    {
                        text += m_tokenStart;
                        text += token;
                        text += m_tokenEnd;
                    }
                    escEndPos = escStartPos = tokenEndPos = tokenStartPos = 0;
                    lastTextNode = "";
                    if (!userData->suspendTextPassThru)
                           userData->lastSuspendSegment.clear();
                    continue;
                }
            }
        }

        if (intoken) {
            if (tokpos < 4090) {
                token[tokpos++] = *from;
                token[tokpos+2] = 0;
            }
        }
        else {
             if ((!userData->supressAdjacentWhitespace) || (*from != ' ')) {
                if (!userData->suspendTextPassThru)
                    text.push_back(*from);
                else    userData->lastSuspendSegment.push_back(*from);
                lastTextNode.push_back(*from);
             }
            userData->supressAdjacentWhitespace = false;
        }

        if (m_processStages & POSTCHAR)
            processStage(POSTCHAR, text, from, userData.get());

    }

    if (m_processStages & FINALIZE)
        processStage(FINALIZE, text, from, userData.get());
    return 0;
}

std::unique_ptr<BasicFilterUserData> SWBasicFilter::createUserData(
        SWModule const * const module,
        SWKey const * const key)
{ return std::make_unique<BasicFilterUserData>(module, key); }

} /* namespace swordxx */
