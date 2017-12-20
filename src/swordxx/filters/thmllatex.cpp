/******************************************************************************
 *
 *  thmllatex.cpp -    ThML to classed LaTeX
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

#include "thmllatex.h"

#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../url.h"
#include "../utilstr.h"


namespace swordxx {


const char *ThMLLaTeX::getHeader() const {
    return "\
    ";
}


ThMLLaTeX::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    if (module) {
        version = module->getName();
        isBiblicalText = (module->getType() == "Biblical Texts");
        isSecHead = false;
    }
}


ThMLLaTeX::ThMLLaTeX() {
    setTokenStart("<");
    setTokenEnd(">");

    setEscapeStart("&");
    setEscapeEnd(";");

    setEscapeStringCaseSensitive(true);
    setPassThruNumericEscapeString(true);

    addAllowedEscapeString("quot");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    addAllowedEscapeString("nbsp");
    addAllowedEscapeString("brvbar"); // "Š"
    addAllowedEscapeString("sect");   // "§"
    addAllowedEscapeString("copy");   // "©"
    addAllowedEscapeString("laquo");  // "«"
    addAllowedEscapeString("reg");    // "®"
    addAllowedEscapeString("acute");  // "Ž"
    addAllowedEscapeString("para");   // "¶"
    addAllowedEscapeString("raquo");  // "»"

    addAllowedEscapeString("Aacute"); // "Á"
    addAllowedEscapeString("Agrave"); // "À"
    addAllowedEscapeString("Acirc");  // "Â"
    addAllowedEscapeString("Auml");   // "Ä"
    addAllowedEscapeString("Atilde"); // "Ã"
    addAllowedEscapeString("Aring");  // "Å"
    addAllowedEscapeString("aacute"); // "á"
    addAllowedEscapeString("agrave"); // "à"
    addAllowedEscapeString("acirc");  // "â"
    addAllowedEscapeString("auml");   // "ä"
    addAllowedEscapeString("atilde"); // "ã"
    addAllowedEscapeString("aring");  // "å"
    addAllowedEscapeString("Eacute"); // "É"
    addAllowedEscapeString("Egrave"); // "È"
    addAllowedEscapeString("Ecirc");  // "Ê"
    addAllowedEscapeString("Euml");   // "Ë"
    addAllowedEscapeString("eacute"); // "é"
    addAllowedEscapeString("egrave"); // "è"
    addAllowedEscapeString("ecirc");  // "ê"
    addAllowedEscapeString("euml");   // "ë"
    addAllowedEscapeString("Iacute"); // "Í"
    addAllowedEscapeString("Igrave"); // "Ì"
    addAllowedEscapeString("Icirc");  // "Î"
    addAllowedEscapeString("Iuml");   // "Ï"
    addAllowedEscapeString("iacute"); // "í"
    addAllowedEscapeString("igrave"); // "ì"
    addAllowedEscapeString("icirc");  // "î"
    addAllowedEscapeString("iuml");   // "ï"
    addAllowedEscapeString("Oacute"); // "Ó"
    addAllowedEscapeString("Ograve"); // "Ò"
    addAllowedEscapeString("Ocirc");  // "Ô"
    addAllowedEscapeString("Ouml");   // "Ö"
    addAllowedEscapeString("Otilde"); // "Õ"
    addAllowedEscapeString("oacute"); // "ó"
    addAllowedEscapeString("ograve"); // "ò"
    addAllowedEscapeString("ocirc");  // "ô"
    addAllowedEscapeString("ouml");   // "ö"
    addAllowedEscapeString("otilde"); // "õ"
    addAllowedEscapeString("Uacute"); // "Ú"
    addAllowedEscapeString("Ugrave"); // "Ù"
    addAllowedEscapeString("Ucirc");  // "Û"
    addAllowedEscapeString("Uuml");   // "Ü"
    addAllowedEscapeString("uacute"); // "ú"
    addAllowedEscapeString("ugrave"); // "ù"
    addAllowedEscapeString("ucirc");  // "û"
    addAllowedEscapeString("uuml");   // "ü"
    addAllowedEscapeString("Yacute"); // "Ý"
    addAllowedEscapeString("yacute"); // "ý"
    addAllowedEscapeString("yuml");   // "ÿ"

    addAllowedEscapeString("deg");    // "°"
    addAllowedEscapeString("plusmn"); // "±"
    addAllowedEscapeString("sup2");   // "²"
    addAllowedEscapeString("sup3");   // "³"
    addAllowedEscapeString("sup1");   // "¹"
    addAllowedEscapeString("nbsp");   // "º"
    addAllowedEscapeString("pound");  // "£"
    addAllowedEscapeString("cent");   // "¢"
    addAllowedEscapeString("frac14"); // "Œ"
    addAllowedEscapeString("frac12"); // "œ"
    addAllowedEscapeString("frac34"); // "Ÿ"
    addAllowedEscapeString("iquest"); // "¿"
    addAllowedEscapeString("iexcl");  // "¡"
    addAllowedEscapeString("ETH");    // "Ð"
    addAllowedEscapeString("eth");    // "ð"
    addAllowedEscapeString("THORN");  // "Þ"
    addAllowedEscapeString("thorn");  // "þ"
    addAllowedEscapeString("AElig");  // "Æ"
    addAllowedEscapeString("aelig");  // "æ"
    addAllowedEscapeString("Oslash"); // "Ø"
    addAllowedEscapeString("curren"); // "€"
    addAllowedEscapeString("Ccedil"); // "Ç"
    addAllowedEscapeString("ccedil"); // "ç"
    addAllowedEscapeString("szlig");  // "ß"
    addAllowedEscapeString("Ntilde"); // "Ñ"
    addAllowedEscapeString("ntilde"); // "ñ"
    addAllowedEscapeString("yen");    // "¥"
    addAllowedEscapeString("not");    // "¬"
    addAllowedEscapeString("ordf");   // "ª"
    addAllowedEscapeString("uml");    // "š"
    addAllowedEscapeString("shy");    // "­"
    addAllowedEscapeString("macr");   // "¯"

    addAllowedEscapeString("micro");  // "µ"
    addAllowedEscapeString("middot"); // "·"
    addAllowedEscapeString("cedil");  // "ž"
    addAllowedEscapeString("ordm");   // "º"
    addAllowedEscapeString("times");  // "×"
    addAllowedEscapeString("divide"); // "÷"
    addAllowedEscapeString("oslash"); // "ø"

    setTokenCaseSensitive(true);
    addTokenSubstitute("scripture", " \\swordquote{ ");
    addTokenSubstitute("/scripture", "}");

    renderNoteNumbers = false;
}

std::unique_ptr<BasicFilterUserData> ThMLLaTeX::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool ThMLLaTeX::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
        MyUserData *u = (MyUserData *)userData;

        XMLTag tag(token);
        if ((!tag.isEndTag()) && (!tag.isEmpty()))
            u->startTag = tag;

        if (tag.name() == "sync") {
            std::string value = tag.attribute("value");
            if (tag.attribute("type") == "morph") { //&gt;
                if (value.length())
                    buf += formatted("\\swordmorph[Greek]{%s}", value.c_str());
            }
            else if (tag.attribute("type") == "lemma") { //&gt;
                if (value.length())
                    // empty "type=" is deliberate.
                    buf += formatted("\\swordmorph[lemma]{%s}", value.c_str());
            }
            else if (tag.attribute("type") == "Strongs") {
                if (!tag.isEndTag()) {
                        char const ch = *value.begin();
                        value.erase(0u, 1u);
                        buf += formatted("\\swordstrong[%s]{%s}{",
                            ((ch == 'H') ? "Hebrew" : "Greek"),
                            value.c_str());
                                        }
                                else {     buf += "}"; }
                        }

            else if (tag.attribute("type") == "Dict") {
                if (!tag.isEndTag()) {
                        buf += formatted("\\sworddict{%s}{",
                            value.c_str());
                                }
                                else { buf += "}"; }
            }

        }
        // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    std::string type = tag.attribute("type");
                    std::string footnoteNumber = tag.attribute("swordFootnote");
                    std::string noteName = tag.attribute("n");
                    std::string footnoteBody = "";
                    if (u->module){
                            footnoteBody += u->module->getEntryAttributes()["Footnote"][footnoteNumber]["body"];
                                        }
                    if (VerseKey const * const vkey =
                            dynamic_cast<VerseKey const *>(u->key))
                    {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x': 'n';
                        buf += formatted("\\swordfootnote[%c]{%s}{%s}{%s}{%s}{",
                            ch,
                            footnoteNumber.c_str(),
                            u->version.c_str(),
                            vkey->getText(),
                            noteName.c_str());
                    }
                    else {
                        char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x': 'n';
                        buf += formatted("\\swordfootnote[%c]{%s}{%s}{%s}{%s}{",
                            ch,
                            footnoteNumber.c_str(),
                            u->version.c_str(),
                            u->key->getText(),
                            noteName.c_str());
                    }
                    u->suspendTextPassThru = true;
                    if (u->module) {
                                                buf += u->module->renderText(footnoteBody.c_str()).c_str();
                                        }
                }
            }
            if (tag.isEndTag()) {
                    buf += "}";
                u->suspendTextPassThru = false;
            }
        }
        else if (tag.name() == "scripture") {
            buf += (tag.isEndTag() ? "\\swordquote" : "}");
        }
        // <scripRef> tag
        else if (tag.name() == "scripRef") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    u->suspendTextPassThru = true;
                }
            }
            if (!tag.isEndTag()) {    //    </scripRef>
                if (!u->isBiblicalText) {
                    std::string refList = u->startTag.attribute("passage");
                    if (!refList.length())
                        refList = u->lastTextNode;
                    std::string version = tag.attribute("version");

                    buf += formatted("\\swordxref{%s}{%s}{",
                        (refList.length()) ? refList.c_str() : "",
                        (version.length()) ? version.c_str() : "");
                    buf += u->lastTextNode.c_str();
                    buf += "}";
                }
                else {
                    std::string footnoteNumber = u->startTag.attribute("swordFootnote");
                    std::string noteName = tag.attribute("n");
                    std::string footnoteBody = "";
                    if (u->module){
                            footnoteBody += u->module->getEntryAttributes()["Footnote"][footnoteNumber]["body"];
                                        }
                    if (VerseKey const * const vkey =
                            dynamic_cast<VerseKey const *>(u->key))
                    {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        //buf.appendFormatted("<a href=\"noteID=%s.x.%s\"><small><sup>*x</sup></small></a> ", vkey->getText(), footnoteNumber.c_str());
                        // char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x': 'n';
                        char ch = 'x';
                        buf += formatted("\\swordfootnote[%c]{%s}{%s}{%s}{%s}{",
                            ch,
                            footnoteNumber.c_str(),
                            u->version.c_str(),
                            vkey->getText(),
                            (renderNoteNumbers ? noteName.c_str() : ""));
                        if (u->module) {
                                                        buf += u->module->renderText(footnoteBody.c_str()).c_str();
                                                }
                    }
                }


                        }
            else if (tag.isEndTag()){
                    buf +="}";
                    // let's let text resume to output again
                u->suspendTextPassThru = false;
            }
        }
        else if (tag.name() == "div") {

                        //VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, u->key);

                //if (!tag.isEndTag() && vkey && !vkey->getChapter())
                //        buf += "\\swordsection{book}{";
                //}


            if (!tag.isEndTag() && u->isSecHead) {
                buf += "\\swordsection{sechead}{";
                u->isSecHead = false;
            }

            else if (!tag.isEndTag() && !tag.attribute("class").empty()) {
                    buf += "\\swordsection{";
                                buf += tag.attribute("class");
                                buf += "}{";

            }
            else if (!tag.isEndTag()) {
                buf += "\\swordsection{}{";
            }

            else if (tag.isEndTag())  {
                    buf += "}";
                        }
        }
        else if ((tag.name() == "img") || (tag.name() == "image")) {
            const char *src = strstr(token, "src");
            if (!src)        // assert we have a src attribute
                return false;

            const char *c, *d;
            if (((c = strchr(src + 3u, '"')) == nullptr) ||
                ((d = strchr(++c , '"')) == nullptr))    // identify endpoints.
                return false;            // abandon hope.


            // images become clickable, if the UI supports showImage.
            buf +="\\figure{";

            for (c = token; *c; c++) {
                if ((*c == '/') && (*(c+1) == '\0'))
                    continue;
                if (c == src) {
                    for (;((*c) && (*c != '"')); c++)
                        buf += *c;

                    if (!*c) { c--; continue; }

                    buf += '"';
                    if (*(c+1) == '/') {
                        buf += "\\includegraphics{";
                        buf += userData->module->getConfigEntry("AbsoluteDataPath");
                        if (buf[buf.length()-2] == '/')
                            c++;        // skip '/'
                    }
                    continue;
                }
                buf += *c;
            }
                buf += "}}";
        }
        else if (tag.name() == "i") {
                if (!tag.isEndTag()) {
                                buf += "\\emph{";
                        }
                        else { buf += "}"; }
                }
        else if (tag.name() == "br") {
                        buf += "\\\\";

                }
                else {
            buf += '<';
            /*for (const char *tok = token; *tok; tok++)
                buf += *tok;*/
            buf += token;
            buf += '>';
            //return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
