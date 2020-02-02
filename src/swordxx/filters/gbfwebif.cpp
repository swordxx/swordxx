/***************************************************************************
 *
 *  gbfwebif.cpp -    GBF to HTML filter with hrefs for strongs and morph
 *            tags
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

#include "gbfwebif.h"

#include <cstdlib>
#include <cstring>
#include "../url.h"
#include "../utilstr.h"


namespace swordxx {

GBFWEBIF::GBFWEBIF() : baseURL(""), passageStudyURL(baseURL + "passagestudy.jsp") {
}

bool GBFWEBIF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    using namespace std::literals::string_view_literals;

    const char *tok;
    char val[128];
    char *valto;
    const char *num;
    std::string url;

    if (!substituteToken(buf, token)) {
        if (startsWith(token, "w"sv)) {
            // OSIS Word (temporary until OSISRTF is done)
            valto = val;
            num = std::strstr(token, "lemma=\"x-Strongs:");
            if (num) {
                for (num+=17; ((*num) && (*num != '\"')); num++)
                    *valto++ = *num;
                *valto = 0;

                if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                    buf += " <small><em>&lt;";
                    url = "";
                    for (tok = val; *tok; tok++) {
                        url += *tok;
                    }
                    if ((url.length() > 1) && std::strchr("GH", url[0])) {
                        if (charIsDigit(url[1]))
                            url = url.c_str()+1;
                    }
                    buf += formatted("<a href=\"%s?showStrong=%s#cv\">", passageStudyURL, URL::encode(url));

                    for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++) {
                        buf += *tok;
                    }
                    buf += "</a>&gt;</em></small> ";
                }
            }
            else {
                num = std::strstr(token, "lemma=\"strong:");
                if (num) {
                    for (num+=14; ((*num) && (*num != '\"')); num++)
                        *valto++ = *num;
                    *valto = 0;

                    if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        buf += " <small><em>&lt;";
                        url = "";
                        for (tok = val; *tok; tok++) {
                            url += *tok;
                        }
                        if ((url.length() > 1) && std::strchr("GH", url[0])) {
                            if (charIsDigit(url[1]))
                                url = url.c_str()+1;
                        }
                        buf += formatted("<a href=\"%s?showStrong=%s#cv\">", passageStudyURL, URL::encode(url));

                        for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++) {
                            buf += *tok;
                        }
                        buf += "</a>&gt;</em></small> ";
                    }
                }
            }
            valto = val;
            num = std::strstr(token, "morph=\"x-Robinson:");
            if (num) {
                for (num+=18; ((*num) && (*num != '\"')); num++)
                    *valto++ = *num;
                *valto = 0;
                buf += " <small><em>(";
                url = "";
                for (tok = val; *tok; tok++) {
                // normal robinsons tense
                    buf += *tok;
                }
                buf += formatted("<a href=\"%s?showMorph=%s#cv\">", passageStudyURL, URL::encode(url));

                for (tok = val; *tok; tok++) {
                    buf += *tok;
                }
                buf += "</a>)</em></small> ";
            }
        }

        else if (startsWith(token, "WG"sv) || startsWith(token, "WH"sv)) { // strong's numbers
            buf += " <small><em>&lt;";
            url = "";

            for (tok = token+1; *tok; tok++) {
                url += *tok;
            }
            if ((url.length() > 1) && std::strchr("GH", url[0])) {
                if (charIsDigit(url[1]))
                    url = url.c_str()+1;
            }
            buf += formatted("<a href=\"%s?showStrong=%s#cv\">", passageStudyURL, URL::encode(url));

            for (tok = token + 2; *tok; tok++) {
                buf += *tok;
            }
            buf += "</a>&gt;</em></small>";
        }

        else if (startsWith(token, "WTG"sv) || startsWith(token, "WTH"sv)) { // strong's numbers tense
            buf += " <small><em>(";
            url = "";
            for (tok = token + 2; *tok; tok++) {
                if(*tok != '\"')
                    url += *tok;
            }
            if ((url.length() > 1) && std::strchr("GH", url[0])) {
                if (charIsDigit(url[1]))
                    url = url.c_str()+1;
            }
            buf += formatted("<a href=\"%s?showStrong=%s#cv\">", passageStudyURL, URL::encode(url));

            for (tok = token + 3; *tok; tok++)
                if(*tok != '\"')
                    buf += *tok;
            buf += "</a>)</em></small>";
        }

        else if (startsWith(token, "WT"sv)) { // morph tags
            buf += " <small><em>(";
            for (tok = token + 2; *tok; tok++) {
                if(*tok != '\"')
                    buf += *tok;
            }
            buf += formatted("<a href=\"%s?showMorph=%s#cv\">", passageStudyURL, URL::encode(url));

            for (tok = token + 2; *tok; tok++) {
                if(*tok != '\"')
                    buf += *tok;
            }
            buf += "</a>)</em></small>";
        }

        else if (startsWith(token, "RX"sv)) {
            buf += "<a href=\"";
            for (tok = token + 3; *tok; tok++) {
              if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
                buf += *tok;
              }
              else {
                break;
              }
            }

            buf += formatted("a href=\"%s?key=%s#cv\">", passageStudyURL, URL::encode(url));
        }
        // ok to leave these in
        else if (startsWith(token, "span"sv) || startsWith(token, "/span"sv)) {
            buf += formatted("<%s>", token);
        }

        else {
            return GBFXHTML::handleToken(buf, token, userData);
        }
    }
    return true;
}

} /* namespace swordxx */
