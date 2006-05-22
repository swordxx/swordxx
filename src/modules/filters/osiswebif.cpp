/***************************************************************************
                          OSISWEBIF.cpp  -   OSIS to HTML filter with hrefs 
			        for strongs and morph tags
                             -------------------
    begin                    : 2003-10-23
    copyright            : 2003 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <osiswebif.h>
#include <utilxml.h>
#include <url.h>
#include <versekey.h>
#include <swmodule.h>
#include <ctype.h>


SWORD_NAMESPACE_START


OSISWEBIF::OSISWEBIF() : baseURL(""), passageStudyURL(baseURL + "passagestudy.jsp"), javascript(false) {
}

bool OSISWEBIF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		MyUserData *u = (MyUserData *)userData;
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
				bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

				if (endTag) {
					tag = u->w.c_str();
					lastText = u->lastTextNode.c_str();
				}
				else lastText = "stuff";

				const char *attrib;
				const char *val;
				if (attrib = tag.getAttribute("xlit")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
//					buf.appendFormatted(" %s", val);
				}
				if (attrib = tag.getAttribute("gloss")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" %s", val);
				}
				if (attrib = tag.getAttribute("lemma")) {
					int count = tag.getAttributePartCount("lemma", ' ');
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("lemma", i, ' ');
						if (i < 0) i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
						const char *val2 = val;
						if ((strchr("GH", *val)) && (isdigit(val[1])))
							val2++;
						if ((!strcmp(val2, "3588")) && (lastText.length() < 1))
							show = false;
						else	buf.appendFormatted(" <small><em>&lt;<a href=\"%s?showStrong=%s#cv\">%s</a>&gt;</em></small> ", passageStudyURL.c_str(), URL::encode(val2).c_str(), val2);
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("morph")) && (show)) {
					SWBuf savelemma = tag.getAttribute("savlm");
					if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
						show = false;
					if (show) {
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
							buf.appendFormatted(" <small><em>(<a href=\"%s?showMorph=%s#cv\">%s</a>)</em></small> ", passageStudyURL.c_str(), URL::encode(val2).c_str(), val2);
						} while (++i < count);
					}
				}
				if (attrib = tag.getAttribute("POS")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" %s", val);
				}

				/*if (endTag)
					buf += "}";*/
			}
		}

		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				if (!tag.isEmpty()) {
					SWBuf type = tag.getAttribute("type");

					if (type != "strongsMarkup") {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
						SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
						SWBuf modName = (u->module) ? u->module->Name() : "";
						VerseKey *vkey;
						// see if we have a VerseKey * or descendant
						SWTRY {
							vkey = SWDYNAMIC_CAST(VerseKey, u->key);
						}
						SWCATCH ( ... ) {	}
						if (vkey) {
							char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
//							buf.appendFormatted("<a href=\"noteID=%s.%c.%s\"><small><sup>*%c</sup></small></a> ", vkey->getText(), ch, footnoteNumber.c_str(), ch);
							buf.appendFormatted("<span class=\"fn\" onclick=\"f(\'%s\',\'%s\',\'%s\');\" >%c</span>", modName.c_str(), u->key->getText(), footnoteNumber.c_str(), ch);
						}
					}
					u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = false;

			}
		}

		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<h3>";
			}
			else if (tag.isEndTag()) {
				buf += "</h3>";
			}
		}

		// <catchWord> & <rdg> tags (italicize)
		else if (!strcmp(tag.getName(), "rdg") || !strcmp(tag.getName(), "catchWord")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (!u->suspendTextPassThru)
					buf += "<i>";
			}
			else if (tag.isEndTag()) {
				if (!u->suspendTextPassThru)
					buf += "</i>";
			}
		}

                // <hi> text highlighting
		else if (!strcmp(tag.getName(), "hi")) {
			SWBuf type = tag.getAttribute("type");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "b" || type == "x-b") {
					if (!u->suspendTextPassThru)
						buf += "<b>";
					u->inBold = true;
				}
				else {	// all other types
					if (!u->suspendTextPassThru)
						buf += "<i>";
					u->inBold = false;
				}
			}
			else if (tag.isEndTag()) {
				if(u->inBold) {
					if (!u->suspendTextPassThru)
						buf += "</b>";
				}
				else {
					if (!u->suspendTextPassThru)
						 buf += "</i>";
				}
			}
		}

		// <q> quote
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf who = tag.getAttribute("who");
			const char *lev = tag.getAttribute("level");
			int level = (lev) ? atoi(lev) : 1;
			
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				/*buf += "{";*/

				//alternate " and '
				if (u->osisQToTick)
					buf += (level % 2) ? '\"' : '\'';
				
				if (who == "Jesus") {
					buf += "<span class=\"wordsOfJesus\"> ";
				}
			}
			else if (tag.isEndTag()) {
				//alternate " and '
				if (u->osisQToTick)
					buf += (level % 2) ? '\"' : '\'';
				buf += "</span>";
			}
			else {	// empty quote marker
				//alternate " and '
				if (u->osisQToTick)
					buf += (level % 2) ? '\"' : '\'';
			}
		}

		// <transChange>
		else if (!strcmp(tag.getName(), "transChange")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				SWBuf type = tag.getAttribute("type");
				u->lastTransChange = type;

				// just do all transChange tags this way for now
				if ((type == "added") || (type == "supplied"))
					buf += "<i>";
				else if (type == "tenseChange")
					buf += "*";
			}
			else if (tag.isEndTag()) {
				SWBuf type = u->lastTransChange;
				if ((type == "added") || (type == "supplied"))
					buf += "</i>";
			}
			else {	// empty transChange marker?
			}
		}
		// ok to leave these in
		else if (!strcmp(tag.getName(), "div")) {
			buf += tag;
		}
		else if (!strcmp(tag.getName(), "span")) {
			buf += tag;
		}
		else if (!strcmp(tag.getName(), "br")) {
			buf += tag;
		}
		else {
			return OSISHTMLHREF::handleToken(buf, token, userData);
		}
	}
	return true;
}


SWORD_NAMESPACE_END

