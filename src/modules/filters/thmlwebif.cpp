/***************************************************************************
                     ThMLWEBIF.cpp  -  ThML to HTML filter with hrefs
                             -------------------
    begin                    : 2001-09-03
    copyright            : 2001 by CrossWire Bible Society
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
#include <string.h>
#include <thmlwebif.h>
#include <swmodule.h>
#include <utilweb.h>
#include <utilxml.h>

SWORD_NAMESPACE_START

ThMLWEBIF::ThMLWEBIF() : baseURL("/"), passageStudyURL(baseURL + "passagestudy.jsp") {
  //all's done in ThMLHTMLHREF
}

bool ThMLWEBIF::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
	const char *tok;

	if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
		XMLTag tag(token);
		std::string url;
		if (!strcmp(tag.getName(), "sync")) {
			const char* value = tag.getAttribute("value");
			url = value;

			if(tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "morph")){
				buf += "<small><em> (";
			}
			else {
				if (value) {
					value++; //skip leading G, H or T
					//url = value;
				}

				buf += "<small><em> &lt;";
			}

			buf.appendFormatted("<a href=\"%s?key=%s\">", passageStudyURL.c_str(), encodeURL(url).c_str() );
			buf += value;
			buf += "</a>";

			if (tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "morph")) {
				buf += ") </em></small>";
			}
			else {
				buf += "&gt; </em></small>";
			}
		}
		else if (!strcmp(tag.getName(), "scripRef")) {
			if (tag.isEndTag()) {
				if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
					userData["inscriptRef"] = "false";
					buf += "</a>";
				}
				else { // end of scripRef like "<scripRef>John 3:16</scripRef>"
					url = userData["lastTextNode"];
					buf.appendFormatted("<a href=\"%s?key=%s\">", passageStudyURL.c_str(), encodeURL(url).c_str());
					buf += userData["lastTextNode"].c_str();
					buf += "</a>";

					// let's let text resume to output again
					userData["suspendTextPassThru"] = "false";
				}
			}
			else if (tag.getAttribute("passage")) { //passage given
				userData["inscriptRef"] = "true";

				buf.appendFormatted("<a href=\"%s?key=%s\">", passageStudyURL.c_str(), encodeURL(tag.getAttribute("passage")).c_str());
			}
			else { //no passage given
				userData["inscriptRef"] = "false";
				// let's stop text from going to output
				userData["suspendTextPassThru"] = "true";
			}
		}
		else {
			return ThMLHTMLHREF::handleToken(buf,token,userData);
		}
	}
	return true;
}


SWORD_NAMESPACE_END
