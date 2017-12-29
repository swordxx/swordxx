/***************************************************************************
 *
 *  gbfhtmlhref.cpp -    GBF to HTML filter with hrefs
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

#include "gbfhtmlhref.h"

#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../url.h"
#include "../utilxml.h"


namespace swordxx {

GBFHTMLHREF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    if (module) {
        version = module->getName();
    }
}

GBFHTMLHREF::GBFHTMLHREF() {
    setTokenStart("<");
    setTokenEnd(">");

    setTokenCaseSensitive(true);

    //addTokenSubstitute("Rf", ")</small></font>");
    addTokenSubstitute("FA", "<font color=\"#800000\">"); // for ASV footnotes to mark text
    addTokenSubstitute("Rx", "</a>");
    addTokenSubstitute("FI", "<i>"); // italics begin
    addTokenSubstitute("Fi", "</i>");
    addTokenSubstitute("FB", "<b>"); // bold begin
    addTokenSubstitute("Fb", "</b>");
    addTokenSubstitute("FR", "<font color=\"#FF0000\">"); // words of Jesus begin
    addTokenSubstitute("Fr", "</font>");
    addTokenSubstitute("FU", "<u>"); // underline begin
    addTokenSubstitute("Fu", "</u>");
    addTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
    addTokenSubstitute("Fo", "</cite>");
    addTokenSubstitute("FS", "<sup>"); // Superscript begin// Subscript begin
    addTokenSubstitute("Fs", "</sup>");
    addTokenSubstitute("FV", "<sub>"); // Subscript begin
    addTokenSubstitute("Fv", "</sub>");
    addTokenSubstitute("TT", "<big>"); // Book title begin
    addTokenSubstitute("Tt", "</big>");
    addTokenSubstitute("PP", "<cite>"); //  poetry  begin
    addTokenSubstitute("Pp", "</cite>");
    addTokenSubstitute("Fn", "</font>"); //  font  end
    addTokenSubstitute("CL", "<br />"); //  new line
    addTokenSubstitute("CM", "<!P><br />"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired
    addTokenSubstitute("CG", ""); //  ???
    addTokenSubstitute("CT", ""); // ???
    addTokenSubstitute("JR", "<div align=\"right\">"); // right align begin
    addTokenSubstitute("JC", "<div align=\"center\">"); // center align begin
    addTokenSubstitute("JL", "</div>"); // align end

    renderNoteNumbers = false;
}

std::unique_ptr<BasicFilterUserData> GBFHTMLHREF::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool GBFHTMLHREF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    const char *tok;
    MyUserData *u = (MyUserData *)userData;

    if (!substituteToken(buf, token)) {
        XMLTag tag(token);
        /*if (!std::strncmp(token, "w", 1)) {
            // OSIS Word (temporary until OSISRTF is done)
            valto = val;
            num = strstr(token, "lemma=\"x-Strongs:");
            if (num) {
                for (num+=17; ((*num) && (*num != '\"')); num++)
                    *valto++ = *num;
                *valto = 0;
                if (atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                    buf += " <small><em>&lt;<a href=\"type=Strongs value=";
                    for (tok = val; *tok; tok++)
                            buf += *tok;
                    buf += "\">";
                    for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++)
                            buf += *tok;
                    buf += "</a>&gt;</em></small> ";
                    //cout << buf;

                }
                //    forget these for now
                //else {
                    // verb morph
                    //sprintf(wordstr, "%03d", word-1);
                    //module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
                //}
            }
            else {
                num = strstr(token, "lemma=\"strong:");
                if (num) {
                    for (num+=14; ((*num) && (*num != '\"')); num++)
                        *valto++ = *num;
                    *valto = 0;
                    if (atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        buf += " <small><em>&lt;<a href=\"type=Strongs value=";
                        for (tok = val; *tok; tok++)
                                buf += *tok;
                        buf += "\">";
                        for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++)
                                buf += *tok;
                        buf += "</a>&gt;</em></small> ";
                        //cout << buf;

                    }
                    //    forget these for now
                    //else {
                        // verb morph
                        //sprintf(wordstr, "%03d", word-1);
                        //module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
                    //}
                }
            }
            valto = val;
            num = strstr(token, "morph=\"x-Robinson:");
            if (num) {
                for (num+=18; ((*num) && (*num != '\"')); num++)
                    *valto++ = *num;
                *valto = 0;
                buf += " <small><em>(<a href=\"type=morph class=Robinson value=";
                for (tok = val; *tok; tok++)
                // normal robinsons tense
                        buf += *tok;
                buf += "\">";
                for (tok = val; *tok; tok++)
                    //if(*tok != '\"')
                        buf += *tok;
                buf += "</a>)</em></small> ";
            }
        }*/

        // else
        if (!std::strncmp(token, "WG", 2)) { // strong's numbers
            //buf += " <small><em>&lt;<a href=\"type=Strongs value=";
            buf += " <small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Greek&value=";
            for (tok = token+2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += "\" class=\"strongs\">";
            for (tok = token + 2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += "</a>&gt;</em></small>";
        }
        else if (!std::strncmp(token, "WH", 2)) { // strong's numbers
            //buf += " <small><em>&lt;<a href=\"type=Strongs value=";
            buf += " <small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Hebrew&value=";
            for (tok = token+2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += "\" class=\"strongs\">";
            for (tok = token + 2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += "</a>&gt;</em></small>";
        }
        else if (!std::strncmp(token, "WTG", 3)) { // strong's numbers tense
            //buf += " <small><em>(<a href=\"type=Strongs value=";
            buf += " <small><em class=\"strongs\">(<a href=\"passagestudy.jsp?action=showStrongs&type=Greek&value=";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\" class=\"strongs\">";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "</a>)</em></small>";
        }
        else if (!std::strncmp(token, "WTH", 3)) { // strong's numbers tense
            //buf += " <small><em>(<a href=\"type=Strongs value=";
            buf += " <small><em class=\"strongs\">(<a href=\"passagestudy.jsp?action=showStrongs&type=Hebrew&value=";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\" class=\"strongs\">";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "</a>)</em></small>";
        }

        else if (!std::strncmp(token, "WT", 2) && std::strncmp(token, "WTH", 3) && std::strncmp(token, "WTG", 3)) { // morph tags
            //buf += " <small><em>(<a href=\"type=morph class=none value=";
            buf += " <small><em class=\"morph\">(<a href=\"passagestudy.jsp?action=showMorph&type=Greek&value=";

            for (tok = token + 2; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\" class=\"morph\">";
            for (tok = token + 2; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "</a>)</em></small>";
        }

        else if (tag.name() == "RX") {
            buf += "<a href=\"";
            for (tok = token + 3; *tok; tok++) {
              if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
                buf += *tok;
              }
              else {
                break;
              }
            }
            buf += "\">";
        }
        else if (tag.name() == "RF") {
            std::string type = tag.attribute("type");
            std::string footnoteNumber = tag.attribute("swordFootnote");
            std::string noteName = tag.attribute("n");
            if (auto const * const vkey = u->verseKey) {
                // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                //char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x' : 'n';
                buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=n&value=%s&module=%s&passage=%s\"><small><sup class=\"n\">*n%s</sup></small></a> ",
                    URL::encode(footnoteNumber).c_str(),
                    URL::encode(u->version).c_str(),
                    URL::encode(vkey->getText()).c_str(),
                    (renderNoteNumbers ? URL::encode(noteName).c_str(): ""));
            }
            u->suspendTextPassThru = true;
        }
        else if (tag.name() == "Rf") {
            u->suspendTextPassThru = false;
        }
/*
        else if (!std::strncmp(token, "RB", 2)) {
            buf += "<i> ";
            u->hasFootnotePreTag = true;
        }

        else if (!std::strncmp(token, "Rf", 2)) {
            buf += "&nbsp<a href=\"note=";
            buf += u->lastTextNode.c_str();
            buf += "\">";
            buf += "<small><sup>*n</sup></small></a>&nbsp";
            // let's let text resume to output again
            u->suspendTextPassThru = false;
        }

        else if (!std::strncmp(token, "RF", 2)) {
            if (u->hasFootnotePreTag) {
                u->hasFootnotePreTag = false;
                buf += "</i> ";
            }
            u->suspendTextPassThru = true;
        }
*/
        else if (!std::strncmp(token, "FN", 2)) {
            buf += "<font face=\"";
            for (tok = token + 2; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\">";
        }

        else if (!std::strncmp(token, "CA", 2)) {    // ASCII value
            buf += (char)atoi(&token[2]);
        }

        else {
            return false;
        }
    }
    return true;
}

} /* namespace swordxx */
