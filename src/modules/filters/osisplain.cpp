/***************************************************************************
                     osisplain.cpp  -  OSIS to Plaintext filter
                             -------------------
    begin                : 2003-02-15
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
#include <osisplain.h>
#include <utilxml.h>
#include <ctype.h>

SWORD_NAMESPACE_START

OSISPlain::OSISPlain() {
	setTokenStart("<");
	setTokenEnd(">");
  
	setEscapeStart("&");
	setEscapeEnd(";");
  
	setEscapeStringCaseSensitive(true);
  
	addEscapeStringSubstitute("amp", "&");
	addEscapeStringSubstitute("apos", "'");
	addEscapeStringSubstitute("lt", "<");
	addEscapeStringSubstitute("gt", ">");
	addEscapeStringSubstitute("quot", "\"");
  
	setTokenCaseSensitive(true);  
}

bool OSISPlain::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		XMLTag tag(token);
		
		// <w> tag
		if (!strcmp(tag.getName(), "w")) {

			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
				userData["w"] = token;
			}

			// end or empty <w> tag
			else {
				bool endTag = tag.isEndTag();
				SWBuf lastText;
				bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

				if (endTag) {
					tag = userData["w"].c_str();
					lastText = userData["lastTextNode"].c_str();
				}
				else lastText = "stuff";
					
				const char *attrib;
				const char *val;
				if (attrib = tag.getAttribute("xlit")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" <%s>", val);
				}
				if (attrib = tag.getAttribute("gloss")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" <%s>", val);
				}
				if (attrib = tag.getAttribute("lemma")) {
					int count = tag.getAttributePartCount("lemma");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("lemma", i);
						if (i < 0) i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
						if ((strchr("GH", *val)) && (isdigit(val[1])))
							val++;
						if ((!strcmp(val, "3588")) && (lastText.length() < 1))
							show = false;
						else	buf.appendFormatted(" <%s>}", val);
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("morph")) && (show)) {
					int count = tag.getAttributePartCount("morph");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("morph", i);
						if (i < 0) i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
						if ((*val == 'T') && (strchr("GH", val[1])) && (isdigit(val[2])))
							val+=2;
						buf.appendFormatted(" (%s)", val);
					} while (++i < count);
				}
				if (attrib = tag.getAttribute("POS")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" <%s>", val);
				}
			}
		}

		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEmpty() && !tag.isEndTag()) {
				SWBuf type = tag.getAttribute("type");

				if (type != "strongsMarkup") {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
					buf += " (";
				}
				else	userData["suspendTextPassThru"] = "true";
			}
			if (tag.isEndTag()) {
				if (userData["suspendTextPassThru"] == "false")
					buf += ")";
				else	userData["suspendTextPassThru"] = "false";
			}
		}

		// <p> paragraph tag
		else if (!strcmp(tag.getName(), "p")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				buf += "\n";
			}
			else if (tag.isEndTag()) {	// end tag
				buf += "\n";
			}
			else {					// empty paragraph break marker
				buf += "\n\n";
			}
		}

		// <line> poetry, etc
		else if (!strcmp(tag.getName(), "line")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
			}
			else if (tag.isEndTag()) {
				buf += "\n";
			}
			else {	// empty line marker
				buf += "\n";
			}
		}

		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
			}
			else if (tag.isEndTag()) {
				buf += "\n";
			}
			else {	// empty title marker
				// what to do?  is this even valid?
				buf += "\n";
			}
		}

		// <q> quote
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf who = tag.getAttribute("who");
			const char *lev = tag.getAttribute("level");
			int level = (lev) ? atoi(lev) : 1;
			
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {

				//alternate " and '
				buf += (level % 2) ? '\'' : '\"';
			}
			else if (tag.isEndTag()) {
				//alternate " and '
				buf += (level % 2) ? '\'' : '\"';
			}
			else {	// empty quote marker
				//alternate " and '
				buf += (level % 2) ? '\'' : '\"';
			}
		}

		else {
			return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END
