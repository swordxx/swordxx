/***************************************************************************
 *
 *  osishtmlhref.cpp -    OSIS to HTML with hrefs filter
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

#include "osishtmlhref.h"

#include <cstdlib>
#include <cstring>
#include <stack>
#include <utility>
#include "../keys/versekey.h"
#include "../SimpleTokenizer.h"
#include "../swmodule.h"
#include "../unicode.h"
#include "../url.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {
// though this might be slightly slower, possibly causing an extra bool check, this is a renderFilter
// so speed isn't the absolute highest priority, and this is a very minor possible hit
template <typename T>
inline void outText(T && t, std::string & o, BasicFilterUserData & u) {
    if (u.suspendTextPassThru) {
        u.lastSuspendSegment += std::forward<T>(t);
    } else {
        o += std::forward<T>(t);
    }
}

void processLemma(XMLTag const & tag, std::string & buf) {
    using namespace std::literals::string_view_literals;
    if (auto const attrib = tag.attribute("lemma"); !attrib.empty()) {
        for (auto const & token : SimpleTokenizer<>::tokenize(attrib, ' ')) {
            auto const separatorPos(token.find(':'));
            auto const val((separatorPos == std::string_view::npos)
                           ? token
                           : token.substr(separatorPos + 1u));
            std::string_view gh;
            auto val2(val);
            if (!val.empty()) {
                if (val.front() == 'G') {
                    gh = "Greek"sv;
                    if ((val.size() >= 2u) && charIsDigit(val[1u]))
                        val2.remove_prefix(1u);
                } else if (val.front() == 'H') {
                    gh = "Hebrew"sv;
                    if ((val.size() >= 2u) && charIsDigit(val[1u]))
                        val2.remove_prefix(1u);
                }
            }

            static constexpr auto const part1(
                        "<small><em class=\"strongs\">&lt;<a href=\""
                        "passagestudy.jsp?action=showStrongs&type="sv);
            static constexpr auto const part2("&value="sv);
            static constexpr auto const part3("\" class=\"strongs\">"sv);
            static constexpr auto const part4("</a>&gt;</em></small>"sv);
            auto const url(URL::encode(val2));
            // Note that no overflow check is strictly needed for the addition:
            buf.reserve(
                    (part1.size() + part2.size() + part3.size() + part4.size())
                    + buf.size() + gh.size() + url.size() + val2.size());
            buf.append(part1).append(gh).append(part2).append(url).append(part3)
               .append(val2).append(part4);
        }
    }
}

void processMorph(XMLTag const & tag, std::string & buf) {
    if (auto const attrib = tag.attribute("morph"); !attrib.empty()) {
        for (auto const & token : SimpleTokenizer<>::tokenize(attrib, ' ')) {
            auto const separatorPos(token.find(':'));
            auto const val((separatorPos == std::string_view::npos)
                           ? token
                           : token.substr(separatorPos + 1u));
            auto val2(val);
            if ((val.size() >= 3u)
                && (val.front() == 'T')
                && ((val[1u] == 'G') || (val[1u] == 'H'))
                && charIsDigit(val[2u]))
                val2.remove_prefix(2u);
            using namespace std::literals::string_view_literals;
            static constexpr auto const part1(
                        "<small><em class=\"morph\">(<a href=\"passagestudy.jsp"
                        "?action=showMorph&type="sv);
            static constexpr auto const part2("&value="sv);
            static constexpr auto const part3("\" class=\"morph\">"sv);
            static constexpr auto const part4("</a>)</em></small>"sv);
            auto const url1(URL::encode(attrib));
            auto const url2(URL::encode(val));
            // Note that no overflow check is strictly needed for the addition:
            buf.reserve(
                    (part1.size() + part2.size() + part3.size() + part4.size())
                    + buf.size() + url1.size() + url2.size() + val2.size());
            buf.append(part1).append(url1).append(part2).append(url2)
               .append(part3).append(val2).append(part4);
        }
    }
}
}    // end anonymous namespace

OSISHTMLHREF::MyUserData::MyUserData(SWModule const * module_,
                                     SWKey const * key_)
    : BasicFilterUserData(module_, key_)
{
    if (module_) {
        osisQToTick = ((!module_->getConfigEntry("OSISqToTick")) || (std::strcmp(module_->getConfigEntry("OSISqToTick"), "false")));
        version = module_->getName();
        isBiblicalText = (module_->getType() == "Biblical Texts");
    } else {
        isBiblicalText = false;
        osisQToTick = true;    // default
    }
}

OSISHTMLHREF::MyUserData::~MyUserData() {}

OSISHTMLHREF::OSISHTMLHREF() {
    setEscapeStringCaseSensitive(true);
    setPassThruNumericEscapeString(true);

    addAllowedEscapeString("quot");
    addAllowedEscapeString("apos");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    setTokenCaseSensitive(true);

    //    addTokenSubstitute("lg",  "<br />");
    //    addTokenSubstitute("/lg", "<br />");

    morphFirst = false;
    renderNoteNumbers = false;
}

std::unique_ptr<BasicFilterUserData> OSISHTMLHREF::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool OSISHTMLHREF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    using namespace std::literals::string_view_literals;

    auto & u = *static_cast<MyUserData *>(userData);
    std::string scratch;
    bool sub = (u.suspendTextPassThru) ? substituteToken(scratch, token) : substituteToken(buf, token);
    if (!sub) {
  // manually process if it wasn't a simple substitution
        XMLTag tag(token);

        // <w> tag
        if (tag.name() == "w") {

            // start <w> tag
            if ((!tag.isEmpty()) && (!tag.isEndTag())) {
                u.w = token;
            }

            // end or empty <w> tag
            else {
                bool endTag = tag.isEndTag();
                std::string lastText;
                //bool show = true;    // to handle unplaced article in kjv2003-- temporary till combined

                if (endTag) {
                    tag = u.w.c_str();
                    lastText = u.lastTextNode.c_str();
                }
                else lastText = "stuff";

                std::string attrib;
                const char *val;
                if (!(attrib = tag.attribute("xlit")).empty()) {
                    val = std::strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    outText(" ", buf, u);
                    outText(val, buf, u);
                }
                if (!(attrib = tag.attribute("gloss")).empty()) {
                    // I'm sure this is not the cleanest way to do it, but it gets the job done
                    // for rendering ruby chars properly ^_^
                    buf.resize(buf.size() - lastText.size());

                    outText("<ruby><rb>", buf, u);
                    outText(lastText.c_str(), buf, u);
                    outText("</rb><rp>(</rp><rt>", buf, u);
                    val = std::strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    outText(val, buf, u);
                    outText("</rt><rp>)</rp></ruby>", buf, u);
                }
                if (!u.suspendTextPassThru) {
                    if (morphFirst) {
                        processMorph(tag, buf);
                        processLemma(tag, buf);
                    } else {
                        processLemma(tag, buf);
                        processMorph(tag, buf);
                    }
                }
                if (!(attrib = tag.attribute("POS")).empty()) {
                    val = std::strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    outText(" ", buf, u);
                    outText(val, buf, u);
                }

                /*if (endTag)
                    buf += "}";*/
            }
        }

        // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                std::string type = tag.attribute("type");
                bool strongsMarkup = (type == "x-strongsMarkup" || type == "strongsMarkup");    // the latter is deprecated
                if (strongsMarkup) {
                    tag.setEmpty(false);    // handle bug in KJV2003 module where some note open tags were <note ... />
                }

                if (!tag.isEmpty()) {

                    if (!strongsMarkup) {    // leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
                        std::string footnoteNumber = tag.attribute("swordFootnote");
                        std::string noteName = tag.attribute("n");
                        char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x':'n';

                        u.inXRefNote = true; // Why this change? Ben Morgan: Any note can have references in, so we need to set this to true for all notes
//                        u.inXRefNote = (ch == 'x');

                        // see if we have a VerseKey * or descendant
                        auto const * const vkey = u.verseKey;
                        buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup class=\"%c\">*%c%s</sup></small></a>",
                                ch,
                            URL::encode(footnoteNumber),
                            URL::encode(u.version),
                            URL::encode(vkey ? vkey->getText() : u.key->getText()),
                            ch,
                            ch,
                            (renderNoteNumbers ? noteName.c_str() : ""));
                    }
                }
                u.suspendTextPassThru = (++u.suspendLevel);
            }
            if (tag.isEndTag()) {
                u.suspendTextPassThru = (--u.suspendLevel);
                u.inXRefNote = false;
                u.lastSuspendSegment = ""; // fix/work-around for nasb divineName in note bug
            }
        }

        // <p> paragraph and <lg> linegroup tags
        else if ((tag.name() == "p") || (tag.name() == "lg")) {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {    // non-empty start tag
                outText("<!P><br />", buf, u);
            }
            else if (tag.isEndTag()) {    // end tag
                outText("<!/P><br />", buf, u);
                userData->supressAdjacentWhitespace = true;
            }
            else {                    // empty paragraph break marker
                outText("<!P><br />", buf, u);
                userData->supressAdjacentWhitespace = true;
            }
        }

        // Milestoned paragraphs, created by osis2mod
        // <div type="paragraph" sID.../>
        // <div type="paragraph" eID.../>
        else if (tag.isEmpty() && (tag.name() == "div") && ((tag.attribute("type") == "x-p") || (tag.attribute("type") == "paragraph"))) {
            // <div type="paragraph"  sID... />
            if (!tag.attribute("sID").empty()) {    // non-empty start tag
                outText("<!P><br />", buf, u);
            }
            // <div type="paragraph"  eID... />
            else if (!tag.attribute("eID").empty()) {
                outText("<!/P><br />", buf, u);
                userData->supressAdjacentWhitespace = true;
            }
        }

        // <reference> tag
        else if (tag.name() == "reference") {
            if (!u.inXRefNote) {    // only show these if we're not in an xref note
                if (!tag.isEndTag()) {
                    std::string work;
                    std::string ref;
                    bool is_scripRef = false;

                    auto target(tag.attribute("osisRef"));
                    const char* the_ref = std::strchr(target.c_str(), ':');

                    if(!the_ref) {
                        // No work
                        ref = target;
                        is_scripRef = true;
                    }
                    else {
                        // Compensate for starting :
                        ref = the_ref + 1;

                        auto const size = target.size() - ref.size() - 1;
                        work.resize(size, '\0');
                        std::strncpy(&work[0u], target.c_str(), size);

                        // For Bible:Gen.3.15 or Bible.vulgate:Gen.3.15
                        if(startsWith(work.c_str(), "Bible"sv))
                            is_scripRef = true;
                    }

                    if(is_scripRef)
                    {
                        buf += formatted("<a href=\"passagestudy.jsp?action=showRef&type=scripRef&value=%s&module=\">",
                            URL::encode(ref)
//                            (work.size()) ? URL::encode(work).c_str() : "")
                            );
                    }
                    else
                    {
                        // Dictionary link, or something
                        buf += formatted("<a href=\"sword://%s/%s\">",
                                         URL::encode(work),
                                         URL::encode(ref));
                    }
                }
                else {
                    outText("</a>", buf, u);
                }
            }
        }

        // <l> poetry, etc
        else if (tag.name() == "l") {
            // end line marker
            if (!tag.attribute("eID").empty()) {
                outText("<br />", buf, u);
            }
            // <l/> without eID or sID
            // Note: this is improper osis. This should be <lb/>
            else if (tag.isEmpty() && tag.attribute("sID").empty()) {
                outText("<br />", buf, u);
            }
            // end of the line
            else if (tag.isEndTag()) {
                outText("<br />", buf, u);
            }
        }

        // <lb.../>
        else if ((tag.name() == "lb") && (tag.attribute("type").empty() || (tag.attribute("type") != "x-optional"))) {
            outText("<br />", buf, u);
            userData->supressAdjacentWhitespace = true;
        }
        // <milestone type="line"/>
        // <milestone type="x-p"/>
        // <milestone type="cQuote" marker="x"/>
        else if ((tag.name() == "milestone") && !tag.attribute("type").empty()) {
            if (tag.attribute("type") == "line") {
                outText("<br />", buf, u);
                if (tag.attribute("subType") == "x-PM") {
                    outText("<br />", buf, u);
                }
                userData->supressAdjacentWhitespace = true;
            }
            else if (tag.attribute("type") == "x-p") {
                if (!tag.attribute("marker").empty())
                    outText(tag.attribute("marker").c_str(), buf, u);
                else outText("<!p>", buf, u);
            }
            else if (tag.attribute("type") == "cQuote") {
                auto mark(tag.attribute("marker"));
                bool hasMark = !mark.empty();
                auto tmp(tag.attribute("level"));
                int level = (!tmp.empty()) ? std::atoi(tmp.c_str()) : 1;

                // first check to see if we've been given an explicit mark
                if (hasMark)
                    outText(mark.c_str(), buf, u);
                // finally, alternate " and ', if config says we should supply a mark
                else if (u.osisQToTick)
                    outText((level % 2) ? '\"' : '\'', buf, u);
            }
        }

        // <title>
        else if (tag.name() == "title") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("<b>", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</b><br />", buf, u);
            }
        }

        // <list>
        else if (tag.name() == "list") {
            if((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("<ul>", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</ul>", buf, u);
            }
        }

        // <item>
        else if (tag.name() == "item") {
            if((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("<li>", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</li>", buf, u);
            }
        }
        // <catchWord> & <rdg> tags (italicize)
        else if ((tag.name() == "rdg") || (tag.name() == "catchWord")) {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("<i>", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</i>", buf, u);
            }
        }

        // divineName
        else if (tag.name() == "divineName") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                u.suspendTextPassThru = (++u.suspendLevel);
            }
            else if (tag.isEndTag()) {
                std::string lastText = u.lastSuspendSegment;
                u.suspendTextPassThru = (--u.suspendLevel);
                if (!lastText.empty()) {
                    lastText = utf8ToUpper(lastText);
                    auto const r(codepointFromUtf8(lastText));
                    auto const char_length = r.second ? r.second : 1;
                    scratch = formatted("%.*s<font size=\"-1\">%s</font>",
                        char_length,
                        lastText.c_str(),
                        lastText.c_str() + char_length
                    );

                    outText(scratch.c_str(), buf, u);
                }
            }
        }

        // <hi> text highlighting
        else if (tag.name() == "hi") {
            std::string type = tag.attribute("type");
            // handle tei rend attribute
            if (!type.length()) type = tag.attribute("rend");
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                if (type == "bold" || type == "b" || type == "x-b") {
                    outText("<b>", buf, u);
                }
                else if (type == "ol" || type == "overline" || type == "x-overline") {
                    outText("<span style=\"text-decoration:overline\">", buf, u);
                }
                else if (type == "super") {
                    outText("<sup>", buf, u);
                }
                else if (type == "sub") {
                    outText("<sub>", buf, u);
                }
                else {    // all other types
                    outText("<i>", buf, u);
                }
                u.hiStack.push(tag.toString());
            }
            else if (tag.isEndTag()) {
                std::string type2 = "";
                if (!u.hiStack.empty()) {
                    XMLTag tag2(u.hiStack.top().c_str());
                    u.hiStack.pop();
                    type2 = tag2.attribute("type");
                    if (!type2.length()) type2 = tag2.attribute("rend");
                }
                if (type2 == "bold" || type2 == "b" || type2 == "x-b") {
                    outText("</b>", buf, u);
                }
                else if (type2 == "ol") {
                    outText("</span>", buf, u);
                }
                else if (type2 == "super") {
                    outText("</sup>", buf, u);
                }
                else if (type2 == "sub") {
                    outText("</sub>", buf, u);
                }
                else {
                    outText("</i>", buf, u);
                }
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
            int level = (!tmp.empty()) ? std::atoi(tmp.c_str()) : 1;
            auto mark(tag.attribute("marker"));
            bool hasMark = !mark.empty();

            // open <q> or <q sID... />
            if ((!tag.isEmpty() && !tag.isEndTag()) || (tag.isEmpty() && !tag.attribute("sID").empty())) {
                // if <q> then remember it for the </q>
                if (!tag.isEmpty()) {
                    u.quoteStack.push(tag.toString());
                }

                // Do this first so quote marks are included as WoC
                if (who == "Jesus")
                    outText(u.wordsOfChristStart.c_str(), buf, u);

                // first check to see if we've been given an explicit mark
                if (hasMark)
                    outText(mark.c_str(), buf, u);
                //alternate " and '
                else if (u.osisQToTick)
                    outText((level % 2) ? '\"' : '\'', buf, u);
            }
            // close </q> or <q eID... />
            else if ((tag.isEndTag()) || (tag.isEmpty() && !tag.attribute("eID").empty())) {
                // if it is </q> then pop the stack for the attributes
                if (tag.isEndTag() && !u.quoteStack.empty()) {
                    XMLTag qTag(u.quoteStack.top().c_str());
                    u.quoteStack.pop();

                    type    = qTag.attribute("type");
                    who     = qTag.attribute("who");
                    tmp     = qTag.attribute("level");
                    level   = (!tmp.empty()) ? std::atoi(tmp.c_str()) : 1;
                    mark    = qTag.attribute("marker");
                    hasMark = !mark.empty();
                }

                // first check to see if we've been given an explicit mark
                if (hasMark)
                    outText(mark.c_str(), buf, u);
                // finally, alternate " and ', if config says we should supply a mark
                else if (u.osisQToTick)
                    outText((level % 2) ? '\"' : '\'', buf, u);

                // Do this last so quote marks are included as WoC
                if (who == "Jesus")
                    outText(u.wordsOfChristEnd.c_str(), buf, u);
            }
        }

        // <transChange>
        else if (tag.name() == "transChange") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                std::string type = tag.attribute("type");
                u.lastTransChange = type;

                // just do all transChange tags this way for now
                if ((type == "added") || (type == "supplied"))
                    outText("<i>", buf, u);
                else if (type == "tenseChange")
                    outText( "*", buf, u);
            }
            else if (tag.isEndTag()) {
                std::string type = u.lastTransChange;
                if ((type == "added") || (type == "supplied"))
                    outText("</i>", buf, u);
            }
            else {    // empty transChange marker?
            }
        }

        // image
        else if (tag.name() == "figure") {
            auto const src(tag.attribute("src"));
            if (src.empty())        // assert we have a src attribute
                return false;

            std::string filepath;
            if (userData->module) {
                filepath = userData->module->getConfigEntry("AbsoluteDataPath");
                if ((!filepath.empty()) && (filepath.back() != '/') && (src[0] != '/'))
                    filepath += '/';
            }
            filepath += src;

            // images become clickable, if the UI supports showImage.
            outText("<a href=\"passagestudy.jsp?action=showImage&value=", buf, u);
            outText(URL::encode(filepath).c_str(), buf, u);
            outText("&module=", buf, u);
            outText(URL::encode(u.version).c_str(), buf, u);
            outText("\">", buf, u);

            outText("<img src=\"file:", buf, u);
            outText(filepath.c_str(), buf, u);
            outText("\" border=\"0\" />", buf, u);

            outText("</a>", buf, u);
        }

        else {
              return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
