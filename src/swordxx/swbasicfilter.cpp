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

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <set>
#include "utilstr.h"


namespace swordxx {

BasicFilterUserData::~BasicFilterUserData() noexcept = default;

SWBasicFilter::SWBasicFilter(Flags flags)
    : m_flags(flags)
{}

void SWBasicFilter::addTokenSubstitute(char const * findString,
                                       char const * replaceString)
{
    m_tokenSubMap[(m_flags & CaseSensitiveTokens)
                  ? findString
                  : utf8ToUpper(findString)] = replaceString;
}

void SWBasicFilter::addAllowedEscapeString(char const * findString) {
    m_escPassSet.emplace((m_flags & CaseSensitiveEscapeStrings)
                         ? findString
                         : utf8ToUpper(findString));
}

void SWBasicFilter::addEscapeStringSubstitute(char const * findString,
                                              char const * replaceString)
{
    m_escSubMap.emplace((m_flags & CaseSensitiveEscapeStrings)
                        ? findString
                        : utf8ToUpper(findString),
                        replaceString);
}

bool SWBasicFilter::substituteToken(std::string & buf, char const * token) {
    if (auto const it = m_tokenSubMap.find((m_flags & CaseSensitiveTokens)
                                           ? token
                                           : utf8ToUpper(token));
        it != m_tokenSubMap.end())
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

std::size_t SWBasicFilter::processPrechars(std::string &,
                                           std::string_view,
                                           BasicFilterUserData *)
{ return 0u; }

char SWBasicFilter::processText(std::string & text,
                                SWKey const * key,
                                SWModule const * module)
{
    std::string token;
    bool intoken = false;
    bool inEsc = false;
    std::string lastTextNode;
    auto const userData(createUserData(module, key));

    std::string_view view(text);
    std::string out;

    /** This appends escString to buf as an entity */
    static auto const appendEscapeString =
            [](std::string & buf, std::string_view escString)
            { buf.append(1u, '&').append(escString).append(1u, ';'); };

    auto const substituteEscapeString =
            [this](std::string & buf, char const * escString)
    {
        if (*escString == '#') {
            if (!(m_flags & PassThroughNumericEscapeStrings))
                return false;
            appendEscapeString(buf, escString);
            return true;
        }
        // Pass allowed escape strings:
        if (auto const it =
                    m_escPassSet.find((m_flags & CaseSensitiveEscapeStrings)
                                      ? escString
                                      : utf8ToUpper(escString));
            it != m_escPassSet.end())
        {
            appendEscapeString(buf, escString);
            return true;
        }

        if (auto const it =
                    m_escSubMap.find((m_flags & CaseSensitiveEscapeStrings)
                                     ? escString
                                     : utf8ToUpper(escString));
            it != m_escSubMap.end())
        {
            buf += it->second;
            return true;
        }
        return false;
    };

    for (; !view.empty(); view.remove_prefix(1u)) {
        // If processStage handled this char:
        if (auto const n = processPrechars(out, view, userData.get())) {
            assert(n <= view.size());
            if (n >= view.size())
                break;
            view.remove_prefix(n);
        }

        if (view.front() == '<') {
            intoken = true;
            token.clear();
            inEsc = false;
            continue;
        }

        if (view.front() == '&') {
            intoken = true;
            token.clear();
            inEsc = true;
            continue;
        }

        if (inEsc) {
            if (view.front() == ';') {
                intoken = inEsc = false;
                userData->lastTextNode = lastTextNode;

                // If text through is disabled no tokens should pass, too:
                if (!userData->suspendTextPassThru
                    && !substituteEscapeString(out, token.c_str())
                    && (m_flags & PassThroughUnknownEscapeStrings))
                    appendEscapeString(out, token);
                lastTextNode.clear();

                continue;
            }
        } else {
            if (view.front() == '>') {
                intoken = false;
                userData->lastTextNode = lastTextNode;
                if (!handleToken(out, token.c_str(), userData.get())
                    && (m_flags & PassThroughUnknownTokens))
                    out.append(1u, '<').append(token).append(1u, '>');
                lastTextNode.clear();
                if (!userData->suspendTextPassThru)
                       userData->lastSuspendSegment.clear();

                continue;
            }
        }

        if (intoken) {
            token += view.front();
        } else {
            if (!userData->supressAdjacentWhitespace || (view.front() != ' ')) {
                if (!userData->suspendTextPassThru) {
                    out.push_back(view.front());
                } else {
                    userData->lastSuspendSegment.push_back(view.front());
                }
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
