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

SWORD_NAMESPACE_START

ThMLWEBIF::ThMLWEBIF() : baseURL("/"), passageStudyURL(baseURL + "passagestudy.jsp") {
  //all's done in ThMLHTMLHREF
}

bool ThMLWEBIF::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
	const char *tok;
	std::string url;

	if (!substituteToken(buf, token)) {
	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync ", 5)) {
			url = "";

			if(strstr(token,"type=\"morph\"")){
				buf += "<small><em> (";
			}
			else {
				buf += "<small><em> &lt;";
			}

			for (tok = token + 5; *(tok+1); tok++) {
				if(*tok != '\"') {
					url += *tok;
				}
			}
			buf.appendFormatted("<a href=\"%s?sync_key=%s\">", passageStudyURL.c_str(), encodeURL(url).c_str() );

                        //scan for value and add it to the buffer
			for (tok = token + 5; *tok; tok++) {
				if (!strncmp(tok, "value=\"", 7)) {
					if(strstr(token,"type=\"morph\""))
						tok += 7;
					else
						tok += 8;
					for (;*tok != '\"'; tok++)
						buf += *tok;
					break;
				}
			}
			if(strstr(token,"type=\"morph\""))
				buf += "</a>) </em></small>";
			else
				buf += "</a>&gt; </em></small>";
		}

		else if (!strncmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) {
			userData["inscriptRef"] = "true";
			url = "";

			for (const char *tok = token + 9; *(tok+1); tok++) {
				if(*tok != '\"')
					url += *tok;
			}

			buf.appendFormatted("<a href=\"%s?p_key=%s\">", passageStudyURL.c_str(), encodeURL(url).c_str());
		}

		// we've ended a scripRef
		else if (!strcmp(token, "/scripRef")) {
			if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
				userData["inscriptRef"] = "false";
				buf +="</a>";
			}

			else { // like "<scripRef>John 3:16</scripRef>"
				url = userData["lastTextNode"].c_str();

				buf.appendFormatted("<a href=\"%s?key=%s\">", passageStudyURL.c_str(), encodeURL(url).c_str() );

				buf += userData["lastTextNode"].c_str();

				// let's let text resume to output again
				userData["suspendTextPassThru"] = "false";

				buf += "</a>";
			}
		}

		else {
			return ThMLHTMLHREF::handleToken(buf,token,userData);
		}
	}
	return true;
}


SWORD_NAMESPACE_END
