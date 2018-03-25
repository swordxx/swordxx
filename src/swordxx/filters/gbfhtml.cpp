/***************************************************************************
 *
 *  gbfhtml.cpp -    GBF to HTML filter
 *
 * $Id$
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "gbfhtml.h"

#include <cstdlib>
#include <cstring>
#include "../utilstr.h"


namespace swordxx {

GBFHTML::GBFHTML() {
    setTokenStart("<");
    setTokenEnd(">");

    setTokenCaseSensitive(true);

    addTokenSubstitute("Rf", ")</small></font>");
    addTokenSubstitute("Rx", "</a>");
    addTokenSubstitute("FI", "<i>"); // italics begin
    addTokenSubstitute("Fi", "</i>");
    addTokenSubstitute("FB", "<n>"); // bold begin
    addTokenSubstitute("Fb", "</n>");
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
    addTokenSubstitute("CM", "<br /><br />"); //  two new-lines
    addTokenSubstitute("CG", ""); //  ???
    addTokenSubstitute("CT", ""); // ???
    addTokenSubstitute("JR", "<div align=\"right\">"); // right align begin
    addTokenSubstitute("JC", "<div align=\"center\">"); // center align begin
    addTokenSubstitute("JL", "</div>"); // align end

}

std::unique_ptr<BasicFilterUserData> GBFHTML::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool GBFHTML::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    const char *tok;
    char val[128];
    char *valto;
    const char *num;
    MyUserData * u = static_cast<MyUserData *>(userData);

    if (!substituteToken(buf, token)) {
        // deal with OSIS note tags.  Just hide till OSISRTF
        if (!std::strncmp(token, "note ", 5)) {
            // let's stop text from going to output
            u->suspendTextPassThru = true;
        }

        else if (!std::strncmp(token, "/note", 5)) {
            u->suspendTextPassThru = false;
        }

        else if (!std::strncmp(token, "w", 1)) {
            // OSIS Word (temporary until OSISRTF is done)
            valto = val;
            num = std::strstr(token, "lemma=\"x-Strongs:");
            if (num) {
                for (num+=17; ((*num) && (*num != '\"')); num++)
                    *valto++ = *num;
                *valto = 0;
                if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                    buf += " <small><em>&lt;";
                    for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++)
                            buf += *tok;
                    buf += "&gt;</em></small> ";
                }
            } else {
                num = std::strstr(token, "lemma=\"strong:");
                if (num) {
                    for (num+=14; ((*num) && (*num != '\"')); num++)
                        *valto++ = *num;
                    *valto = 0;
                    if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        buf += " <small><em>&lt;";
                        for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++)
                                buf += *tok;
                        buf += "&gt;</em></small> ";
                    }
                }
            }
            valto = val;
            num = std::strstr(token, "morph=\"x-Robinson:");
            if (num) {
                for (num+=18; ((*num) && (*num != '\"')); num++)
                    *valto++ = *num;
                *valto = 0;
                // normal robinsons tense
                buf += " <small><em>(";
                for (tok = val; *tok; tok++)
                        buf += *tok;
                buf += ")</em></small> ";
            }
        }

        else if (!std::strncmp(token, "WG", 2) || !std::strncmp(token, "WH", 2)) { // strong's numbers
            buf += " <small><em>&lt;";
            for (tok = token + 2; *tok; tok++)
                    buf += *tok;
            buf += "&gt;</em></small> ";
        }

        else if (!std::strncmp(token, "WTG", 3) || !std::strncmp(token, "WTH", 3)) { // strong's numbers tense
            buf += " <small><em>&lt;";
            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += ")</em></small> ";
        }

        else if (!std::strncmp(token, "RX", 2)) {
            buf += "<i>";
            for (tok = token + 3; *tok; tok++) {
              if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
                buf += *tok;
              }
              else {
                break;
              }
            }
            buf += "</i>";
        }

        else if (!std::strncmp(token, "RB", 2)) {
            buf += "<i>";
            u->hasFootnotePreTag = true;
        }

        else if (!std::strncmp(token, "RF", 2)) {
            if (u->hasFootnotePreTag) {
                u->hasFootnotePreTag = false;
                buf += "</i> ";
            }
            buf += "<font color=\"#800000\"><small> (";
        }

        else if (!std::strncmp(token, "FN", 2)) {
            buf += "<font face=\"";
            for (tok = token + 2; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "\">";
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
