/******************************************************************************
 *
 *  osisxhtml.cpp -    Render filter for classed XHTML of an OSIS module
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

#include "osisxhtml.h"

#include <cstdlib>
#include <cstring>
#include <stack>
#include <utility>
#include "../keys/versekey.h"
#include "../SimpleTokenizer.h"
#include "../swmodule.h"
#include "../url.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

const char *OSISXHTML::getHeader() const {
    static char const header[] =
        ".divineName { font-variant: small-caps; }\n"
        ".wordsOfJesus { color: red; }\n"
        ".transChange { font-style: italic; }\n"
        ".transChange.transChange-supplied { font-style: italic; }\n"
        ".transChange.transChange-added { font-style: italic; }\n"
        ".transChange.transChange-tenseChange::before { content: '*'; }\n"
        ".transChange.transChange-tenseChange { font-style: normal; }\n"
        ".transChange:lang(zh) { font-style: normal; text-decoration: dotted underline; }\n"
        ".overline { text-decoration: overline; }\n"
        ".indent1 { margin-left: 1em; }\n"
        ".indent2 { margin-left: 2em; }\n"
        ".indent3 { margin-left: 3em; }\n"
        ".indent4 { margin-left: 4em; }\n"
        "abbr { &:hover { &:before { content: attr(title); } } }\n"
        ".small-caps { font-variant: small-caps; }\n"
        ".otPassage { font-variant: small-caps; }\n"
        ".selah { text-align: right; width: 50%; margin: 0; padding: 0; }\n"
        ".acrostic { text-align: center; }\n"
        ".colophon { font-style: italic; font-size: small; display: block; }\n"
        ".rdg { font-style: italic; }\n"
        ".inscription {font-variant: small-caps; }\n"
        ".catchWord { font-style: bold; }\n"
        ".x-p-indent { text-indent: 1em; }\n"
    ;
    // Acrostic for things like the titles in Psalm 119
    return header;
}


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
            if (val.empty()) {
                gh = token.substr(0u, separatorPos); // Prefix
            } else if (val.front() == 'G') {
                gh = "Greek"sv;
                if ((val.size() >= 2u) && charIsDigit(val[1u]))
                    val2.remove_prefix(1u);
            } else if (val.front() == 'H') {
                gh = "Hebrew"sv;
                if ((val.size() >= 2u) && charIsDigit(val[1u]))
                    val2.remove_prefix(1u);
            } else {
                gh = token.substr(0u, separatorPos); // Prefix
            }
            static constexpr auto const part1(
                        "<small><em class=\"strongs\">&lt;<a class=\"strongs\" "
                        "href=\"passagestudy.jsp?action=showStrongs&type="sv);
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
                        "<small><em class=\"morph\">(<a class=\"morph\" "
                        "href=\"passagestudy.jsp?action=showMorph&type="sv);
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

std::unique_ptr<BasicFilterUserData> OSISXHTML::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

OSISXHTML::OSISXHTML()
    : SWBasicFilter(CaseSensitiveTokens
                    | CaseSensitiveEscapeStrings
                    | PassThroughNumericEscapeStrings)
{
    addAllowedEscapeString("quot");
    addAllowedEscapeString("apos");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    //    addTokenSubstitute("lg",  "<br />");
    //    addTokenSubstitute("/lg", "<br />");

    morphFirst = false;
    renderNoteNumbers = false;
}

OSISXHTML::MyUserData::MyUserData(SWModule const * module_, SWKey const * key_)
    : BasicFilterUserData(module_, key_)
{
    if (module_) {
        osisQToTick = ((!module_->getConfigEntry("OSISqToTick")) || (std::strcmp(module_->getConfigEntry("OSISqToTick"), "false")));
        version = module_->getName();
        isBiblicalText = (module_->getType() == "Biblical Texts");
    } else {
        osisQToTick = true;
        isBiblicalText = false;
    }
}

OSISXHTML::MyUserData::~MyUserData() {}

void OSISXHTML::MyUserData::outputNewline(std::string &buf) {
    if (++consecutiveNewlines <= 2) {
        /* Any newlines at the start of a verse should get appended to a
           preverse heading since preverse cause a newline, simply be sure we
           have a preverse: */
        if (buf.empty() && verseKey && verseKey->getVerse() && module
            && module->isProcessEntryAttributes())
        {
            module->getEntryAttributes()["Heading"]["Preverse"]["0"] +=
                    "<div></div>";
        } else {
            outText("<br />\n", buf, *this);
        }
        supressAdjacentWhitespace = true;
    }
}
bool OSISXHTML::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
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
                //bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

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
                    outText( "}", buf, u);*/
            }
        }

        // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                auto const type(tag.attribute("type"));
                auto const subType(tag.attribute("subType"));
                std::string classExtras;
                if (!type.empty())
                    classExtras.append(1, ' ').append(type);
                if (!subType.empty())
                    classExtras.append(1, ' ').append(subType);

                bool strongsMarkup = (type == "x-strongsMarkup" || type == "strongsMarkup");	// the latter is deprecated
                if (strongsMarkup) {
                    tag.setEmpty(false);	// handle bug in KJV2003 module where some note open tags were <note ... />
                }

                if (!tag.isEmpty()) {

                    if (!strongsMarkup) {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
                        std::string footnoteNumber = tag.attribute("swordFootnote");
                        std::string noteName = tag.attribute("n");
                        char ch = ((!tag.attribute("type").empty() && ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref"))) ? 'x':'n');

                        u.inXRefNote = true; // Why this change? Ben Morgan: Any note can have references in, so we need to set this to true for all notes
//						u->inXRefNote = (ch == 'x');

                        if (auto const * const vkey = u.verseKey) {
                            //printf("URL = %s\n",URL::encode(vkey->getText()).c_str());
                            buf += formatted("<a class=\"noteMarker%s\" href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup class=\"%c\">*%c%s</sup></small></a>",
                                classExtras,
                                ch,
                                URL::encode(footnoteNumber),
                                URL::encode(u.version),
                                URL::encode(vkey->getText()),
                                ch,
                                ch,
                                (renderNoteNumbers ? noteName.c_str() : ""));
                        }
                        else {
                            buf += formatted("<a class=\"noteMarker%s\" href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup class=\"%c\">*%c%s</sup></small></a>",
                                classExtras,
                                ch,
                                URL::encode(footnoteNumber),
                                URL::encode(u.version),
                                URL::encode(u.key->getText()),
                                ch,
                                ch,
                                (renderNoteNumbers ? noteName.c_str() : ""));
                        }
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

        /* <p> paragraph and <lg> linegroup tags except newline at start of
           verse (immediately after verse number): */
        else if ((tag.name() == "p") || (tag.name() == "lg")) {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
                u.outputNewline(buf);
            }
            else if (tag.isEndTag()) {	// end tag
                u.outputNewline(buf);
            }
            else {					// empty paragraph break marker
                u.outputNewline(buf);
            }
        }

        // Milestoned paragraphs, created by osis2mod
        // <div type="paragraph" sID.../>
        // <div type="paragraph" eID.../>
        else if (tag.isEmpty() && (tag.name() == "div") && ((tag.attribute("type") == "x-p") || (tag.attribute("type") == "paragraph") || (tag.attribute("type") == "colophon"))) {
            // <div type="paragraph"  sID... />
            if (!tag.attribute("sID").empty()) {	// non-empty start tag
                u.outputNewline(buf);
                // safe because we've verified type is present from if statement above
                if (tag.attribute("type") == "colophon") {
                    outText("<div class=\"colophon\">", buf, u);
                }

            }
            // <div type="paragraph"  eID... />
            else if (!tag.attribute("eID").empty()) {
                u.outputNewline(buf);
                // safe because we've verified type is present from if statement above
                if (tag.attribute("type") == "colophon") {
                    outText("</div>", buf, u);
                }

            }
        }

        // <reference> tag
        else if (tag.name() == "reference") {
            if (!u.inXRefNote) {	// only show these if we're not in an xref note
                if (!tag.isEndTag()) {
                    std::string work;
                    std::string ref;
                    std::string classes;
                    bool is_scripRef = false;

                    std::string target(tag.attribute("osisRef"));
                    const char* the_ref = std::strchr(target.c_str(), ':');
                    {
                        auto const type(tag.attribute("type"));
                        if (!type.empty())
                            classes.append(type);
                    }

                    if(!the_ref) {
                        // No work
                        ref = target;
                        is_scripRef = true;
                    }
                    else {
                        // Compensate for starting :
                        ref = the_ref + 1;

                        auto const size = target.size() - ref.size() - 1u;
                        work.resize(size, '\0');
                        std::strncpy(&work[0u], target.c_str(), size);

                        // For Bible:Gen.3.15 or Bible.vulgate:Gen.3.15
                        if(startsWith(work, "Bible"sv))
                            is_scripRef = true;
                    }

                    if(is_scripRef)
                    {
                        buf += formatted("<a class=\"%s\" href=\"passagestudy.jsp?action=showRef&type=scripRef&value=%s&module=\">",
                            classes,
                            URL::encode(ref)
//							(work.size()) ? URL::encode(work).c_str() : "")
                            );
                    }
                    else
                    {
                        // Dictionary link, or something
                        buf += formatted(u.interModuleLinkStart.c_str(),
                            classes,
                            URL::encode(work),
                            URL::encode(ref)
                            );
                    }
                }
                else {
                    outText(u.interModuleLinkEnd.c_str(), buf, u);
                }
            }
        }

        // <l> poetry, etc
        else if (tag.name() == "l") {
            // start line marker
            if (!tag.attribute("sID").empty() || (!tag.isEndTag() && !tag.isEmpty())) {
                std::string type = tag.attribute("type");
                if (type == "selah") {
                    outText("<p class=\"selah\">", buf, u);
                } else {
                    // nested lines plus if the line itself has an x-indent type attribute value
                    outText(formatted("<span class=\"line indent%d\">", u.lineStack.size() + (std::string("x-indent") == tag.attribute("type").c_str()?1:0)).c_str(), buf, u);
                }
                u.lineStack.push(tag.toString());
            }
            // end line marker
            else if (!tag.attribute("eID").empty() || tag.isEndTag()) {
                std::string type = "";
                if (!u.lineStack.empty()) {
                    XMLTag startTag(u.lineStack.top().c_str());
                    type = startTag.attribute("type");
                }
                if (type == "selah") {
                    outText("</p>", buf, u);
                } else {
                    outText("</span>", buf, u);
                    u.outputNewline(buf);
                }
                if (!u.lineStack.empty())
                    u.lineStack.pop();
            }
            // <l/> without eID or sID
            // Note: this is improper osis. This should be <lb/>
            else if (tag.isEmpty() && tag.attribute("sID").empty()) {
                u.outputNewline(buf);
            }
        }

        // <lb.../>
        else if ((tag.name() == "lb") && (tag.attribute("type").empty() || (tag.attribute("type") != "x-optional"))) {
                u.outputNewline(buf);
        }
        // <milestone type="line"/>
        // <milestone type="x-p"/>
        // <milestone type="cQuote" marker="x"/>
        else if ((tag.name() == "milestone") && (!tag.attribute("type").empty())) {
            // safe because we've verified type is present from if statement above
            auto type(tag.attribute("type"));
            if (type == "line") {
                u.outputNewline(buf);
                if (!tag.attribute("subType").empty() && (tag.attribute("subType") == "x-PM")) {
                    u.outputNewline(buf);
                }
            }
            else if (type == "x-p")  {
                if (!tag.attribute("marker").empty())
                    outText(tag.attribute("marker").c_str(), buf, u);
                else outText("<!p>", buf, u);
            }
            else if (type == "cQuote") {
                std::string mark(tag.attribute("marker"));
                bool hasMark = !mark.empty();
                auto tmp(tag.attribute("level"));
                int level = (!tmp.empty()) ? std::atoi(tmp.c_str()) : 1;

                // first check to see if we've been given an explicit mark
                if (hasMark)
                    outText(mark.c_str(), buf, u);
                // finally, alternate " and ', if config says we should supply a mark
                else if (u.osisQToTick)
                    outText((level % 2) ? '\"' : '\'', buf, u);
            } else if (type == "x-importer") {
                //drop tag as not relevant
            } else {
                outText(("<span class=\""
                         + tag.attribute("type")
                         + '"').c_str(),
                        buf,
                        u);
                for (auto const & [attributeName, value] : tag.attributes())
                    outText((" data-" + attributeName + "=\"" + value + "\"").c_str(),
                            buf,
                            u);
                outText("/>", buf, u);
            }
        }

        // <title>
        else if (tag.name() == "title") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {

                std::string classExtras;
                {
                    auto const type(tag.attribute("type"));
                    if (!type.empty())
                        classExtras.append(1, ' ').append(type);
                }
                if (tag.attribute("canonical") == "true")
                    classExtras.append(" canonical");
                auto const * const vkey = u.verseKey;
                if (vkey && !vkey->getVerse()) {
                    if (!vkey->getChapter()) {
                        if (!vkey->getBook()) {
                            if (!vkey->getTestament()) {
                                outText((std::string("<h1 class=\"moduleHeader") + classExtras + "\">").c_str(),  buf, u);
                                tag.attributes()["pushed"] = "h1";
                            }
                            else {
                                outText((std::string("<h1 class=\"testamentHeader") + classExtras + "\">").c_str(), buf, u);
                                tag.attributes()["pushed"] = "h1";
                            }
                        }
                        else {
                            outText((std::string("<h1 class=\"bookHeader") + classExtras + "\">").c_str(), buf, u);
                            tag.attributes()["pushed"] = "h1";
                        }
                    }
                    else {
                        outText((std::string("<h2 class=\"chapterHeader") + classExtras + "\">").c_str(), buf, u);
                        tag.attributes()["pushed"] = "h2";
                    }
                }
                else {
                    outText((std::string("<h3 class=\"title") + classExtras + "\">").c_str(), buf, u);
                    tag.attributes()["pushed"] = "h3";
                }
                u.titleStack.push(tag.toString());
            }
            else if (tag.isEndTag()) {
                if (!u.titleStack.empty()) {
                    XMLTag tag2(u.titleStack.top().c_str());
                    u.titleStack.pop();
                    std::string pushed = tag2.attribute("pushed");
                    if (!pushed.empty()) {
                        outText((std::string("</") + pushed + ">\n\n").c_str(), buf, u);
                    }
                    else {
                        outText( "</h3>\n\n", buf, u);
                    }
                    ++u.consecutiveNewlines;
                    u.supressAdjacentWhitespace = true;
                }
            }
        }

        // <list>
        else if (tag.name() == "list") {
            if((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("<ul>\n", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</ul>\n", buf, u);
                ++u.consecutiveNewlines;
                u.supressAdjacentWhitespace = true;
            }
        }

        // <item>
        else if (tag.name() == "item") {
            if((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("\t<li>", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</li>\n", buf, u);
                ++u.consecutiveNewlines;
                u.supressAdjacentWhitespace = true;
            }
        }
        // <catchWord>, <rdg> and <inscription> tags (italicize):
        else if ((tag.name() == "rdg")
                 || (tag.name() == "catchWord")
                 || (tag.name() == "inscription"))
        {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("<span class=\"", buf, u);
                outText(tag.name().c_str(), buf, u);
                outText("\">", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</span>", buf, u);
            }
        } else if (tag.name() == "seg") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText("<span class=\"", buf, u);
                outText(tag.attribute("type").c_str(), buf, u);
                outText("\">", buf, u);
            } else if (tag.isEndTag()) {
                outText("</span>", buf, u);
            }
        }

        // divineName
        else if (tag.name() == "divineName") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                u.suspendTextPassThru = (++u.suspendLevel);
            }
            else if (tag.isEndTag()) {
                std::string lastText = u.lastSuspendSegment.c_str();
                u.suspendTextPassThru = (--u.suspendLevel);
                if (!lastText.empty()) {
                    scratch = formatted("<span class=\"divineName\">%s</span>", lastText);
                    outText(scratch.c_str(), buf, u);
                }
            }
        }

        // <hi> text highlighting
        else if (tag.name() == "hi") {
            std::string type = tag.attribute("type");

            // handle tei rend attribute if type doesn't exist
            if (!type.length()) type = tag.attribute("rend");

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                if (type == "bold" || type == "b" || type == "x-b") {
                    outText("<b>", buf, u);
                }

                // there is no officially supported OSIS overline attribute,
                // thus either TEI overline or OSIS x-overline would be best,
                // but we have used "ol" in the past, as well.  Once a valid
                // OSIS overline attribute is made available, these should all
                // eventually be deprecated and never documented that they are supported.
                else if (type == "ol" || type == "overline" || type == "x-overline") {
                    outText("<span class=\"overline\">", buf, u);
                }

                else if (type == "super") {
                    outText("<sup>", buf, u);
                }
                else if (type == "sub") {
                    outText("<sub>", buf, u);
                }
                else if (type == "i" || type == "italic") {
                    outText("<i>", buf, u);
                } else {	// all other types
                    outText((std::string("<span class=\"") + type + "\">").c_str(), buf, u);
                }
                u.hiStack.push(tag.toString());
            }
            else if (tag.isEndTag()) {
                std::string type2;
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
                else if (type2 == "i" || type2 == "italic") {
                    outText("</i>", buf, u);
                } else {
                    outText("</span>", buf, u);
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

                outText("<span class=\"transChange", buf, u);
                if (type.length()) {
                    outText(" transChange-", buf, u);
                    outText(type.c_str(), buf, u);
                }
                outText("\">", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</span>", buf, u);
            }
            else {	// empty transChange marker?
            }
        }

        // image
        else if (tag.name() == "figure") {
            auto src(tag.attribute("src"));
            if (!src.empty()) {		// assert we have a src attribute
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
        }

        // ok to leave these in
        else if (tag.name() == "div") {
            auto const type(tag.attribute("type"));
            if ((type == "bookGroup")
                || (type == "book")
                || (type == "section")
                || (type == "majorSection"))
            {
                // Skip these types of divs
            } else if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                auto const type2(tag.attribute("type"));
                outText("<div class=\"", buf, u);
                outText(type2.c_str(), buf, u);
                outText("\">", buf, u);
            } else if (tag.isEndTag()) {
                outText("</div>", buf, u);
            } else if (type != "colophon") {
                outText(tag.toString().c_str(), buf, u);
            }

        }
        else if (tag.name() == "span") {
            outText(tag.toString().c_str(), buf, u);
        }
        else if (tag.name() == "abbr") {
            if (!tag.isEndTag()) {
                std::string title = tag.attribute("expansion");
                outText("<abbr title=\"", buf, u);
                outText(title.c_str(), buf, u);
                outText("\">", buf, u);
            }
            else if (tag.isEndTag()) {
                outText("</abbr>", buf, u);
            }

        }
        else if (tag.name() == "br") {
            outText(tag.toString().c_str(), buf, u);
        }
        else if (tag.name() == "table") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText( "<table><tbody>\n", buf, u);
            }
            else if (tag.isEndTag()) {
                outText( "</tbody></table>\n", buf, u);
                ++u.consecutiveNewlines;
                u.supressAdjacentWhitespace = true;
            }

        }
        else if (tag.name() == "row") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText( "\t<tr>", buf, u);
            }
            else if (tag.isEndTag()) {
                outText( "</tr>\n", buf, u);
            }

        }
        else if (tag.name() == "cell") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                outText( "<td>", buf, u);
            }
            else if (tag.isEndTag()) {
                outText( "</td>", buf, u);
            }
        }
        else {
            if (!u.supressAdjacentWhitespace)
                u.consecutiveNewlines = 0;
            return false;  // we still didn't handle token
        }
    }
    if (!u.supressAdjacentWhitespace)
        u.consecutiveNewlines = 0;
    return true;
}


} /* namespace swordxx */
