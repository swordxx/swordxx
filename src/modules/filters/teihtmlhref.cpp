/***************************************************************************
					 teirtf.cpp  -  TEI to HTMLHREF filter
							 -------------------
	begin				: 2006-07-03
	copyright			: 2006 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/

#include <stdlib.h>
#include <ctype.h>
#include <teihtmlhref.h>
#include <utilxml.h>
#include <swmodule.h>
#include <url.h>


SWORD_NAMESPACE_START


TEIHTMLHREF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	BiblicalText = false;
	if (module) {
		version = module->Name();
		BiblicalText = (!strcmp(module->Type(), "Biblical Texts"));
	}
}


TEIHTMLHREF::TEIHTMLHREF() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

	addAllowedEscapeString("quot");
	addAllowedEscapeString("apos");
	addAllowedEscapeString("amp");
	addAllowedEscapeString("lt");
	addAllowedEscapeString("gt");
	
	setTokenCaseSensitive(true);
}

bool TEIHTMLHREF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		MyUserData *u = (MyUserData *)userData;
		XMLTag tag(token);

		if (!strcmp(tag.getName(), "p")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				buf += "<!P><br />";
			}
			else if (tag.isEndTag()) {	// end tag
				buf += "<!/P><br />";
				//userData->supressAdjacentWhitespace = true;
			}
			else {					// empty paragraph break marker
				buf += "<!P><br />";
				//userData->supressAdjacentWhitespace = true;
			}
		}
		
		// <hi>
		else if (!strcmp(tag.getName(), "hi")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				SWBuf rend = tag.getAttribute("rend");
				
				u->lastHi = rend;
				if (rend == "ital")
					buf += "<i>";
				else if (rend == "bold")
					buf += "<b>";
				else if (rend == "sup")
					buf += "<small><sup>";

			}
			else if (tag.isEndTag()) {
				SWBuf rend = u->lastHi;
				if (rend == "ital")
					buf += "</i>";
				else if (rend == "bold")
					buf += "</b>";
				else if (rend == "sup")
					buf += "</sup></small>";
			}
		}

		// <entryFree>
		else if (!strcmp(tag.getName(), "entryFree")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				SWBuf n = tag.getAttribute("n");				
				if (n != "") {
					buf += "<b>";
					buf += n;
					buf += "</b>";
				}
			}
		}

		// <sense>
		else if (!strcmp(tag.getName(), "sense")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				SWBuf n = tag.getAttribute("n");
				if (n != "") {
					buf += "<br /><b>";
					buf += n;
					buf += "</b>";
				}
			}
		}

		// <div>
		else if (!strcmp(tag.getName(), "div")) {

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<!P>";
			}
			else if (tag.isEndTag()) {
			}
		}

		// <pos>, <gen>, <case>, <gram>, <number>, <mood>, <pron>, <def>
		else if (!strcmp(tag.getName(), "pos") || 
				 !strcmp(tag.getName(), "gen") || 
				 !strcmp(tag.getName(), "case") || 
				 !strcmp(tag.getName(), "gram") || 
				 !strcmp(tag.getName(), "number") || 
				 !strcmp(tag.getName(), "pron") ||
				 !strcmp(tag.getName(), "def")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<i>";
			}
			else if (tag.isEndTag()) {
				buf += "</i>";
			}
		}

		// <tr>
		else if (!strcmp(tag.getName(), "tr")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<i>";
			}
			else if (tag.isEndTag()) {
				buf += "</i>";
			}
		}
		
		// orth
		else if (!strcmp(tag.getName(), "orth")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "<b>";
			}
			else if (tag.isEndTag()) {
				buf += "</b>";
			}
		}

		// <etym>, <usg>
		else if (!strcmp(tag.getName(), "etym") || 
				 !strcmp(tag.getName(), "usg")) {
			// do nothing here
		}

	   	// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				if (!tag.isEmpty()) {
					u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
				SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
				
				buf.appendFormatted("<a href=\"passagestudy.jsp?action=showNote&type=n&value=%s&module=%s&passage=%s\"><small><sup>*n</sup></small></a>",
					URL::encode(footnoteNumber.c_str()).c_str(), 
					URL::encode(u->version.c_str()).c_str(),
					URL::encode(u->key->getText()).c_str());
				
				u->suspendTextPassThru = false;
			}
		}

		else {
			return false;  // we still didn't handle token
		}

	}
	return true;
}


SWORD_NAMESPACE_END

