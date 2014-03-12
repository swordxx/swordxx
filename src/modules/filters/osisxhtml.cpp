/******************************************************************************
 *
 *  osisxhtml.cpp -	Render filter for classed XHTML of an OSIS module
 *
 * $Id$
 *
 * Copyright 2011-2014 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

#include <stdlib.h>
#include <ctype.h>
#include <osisxhtml.h>
#include <utilxml.h>
#include <utilstr.h>
#include <versekey.h>
#include <swmodule.h>
#include <url.h>
#include <stringmgr.h>
#include <stack>

SWORD_NAMESPACE_START

const char *OSISXHTML::getHeader() const {
	const static char *header = "\
		.divineName { font-variant: small-caps; }\n\
		.wordsOfJesus { color: red; }\n\
		.transChangeSupplied { font-style: italic; }\n\
		.small, .sub, .sup { font-size: .83em }\n\
		.sub             { vertical-align: sub }\n\
		.sup             { vertical-align: super }\n\
		.indent1         { margin-left: 10px }\n\
		.indent2         { margin-left: 20px }\n\
		.indent3         { margin-left: 30px }\n\
		.indent4         { margin-left: 40px }\n\
	";
	return header;
}


namespace {

// though this might be slightly slower, possibly causing an extra bool check, this is a renderFilter
// so speed isn't the absolute highest priority, and this is a very minor possible hit
static inline void outText(const char *t, SWBuf &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; else u->lastSuspendSegment += t; }
static inline void outText(char t, SWBuf &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; else u->lastSuspendSegment += t; }

void processLemma(bool suspendTextPassThru, XMLTag &tag, SWBuf &buf) {
	const char *attrib;
	const char *val;
	if ((attrib = tag.getAttribute("lemma"))) {
		int count = tag.getAttributePartCount("lemma", ' ');
		int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
		do {
			attrib = tag.getAttribute("lemma", i, ' ');
			if (i < 0) i = 0;	// to handle our -1 condition
			val = strchr(attrib, ':');
			val = (val) ? (val + 1) : attrib;
			SWBuf gh;
			if(*val == 'G')
				gh = "Greek";
			if(*val == 'H')
				gh = "Hebrew";
			const char *val2 = val;
			if ((strchr("GH", *val)) && (isdigit(val[1])))
				val2++;
			//if ((!strcmp(val2, "3588")) && (lastText.length() < 1))
			//	show = false;
			//else {
				if (!suspendTextPassThru) {
					buf.appendFormatted("<small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=%s&value=%s\" class=\"strongs\">%s</a>&gt;</em></small>",
							(gh.length()) ? gh.c_str() : "", 
							URL::encode(val2).c_str(),
							val2);
				}
			//}
			
		} while (++i < count);
	}
}



void processMorph(bool suspendTextPassThru, XMLTag &tag, SWBuf &buf) {
	const char * attrib;
	const char *val;
	if ((attrib = tag.getAttribute("morph"))) { // && (show)) {
		SWBuf savelemma = tag.getAttribute("savlm");
		//if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
		//	show = false;
		//if (show) {
			int count = tag.getAttributePartCount("morph", ' ');
			int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
			do {
				attrib = tag.getAttribute("morph", i, ' ');
				if (i < 0) i = 0;	// to handle our -1 condition
				val = strchr(attrib, ':');
				val = (val) ? (val + 1) : attrib;
				const char *val2 = val;
				if ((*val == 'T') && (strchr("GH", val[1])) && (isdigit(val[2])))
					val2+=2;
				if (!suspendTextPassThru) {
					buf.appendFormatted("<small><em class=\"morph\">(<a href=\"passagestudy.jsp?action=showMorph&type=%s&value=%s\" class=\"morph\">%s</a>)</em></small>",
							URL::encode(tag.getAttribute("morph")).c_str(),
							URL::encode(val).c_str(), 
							val2);
				}
			} while (++i < count);
		//}
	}
}


}	// end anonymous namespace

BasicFilterUserData *OSISXHTML::createUserData(const SWModule *module, const SWKey *key) {
	return new MyUserData(module, key);
}


OSISXHTML::OSISXHTML() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);
	setPassThruNumericEscapeString(true);

	addAllowedEscapeString("quot");
	addAllowedEscapeString("apos");
	addAllowedEscapeString("amp");
	addAllowedEscapeString("lt");
	addAllowedEscapeString("gt");

	setTokenCaseSensitive(true);
	
	//	addTokenSubstitute("lg",  "<br />");
	//	addTokenSubstitute("/lg", "<br />");

	morphFirst = false;
	renderNoteNumbers = false;
}

class OSISXHTML::TagStack : public std::stack<SWBuf> {
};

OSISXHTML::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key), quoteStack(new TagStack()), hiStack(new TagStack()), titleStack(new TagStack()), lineStack(new TagStack()) {
	inXRefNote    = false;
	suspendLevel = 0;
	wordsOfChristStart = "<span class=\"wordsOfJesus\"> ";
	wordsOfChristEnd   = "</span> ";
	if (module) {
		osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));
		version = module->getName();
		BiblicalText = (!strcmp(module->getType(), "Biblical Texts"));
	}
	else {
		osisQToTick = true;	// default
		version = "";
	}
	consecutiveNewlines = 0;
}

OSISXHTML::MyUserData::~MyUserData() {
	delete quoteStack;
	delete hiStack;
	delete titleStack;
	delete lineStack;
}

void OSISXHTML::MyUserData::outputNewline(SWBuf &buf) {
	if (++consecutiveNewlines <= 2) {
		outText("<br />\n", buf, this);
		supressAdjacentWhitespace = true;
	}
}
bool OSISXHTML::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
	MyUserData *u = (MyUserData *)userData;
	SWBuf scratch;
	bool sub = (u->suspendTextPassThru) ? substituteToken(scratch, token) : substituteToken(buf, token);
	if (!sub) {
  // manually process if it wasn't a simple substitution
		XMLTag tag(token);
		
		// <w> tag
		if (!strcmp(tag.getName(), "w")) {
 
			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
				u->w = token;
			}

			// end or empty <w> tag
			else {
				bool endTag = tag.isEndTag();
				SWBuf lastText;
				//bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

				if (endTag) {
					tag = u->w.c_str();
					lastText = u->lastTextNode.c_str();
				}
				else lastText = "stuff";

				const char *attrib;
				const char *val;
				if ((attrib = tag.getAttribute("xlit"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText(" ", buf, u);
					outText(val, buf, u);
				}
				if ((attrib = tag.getAttribute("gloss"))) {
					// I'm sure this is not the cleanest way to do it, but it gets the job done
					// for rendering ruby chars properly ^_^
					buf -= lastText.length();
					
					outText("<ruby><rb>", buf, u);
					outText(lastText, buf, u);
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText("</rb><rp>(</rp><rt>", buf, u);
					outText(val, buf, u);
					outText("</rt><rp>)</rp></ruby>", buf, u);
				}
				if (!morphFirst) {
					processLemma(u->suspendTextPassThru, tag, buf);
					processMorph(u->suspendTextPassThru, tag, buf);
				}
				else {
					processMorph(u->suspendTextPassThru, tag, buf);
					processLemma(u->suspendTextPassThru, tag, buf);
				}
				if ((attrib = tag.getAttribute("POS"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText(" ", buf, u);
					outText(val, buf, u);
				}

				/*if (endTag)
					buf += "}";*/
			}
		}

		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				SWBuf type = tag.getAttribute("type");
				bool strongsMarkup = (type == "x-strongsMarkup" || type == "strongsMarkup");	// the latter is deprecated
				if (strongsMarkup) {
					tag.setEmpty(false);	// handle bug in KJV2003 module where some note open tags were <note ... />
				}

				if (!tag.isEmpty()) {

					if (!strongsMarkup) {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
						SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
						SWBuf noteName = tag.getAttribute("n");
						VerseKey *vkey = NULL;
						char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');

						u->inXRefNote = true; // Why this change? Ben Morgan: Any note can have references in, so we need to set this to true for all notes
//						u->inXRefNote = (ch == 'x');

						// see if we have a VerseKey * or descendant
						SWTRY {
							vkey = SWDYNAMIC_CAST(VerseKey, u->key);
						}
						SWCATCH ( ... ) {	}
						if (vkey) {
							//printf("URL = %s\n",URL::encode(vkey->getText()).c_str());
							buf.appendFormatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup class=\"%c\">*%c%s</sup></small></a>",
								ch, 
								URL::encode(footnoteNumber.c_str()).c_str(), 
								URL::encode(u->version.c_str()).c_str(), 
								URL::encode(vkey->getText()).c_str(), 
								ch,
								ch, 
								(renderNoteNumbers ? URL::encode(noteName.c_str()).c_str() : ""));
						}
						else {
							buf.appendFormatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup class=\"%c\">*%c%s</sup></small></a>",
								ch, 
								URL::encode(footnoteNumber.c_str()).c_str(), 
								URL::encode(u->version.c_str()).c_str(), 
								URL::encode(u->key->getText()).c_str(),  
								ch,
								ch, 
								(renderNoteNumbers ? URL::encode(noteName.c_str()).c_str() : ""));
						}
					}
				}
				u->suspendTextPassThru = (++u->suspendLevel);
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = (--u->suspendLevel);
				u->inXRefNote = false;
				u->lastSuspendSegment = ""; // fix/work-around for nasb devineName in note bug
			}
		}

		// <p> paragraph and <lg> linegroup tags
		else if (!strcmp(tag.getName(), "p") || !strcmp(tag.getName(), "lg")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				u->outputNewline(buf);
			}
			else if (tag.isEndTag()) {	// end tag
				u->outputNewline(buf);
			}
			else {					// empty paragraph break marker
				u->outputNewline(buf);
			}
		}

		// Milestoned paragraphs, created by osis2mod
		// <div type="paragraph" sID.../>
		// <div type="paragraph" eID.../>
		else if (tag.isEmpty() && !strcmp(tag.getName(), "div") && tag.getAttribute("type") && (!strcmp(tag.getAttribute("type"), "x-p") || !strcmp(tag.getAttribute("type"), "paragraph"))) {
			// <div type="paragraph"  sID... />
			if (tag.getAttribute("sID")) {	// non-empty start tag
				u->outputNewline(buf);
			}
			// <div type="paragraph"  eID... />
			else if (tag.getAttribute("eID")) {
				u->outputNewline(buf);
			}
		}

		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {	
			if (!u->inXRefNote) {	// only show these if we're not in an xref note				
				if (!tag.isEndTag()) {
					SWBuf target;
					SWBuf work;
					SWBuf ref;
					bool is_scripRef = false;

					target = tag.getAttribute("osisRef");
					const char* the_ref = strchr(target, ':');
					
					if(!the_ref) {
						// No work
						ref = target;
						is_scripRef = true;
					}
					else {
						// Compensate for starting :
						ref = the_ref + 1;

						int size = target.size() - ref.size() - 1;
						work.setSize(size);
						strncpy(work.getRawData(), target, size);

						// For Bible:Gen.3.15 or Bible.vulgate:Gen.3.15
						if(!strncmp(work, "Bible", 5))
							is_scripRef = true;
					}

					if(is_scripRef)
					{
						buf.appendFormatted("<a href=\"passagestudy.jsp?action=showRef&type=scripRef&value=%s&module=\">",
							URL::encode(ref.c_str()).c_str()
//							(work.size()) ? URL::encode(work.c_str()).c_str() : "")
							);
					}
					else
					{
						// Dictionary link, or something
						buf.appendFormatted("<a href=\"sword://%s/%s\">",
							URL::encode(work.c_str()).c_str(),
							URL::encode(ref.c_str()).c_str()
							);
					}
				}
				else {
					outText("</a>", buf, u);
				}
			}
		}

		// <l> poetry, etc
		else if (!strcmp(tag.getName(), "l")) {
			// start line marker
			if (tag.getAttribute("sID") || (!tag.isEndTag() && !tag.isEmpty())) {
				// nested lines plus if the line itself has an x-indent type attribute value
				outText(SWBuf("<span class=\"line indent").appendFormatted("%d\">", u->lineStack->size() + (SWBuf("x-indent") == tag.getAttribute("type")?1:0)).c_str(), buf, u);
				u->lineStack->push(tag.toString());
			}
			// end line marker
			else if (tag.getAttribute("eID") || tag.isEndTag()) {
				outText("</span>", buf, u);
				u->outputNewline(buf);
				if (u->lineStack->size()) u->lineStack->pop();
			}
			// <l/> without eID or sID
			// Note: this is improper osis. This should be <lb/>
			else if (tag.isEmpty() && !tag.getAttribute("sID")) {
				u->outputNewline(buf);
			}
		}

		// <lb.../>
		else if (!strcmp(tag.getName(), "lb") && (!tag.getAttribute("type") || strcmp(tag.getAttribute("type"), "x-optional"))) {
				u->outputNewline(buf);
		}
		// <milestone type="line"/>
		// <milestone type="x-p"/>
		// <milestone type="cQuote" marker="x"/>
		else if ((!strcmp(tag.getName(), "milestone")) && (tag.getAttribute("type"))) {
			if (!strcmp(tag.getAttribute("type"), "line")) {
				u->outputNewline(buf);
				if (tag.getAttribute("subType") && !strcmp(tag.getAttribute("subType"), "x-PM")) {
					u->outputNewline(buf);
				}
			}
			else if (!strcmp(tag.getAttribute("type"),"x-p"))  {
				if (tag.getAttribute("marker"))
					outText(tag.getAttribute("marker"), buf, u);
				else outText("<!p>", buf, u);
			}
			else if (!strcmp(tag.getAttribute("type"), "cQuote")) {
				const char *tmp = tag.getAttribute("marker");
				bool hasMark    = tmp;
				SWBuf mark      = tmp;
				tmp             = tag.getAttribute("level");
				int level       = (tmp) ? atoi(tmp) : 1;

				// first check to see if we've been given an explicit mark
				if (hasMark)
					outText(mark, buf, u);
				// finally, alternate " and ', if config says we should supply a mark
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);
			}
		}

		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, u->key);
				if (vkey && !vkey->getVerse()) {
					if (!vkey->getChapter()) {
						if (!vkey->getBook()) {
							if (!vkey->getTestament()) {
								buf += "<h1 class=\"moduleHeader\">";
								tag.setAttribute("pushed", "h1");
							}
							else {
								buf += "<h1 class=\"testamentHeader\">";
								tag.setAttribute("pushed", "h1");
							}
						}
						else {
							buf += "<h1 class=\"bookHeader\">";
							tag.setAttribute("pushed", "h1");
						}
					}
					else {
						buf += "<h2 class=\"chapterHeader\">";
						tag.setAttribute("pushed", "h2");
					}
				}
				else {
					buf += "<h3>";
					tag.setAttribute("pushed", "h3");
				}
				u->titleStack->push(tag.toString());
			}
			else if (tag.isEndTag()) {
				if (!u->titleStack->empty()) {
					XMLTag tag(u->titleStack->top());
					if (u->titleStack->size()) u->titleStack->pop();
					SWBuf pushed = tag.getAttribute("pushed");
					if (pushed.size()) {
						buf += (SWBuf)"</" + pushed + ">\n\n";
					}
					else {
						buf += "</h3>\n\n";
					}
					++u->consecutiveNewlines;
					u->supressAdjacentWhitespace = true;
				}
			}
		}
		
		// <list>
		else if (!strcmp(tag.getName(), "list")) {
			if((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("<ul>\n", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("</ul>\n", buf, u);
				++u->consecutiveNewlines;
				u->supressAdjacentWhitespace = true;
			}
		}

		// <item>
		else if (!strcmp(tag.getName(), "item")) {
			if((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("\t<li>", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("</li>\n", buf, u);
				++u->consecutiveNewlines;
				u->supressAdjacentWhitespace = true;
			}
		}
		// <catchWord> & <rdg> tags (italicize)
		else if (!strcmp(tag.getName(), "rdg") || !strcmp(tag.getName(), "catchWord")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("<i>", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("</i>", buf, u);
			}
		}

		// divineName  
		else if (!strcmp(tag.getName(), "divineName")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				u->suspendTextPassThru = (++u->suspendLevel);
			}
			else if (tag.isEndTag()) {
				SWBuf lastText = u->lastSuspendSegment.c_str();
				u->suspendTextPassThru = (--u->suspendLevel);
				if (lastText.size()) {
					scratch.setFormatted("<span class=\"divineName\">%s</span>", lastText.c_str());
					outText(scratch.c_str(), buf, u);
				}               
			} 
		}

		// <hi> text highlighting
		else if (!strcmp(tag.getName(), "hi")) {
			SWBuf type = tag.getAttribute("type");

			// handle tei rend attribute if type doesn't exist
			if (!type.length()) type = tag.getAttribute("rend");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "bold" || type == "b" || type == "x-b") {
					outText("<b>", buf, u);
				}

				// there is no officially supported OSIS overline attribute,
				// thus either TEI overline or OSIS x-overline would be best,
				// but we have used "ol" in the past, as well.  Once a valid
				// OSIS overline attribute is made available, these should all
				// eventually be deprecated and never documented that they are supported.
				else if (type == "ol"  || type == "overline" || type == "x-overline") {
					outText("<span style=\"text-decoration:overline\">", buf, u);
				}

				else if (type == "super") {
					outText("<sup>", buf, u);
				}
				else if (type == "sub") {
					outText("<sub>", buf, u);
				}
				else {	// all other types
					outText("<i>", buf, u);
				}
				u->hiStack->push(tag.toString());
			}
			else if (tag.isEndTag()) {
				SWBuf type = "";
				if (!u->hiStack->empty()) {
					XMLTag tag(u->hiStack->top());
					if (u->hiStack->size()) u->hiStack->pop();
					type = tag.getAttribute("type");
					if (!type.length()) type = tag.getAttribute("rend");
				}
				if (type == "bold" || type == "b" || type == "x-b") {
					outText("</b>", buf, u);
				}
				else if (type == "ol") {
					outText("</span>", buf, u);
				}
				else if (type == "sup") {
					outText("</sup>", buf, u);
				}
				else if (type == "sub") {
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
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type      = tag.getAttribute("type");
			SWBuf who       = tag.getAttribute("who");
			const char *tmp = tag.getAttribute("level");
			int level       = (tmp) ? atoi(tmp) : 1;
			tmp             = tag.getAttribute("marker");
			bool hasMark    = tmp;
			SWBuf mark      = tmp;

			// open <q> or <q sID... />
			if ((!tag.isEmpty() && !tag.isEndTag()) || (tag.isEmpty() && tag.getAttribute("sID"))) {
				// if <q> then remember it for the </q>
				if (!tag.isEmpty()) {
					u->quoteStack->push(tag.toString());
				}

				// Do this first so quote marks are included as WoC
				if (who == "Jesus")
					outText(u->wordsOfChristStart, buf, u);

				// first check to see if we've been given an explicit mark
				if (hasMark)
					outText(mark, buf, u);
				//alternate " and '
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);
			}
			// close </q> or <q eID... />
			else if ((tag.isEndTag()) || (tag.isEmpty() && tag.getAttribute("eID"))) {
				// if it is </q> then pop the stack for the attributes
				if (tag.isEndTag() && !u->quoteStack->empty()) {
					XMLTag qTag(u->quoteStack->top());
					if (u->quoteStack->size()) u->quoteStack->pop();

					type    = qTag.getAttribute("type");
					who     = qTag.getAttribute("who");
					tmp     = qTag.getAttribute("level");
					level   = (tmp) ? atoi(tmp) : 1;
					tmp     = qTag.getAttribute("marker");
					hasMark = tmp;
					mark    = tmp;
				}

				// first check to see if we've been given an explicit mark
				if (hasMark)
					outText(mark, buf, u);
				// finally, alternate " and ', if config says we should supply a mark
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);

				// Do this last so quote marks are included as WoC
				if (who == "Jesus")
					outText(u->wordsOfChristEnd, buf, u);
			}
		}

		// <transChange>
		else if (!strcmp(tag.getName(), "transChange")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				SWBuf type = tag.getAttribute("type");
				u->lastTransChange = type;

				// just do all transChange tags this way for now
				if ((type == "added") || (type == "supplied"))
					outText("<span class=\"transChangeSupplied\">", buf, u);
				else if (type == "tenseChange")
					buf += "*";
			}
			else if (tag.isEndTag()) {
				SWBuf type = u->lastTransChange;
				if ((type == "added") || (type == "supplied"))
					outText("</span>", buf, u);
			}
			else {	// empty transChange marker?
			}
		}

		// image
		else if (!strcmp(tag.getName(), "figure")) {
			const char *src = tag.getAttribute("src");
			if (src) {		// assert we have a src attribute 
				SWBuf filepath;
				if (userData->module) {
					filepath = userData->module->getConfigEntry("AbsoluteDataPath");
					if ((filepath.size()) && (filepath[filepath.size()-1] != '/') && (src[0] != '/'))
						filepath += '/';
				}
				filepath += src;

				// images become clickable, if the UI supports showImage.
				outText("<a href=\"passagestudy.jsp?action=showImage&value=", buf, u);
				outText(URL::encode(filepath.c_str()).c_str(), buf, u);
				outText("&module=", buf, u);
				outText(URL::encode(u->version.c_str()).c_str(), buf, u);
				outText("\">", buf, u);

				outText("<img src=\"file:", buf, u);
				outText(filepath, buf, u);
				outText("\" border=\"0\" />", buf, u);

				outText("</a>", buf, u);
			}
		}

		// ok to leave these in
		else if (!strcmp(tag.getName(), "div")) {
			SWBuf type = tag.getAttribute("type");
			if (type == "bookGroup") {
			}
			else if (type == "book") {
			}
			else if (type == "section") {
			}
			else if (type == "majorSection") {
			}
			else {
				buf += tag;
			}
		}
		else if (!strcmp(tag.getName(), "span")) {
			buf += tag;
		}
		else if (!strcmp(tag.getName(), "br")) {
			buf += tag;
		}
		else if (!strcmp(tag.getName(), "table")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<table><tbody>\n";
			}
			else if (tag.isEndTag()) {
				buf += "</tbody></table>\n";
				++u->consecutiveNewlines;
				u->supressAdjacentWhitespace = true;
			}
			
		}
		else if (!strcmp(tag.getName(), "row")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "\t<tr>";
			}
			else if (tag.isEndTag()) {
				buf += "</tr>\n";
			}
			
		}
		else if (!strcmp(tag.getName(), "cell")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<td>";
			}
			else if (tag.isEndTag()) {
				buf += "</td>";
			}
		}
		else {
			if (!u->supressAdjacentWhitespace) u->consecutiveNewlines = 0;
			return false;  // we still didn't handle token
		}
	}
	if (!u->supressAdjacentWhitespace) u->consecutiveNewlines = 0;
	return true;
}


SWORD_NAMESPACE_END
