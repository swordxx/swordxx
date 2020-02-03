/******************************************************************************
 *
 *  swbasicfilter.cpp -    definition of class SWBasicFilter.  An SWFilter
 *              impl that provides some basic methods that
 *              many filters will need and can use as a starting
 *              point.
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
#include "utilstr.h"


namespace swordxx {

BasicFilterUserData::~BasicFilterUserData() noexcept = default;

void SWBasicFilter::addTokenSubstitute(char const * findString,
                                       char const * replaceString)
{
    m_tokenSubMap[m_tokenCaseSensitive ? findString : utf8ToUpper(findString)] =
            replaceString;
}

void SWBasicFilter::addAllowedEscapeString(char const * findString) {
    m_escPassSet.emplace(m_escStringCaseSensitive
                         ? findString
                         : utf8ToUpper(findString));
}

void SWBasicFilter::addEscapeStringSubstitute(char const * findString,
                                              char const * replaceString)
{
    m_escSubMap.emplace(m_escStringCaseSensitive
                        ? findString
                        : utf8ToUpper(findString),
                        replaceString);
}

bool SWBasicFilter::substituteToken(std::string & buf, char const * token) {
    if (auto const it = m_tokenSubMap.find(m_tokenCaseSensitive
                                           ? token
                                           : utf8ToUpper(token));
        it != m_tokenSubMap.end())
    {
        buf += it->second;
        return true;
    }
    return false;
}

bool SWBasicFilter::passAllowedEscapeString(std::string & buf,
                                            char const * escString)
{
    if (auto const it = m_escPassSet.find(m_escStringCaseSensitive
                                          ? escString
                                          : utf8ToUpper(escString));
        it != m_escPassSet.end())
    {
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

    if (auto const it = m_escSubMap.find(m_escStringCaseSensitive
                                         ? escString
                                         : utf8ToUpper(escString));
        it != m_escSubMap.end())
    {
        buf += it->second;
        return true;
    }
    return false;
}

bool SWBasicFilter::handleToken(std::string & buf,
                                char const * token,
                                BasicFilterUserData * /* userData */)
{ return substituteToken(buf, token); }

bool SWBasicFilter::processPrechar(std::string &,
                                   std::string_view &,
                                   BasicFilterUserData *)
{ return false; }

bool SWBasicFilter::handleEscapeString(std::string & buf,
                                       char const * escString,
                                       BasicFilterUserData * /* userData */)
{ return substituteEscapeString(buf, escString); }

char SWBasicFilter::processText(std::string & text,
                                SWKey const * key,
                                SWModule const * module)
{
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

    std::string_view view(text);
    std::string out;

    for (; !view.empty(); view.remove_prefix(1u)) {
        // If processStage handled this char:
        if (processPrechar(out, view, userData.get()))
            continue;

        if (view.front() == m_tokenStart[tokenStartPos]) {
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

        if (view.front() == m_escStart[escStartPos]) {
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
            if (view.front() == m_escEnd[escEndPos]) {
                if (escEndPos == (m_escEnd.size() - 1u)) {
                    intoken = inEsc = false;
                    userData->lastTextNode = lastTextNode;

                    // If text through is disabled no tokens should pass, too:
                    if (!userData->suspendTextPassThru
                        && !handleEscapeString(out, token, userData.get())
                        && m_passThruUnknownEsc)
                        appendEscapeString(out, token);
                    escEndPos = escStartPos = tokenEndPos = tokenStartPos = 0;
                    lastTextNode = "";
                    continue;
                }
            }
        }

        if (!inEsc) {
            if (view.front() == m_tokenEnd[tokenEndPos]) {
                if (tokenEndPos == (m_tokenEnd.size() - 1u)) {
                    intoken = false;
                    userData->lastTextNode = lastTextNode;
                    if (!handleToken(out, token, userData.get())
                        && m_passThruUnknownToken)
                    {
                        out += m_tokenStart;
                        out += token;
                        out += m_tokenEnd;
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
                token[tokpos++] = view.front();
                token[tokpos+2] = 0;
            }
        }
        else {
             if ((!userData->supressAdjacentWhitespace) || (view.front() != ' ')) {
                if (!userData->suspendTextPassThru)
                    out.push_back(view.front());
                else    userData->lastSuspendSegment.push_back(view.front());
                lastTextNode.push_back(view.front());
             }
            userData->supressAdjacentWhitespace = false;
        }
    }

    text = std::move(out);
    return 0;
}

std::unique_ptr<BasicFilterUserData> SWBasicFilter::createUserData(
        SWModule const * const module,
        SWKey const * const key)
{ return std::make_unique<BasicFilterUserData>(module, key); }

} /* namespace swordxx */
