/***************************************************************************
                          gbfhtmlhref.cpp  -   GBF to HTML filter with hrefs 
			        for strongs and morph tags
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
#include <gbfhtmlhref.h>

GBFHTMLHREF::GBFHTMLHREF() {
	setTokenStart("<");
	setTokenEnd(">");
	
	setTokenCaseSensitive(true);

	addTokenSubstitute("RF", "<FONT COLOR=\"#800000\"> <SMALL> (");
	addTokenSubstitute("Rf", ") </SMALL> </FONT>");
	addTokenSubstitute("FI", "<I>"); // italics begin
	addTokenSubstitute("Fi", "</I>");
	addTokenSubstitute("FB", "<B>"); // bold begin
	addTokenSubstitute("Fb", "</B>");
	addTokenSubstitute("FR", "<FONT COLOR=\"#FF0000\">"); // words of Jesus begin
	addTokenSubstitute("Fr", "</FONT>");
	addTokenSubstitute("FU", "<U>"); // underline begin
	addTokenSubstitute("Fu", "</U>");
	addTokenSubstitute("FO", "<CITE>"); //  Old Testament quote begin
	addTokenSubstitute("Fo", "</CITE>");
	addTokenSubstitute("FS", "<SUP>"); // Superscript begin// Subscript begin
	addTokenSubstitute("Fs", "</SUP>");
	addTokenSubstitute("FV", "<SUB>"); // Subscript begin
	addTokenSubstitute("Fv", "</SUB>");
	addTokenSubstitute("TT", "<BIG>"); // Book title begin
	addTokenSubstitute("Tt", "</BIG>");
	addTokenSubstitute("PP", "<CITE>"); //  poetry  begin
	addTokenSubstitute("Pp", "</CITE>");
	addTokenSubstitute("Fn", "</FONT>"); //  font  end
	addTokenSubstitute("CL", "<BR>"); //  new line
	addTokenSubstitute("CM", "<P>"); //  paragraph
	addTokenSubstitute("CG", ""); //  ???
	addTokenSubstitute("CT", ""); // ???
	addTokenSubstitute("JR", "<DIV ALIGN=\"RIGHT\">"); // right align begin
	addTokenSubstitute("JC", "<DIV ALIGN=\"CENTER\">"); // center align begin
	addTokenSubstitute("JL", "</DIV>"); // align end
	
}


bool GBFHTMLHREF::handleToken(char **buf, const char *token, DualStringMap &userData) {
	if (!substituteToken(buf, token)) {
		if (!strncmp(token, "WG", 2) || !strncmp(token, "WH", 2)) { // strong's numbers
			pushString(buf, " <SMALL><EM>&lt;<A HREF=\"#");
			for (unsigned int i = 1; i < strlen(token); i++)				
				//if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
			for (unsigned int i = 2; i < strlen(token); i++)				
				//if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "</A>&gt</EM></SMALL>");
		}

		else if (!strncmp(token, "WTG", 3) || !strncmp(token, "WTH", 3)) { // strong's numbers tense
			pushString(buf, " <SMALL><EM>(<A HREF=\"#");
			for (unsigned int i = 2; i < strlen(token); i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
			for (unsigned int i = 3; i < strlen(token); i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "</A>)</EM></SMALL>");
		}

		else if (!strncmp(token, "WT", 2) && strncmp(token, "WTH", 3) && strncmp(token, "WTG", 3)) { // morph tags
			pushString(buf, " <SMALL><EM>(<A HREF=\"M");
			for (unsigned int i = 2; i < strlen(token); i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
			for (unsigned int i = 2; i < strlen(token); i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "</A>)</EM></SMALL>");
		}

		else if (!strncmp(token, "FN", 2)) {
			pushString(buf, "<FONT FACE=\"");
			for (unsigned int i = 2; i < strlen(token); i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
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
