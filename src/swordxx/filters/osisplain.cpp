/******************************************************************************
 *
 *  osisplain.cpp -    An SWFilter that provides stripping of OSIS tags
 *
 * $Id$
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
#include "../keys/versekey.h"
#include "../stringmgr.h"
#include "../swmodule.h"
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
        MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {}
    };
}


OSISPlain::OSISPlain() {
    setTokenStart("<");
    setTokenEnd(">");

    setEscapeStart("&");
    setEscapeEnd(";");

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

    setStageProcessing(PRECHAR);
}

std::unique_ptr<BasicFilterUserData> OSISPlain::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool OSISPlain::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
       // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        MyUserData *u = (MyUserData *)userData;
        if (((*token == 'w') && (token[1] == ' ')) ||
            ((*token == '/') && (token[1] == 'w') && (!token[2]))) {
                 u->tag = token;

            bool start = false;
            if (*token == 'w') {
                if (token[strlen(token)-1] != '/') {
                    u->w = token;
                    return true;
                }
                start = true;
            }
            u->tag = (start) ? token : u->w.c_str();
            bool show = true;    // to handle unplaced article in kjv2003-- temporary till combined

            std::string lastText = (start) ? "stuff" : u->lastTextNode.c_str();

            std::string attrib;
            const char *val;
            if (!(attrib = u->tag.attribute("xlit")).empty()) {
                val = strchr(attrib.c_str(), ':');
                val = (val) ? (val + 1) : attrib.c_str();
                buf.append(" <");
                buf.append(val);
                buf.push_back('>');
            }
            if (!(attrib = u->tag.attribute("gloss")).empty()) {
                buf.append(" <");
                buf.append(attrib);
                buf.push_back('>');
            }
            if (!(attrib = u->tag.attribute("lemma")).empty()) {
                int count = u->tag.attributePartCount("lemma", ' ');
                int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                do {
                    char gh;
                    attrib = u->tag.attribute("lemma", i, ' ');
                    if (i < 0) i = 0;    // to handle our -1 condition
                    val = strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    if ((strchr("GH", *val)) && (charIsDigit(val[1]))) {
                        gh = *val;
                        val++;
                    }
                    else {
                        gh = (u->testament>1) ? 'G' : 'H';
                    }
                    if ((!strcmp(val, "3588")) && (lastText.length() < 1))
                        show = false;
                    else    {
                        buf.append(" <");
                        buf.push_back(gh);
                        buf.append(val);
                        buf.append(">");
                    }
                } while (++i < count);
            }
            if (!(attrib = u->tag.attribute("morph")).empty() && (show)) {
                int count = u->tag.attributePartCount("morph", ' ');
                int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                do {
                    attrib = u->tag.attribute("morph", i, ' ');
                    if (i < 0) i = 0;    // to handle our -1 condition
                    val = strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    if ((*val == 'T') && (strchr("GH", val[1])) && (charIsDigit(val[2])))
                        val+=2;
                    buf.append(" (");
                    buf.append(val);
                    buf.push_back(')');
                } while (++i < count);
            }
            if (!(attrib = u->tag.attribute("POS")).empty()) {
                val = strchr(attrib.c_str(), ':');
                val = (val) ? (val + 1) : attrib.c_str();

                buf.append(" <");
                buf.append(val);
                buf.push_back('>');
            }
        }

        // <note> tag
        else if (!strncmp(token, "note", 4)) {
                if (!strstr(token, "strongsMarkup")) {    // leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
                    buf.append(" [");
                }
                else    u->suspendTextPassThru = true;
                if (u->module) {
                    XMLTag tag = token;
                    std::string swordFootnote = tag.attribute("swordFootnote");
                    std::string footnoteBody = u->module->getEntryAttributes()["Footnote"][swordFootnote]["body"];
                    buf.append(u->module->renderText(footnoteBody.c_str()));
                }
            }
        else if (!strncmp(token, "/note", 5)) {
            if (!u->suspendTextPassThru)
                buf.append("] ");
            else    u->suspendTextPassThru = false;
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
        else if ((u->tag.name() == "div") && ((u->tag.attribute("type") == "x-p") || (u->tag.attribute("type") == "paragraph")) &&
            (u->tag.isEmpty() && (!u->tag.attribute("sID").empty() || !u->tag.attribute("eID").empty()))) {
                userData->supressAdjacentWhitespace = true;
                buf.push_back('\n');
        }

                // <lb .../>
                else if (!strncmp(token, "lb", 2)) {
            userData->supressAdjacentWhitespace = true;
            buf.push_back('\n');
        }
        else if (!strncmp(token, "l", 1) && strstr(token, "eID")) {
            userData->supressAdjacentWhitespace = true;
            buf.push_back('\n');
        }
        else if (!strncmp(token, "/divineName", 11)) {
            // Get the end portion of the string, and upper case it
            assert(buf.size() >= u->lastTextNode.size());
            auto const endPos(buf.size() - u->lastTextNode.size());
            auto end(buf.substr(endPos));
            auto const endSize(end.size());
            toupperstr(end);
            buf.replace(endPos, endSize, end);
        }
        else if (!strncmp(token, "hi", 2)) {

                // handle both OSIS 'type' and TEI 'rend' attributes
                // there is no officially supported OSIS overline attribute,
                // thus either TEI overline or OSIS x-overline would be best,
                // but we have used "ol" in the past, as well.  Once a valid
                // OSIS overline attribute is made available, these should all
                // eventually be deprecated and never documented that they are supported.
                if (strstr(token, "rend=\"ol\"") || strstr(token, "rend=\"x-overline\"") || strstr(token, "rend=\"overline\"")
                   || strstr(token, "type=\"ol\"") || strstr(token, "type=\"x-overline\"") || strstr(token, "type=\"overline\"")) {
                    u->hiType = "overline";
                }
                else u->hiType = "";
                u->suspendTextPassThru = true;
            }
        else if (!strncmp(token, "/hi", 3)) {
            if (u->hiType == "overline") {
                const unsigned char *b = (const unsigned char *)u->lastTextNode.c_str();
                while (*b) {
                    const unsigned char *o = b;
                    if (getUniCharFromUTF8(&b)) {
                        while (o != b) buf.push_back(*(o++));
                        buf.push_back((unsigned char)0xCC);
                        buf.push_back((unsigned char)0x85);
                    }
                }
            }
            else {
                buf.append("* ");
                buf.append(u->lastSuspendSegment);
                buf.append(" *");
            }
            u->suspendTextPassThru = false;
        } else if (!strncmp(token, "q", 1)
                   && !u->tag.attribute("marker").empty())
        {
            buf.append(u->tag.attribute("marker"));
        }
                // <milestone type="line"/>
                else if (!strncmp(token, "milestone", 9)) {
            const char* type = strstr(token+10, "type=\"");
            if (type && strncmp(type+6, "line", 4)) { //we check for type != line
                userData->supressAdjacentWhitespace = true;
                    buf.push_back('\n');
            }
            if (!u->tag.attribute("marker").empty()) {
                buf.append(u->tag.attribute("marker"));
            }
                }

        else {
            return false;  // we still didn't handle token
        }
    }
    return true;
}

bool OSISPlain::processStage(char stage,
                             std::string &,
                             char *& from,
                             BasicFilterUserData *)
{
    // this is a strip filter so we want to do this as optimized as possible.
    // Avoid calling getUniCharFromUTF8 for slight speed improvement

    // skip soft hyphens:
    return (stage == PRECHAR) && (from[0] == '\xC2') && (from[1] == '\xAD');
}

} /* namespace swordxx */
