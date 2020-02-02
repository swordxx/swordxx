/***************************************************************************
 *
 *  gbfhtmlhref.cpp -    GBF to HTML filter with hrefs
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
#include "../utilstr.h"


namespace swordxx {

GBFHTMLHREF::MyUserData::MyUserData(SWModule const * module_,
                                    SWKey const * key_)
    : BasicFilterUserData(module_, key_)
{
    if (module_) {
        version = module_->getName();
    }
}

GBFHTMLHREF::MyUserData::~MyUserData() noexcept = default;

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
    MyUserData * u = static_cast<MyUserData *>(userData);

    using namespace std::literals::string_view_literals;

    if (!substituteToken(buf, token)) {
        XMLTag tag(token);
        if (startsWith(token, "WG"sv)) { // strong's numbers
            buf += " <small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Greek&value=";
            buf += token + 2;
            buf += "\" class=\"strongs\">";
            buf += token + 2;
            buf += "</a>&gt;</em></small>";
        }
        else if (startsWith(token, "WH"sv)) { // strong's numbers
            buf += " <small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Hebrew&value=";
            buf += token + 2;
            buf += "\" class=\"strongs\">";
            buf += token + 2;
            buf += "</a>&gt;</em></small>";
        }
        else if (startsWith(token, "WTG"sv)) { // strong's numbers tense
            buf += " <small><em class=\"strongs\">(<a href=\"passagestudy.jsp?action=showStrongs&type=Greek&value=";
            for (auto tok = token + 3; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\" class=\"strongs\">";
            for (auto tok = token + 3; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += "</a>)</em></small>";
        }
        else if (startsWith(token, "WTH"sv)) { // strong's numbers tense
            buf += " <small><em class=\"strongs\">(<a href=\"passagestudy.jsp?action=showStrongs&type=Hebrew&value=";
            for (auto tok = token + 3; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\" class=\"strongs\">";
            for (auto tok = token + 3; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += "</a>)</em></small>";
        }

        else if (startsWith(token, "WT"sv)) { // morph tags
            buf += " <small><em class=\"morph\">(<a href=\"passagestudy.jsp?action=showMorph&type=Greek&value=";

            for (auto tok = token + 2; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\" class=\"morph\">";
            for (auto tok = token + 2; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += "</a>)</em></small>";
        }

        else if (tag.name() == "RX") {
            buf += "<a href=\"";
            for (auto tok = token + 3; *tok; ++tok) {
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
                buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=n&value=%s&module=%s&passage=%s\"><small><sup class=\"n\">*n%s</sup></small></a> ",
                    URL::encode(footnoteNumber),
                    URL::encode(u->version),
                    URL::encode(vkey->getText()),
                    (renderNoteNumbers ? URL::encode(noteName).c_str(): ""));
            }
            u->suspendTextPassThru = true;
        }
        else if (tag.name() == "Rf") {
            u->suspendTextPassThru = false;
        }
        else if (startsWith(token, "FN"sv)) {
            buf += "<font face=\"";
            for (auto tok = token + 2; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\">";
        }

        else if (startsWith(token, "CA"sv)) {    // ASCII value
            buf += (char)std::atoi(&token[2]);
        }

        else {
            return false;
        }
    }
    return true;
}

} /* namespace swordxx */
