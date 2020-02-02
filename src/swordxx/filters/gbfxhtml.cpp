/******************************************************************************
 *
 *  gbfxhtml.cpp -    GBF to classed XHTML
 *
 * Copyright 2011-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "gbfxhtml.h"

#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../url.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

const char *GBFXHTML::getHeader() const {
    return "\
        .wordsOfJesus {\
            color: red;\
        }\
    ";
}

GBFXHTML::MyUserData::MyUserData(SWModule const * module_,
                                 SWKey const * key_)
    : BasicFilterUserData(module_, key_)
{
    if (module_) {
        version = module_->getName();
    }
}

GBFXHTML::MyUserData::~MyUserData() noexcept = default;

GBFXHTML::GBFXHTML() {
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
    addTokenSubstitute("FR", "<span class=\"wordsOfJesus\">"); // words of Jesus begin
    addTokenSubstitute("Fr", "</span>");
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

std::unique_ptr<BasicFilterUserData> GBFXHTML::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool GBFXHTML::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    using namespace std::literals::string_view_literals;

    const char *tok;
    MyUserData * u = static_cast<MyUserData *>(userData);

    if (!substituteToken(buf, token)) {
        XMLTag tag(token);

        if (startsWith(token, "WG"sv)) { // strong's numbers
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
        else if (startsWith(token, "WH"sv)) { // strong's numbers
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
        else if (startsWith(token, "WTG"sv)) { // strong's numbers tense
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
        else if (startsWith(token, "WTH"sv)) { // strong's numbers tense
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

        else if (startsWith(token, "WT"sv)) { // morph tags
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
                    URL::encode(footnoteNumber),
                    URL::encode(u->version),
                    URL::encode(vkey->getText()),
                    (renderNoteNumbers ? URL::encode(noteName).c_str() : ""));
            }
            u->suspendTextPassThru = true;
        }
        else if (tag.name() == "Rf") {
            u->suspendTextPassThru = false;
        }
        else if (startsWith(token, "FN"sv)) {
            buf += "<font face=\"";
            for (tok = token + 2; *tok; tok++)
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
