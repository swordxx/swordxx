/***************************************************************************
                     thmlrtf.cpp  -  ThML to RTF filter
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
#include <thmlrtf.h>
#include <swmodule.h>

SWORD_NAMESPACE_START

ThMLRTF::ThMLRTF()
{
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

	addEscapeStringSubstitute("nbsp", " ");
	addEscapeStringSubstitute("apos", "'");
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

	setTokenCaseSensitive(true);

	addTokenSubstitute("/note", ") }");

        addTokenSubstitute("br", "\\line ");
        addTokenSubstitute("br /", "\\line ");
        addTokenSubstitute("i", "{\\i1 ");
        addTokenSubstitute("/i", "}");
        addTokenSubstitute("b", "{\\b1 ");
        addTokenSubstitute("/b", "}");
        addTokenSubstitute("p", "\\par ");

        //we need uppercase forms for the moment to support a few early ThML modules that aren't XHTML compliant
        addTokenSubstitute("BR", "\\line ");
        addTokenSubstitute("I", "{\\i1 ");
        addTokenSubstitute("/I", "}");
        addTokenSubstitute("B", "{\\b1 ");
        addTokenSubstitute("/B", "}");
        addTokenSubstitute("P", "\\par ");
}

bool ThMLRTF::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
	if (!substituteToken(buf, token)) {
	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync type=\"Strongs\" value=\"", 27)) {
			if (token[27] == 'H' || token[27] == 'G' || token[27] == 'A') {
				buf += " {\\fs15 <";
				for (unsigned int i = 28; token[i] != '\"'; i++)
					buf += token[i];
				buf += ">}";
			}
			else if (token[27] == 'T') {
				buf += " {\\fs15 (";
				for (unsigned int i = 28; token[i] != '\"'; i++)
					buf += token[i];
				buf += ")}";
			}
		}
		else if (!strncmp(token, "sync type=\"morph\" ", 18)) {
			buf += " {\\fs15 (";
			for (const char *tok = token + 5; *tok; tok++) {
				if (!strncmp(tok, "value=\"", 7)) {
					tok += 7;
					for (;*tok != '\"'; tok++)
						buf += *tok;
					break;
				}
			}

			buf += ")}";
		}
		else if (!strncmp(token, "sync type=\"lemma\" value=\"", 25)) {
			buf += "{\\fs15 (";
			for (unsigned int i = 25; token[i] != '\"'; i++)
				buf += token[i];
			buf += ")}";
		}
		else if (!strncmp(token, "scripRef", 8)) {
//			buf += "{\\cf2 #";
			buf += "<a href=\"\">";
		}
		else if (!strncmp(token, "/scripRef", 9)) {
			buf += "</a>";
		}
		else if (!strncmp(token, "div", 3)) {
			buf += '{';
			if (!strncmp(token, "div class=\"title\"", 17)) {
				buf += "\\par\\i1\\b1 ";
				userData["sechead"] = "true";
			}
			else if (!strncmp(token, "div class=\"sechead\"", 19)) {
				buf += "\\par\\i1\\b1 ";
				userData["sechead"] = "true";
			}
		}
		else if (!strncmp(token, "/div", 4)) {
			if (userData["sechead"] == "true") {
				buf += "\\par ";
				userData["sechead"] = "false";
			}
			buf += '}';
		}
		else if (!strncmp(token, "note", 4)) {
			buf += " {\\i1\\fs15 (";
		}
		else if (!strncmp(token, "img ", 4)) {
			const char *src = strstr(token, "src");
			if (!src)		// assert we have a src attribute
				return false;

                        char* filepath = new char[strlen(module->getConfigEntry("AbsoluteDataPath")) + strlen(token)];
                        *filepath = 0;
                        strcpy(filepath, module->getConfigEntry("AbsoluteDataPath"));
                        unsigned long i = strlen(filepath);
                        const char *c;
			for (c = (src + 5); *c != '"'; c++) {
				filepath[i] = *c;
                                i++;
			}
                        filepath[i] = 0;

                        for (c = filepath + strlen(filepath); c > filepath && *c != '.'; c--);
                        c++;

                        char imgc;
                        FILE* imgfile;
/*
                        if (stricmp(c, "jpg") || stricmp(c, "jpeg")) {
                                imgfile = fopen(filepath, "r");
                                if (imgfile != NULL) {
                                        buf += "{\\nonshppict {\\pict\\jpegblip ";
                                        while (feof(imgfile) != EOF) {
                                                buf.appendFormatted("%2x", fgetc(imgfile));
                                        }
                                        fclose(imgfile);
                                        buf += "}}";
                                }
                        }
                        else if (stricmp(c, "png")) {
                                buf += "{\\*\\shppict {\\pict\\pngblip ";

                                buf += "}}";
                        }
*/
                        delete [] filepath;
		}
		else {
			return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END
