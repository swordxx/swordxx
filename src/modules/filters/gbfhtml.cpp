/***************************************************************************
                          gbfhtml.cpp  -   GBF to HTML filter 
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
#include <gbfhtml.h>
#include <ctype.h>
#include <string>

using std::string;

SWORD_NAMESPACE_START

GBFHTML::GBFHTML() {
	setTokenStart("<");
	setTokenEnd(">");
	
	setTokenCaseSensitive(true);

	addTokenSubstitute("Rf", ")</small></font>");
	addTokenSubstitute("Rx", "</a>");
	addTokenSubstitute("FI", "<i>"); // italics begin
	addTokenSubstitute("Fi", "</i>");
	addTokenSubstitute("FB", "<n>"); // bold begin
	addTokenSubstitute("Fb", "</n>");
	addTokenSubstitute("FR", "<font color=\"#FF0000\">"); // words of Jesus begin
	addTokenSubstitute("Fr", "</font>");
	addTokenSubstitute("FU", "<u>"); // underline begin
	addTokenSubstitute("Fu", "</u>");
	addTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
	addTokenSubstitute("Fo", "</cite>");
	addTokenSubstitute("FS", "<sup>"); // Superscript begin// Subscript begin
	addTokenSubstitute("Fs", "</sup>");
	addTokenSubstitute("FV", "<sub>"); // Subscript begin
	addTokenSubstitute("Fv", "</sub>");
	addTokenSubstitute("TT", "<big>"); // Book title begin
	addTokenSubstitute("Tt", "</big>");
	addTokenSubstitute("PP", "<cite>"); //  poetry  begin
	addTokenSubstitute("Pp", "</cite>");
	addTokenSubstitute("Fn", "</font>"); //  font  end
	addTokenSubstitute("CL", "<br />"); //  new line
	addTokenSubstitute("CM", "<!P><br />"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired
	addTokenSubstitute("CG", ""); //  ???
	addTokenSubstitute("CT", ""); // ???
	addTokenSubstitute("JR", "<div align=\"right\">"); // right align begin
	addTokenSubstitute("JC", "<div align=\"center\">"); // center align begin
	addTokenSubstitute("JL", "</div>"); // align end
	
}


bool GBFHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	const char *tok;
	char val[128];
	char *valto;
	const char *num;

	if (!substituteToken(buf, token)) {		
		// deal with OSIS note tags.  Just hide till OSISRTF		
		if (!strncmp(token, "note ", 5)) {
			// let's stop text from going to output
			userData["suspendTextPassThru"] = "true";
		}
		
		else if (!strncmp(token, "/note", 5)) {
			userData["suspendTextPassThru"] = "false";
		}		

		else if (!strncmp(token, "w", 1)) {
			// OSIS Word (temporary until OSISRTF is done)
			valto = val;
			num = strstr(token, "lemma=\"x-Strongs:");
			if (num) {
				for (num+=17; ((*num) && (*num != '\"')); num++)
					*valto++ = *num;
				*valto = 0;
				if (atoi((!isdigit(*val))?val+1:val) < 5627) {
					pushString(buf, " <small><em>&lt;");
					for (tok = (!isdigit(*val))?val+1:val; *tok; tok++)
							*(*buf)++ = *tok;
					pushString(buf, "&gt;</em></small> ");					
				}
			}
			valto = val;
			num = strstr(token, "morph=\"x-Robinson:");
			if (num) {
				for (num+=18; ((*num) && (*num != '\"')); num++)
					*valto++ = *num;
				*valto = 0;
				pushString(buf, " <small><em>(");
				for (tok = val; *tok; tok++)
				// normal robinsons tense
						*(*buf)++ = *tok;		
				pushString(buf, ")</em></small> ");					
			}
		}
		
		else if (!strncmp(token, "WG", 2) || !strncmp(token, "WH", 2)) { // strong's numbers
			pushString(buf, " <small><em>&lt;");
			for (tok = token + 2; *tok; tok++)
					*(*buf)++ = *tok;
			pushString(buf, "&gt;</em></small> ");
		}

		else if (!strncmp(token, "WTG", 3) || !strncmp(token, "WTH", 3)) { // strong's numbers tense
			pushString(buf, " <small><em>&lt;");
			for (tok = token + 3; *tok; tok++)
				if(*tok != '\"')
					*(*buf)++ = *tok;
			pushString(buf, ")</em></small> ");
		}

		else if (!strncmp(token, "RX", 2)) {
			pushString(buf, "<i>");
			for (tok = token + 3; *tok; tok++) {
			  if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
			    *(*buf)++ = *tok;
			  }
			  else {
			    break;
			  }
			}
			pushString(buf, "</i>");
		}

		else if (!strncmp(token, "RB", 2)) {
			pushString(buf, "<i>");
			userData["hasFootnotePreTag"] = "true";
		}

		else if (!strncmp(token, "RF", 2)) {
			if(userData["hasFootnotePreTag"] == "true") {
				userData["hasFootnotePreTag"] = "false";
				pushString(buf, "</i> ");
			}
			pushString(buf, "<font color=\"#800000\"><small> (");
		}

		else if (!strncmp(token, "FN", 2)) {
			pushString(buf, "<font face=\"");
			for (tok = token + 2; *tok; tok++)				
				if(*tok != '\"') 			
					*(*buf)++ = *tok;
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
		}

		else if (!strncmp(token, "CA", 2)) {	// ASCII value
			*(*buf)++ = (char)atoi(&token[2]);
		}
		
		else {
			return false;
		}
	}
	return true;
}

SWORD_NAMESPACE_END
