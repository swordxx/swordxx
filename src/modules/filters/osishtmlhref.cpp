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

SWORD_NAMESPACE_START

OSISHTMLHref::OSISHTMLHref()
{
  setTokenStart("<");
  setTokenEnd(">");
	
  setTokenCaseSensitive(true);  
}

bool OSISHTMLHref::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		XMLTag tag(token);
		//printf("token = %s\n",token);
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
					buf.appendFormatted(" %s", val);
				}
				if (attrib = tag.getAttribute("gloss")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
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
						const char *val2 = val;
						if ((strchr("GH", *val)) && (isdigit(val[1])))
							val2++;
						if ((!strcmp(val2, "3588")) && (lastText.length() < 1))
							show = false;
						else	buf.appendFormatted(" <small><em>&lt;<a href=\"type=Strongs value=%s\">%s</a>&gt;</em></small> ", val, val2);
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
						const char *val2 = val;
						if ((*val == 'T') && (strchr("GH", val[1])) && (isdigit(val[2]))) 
							val2+=2;
						buf.appendFormatted(" <small><em>(<a href=\"type=morph class=%s value=%s\">%s</a>)</em></small> ", tag.getAttribute("morph"), val, val2);
					} while (++i < count);
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
			if (!tag.isEmpty() && !tag.isEndTag()) {		
				SWBuf footnoteNum = userData["fn"];
				SWBuf type = tag.getAttribute("type");
				
				if (type != "strongsMarkup") {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
					int footnoteNumber = (footnoteNum.length()) ? atoi(footnoteNum.c_str()) : 1;
					VerseKey *vkey;
					// see if we have a VerseKey * or descendant
					try {
						vkey = SWDYNAMIC_CAST(VerseKey, this->key);
					}
					catch ( ... ) {	}
					if (vkey) {
						char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
						    buf.appendFormatted("<a href=\"noteID=%s.%c.%i\"><small><sup>*%c</sup></small></a> ", vkey->getText(), ch, footnoteNumber, ch);
						SWBuf tmp;
						tmp.appendFormatted("%i", ++footnoteNumber);
						userData["fn"] = tmp.c_str();
					}
				}
				userData["suspendTextPassThru"] = "true";
			}	   
			if (tag.isEndTag()) {	
				userData["suspendTextPassThru"] = "false";
			}
		}
		// <p> paragraph tag
		else if (!strcmp(tag.getName(), "p")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				buf += "<!P><br />";
			}
			else if (tag.isEndTag()) {	// end tag
				buf += "<!/P><br />";
			}
			else {					// empty paragraph break marker
				buf += "<!P><br />";
			}
		}
		
		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<a href=\"\">";
			}
			else if (tag.isEndTag()) {
				buf += "</a>";
			}
			else {	// empty reference marker
				// -- what should we do?  nothing for now.
			}
		}

		// <line> poetry, etc
		else if (!strcmp(tag.getName(), "line")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<>";
			}
			else if (tag.isEndTag()) {
				buf += "<br />";
			}
			else {	// empty line marker
				buf += "<br />";
			}
		}

		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<b>";
			}
			else if (tag.isEndTag()) {
				buf += "</b><br />";
			}
			else {	// empty title marker
				// what to do?  is this even valid?
				buf += "<br />";
			}
		}

		// <hi> hi?  hi contrast?
		else if (!strcmp(tag.getName(), "hi")) {
			SWBuf type = tag.getAttribute("type");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "b") {
					buf += "<b> ";
					userData["inBold"] = "true";
				}
				else	// all other types
					buf += "<i> ";
			}
			else if (tag.isEndTag()) {
				if(userData["inBold"] == "true") {
					buf += "</b>";
					userData["inBold"] = "false";
				}
				else
				      buf += "</i>";
			}
			else {	// empty hi marker
				// what to do?  is this even valid?
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
				if(lev)
					buf += (level % 2) ? '\'' : '\"';
				
				if (who == "Jesus") {
					buf += "<font color=\"red\"> ";
				}
			}
			else if (tag.isEndTag()) {
				//alternate " and '
				if(lev)
					buf += (level % 2) ? '\'' : '\"';
				//buf += "</font>";
			}
			else {	// empty quote marker
				//alternate " and '
				if(lev)
					buf += (level % 2) ? '\'' : '\"';
			}
		}

		// <transChange>
		else if (!strcmp(tag.getName(), "transChange")) {
			SWBuf type = tag.getAttribute("type");
			
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {

// just do all transChange tags this way for now
//				if (type == "supplied")
					buf += "<i>";
			}
			else if (tag.isEndTag()) {
				buf += "</i>";
			}
			else {	// empty transChange marker?
			}
		}
		
		else {
		      return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END
