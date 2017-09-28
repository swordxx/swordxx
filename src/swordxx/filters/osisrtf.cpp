/***************************************************************************
 *
 *  osisrtf.cpp -    OSIS to RTF filter
 *
 * $Id$ *
 *
 * Copyright 2003-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osisrtf.h"

#include <cctype>
#include <cstdlib>
#include <stack>
#include <string>
#include "../keys/versekey.h"
#include "../stringmgr.h"
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {
    class MyUserData : public BasicFilterUserData {
    public:
        bool osisQToTick;
        bool BiblicalText;
        bool inXRefNote;
        int suspendLevel;
        std::stack<std::string> quoteStack;
        std::string w;
        std::string version;
        MyUserData(const SWModule *module, const SWKey *key);
    };


    MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
        inXRefNote    = false;
        BiblicalText  = false;
        suspendLevel  = 0;
        if (module) {
            version = module->getName();
            BiblicalText = (module->getType() == "Biblical Texts");
        }
        osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));
    }

    static inline void outText(const char *t, std::string &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; else u->lastSuspendSegment += t; }
    static inline void outText(char t, std::string &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; else u->lastSuspendSegment += t; }

}


OSISRTF::OSISRTF() {
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
    //    addTokenSubstitute("lg", "{\\par}");
    //    addTokenSubstitute("/lg", "{\\par}");

    setTokenCaseSensitive(true);
}

std::unique_ptr<BasicFilterUserData> OSISRTF::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

char OSISRTF::processText(std::string &text, const SWKey *key, const SWModule *module) {

    // preprocess text buffer to escape RTF control codes
    const char *from;
    std::string orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++) {  //loop to remove extra spaces
        switch (*from) {
        case '{':
        case '}':
        case '\\':
            text += "\\";
            text += *from;
            break;
        default:
            text += *from;
        }
    }

    SWBasicFilter::processText(text, key, module);  //handle tokens as usual

    orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++) {  //loop to remove extra spaces
        if ((strchr(" \t\n\r", *from))) {
            while (*(from+1) && (strchr(" \t\n\r", *(from+1)))) {
                from++;
            }
            text += " ";
        }
        else {
            text += *from;
        }
    }
    return 0;
}


bool OSISRTF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
    MyUserData *u = (MyUserData *)userData;
    std::string scratch;
    bool sub = (u->suspendTextPassThru) ? substituteToken(scratch, token) : substituteToken(buf, token);
    if (!sub) {
        XMLTag tag(token);

        // <w> tag
        if (tag.name() == "w") {

            // start <w> tag
            if ((!tag.isEmpty()) && (!tag.isEndTag())) {
                outText('{', buf, u);
                u->w = token;
            }

            // end or empty <w> tag
            else {
                bool endTag = tag.isEndTag();
                std::string lastText;
                bool show = true;    // to handle unplaced article in kjv2003-- temporary till combined

                if (endTag) {
                    tag = u->w.c_str();
                    lastText = u->lastTextNode.c_str();
                }
                else lastText = "stuff";

                std::string attrib;
                const char *val;
                if (!(attrib = tag.attribute("xlit")).empty()) {
                    val = strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    scratch = formatted(" {\\fs15 <%s>}", val);
                    outText(scratch.c_str(), buf, u);
                }
                if (!(attrib = tag.attribute("gloss")).empty()) {
                    val = strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    scratch = formatted(" {\\fs15 <%s>}", val);
                    outText(scratch.c_str(), buf, u);
                }
                if (!(attrib = tag.attribute("lemma")).empty()) {
                    int count = tag.attributePartCount("lemma", ' ');
                    int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                    do {
                        attrib = tag.attribute("lemma", i, ' ');
                        if (i < 0) i = 0;    // to handle our -1 condition
                        val = strchr(attrib.c_str(), ':');
                        val = (val) ? (val + 1) : attrib.c_str();
                        const char *val2 = val;
                        if ((strchr("GH", *val)) && (isdigit(val[1])))
                            val2++;
                        if ((!strcmp(val2, "3588")) && (lastText.length() < 1))
                            show = false;
                        else    {
                            scratch = formatted(" {\\cf3 \\sub <%s>}", val2);
                            outText(scratch.c_str(), buf, u);
                        }
                    } while (++i < count);
                }
                if (!(attrib = tag.attribute("morph")).empty() && (show)) {
                    std::string savelemma = tag.attribute("savlm");
                    if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
                        show = false;
                    if (show) {
                        int count = tag.attributePartCount("morph", ' ');
                        int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                        do {
                            attrib = tag.attribute("morph", i, ' ');
                            if (i < 0) i = 0;    // to handle our -1 condition
                            val = strchr(attrib.c_str(), ':');
                            val = (val) ? (val + 1) : attrib.c_str();
                            const char *val2 = val;
                            if ((*val == 'T') && (strchr("GH", val[1])) && (isdigit(val[2])))
                                val2+=2;
                            scratch = formatted(" {\\cf4 \\sub (%s)}", val2);
                            outText(scratch.c_str(), buf, u);
                        } while (++i < count);
                    }
                }
                if (!(attrib = tag.attribute("POS")).empty()) {
                    val = strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    scratch = formatted(" {\\fs15 <%s>}", val);
                    outText(scratch.c_str(), buf, u);
                }

                if (endTag)
                    outText('}', buf, u);
            }
        }

        // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    std::string type = tag.attribute("type");

                    if ((type != "x-strongsMarkup")            // leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
                            && (type != "strongsMarkup")    // deprecated
                            ) {
                        std::string footnoteNumber = tag.attribute("swordFootnote");
                        if (VerseKey const * const vkey =
                                dynamic_cast<VerseKey const *>(u->key))
                        {
                            char const ch = ((type == "crossReference") || (type == "x-cross-ref")) ? 'x':'n';
                            scratch = formatted("{\\super <a href=\"\">*%c%i.%s</a>} ", ch, vkey->getVerse(), footnoteNumber.c_str());
                            outText(scratch.c_str(), buf, u);
                            u->inXRefNote = (ch == 'x');
                        }
                    }
                    u->suspendTextPassThru = (++u->suspendLevel);
                }
            }
            if (tag.isEndTag()) {
                u->suspendTextPassThru = (--u->suspendLevel);
                u->inXRefNote = false;
            }
        }

        // <p> paragraph and <lg> linegroup tags
        else if ((tag.name() == "p") || (tag.name() == "lg")) {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {    // non-empty start tag
                outText("{\\fi200\\par}", buf, u);
            }
            else if (tag.isEndTag()) {    // end tag
                outText("{\\par}", buf, u);
                userData->supressAdjacentWhitespace = true;
            }
            else {                    // empty paragraph break marker
                outText("{\\pard\\par}", buf, u);
                userData->supressAdjacentWhitespace = true;
            }
        }

        // Milestoned paragraphs, created by osis2mod
        // <div type="paragraph" sID.../>
        // <div type="paragraph" eID.../>
        else if (tag.isEmpty() && (tag.name() == "div") && ((tag.attribute("type") == "x-p") || (tag.attribute("type") == "paragraph"))) {
            // <div type="paragraph"  sID... />
            if (!tag.attribute("sID").empty()) {    // non-empty start tag
                outText("{\\fi200\\par}", buf, u);
            }
            // <div type="paragraph"  eID... />
            else if (!tag.attribute("eID").empty()) {
                outText("{\\par}", buf, u);
                userData->supressAdjacentWhitespace = true;
            }
        }

        // <reference> tag
        else if (tag.name() == "reference") {
            if (!u->inXRefNote) {    // only show these if we're not in an xref note
                if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                    outText("{<a href=\"\">", buf, u);
                }
                else if (tag.isEndTag()) {
                    outText("</a>}", buf, u);
                }
            }
        }

        // <l> poetry
        else if (tag.name() == "l") {
            // end line marker
            if (!tag.attribute("eID").empty()) {
                outText("{\\par}", buf, u);
            }
            // <l/> without eID or sID
            // Note: this is improper osis. This should be <lb/>
            else if (tag.isEmpty() && tag.attribute("sID").empty()) {
                outText("{\\par}", buf, u);
            }
            // end of the line
            else if (tag.isEndTag()) {
                outText("{\\par}", buf, u);
            }
        }

        // <milestone type="line"/> or <lb.../>
        else if (((tag.name() == "lb") && (tag.attribute("type").empty() || (tag.attribute("type") != "x-optional"))) || ((tag.name() == "milestone") && (tag.attribute("type") == "line"))) {
            outText("{\\par}", buf, u);
            userData->supressAdjacentWhitespace = true;
        }

        // <title>
        else if (tag.name() == "title") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("{\\par\\i1\\b1 ", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("\\par}", buf, u);
            }
        }
        // <list>     - how do we support these better in RTF?
        else if (tag.name() == "list") {
            if((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("\\par\\pard", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("\\par\\pard", buf, u);
            }
        }

        // <item> - support better
        else if (tag.name() == "item") {
            if((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("* ", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("\\par", buf, u);
            }
        }

        // <catchWord> & <rdg> tags (italicize)
        else if ((tag.name() == "rdg") || (tag.name() == "catchWord")) {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("{\\i1 ", buf, u);
            }
            else if (tag.isEndTag()) {
                outText('}', buf, u);
            }
        }

        // <hi>
        else if (tag.name() == "hi") {
            std::string type = tag.attribute("type");
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                if (type == "bold" || type == "b" || type == "x-b")
                    outText("{\\b1 ", buf, u);
                else    // all other types
                    outText("{\\i1 ", buf, u);
            }
            else if (tag.isEndTag()) {
                outText('}', buf, u);
            }
        }

        // <q> quote
        // Rules for a quote element:
        // If the tag is empty with an sID or an eID then use whatever it specifies for quoting.
        //    Note: empty elements without sID or eID are ignored.
        // If the tag is <q> then use it's specifications and push it onto a stack for </q>
        // If the tag is </q> then use the pushed <q> for specification
        // If there is a marker attribute, possibly empty, this overrides osisQToTick.
        // If osisQToTick, then output the marker, using level to determine the type of mark.
        else if (tag.name() == "q") {
            std::string type      = tag.attribute("type");
            std::string who       = tag.attribute("who");
            auto tmp(tag.attribute("level"));
            int level       = (!tmp.empty()) ? atoi(tmp.c_str()) : 1;
            std::string mark(tag.attribute("marker"));
            bool hasMark = !mark.empty();

            // open <q> or <q sID... />
            if ((!tag.isEmpty() && !tag.isEndTag()) || (tag.isEmpty() && !tag.attribute("sID").empty())) {
                // if <q> then remember it for the </q>
                if (!tag.isEmpty())
                    u->quoteStack.push(tag.toString());

                // Do this first so quote marks are included as WoC
                if (who == "Jesus")
                    outText("\\cf6 ", buf, u);

                // first check to see if we've been given an explicit mark
                if (hasMark)
                    outText(mark.c_str(), buf, u);
                //alternate " and '
                else if (u->osisQToTick)
                    outText((level % 2) ? '\"' : '\'', buf, u);
            }
            // close </q> or <q eID... />
            else if ((tag.isEndTag()) || (!tag.attribute("eID").empty())) {
                // if it is </q> then pop the stack for the attributes
                if (tag.isEndTag() && !u->quoteStack.empty()) {
                    XMLTag qTag(u->quoteStack.top().c_str());
                    u->quoteStack.pop();

                    type    = qTag.attribute("type");
                    who     = qTag.attribute("who");
                    tmp     = qTag.attribute("level");
                    level   = (!tmp.empty()) ? atoi(tmp.c_str()) : 1;
                    tmp     = qTag.attribute("marker");
                    hasMark = !tmp.empty();
                    mark    = tmp;
                }

                // first check to see if we've been given an explicit mark
                if (hasMark)
                    outText(mark.c_str(), buf, u);
                // finally, alternate " and ', if config says we should supply a mark
                else if (u->osisQToTick)
                    outText((level % 2) ? '\"' : '\'', buf, u);

                // Do this last so quote marks are included as WoC
                if (who == "Jesus")
                    outText("\\cf0 ", buf, u);
            }
        }


        // <milestone type="cQuote" marker="x"/>
        else if ((tag.name() == "milestone") && (tag.attribute("type") == "cQuote")) {
            auto mark(tag.attribute("marker"));
            bool hasMark = !mark.empty();
            auto tmp(tag.attribute("level"));
            int level = (!tmp.empty()) ? atoi(tmp.c_str()) : 1;

            // first check to see if we've been given an explicit mark
            if (hasMark)
                outText(mark.c_str(), buf, u);
            // finally, alternate " and ', if config says we should supply a mark
            else if (u->osisQToTick)
                outText((level % 2) ? '\"' : '\'', buf, u);
        }

        // <transChange>
        else if (tag.name() == "transChange") {
            std::string type = tag.attribute("type");

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {

// just do all transChange tags this way for now
//                if (type == "supplied")
                    outText("{\\i1 ", buf, u);
            }
            else if (tag.isEndTag()) {
                outText('}', buf, u);
            }
        }

        // <divineName>
        else if (tag.name() == "divineName") {

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                 outText("{\\scaps ", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("}", buf, u);
            }
        }

        // <div>
        else if (tag.name() == "div") {

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("\\pard ", buf, u);
            }
            else if (tag.isEndTag()) {
                            outText("\\par ", buf, u);
                        }
        }

        // image
        else if (tag.name() == "figure") {
            auto const src(tag.attribute("src"));
            if (src.empty())        // assert we have a src attribute
                return false;

            char const * const absoluteDataPath =
                    userData->module->getConfigEntry("AbsoluteDataPath");
            auto const filepath(
                        std::string(absoluteDataPath ? absoluteDataPath : "")
                        + src);

// we do this because BibleCS looks for this EXACT format for an image tag
            outText("<img src=\"", buf, u);
            outText(filepath.c_str(), buf, u);
            outText("\" />", buf, u);
/*
            char imgc;
            for (c = filepath + strlen(filepath); c > filepath && *c != '.'; c--);
            c++;
            FILE* imgfile;
                    if (!caseInsensitiveEquals(c, "jpg") || !caseInsensitiveEquals(c, "jpeg")) {
                          imgfile = fopen(filepath, "r");
                          if (imgfile != nullptr) {
                                outText("{\\nonshppict {\\pict\\jpegblip ", buf, u);
                                while (feof(imgfile) != EOF) {
                                       scratch.setFormatted("%2x", fgetc(imgfile));
                                          outText(scratch.c_str(), buf, u);

                                }
                                fclose(imgfile);
                                outText("}}", buf, u);
                          }
                    }
                    else if (!caseInsensitiveEquals(c, "png")) {
                          outText("{\\*\\shppict {\\pict\\pngblip ", buf, u);

                          outText("}}", buf, u);
                    }
*/
        }
        else {
            return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
