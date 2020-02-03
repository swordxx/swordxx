/***************************************************************************
 *
 *  gbfhtml.cpp -    GBF to HTML filter
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

GBFHTML::MyUserData::~MyUserData() noexcept = default;

GBFHTML::GBFHTML() {
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
    MyUserData * u = static_cast<MyUserData *>(userData);

    using namespace std::literals::string_view_literals;

    if (!substituteToken(buf, token)) {
        // deal with OSIS note tags.  Just hide till OSISRTF
        if (startsWith(token, "note "sv)) {
            // let's stop text from going to output
            u->suspendTextPassThru = true;
        }

        else if (startsWith(token, "/note"sv)) {
            u->suspendTextPassThru = false;
        }

        else if (startsWith(token, "w"sv)) {
            // OSIS Word (temporary until OSISRTF is done)
            auto num = std::strstr(token, "lemma=\"x-Strongs:");
            if (num) {
                std::string numValue;
                for (num+=17; ((*num) && (*num != '\"')); num++)
                    numValue += *num;
                auto const * const val = numValue.c_str();
                if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                    buf += " <small><em>&lt;";
                    for (auto tok = (!charIsDigit(*val))?val+1:val; *tok; ++tok)
                        buf += *tok;
                    buf += "&gt;</em></small> ";
                }
            } else {
                num = std::strstr(token, "lemma=\"strong:");
                if (num) {
                    std::string numValue;
                    for (num+=14; ((*num) && (*num != '\"')); num++)
                        numValue += *num;
                    auto const * const val = numValue.c_str();
                    if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        buf += " <small><em>&lt;";
                        for (auto tok = (!charIsDigit(*val))?val+1:val; *tok; ++tok)
                            buf += *tok;
                        buf += "&gt;</em></small> ";
                    }
                }
            }
            num = std::strstr(token, "morph=\"x-Robinson:");
            if (num) {
                std::string numValue;
                for (num+=18; ((*num) && (*num != '\"')); num++)
                    numValue += *num;
                // normal robinsons tense
                buf += " <small><em>(";
                for (auto tok = numValue.c_str(); *tok; ++tok)
                    buf += *tok;
                buf += ")</em></small> ";
            }
        }

        else if (startsWith(token, "WG"sv) || startsWith(token, "WH"sv)) { // strong's numbers
            buf += " <small><em>&lt;";
            for (auto tok = token + 2; *tok; ++tok)
                buf += *tok;
            buf += "&gt;</em></small> ";
        }

        else if (startsWith(token, "WTG"sv) || startsWith(token, "WTH"sv)) { // strong's numbers tense
            buf += " <small><em>&lt;";
            for (auto tok = token + 3; *tok; ++tok)
                if(*tok != '\"')
                    buf += *tok;
            buf += ")</em></small> ";
        }

        else if (startsWith(token, "RX"sv)) {
            buf += "<i>";
            for (auto tok = token + 3; *tok; ++tok) {
              if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
                buf += *tok;
              }
              else {
                break;
              }
            }
            buf += "</i>";
        }

        else if (startsWith(token, "RB"sv)) {
            buf += "<i>";
            u->hasFootnotePreTag = true;
        }

        else if (startsWith(token, "RF"sv)) {
            if (u->hasFootnotePreTag) {
                u->hasFootnotePreTag = false;
                buf += "</i> ";
            }
            buf += "<font color=\"#800000\"><small> (";
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
