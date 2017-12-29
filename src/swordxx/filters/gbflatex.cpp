/******************************************************************************
 *
 *  gbflatex.cpp -    GBF to LaTeX
 *
 * $Id$
 *
 * Copyright 2011-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include "gbflatex.h"

#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../url.h"
#include "../utilxml.h"


namespace swordxx {

const char *GBFLaTeX::getHeader() const {
    return "\\usepackage{color}";
}

GBFLaTeX::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    if (module) {
        version = module->getName();
    }
}

GBFLaTeX::GBFLaTeX() {
    setTokenStart("<");
    setTokenEnd(">");

    setTokenCaseSensitive(true);

    //addTokenSubstitute("Rf", ")</small></font>");
    addTokenSubstitute("FA", "{\\color{maroon}"); // for ASV footnotes to mark text
    addTokenSubstitute("Rx", "}");
    addTokenSubstitute("FI", "\\emph{"); // italics begin
    addTokenSubstitute("Fi", "}");
    addTokenSubstitute("FB", "\\bold{"); // bold begin
    addTokenSubstitute("Fb", "}");
    addTokenSubstitute("FR", "{\\swordwoj{"); // words of Jesus begin
    addTokenSubstitute("Fr", "}");
    addTokenSubstitute("FU", "\\underline{"); // underline begin
    addTokenSubstitute("Fu", "}");
    addTokenSubstitute("FO", "\\begin{quote}"); //  Old Testament quote begin
    addTokenSubstitute("Fo", "\\end{quote}");
    addTokenSubstitute("FS", "\\textsuperscript{"); // Superscript begin// Subscript begin
    addTokenSubstitute("Fs", "}");
    addTokenSubstitute("FV", "\\textsubscript{"); // Subscript begin
    addTokenSubstitute("Fv", "}");
    addTokenSubstitute("TT", "\\section*{"); // Book title begin
    addTokenSubstitute("Tt", "}");
    addTokenSubstitute("PP", "\\begin{swordpoetry}"); //  poetry  begin
    addTokenSubstitute("Pp", "\\end{swordpoetry}");
    addTokenSubstitute("Fn", ""); //  font  end
    addTokenSubstitute("CL", "\\\\"); //  new line
    addTokenSubstitute("CM", "\\\\"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired
    addTokenSubstitute("CG", ""); //  ???
    addTokenSubstitute("CT", ""); // ???
    addTokenSubstitute("JR", "{\\raggedright{}"); // right align begin
    addTokenSubstitute("JC", "{\\raggedcenter{}"); // center align begin
    addTokenSubstitute("JL", "}"); // align end

    renderNoteNumbers = false;
}

std::unique_ptr<BasicFilterUserData> GBFLaTeX::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool GBFLaTeX::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    const char *tok;
    MyUserData *u = (MyUserData *)userData;

    if (!substituteToken(buf, token)) {
        XMLTag tag(token);

        if (!std::strncmp(token, "WG", 2)) { // strong's numbers
            //buf += " <small><em>&lt;<a href=\"type=Strongs value=";
            buf += " \\swordstrong[Greek]{";
            for (tok = token+2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += ", ";
            for (tok = token + 2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += "}";
        }
        else if (!std::strncmp(token, "WH", 2)) { // strong's numbers
            buf += " \\swordstrong[Hebrew]{";
            for (tok = token+2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += ", ";
            for (tok = token + 2; *tok; tok++)
                //if(token[i] != '\"')
                    buf += *tok;
            buf += "}";
        }
        else if (!std::strncmp(token, "WTG", 3)) { // strong's numbers tense
            buf += " \\swordstrong[Greektense]{";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += ", ";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "}";
        }
        else if (!std::strncmp(token, "WTH", 3)) { // strong's numbers tense
            buf += " \\swordstrong[Hebrewtense]{";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += ",";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "}";
        }

        else if (!std::strncmp(token, "WT", 2) && std::strncmp(token, "WTH", 3) && std::strncmp(token, "WTG", 3)) { // morph tags
            buf += " \\swordmorph{";

            for (tok = token + 2; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += ", >";
            for (tok = token + 2; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "}";
        }

        else if (tag.name() == "RX") {
            buf += "\\swordxref{";
            for (tok = token + 3; *tok; tok++) {
              if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
                buf += *tok;
              }
              else {
                break;
              }
            }
            buf += "}";
        }
        else if (tag.name() == "RF") {
            std::string type = tag.attribute("type");
            std::string footnoteNumber = tag.attribute("swordFootnote");
            std::string noteName = tag.attribute("n");
            if (auto const * const vkey = u->verseKey) {
                buf += formatted("\\swordfootnote{%s}{%s}{%s}{",
                    footnoteNumber.c_str(),
                    u->version.c_str(),
                    vkey->getText()).c_str();
            }
            u->suspendTextPassThru = false;
        }
        else if (tag.name() == "Rf") {
            u->suspendTextPassThru = false;
            buf += "}";
        }
        else if (!std::strncmp(token, "FN", 2)) {
            buf += "\\swordfont{";
            for (tok = token + 2; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "}";
        }

        else if (!std::strncmp(token, "CA", 2)) {    // ASCII value
            buf += (char)std::atoi(&token[2]);
        }

        else {
            return false;
        }
    }
    return true;
}

} /* namespace swordxx */
