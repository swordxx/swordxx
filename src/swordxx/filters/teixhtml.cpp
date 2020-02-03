/***************************************************************************
 *
 *  teixhtml.cpp -    TEI to XHTML filter
 *
 * Copyright 2012-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "teixhtml.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "../swmodule.h"
#include "../url.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {


const char *TEIXHTML::getHeader() const {
        // <pos>, <gen>, <case>, <gram>, <number>, <mood>, <pron>, <def> <tr> <orth> <etym> <usg>
    const static char *header = "\n\
        .entryFree, .form, .etym, .def, .usg, .quote {display:block;}\n\
        .pron, .pos, .oVar, .ref, {display:inline}\n\
        [type=headword] {font-weight:bold; font-variant:small-caps; text-decoration:underline;}\n\
        [type=derivative] {font-weight:bold; font-variant:small-caps;}\n\
        [rend=italic] {font-style:italic;}\n\
        [rend=bold] {font-weight:bold;}\n\
        [rend=small-caps] {font-variant:small-caps}\n\
        .pos:before {content: \"Pos.: \"; font-weight:bold;}\n\
        .pron:before {content:\" \\\\ \";}\n\
        .pron:after {content:\" \\\\ \";}\n\
        .etym:before {content:\"Etym.:\"; display:block; font-weight:bold;}\n\
        .usg:before {content:\"Usg.:\"; display:block; font-weight:bold;}\n\
        .def:before {content:\"Def.:\" display:block; font-weight:bold;}\n\
        .quote {background-color:#cfcfdf; padding:0.3em; margin:0.5em; border-width:1px; border-style:solid;}\n\
        .cit:before {content:\"quote:\" ; display:block; margin-top:0.5em; font-size:small;}\n\
        .cit {align:center;}\n\
        .persName:before {content:\" (\"; font-size:small;}\n\
        .persName:after {content:\") \"; font-size:small;}\n\
        .persName {font-size:small;}\n\
        .number {font-style:bold;}\n\
        .def {font-style:bold;}\n\
        ";
    return header;
}



TEIXHTML::MyUserData::MyUserData(SWModule const * module_,
                                 SWKey const * key_)
    : BasicFilterUserData(module_, key_)
{
    isBiblicalText = false;
    if (module_) {
        version = module_->getName();
        isBiblicalText = (module_->getType() == "Biblical Texts");
    }
}

TEIXHTML::MyUserData::~MyUserData() noexcept = default;

TEIXHTML::TEIXHTML() {
    setEscapeStringCaseSensitive(true);

    addAllowedEscapeString("quot");
    addAllowedEscapeString("apos");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    setTokenCaseSensitive(true);

    renderNoteNumbers = false;
}

std::unique_ptr<BasicFilterUserData> TEIXHTML::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool TEIXHTML::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        MyUserData * u = static_cast<MyUserData *>(userData);
        XMLTag tag(token);

        if (tag.name() == "p") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {    // non-empty start tag
                buf += "<!P><br />";
            }
            else if (tag.isEndTag()) {    // end tag
                buf += "<!/P><br />";
                //userData->supressAdjacentWhitespace = true;
            }
            else {                    // empty paragraph break marker
                buf += "<!P><br />";
                //userData->supressAdjacentWhitespace = true;
            }
        }

        // <hi>
        else if (tag.name() == "hi") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                std::string rend = tag.attribute("rend");

                u->lastHi = rend;
                if (rend == "italic" || rend == "ital")
                    buf += "<i>";
                else if (rend == "bold")
                    buf += "<b>";
                else if (rend == "super" || rend == "sup")
                    buf += "<sup>";
                else if (rend == "sub")
                    buf += "<sub>";
                else if (rend == "overline")
                    buf += "<span style=\"text-decoration:overline\">";

            }
            else if (tag.isEndTag()) {
                std::string rend = u->lastHi;
                if (rend == "italic" || rend == "ital")
                    buf += "</i>";
                else if (rend == "bold")
                    buf += "</b>";
                else if (rend == "super" || rend == "sup")
                    buf += "</sup>";
                else if (rend == "sub")
                    buf += "</sub>";
                else if (rend == "overline")
                    buf += "</span>";
            }
        }

        // <entryFree>
        else if (tag.name() == "entryFree") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                std::string n = tag.attribute("n");
                if (n != "") {
                    buf += "<span class=\"entryFree\">";
                    buf += n;
                    buf += "</span>";
                }
            }
        }

        // <sense>
        else if (tag.name() == "sense") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                std::string n = tag.attribute("n");
                buf += "<br/><span class=\"sense";
                if (n != "") {
                    buf += "\" n=\"";
                    buf += n;


                }
                buf += "\">";
            }
            else if (tag.isEndTag()) {
                buf += "</span> ";
            }
        }

        // <div>
        else if (tag.name() == "div") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<!P>";
            }
        }

        // <lb.../>
        else if (tag.name() == "lb") {
            buf += "<br />";
        }

        // <pos>, <gen>, <case>, <gram>, <number>, <mood>, <pron>, <def> <tr> <orth> <etym> <usg>
        else if ((tag.name() == "pos") ||
                 (tag.name() == "gen") ||
                 (tag.name() == "case") ||
                 (tag.name() == "gram") ||
                 (tag.name() == "number") ||
                 (tag.name() == "pron") ||
                 (tag.name() == "def") ||
                 (tag.name() == "tr") ||
                 (tag.name() == "orth") ||
                 (tag.name() == "etym") ||
                 (tag.name() == "usg") ||
                 (tag.name() == "quote")||
                 (tag.name() == "cit")||
                 (tag.name() == "persName")||
                 (tag.name() == "oVar"))
                 {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<span class=\"";
                buf += tag.name();
                if (!tag.attribute("type").empty()) {
                    buf += "\" type =\"";
                    buf += tag.attribute("type");
                }
                if (!tag.attribute("rend").empty()) {
                    buf += "\" rend =\"";
                    buf += tag.attribute("rend");
                }
                buf += "\">";
            }
            else if (tag.isEndTag()) {
                buf += "</span>";
            }
        }

        else if (tag.name() == "ref") {
            if (!tag.isEndTag()) {
                u->suspendTextPassThru = true;
                std::string target;
                std::string work;
                std::string ref;

                int was_osisref = false;
                if(!tag.attribute("osisRef").empty())
                {
                    target += tag.attribute("osisRef");
                    was_osisref=true;
                }
                else if(!tag.attribute("target").empty())
                    target += tag.attribute("target");

                if(target.size())
                {
                    const char* the_ref = std::strchr(target.c_str(), ':');

                    if(!the_ref) {
                        // No work
                        ref = target;
                    }
                    else {
                        // Compensate for starting :
                        ref = the_ref + 1;

                        int size = target.size() - ref.size() - 1;
                        work.resize(size, '\0');
                        std::strncpy(&work[0u], target.c_str(), size);
                    }

                    if(was_osisref)
                    {
                        buf += formatted("<a href=\"passagestudy.jsp?action=showRef&type=scripRef&value=%s&module=%s\">",
                            (!ref.empty()) ? URL::encode(ref).c_str() : "",
                            (!work.empty()) ? URL::encode(work).c_str() : "");
                    }
                    else
                    {
                        // Dictionary link, or something
                        buf += formatted("<a href=\"sword://%s/%s\">",
                            (!work.empty()) ? URL::encode(work) : u->version,
                            (!ref.empty()) ? URL::encode(ref).c_str() : ""
                            );
                    }
                }
                else
                {
                    //std::cout << "TARGET WASN'T\n";
                }

            }
            else {
                buf += u->lastTextNode.c_str();
                buf += "</a>";

                u->suspendTextPassThru = false;
            }
        }

           // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {
                std::string footnoteNumber = tag.attribute("swordFootnote");
                std::string noteName = tag.attribute("n");

                buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=n&value=%s&module=%s&passage=%s\"><small><sup class=\"n\">*n%s</sup></small></a>",
                    URL::encode(footnoteNumber),
                    URL::encode(u->version),
                    URL::encode(u->key->getText()),
                    (renderNoteNumbers ? URL::encode(noteName).c_str() : ""));

                u->suspendTextPassThru = false;
            }
        }
        // <graphic> image tag
        else if (tag.name() == "graphic") {
            auto const url(tag.attribute("url"));
            if (!url.empty()) {        // assert we have a url attribute
                std::string filepath;
                if (userData->module) {
                    filepath = userData->module->getConfigEntry("AbsoluteDataPath");
                    if ((filepath.size()) && (filepath.back() != '/') && (url[0] != '/'))
                        filepath += '/';
                }
                filepath += url;
                buf += formatted("<a href=\"passagestudy.jsp?action=showImage&value=%s&module=%s\"><img src=\"file:%s\" border=\"0\" /></a>",
                            URL::encode(filepath),
                            URL::encode(u->version),
                            filepath);
                u->suspendTextPassThru = false;
            }
        }
        // <table> <row> <cell>
        else if (tag.name() == "table") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<table><tbody>\n";
            }
            else if (tag.isEndTag()) {
                buf += "</tbody></table>\n";
                u->supressAdjacentWhitespace = true;
            }
        }
        else if (tag.name() == "row") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "\t<tr>";
            }
            else if (tag.isEndTag()) {
                buf += "</tr>\n";
            }
        }
        else if (tag.name() == "cell") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<td>";
            }
            else if (tag.isEndTag()) {
                buf += "</td>";
            }
        }

        // <list> <item>
        else if (tag.name() == "list") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                u->lastHi = tag.attribute("rend");
                auto const & rend = u->lastHi;
                if (rend == "numbered") {
                    buf += "<ol>\n";
                } else if (rend == "bulleted") {
                    buf += "<ul>\n";
                } else {
                    buf.append("<span class=\"list ").append(rend.c_str())
                            .append("\">");
                }
            } else if (tag.isEndTag()) {
                auto const & rend = u->lastHi;
                if (rend == "numbered") {
                    buf += "</ol>\n>";
                } else if (rend == "bulleted") {
                    buf += "</ul>\n";
                } else {
                    buf += "</span>\n";
                }
                u->supressAdjacentWhitespace = true;
            }
        } else if (tag.name() == "item") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<li>";
            } else if (tag.isEndTag()) {
                buf += "</li>\n";
            }
        } else {
            return false;  // we still didn't handle token
        }

    }
    return true;
}


} /* namespace swordxx */

