/***************************************************************************
                     osishtmlhref.cpp  -  OSIS to HTML with hrefs filter
                             -------------------
    begin                : 2003-06-24
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
#include <osishtmlhref.h>
#include <utilxml.h>
#include <versekey.h>
#include <swmodule.h>
#include <url.h>

SWORD_NAMESPACE_START


OSISHTMLHREF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	if (module) {
		osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));
		version = module->Name();
	}
	else {
		osisQToTick = true;	// default
		version = "";
	}
}


OSISHTMLHREF::OSISHTMLHREF() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

	addEscapeStringSubstitute("amp",  "&");
	addEscapeStringSubstitute("apos", "'");
	addEscapeStringSubstitute("lt",   "<");
	addEscapeStringSubstitute("gt",   ">");
	addEscapeStringSubstitute("quot", "\"");
	
	setTokenCaseSensitive(true);
	
	addTokenSubstitute("lg",  "<br />");
	addTokenSubstitute("/lg", "<br />");
}


bool OSISHTMLHREF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
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
					if (!u->suspendTextPassThru)
						buf.appendFormatted(" %s", val);
				}
				if (attrib = tag.getAttribute("gloss")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					if (!u->suspendTextPassThru)
						buf.appendFormatted(" %s", val);
				}
				if (attrib = tag.getAttribute("lemma")) {
					int count = tag.getAttributePartCount("lemma");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("lemma", i);
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
						if ((!strcmp(val2, "3588")) && (lastText.length() < 1))
							show = false;
						else {
							if (!u->suspendTextPassThru)
								buf.appendFormatted(" <small><em>&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=%s&value=%s\">%s</a>&gt;</em></small> ", 
										(gh.length()) ? gh.c_str() : "", 
										URL::encode(val2).c_str(), 
										val2);
						}
						
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("morph")) && (show)) {
					SWBuf savelemma = tag.getAttribute("savlm");
					if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
						show = false;
					if (show) {
						int count = tag.getAttributePartCount("morph");
						int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
						do {
							attrib = tag.getAttribute("morph", i);
							if (i < 0) i = 0;	// to handle our -1 condition
							val = strchr(attrib, ':');
							val = (val) ? (val + 1) : attrib;
							const char *val2 = val;
							if ((*val == 'T') && (strchr("GH", val[1])) && (isdigit(val[2])))
								val2+=2;
							if (!u->suspendTextPassThru)
								buf.appendFormatted(" <small><em>(<a href=\"passagestudy.jsp?action=showMorph&type=%s&value=%s\">%s</a>)</em></small> ", 
										URL::encode(tag.getAttribute("morph")).c_str(),
										URL::encode(val).c_str(), 
										val2);
						} while (++i < count);
					}
				}
				if (attrib = tag.getAttribute("POS")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					if (!u->suspendTextPassThru)
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
						VerseKey *vkey;
						// see if we have a VerseKey * or descendant
						SWTRY {
							vkey = SWDYNAMIC_CAST(VerseKey, u->key);
						}
						SWCATCH ( ... ) {	}
						if (vkey) {
							//printf("URL = %s\n",URL::encode(vkey->getText()).c_str());
							char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
							buf.appendFormatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup>*%c</sup></small></a> ", 
								ch, 
								URL::encode(footnoteNumber.c_str()).c_str(), 
								URL::encode(u->version.c_str()).c_str(), 
								URL::encode(vkey->getText()).c_str(), 
								ch);
						}
					}
				}
				u->suspendTextPassThru = true;
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = false;
			}
		}

		// <p> paragraph tag
		else if (!strcmp(tag.getName(), "p")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				if (!u->suspendTextPassThru)
					buf += "<!P><br />";
			}
			else if (tag.isEndTag()) {	// end tag
				if (!u->suspendTextPassThru)
					buf += "<!/P><br />";
				userData->supressAdjacentWhitespace = true;
			}
			else {					// empty paragraph break marker
				if (!u->suspendTextPassThru)
					buf += "<!P><br />";
				userData->supressAdjacentWhitespace = true;
			}
		}

		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {			
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (!u->suspendTextPassThru)
					buf += "<a href=\"\">";
			}
			else if (tag.isEndTag()) {
				if (!u->suspendTextPassThru)
					buf += "</a>";
			}
		}

		// <l> poetry, etc
		else if (!strcmp(tag.getName(), "l")) {
			if (tag.isEmpty()) {
				if (!u->suspendTextPassThru)
					buf += "<br />";
			}
			else if (tag.isEndTag()) {
				if (!u->suspendTextPassThru)
					buf += "<br />";
			}
			else if (tag.getAttribute("sID")) {	// empty line marker
				if (!u->suspendTextPassThru)
					buf += "<br />";
			}
		}

		// <milestone type="line"/>
		else if ((!strcmp(tag.getName(), "milestone")) && (tag.getAttribute("type")) && (!strcmp(tag.getAttribute("type"), "line"))) {
			if (!u->suspendTextPassThru)
				buf += "<br />";
			userData->supressAdjacentWhitespace = true;
		}

		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (!u->suspendTextPassThru)
					buf += "<b>";
			}
			else if (tag.isEndTag()) {
				if (!u->suspendTextPassThru)
					buf += "</b><br />";
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
					u->inBold = false;
				}
				else
					if (!u->suspendTextPassThru)
						 buf += "</i>";
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
					if (!u->suspendTextPassThru)
						buf += (level % 2) ? '\"' : '\'';
				
				if (who == "Jesus") {
					if (!u->suspendTextPassThru)
						buf += "<font color=\"red\">";
				}
			}
			else if (tag.isEndTag()) {
				//alternate " and '
				if (u->osisQToTick)
					if (!u->suspendTextPassThru)
						buf += (level % 2) ? '\"' : '\'';
				//buf += "</font>";
			}
			else {	// empty quote marker
				//alternate " and '
				if (u->osisQToTick)
					if (!u->suspendTextPassThru)
						buf += (level % 2) ? '\"' : '\'';
			}
		}

		// <transChange>
		else if (!strcmp(tag.getName(), "transChange")) {
			SWBuf type = tag.getAttribute("type");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {

// just do all transChange tags this way for now
//				if (type == "supplied")
					if (!u->suspendTextPassThru)
						buf += "<i>";
			}
			else if (tag.isEndTag()) {
				if (!u->suspendTextPassThru)
					buf += "</i>";
			}
			else {	// empty transChange marker?
			}
		}

		// image
		else if (!strcmp(tag.getName(), "figure")) {
			const char *src = tag.getAttribute("src");
			if (!src)		// assert we have a src attribute
				return false;

			SWBuf filepath;
			if (userData->module) {
				filepath = userData->module->getConfigEntry("AbsoluteDataPath");
				if ((filepath.size()) && (filepath[filepath.size()-1] != '/') && (src[0] != '/'))
					filepath += '/';
			}
			filepath += src;

// we do this because BibleCS looks for this EXACT format for an image tag
				if (!u->suspendTextPassThru) {
					buf+="<image src=\"";
					buf+=filepath;
					buf+="\" />";
				}
		}

		else {
		      return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END
