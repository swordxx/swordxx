/******************************************************************************
 *
 *  osisplain.cpp -    An SWFilter that provides stripping of OSIS tags
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osisplain.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include "../keys/versekey.h"
#include "../SimpleTokenizer.h"
#include "../swmodule.h"
#include "../unicode.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {


namespace {

    class MyUserData : public BasicFilterUserData {
    public:
        std::string w;
        XMLTag tag;
        char testament = (verseKey) ? verseKey->getTestament() : 2; // default to NT;
        std::string hiType;
        using BasicFilterUserData::BasicFilterUserData;
    };
}


OSISPlain::OSISPlain() {
    setEscapeStringCaseSensitive(true);

    addEscapeStringSubstitute("amp", "&");
    addEscapeStringSubstitute("apos", "'");
    addEscapeStringSubstitute("lt", "<");
    addEscapeStringSubstitute("gt", ">");
    addEscapeStringSubstitute("quot", "\"");

       setTokenCaseSensitive(true);
       addTokenSubstitute("title", "\n");
       addTokenSubstitute("/title", "\n");
       addTokenSubstitute("/l", "\n");
       addTokenSubstitute("lg", "\n");
       addTokenSubstitute("/lg", "\n");
}

std::unique_ptr<BasicFilterUserData> OSISPlain::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool OSISPlain::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    using namespace std::literals::string_view_literals;

       // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        auto & u = *static_cast<MyUserData *>(userData);
        if (((*token == 'w') && (token[1] == ' ')) ||
            ((*token == '/') && (token[1] == 'w') && (!token[2]))) {
                 u.tag = token;

            bool start = false;
            if (*token == 'w') {
                if (token[std::strlen(token)-1] != '/') {
                    u.w = token;
                    return true;
                }
                start = true;
            }
            u.tag = (start) ? token : u.w.c_str();
            bool show = true;    // to handle unplaced article in kjv2003-- temporary till combined

            std::string lastText = (start) ? "stuff" : u.lastTextNode.c_str();

            std::string attrib;
            if (!(attrib = u.tag.attribute("xlit")).empty()) {
                char const * val = std::strchr(attrib.c_str(), ':');
                val = (val) ? (val + 1) : attrib.c_str();
                buf.append(" <");
                buf.append(val);
                buf.push_back('>');
            }
            if (!(attrib = u.tag.attribute("gloss")).empty()) {
                buf.append(" <");
                buf.append(attrib);
                buf.push_back('>');
            }
            if (!(attrib = u.tag.attribute("lemma")).empty()) {
                for (auto const & lemmaToken
                     : SimpleTokenizer<>::tokenize(attrib, ' '))
                {
                    auto const separatorPos(lemmaToken.find(':'));
                    auto val((separatorPos == std::string_view::npos)
                             ? lemmaToken
                             : lemmaToken.substr(separatorPos + 1u));
                    char gh;
                    if ((val.size() >= 2u)
                        && ((val[0u] == 'G') || (val[0u] == 'H'))
                        && (charIsDigit(val[1u])))
                    {
                        gh = val.front();
                        val.remove_prefix(1u);
                    }
                    else {
                        gh = (u.testament>1) ? 'G' : 'H';
                    }
                    if ((val == "3588") && (lastText.length() < 1))
                        show = false;
                    else    {
                        buf.append(" <");
                        buf.push_back(gh);
                        buf.append(val);
                        buf.append(">");
                    }
                }
            }
            if (!(attrib = u.tag.attribute("morph")).empty() && (show)) {
                for (auto const & morphToken
                     : SimpleTokenizer<>::tokenize(attrib, ' '))
                {
                    auto const separatorPos(morphToken.find(':'));
                    auto val((separatorPos == std::string_view::npos)
                             ? morphToken
                             : morphToken.substr(separatorPos + 1u));
                    if ((val.size() >= 3u)
                        && (val.front() == 'T')
                        && ((val[1u] == 'G') || (val[1u] == 'H'))
                        && charIsDigit(val[2u]))
                        val.remove_prefix(2u);
                    buf.append(" (");
                    buf.append(val);
                    buf.push_back(')');
                }
            }
            if (!(attrib = u.tag.attribute("POS")).empty()) {
                char const * val = std::strchr(attrib.c_str(), ':');
                val = (val) ? (val + 1) : attrib.c_str();

                buf.append(" <");
                buf.append(val);
                buf.push_back('>');
            }
        }

        // <note> tag
        else if (startsWith(token, "note"sv)) {
                if (!std::strstr(token, "strongsMarkup")) {    // leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
                    buf.append(" [");
                }
                else    u.suspendTextPassThru = true;
                if (u.module) {
                    XMLTag tag = token;
                    std::string swordFootnote = tag.attribute("swordFootnote");
                    std::string footnoteBody = u.module->getEntryAttributes()["Footnote"][swordFootnote]["body"];
                    buf.append(u.module->renderText(footnoteBody.c_str()));
                }
            }
        else if (startsWith(token, "/note"sv)) {
            if (!u.suspendTextPassThru)
                buf.append("] ");
            else    u.suspendTextPassThru = false;
        }

        // <p> paragraph tag
        else if (((*token == 'p') && ((token[1] == ' ') || (!token[1]))) ||
            ((*token == '/') && (token[1] == 'p') && (!token[2]))) {
                userData->supressAdjacentWhitespace = true;
                buf.push_back('\n');
        }

        // Milestoned paragraph, created by osis2mod
        // <div type="paragraph"  sID... />
        // <div type="paragraph"  eID... />
        else if ((u.tag.name() == "div") && ((u.tag.attribute("type") == "x-p") || (u.tag.attribute("type") == "paragraph")) &&
            (u.tag.isEmpty() && (!u.tag.attribute("sID").empty() || !u.tag.attribute("eID").empty()))) {
                userData->supressAdjacentWhitespace = true;
                buf.push_back('\n');
        }

                // <lb .../>
                else if (startsWith(token, "lb"sv)) {
            userData->supressAdjacentWhitespace = true;
            buf.push_back('\n');
        }
        else if (startsWith(token, "l"sv) && std::strstr(token, "eID")) {
            userData->supressAdjacentWhitespace = true;
            buf.push_back('\n');
        }
        else if (startsWith(token, "/divineName"sv)) {
            // Get the end portion of the string, and upper case it
            assert(buf.size() >= u.lastTextNode.size());
            auto const endPos(buf.size() - u.lastTextNode.size());
            auto end(buf.substr(endPos));
            auto const endSize(end.size());
            end = utf8ToUpper(end);
            buf.replace(endPos, endSize, end);
        }
        else if (startsWith(token, "hi"sv)) {

                // handle both OSIS 'type' and TEI 'rend' attributes
                // there is no officially supported OSIS overline attribute,
                // thus either TEI overline or OSIS x-overline would be best,
                // but we have used "ol" in the past, as well.  Once a valid
                // OSIS overline attribute is made available, these should all
                // eventually be deprecated and never documented that they are supported.
                if (std::strstr(token, "rend=\"ol\"") || std::strstr(token, "rend=\"x-overline\"") || std::strstr(token, "rend=\"overline\"")
                   || std::strstr(token, "type=\"ol\"") || std::strstr(token, "type=\"x-overline\"") || std::strstr(token, "type=\"overline\"")) {
                    u.hiType = "overline";
                }
                else u.hiType = "";
                u.suspendTextPassThru = true;
            }
        else if (startsWith(token, "/hi"sv)) {
            if (u.hiType == "overline") {
                std::string_view sv = u.lastTextNode;
                while (!sv.empty()) {
                    auto const r(codepointFromUtf8(sv));
                    if (r.second) {
                        buf.append(sv.data(), r.second);
                        buf.append("\u0305"); // U+0305 COMBINING OVERLINE
                        sv.remove_prefix(r.second);
                    }
                }
            }
            else {
                buf.append("* ");
                buf.append(u.lastSuspendSegment);
                buf.append(" *");
            }
            u.suspendTextPassThru = false;
        } else if (startsWith(token, "q"sv)
                   && !u.tag.attribute("marker").empty())
        {
            buf.append(u.tag.attribute("marker"));
        }
                // <milestone type="line"/>
                else if (startsWith(token, "milestone"sv)) {
            const char* type = std::strstr(token+10, "type=\"");
            if (type && !startsWith(type+6, "line"sv)) { //we check for type != line
                userData->supressAdjacentWhitespace = true;
                    buf.push_back('\n');
            }
            if (!u.tag.attribute("marker").empty()) {
                buf.append(u.tag.attribute("marker"));
            }
                }

        else {
            return false;  // we still didn't handle token
        }
    }
    return true;
}

bool OSISPlain::processPrechar(std::string &,
                               std::string_view & view,
                               BasicFilterUserData *)
{
    // this is a strip filter so we want to do this as optimized as possible.
    // Avoid calling getUniCharFromUTF8 for slight speed improvement

    // skip soft hyphens:
    return startsWith(view, "\xC2\xAD");
}

} /* namespace swordxx */
