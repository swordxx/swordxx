/***************************************************************************
                     thmlcgi.cpp  -  ThML to Diatheke/CGI format
                             -------------------
    begin                : 2001-11-12
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
#include "thmlcgi.h"


ThMLCGI::ThMLCGI() {
	setTokenStart("<");
	setTokenEnd(">");

	setTokenCaseSensitive(true);

	addTokenSubstitute("note", " <font color=\"#800000\"><small>(");
	addTokenSubstitute("/note", ")</small></font> ");
}


bool ThMLCGI::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i;
	if (!substituteToken(buf, token)) {
	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync ", 5)) {
			pushString(buf, "<a href=\"");
			for (i = 5; i < strlen(token)-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';

                        //scan for value and add it to the buffer
			for (unsigned int j = 5; j < strlen(token); j++) {
                                if (!strncmp(token+j, "value=\"", 7)) {
                                        j += 7;
                                        for (;token[j] != '\"'; j++)
                				*(*buf)++ = token[j];
                                        break;
                                }
                        }
			pushString(buf, "</a>");
		}

		else if (!strncmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) {
			userData["inscriptRef"] = "true";
			pushString(buf, "<a href=\"");
			for (i = 9; i < strlen(token)-1; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
		} 

		// we're starting a scripRef like "<scripRef>John 3:16</scripRef>"
		else if (!strcmp(token, "scripRef")) {
			userData["inscriptRef"] = "false";
			// let's stop text from going to output
			userData["suspendTextPassThru"] = "true";
		}

		// we've ended a scripRef 
		else if (!strcmp(token, "/scripRef")) {
			if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
				userData["inscriptRef"] = "false";
				pushString(buf, "</a>");
			}
			
			else { // like "<scripRef>John 3:16</scripRef>"
				pushString(buf, "<a href=\"passage=");
				//char *strbuf = (char *)userData["lastTextNode"].c_str();
				pushString(buf, userData["lastTextNode"].c_str());
				*(*buf)++ = '\"';
				*(*buf)++ = '>';
				pushString(buf, userData["lastTextNode"].c_str());
				// let's let text resume to output again
				userData["suspendTextPassThru"] = "false";	
				pushString(buf, "</a>");
			}
		}
			
		else if (!strncmp(token, "div class=\"sechead\"", 19)) {
		        userData["SecHead"] = "true";
			pushString(buf, "<br /><b><i>");
		}
		else if (!strncmp(token, "div class=\"title\"", 19)) {
		        userData["SecHead"] = "true";
			pushString(buf, "<br /><b><i>");
		}
		else if (!strncmp(token, "/div", 4)) {
		        if (userData["SecHead"] == "true") {
			        pushString(buf, "</i></b><br />");
				userData["SecHead"] = "false";
			}
		}

		else if (!strncmp(token, "sync type=\"Strongs\" value=\"T", 28)) {
			pushString(buf, "<a href=\"");
			for (i = 5; i < strlen(token)-1; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
			for (i = 29; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "</a>");
		}
                else if(!strncmp(token, "note", 4)) {
                        pushString(buf, " <small><font color=\"#800000\">(");
                }                

		else {
			*(*buf)++ = '<';
			for (i = 0; i < strlen(token); i++)
				*(*buf)++ = token[i];
				*(*buf)++ = '>';
			//return false;  // we still didn't handle token
		}
	}
	return true;
}

