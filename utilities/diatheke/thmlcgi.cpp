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

	addTokenSubstitute("note", " <font color=\"#008000\"><small>(");
	addTokenSubstitute("/note", ")</small></font> ");
}


bool ThMLCGI::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i;
	if (!substituteToken(buf, token)) {
	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync ", 5)) {
			pushString(buf, "<a href=\"!DIATHEKE_URL!");
			char* pbuf;
			char typ[32]; typ[0] = 0;
			char val[32]; val[0] = 0;
			char cls[32]; cls[0] = 0;
			for (unsigned int j = 5; j < strlen(token); j++) {
                                if (!strncmp(token+j, "type=\"", 6)) {
				        pbuf = typ;
                                        j += 6;
                                        for (;token[j] != '\"'; j++)
                				*(pbuf)++ = token[j];
					*(pbuf) = 0;
                                }
                                else if (!strncmp(token+j, "value=\"", 7)) {
				        pbuf = val;
                                        j += 7;
                                        for (;token[j] != '\"'; j++)
                				*(pbuf)++ = token[j];
					*(pbuf) = 0;
                                }
                                else if (!strncmp(token+j, "class=\"", 7)) {
				        pbuf = cls;
                                        j += 7;
                                        for (;token[j] != '\"'; j++)
                				*(pbuf)++ = token[j];
					*(pbuf) = 0;					
                                }
                        }
			if (*cls && *val) {
			        pushString(buf, "%s=on&value=%s", cls, val);
			}
			else if (*typ && *val) {
			  if (!strnicmp(typ, "Strongs", 7)) {
			    if (*val == 'G') {
			      pushString(buf, "Thayer=on&value=%s", val + 1);
			    }
			    else if (*val == 'H') {
			      pushString(buf, "BDB=on&value=%s", val + 1);
			    }
			  }

			  else if (!strnicmp(typ, "Morph", 5)) {
			    if (*val == 'G') {
			      pushString(buf, "Thayer=on&value=%s", val + 1);
			    }
			    else if (*val == 'H') {
			      pushString(buf, "BDB=on&value=%s", val + 1);
			    }
			    else {
			      pushString(buf, "Packard=on&value=%s", val);
			    }
			  }
			}

			for (i = 5; i < strlen(token)-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
			
			if (*val) {
			        pushString(buf, val);
			}
			pushString(buf, "</a>");
		}

		else if (!strncmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) {
        		userData["inscriptRef"] = "true";
			pushString(buf, "<a href=\"!DIATHEKE_URL!");
			for (i = 9; i < strlen(token)-1; i++) {
			  if(token[i] != '\"') {
			    if (token[i] == ' ') *(*buf)++ = '+';
			    else *(*buf)++ = token[i];
			  }
			}
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
				char* vref = (char*)userData["lastTextNode"].c_str();
				while (*vref) {
				  if (*vref == ' ') *(*buf)++ = '+';
				  else *(*buf)++ = *vref;
				  vref++;
				}
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

                else if(!strncmp(token, "note", 4)) {
                        pushString(buf, " <small><font color=\"#008000\">{");
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






