/***************************************************************************
                     thmlhtml.cpp  -  ThML to HTML filter
                             -------------------
    begin                : 1999-10-27
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
#include <thmlhtml.h>


ThMLHTML::ThMLHTML() {
	setTokenStart("<");
	setTokenEnd(">");
/*
	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

	addEscapeStringSubstitute("nbsp", " ");
	addEscapeStringSubstitute("quot", "\"");
	addEscapeStringSubstitute("amp", "&");
	addEscapeStringSubstitute("lt", "<");
	addEscapeStringSubstitute("gt", ">");
	addEscapeStringSubstitute("brvbar", "|");
	addEscapeStringSubstitute("sect", "§");
	addEscapeStringSubstitute("copy", "©");
	addEscapeStringSubstitute("laquo", "«");
	addEscapeStringSubstitute("reg", "®");
	addEscapeStringSubstitute("acute", "´");
	addEscapeStringSubstitute("para", "¶");
	addEscapeStringSubstitute("raquo", "»");

	addEscapeStringSubstitute("Aacute", "Á");
	addEscapeStringSubstitute("Agrave", "À");
	addEscapeStringSubstitute("Acirc", "Â");
	addEscapeStringSubstitute("Auml", "Ä");
	addEscapeStringSubstitute("Atilde", "Ã");
	addEscapeStringSubstitute("Aring", "Å");
	addEscapeStringSubstitute("aacute", "á");
	addEscapeStringSubstitute("agrave", "à");
	addEscapeStringSubstitute("acirc", "â");
	addEscapeStringSubstitute("auml", "ä");
	addEscapeStringSubstitute("atilde", "ã");
	addEscapeStringSubstitute("aring", "å");
	addEscapeStringSubstitute("Eacute", "É");
	addEscapeStringSubstitute("Egrave", "È");
	addEscapeStringSubstitute("Ecirc", "Ê");
	addEscapeStringSubstitute("Euml", "Ë");
	addEscapeStringSubstitute("eacute", "é");
	addEscapeStringSubstitute("egrave", "è");
	addEscapeStringSubstitute("ecirc", "ê");
	addEscapeStringSubstitute("euml", "ë");
	addEscapeStringSubstitute("Iacute", "Í");
	addEscapeStringSubstitute("Igrave", "Ì");
	addEscapeStringSubstitute("Icirc", "Î");
	addEscapeStringSubstitute("Iuml", "Ï");
	addEscapeStringSubstitute("iacute", "í");
	addEscapeStringSubstitute("igrave", "ì");
	addEscapeStringSubstitute("icirc", "î");
	addEscapeStringSubstitute("iuml", "ï");
	addEscapeStringSubstitute("Oacute", "Ó");
	addEscapeStringSubstitute("Ograve", "Ò");
	addEscapeStringSubstitute("Ocirc", "Ô");
	addEscapeStringSubstitute("Ouml", "Ö");
	addEscapeStringSubstitute("Otilde", "Õ");
	addEscapeStringSubstitute("oacute", "ó");
	addEscapeStringSubstitute("ograve", "ò");
	addEscapeStringSubstitute("ocirc", "ô");
	addEscapeStringSubstitute("ouml", "ö");
	addEscapeStringSubstitute("otilde", "õ");
	addEscapeStringSubstitute("Uacute", "Ú");
	addEscapeStringSubstitute("Ugrave", "Ù");
	addEscapeStringSubstitute("Ucirc", "Û");
	addEscapeStringSubstitute("Uuml", "Ü");
	addEscapeStringSubstitute("uacute", "ú");
	addEscapeStringSubstitute("ugrave", "ù");
	addEscapeStringSubstitute("ucirc", "û");
	addEscapeStringSubstitute("uuml", "ü");
	addEscapeStringSubstitute("Yacute", "Ý");
	addEscapeStringSubstitute("yacute", "ý");
	addEscapeStringSubstitute("yuml", "ÿ");

	addEscapeStringSubstitute("deg", "°");
	addEscapeStringSubstitute("plusmn", "±");
	addEscapeStringSubstitute("sup2", "²");
	addEscapeStringSubstitute("sup3", "³");
	addEscapeStringSubstitute("sup1", "¹");
	addEscapeStringSubstitute("nbsp", "º");
	addEscapeStringSubstitute("pound", "£");
	addEscapeStringSubstitute("cent", "¢");
	addEscapeStringSubstitute("frac14", "¼");
	addEscapeStringSubstitute("frac12", "½");
	addEscapeStringSubstitute("frac34", "¾");
	addEscapeStringSubstitute("iquest", "¿");
	addEscapeStringSubstitute("iexcl", "¡");
	addEscapeStringSubstitute("ETH", "Ð");
	addEscapeStringSubstitute("eth", "ð");
	addEscapeStringSubstitute("THORN", "Þ");
	addEscapeStringSubstitute("thorn", "þ");
	addEscapeStringSubstitute("AElig", "Æ");
	addEscapeStringSubstitute("aelig", "æ");
	addEscapeStringSubstitute("Oslash", "Ø");
	addEscapeStringSubstitute("curren", "¤");
	addEscapeStringSubstitute("Ccedil", "Ç");
	addEscapeStringSubstitute("ccedil", "ç");
	addEscapeStringSubstitute("szlig", "ß");
	addEscapeStringSubstitute("Ntilde", "Ñ");
	addEscapeStringSubstitute("ntilde", "ñ");
	addEscapeStringSubstitute("yen", "¥");
	addEscapeStringSubstitute("not", "¬");
	addEscapeStringSubstitute("ordf", "ª");
	addEscapeStringSubstitute("uml", "¨");
	addEscapeStringSubstitute("shy", "­");
	addEscapeStringSubstitute("macr", "¯");
*/
	setTokenCaseSensitive(true);

	addTokenSubstitute("/scripRef", " </a>");
	addTokenSubstitute("note", " <small>(");
	addTokenSubstitute("/note", ")</small> ");
}


bool ThMLHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	if (!substituteToken(buf, token)) {
	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync type=\"Strongs\" value=\"", 27)) {
                        if (token[27] == 'H' || token[27] == 'G' || token[27] == 'A') {
        			pushString(buf, "<small><em>");
	        		for (unsigned int i = 5; i < strlen(token); i++)
		        		if(token[i] != '\"')
			        		*(*buf)++ = token[i];
        			pushString(buf, "</em></small>");
                        }
                        else if (token[27] == 'T') {
        			pushString(buf, "<small><i>");
        			for (unsigned int i = 29; token[i] != '\"'; i++)
        				*(*buf)++ = token[i];
        			pushString(buf, "</i></small>");
                        }
		}
		else if (!strncmp(token, "sync type=\"morph\" value=\"", 25)) {
			pushString(buf, "<small><em>");
			for (unsigned int i = 25; token[i] != '\"'; i++)
				*(*buf)++ = token[i];
			pushString(buf, "</em></small>");
		}
		else if (!strncmp(token, "sync type=\"lemma\" value=\"", 25)) {
			pushString(buf, "<small><em>(");
			for (unsigned int i = 25; token[i] != '\"'; i++)
				*(*buf)++ = token[i];
			pushString(buf, ")</em></small>");
		}
		else if (!strncmp(token, "scripRef", 8)) {
			pushString(buf, "<a href=\"");
			for (unsigned int i = 9; i < strlen(token); i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
		}
                else if(!strncmp(token, "note", 4)) {
                        pushString(buf, "<small>(");
                }

		else {
			return false;  // we still didn't handle token
		}
	}
	return true;
}

