/***************************************************************************
                     thmlhtmlhref.cpp  -  ThML to HTML filter with hrefs  
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
#include <thmlhtmlhref.h>
#include <swmodule.h>
#include <utilxml.h>

SWORD_NAMESPACE_START
 
ThMLHTMLHREF::ThMLHTMLHREF() {
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

	addTokenSubstitute("note", " <font color=\"#800000\"><small>(");
	addTokenSubstitute("/note", ")</small></font> ");
	addTokenSubstitute("/scripture", "</i> ");
}


bool ThMLHTMLHREF::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
	const char *tok;
	if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
		XMLTag tag(token);
		if (!strcmp(tag.getName(), "sync")) {
			if( tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "morph")) {
				buf += "<small><em> (<a href=\"";
			}
			else {
				buf += "<small><em> &lt;<a href=\"";
			}

			buf += "type=";
			buf += tag.getAttribute("type");

			const char* value = tag.getAttribute("value");
			buf += " value=";
			buf += value ? value : "";
			buf += "\">";

			//add the value
			if (value && tag.getAttribute("type") && strcmp(tag.getAttribute("type"), "morph")) { //not morph type
				value++;
			}
			buf += value;

			if(tag.getAttribute("type") && strcmp(tag.getAttribute("type"), "morph"))
				buf += "</a>&gt; </em></small>";
			else
				buf += "</a>) </em></small>";
		}
		else if (!strcmp(tag.getName(), "scripture")) {
			userData["inscriptRef"] = "true";
			buf += "<i>";
		}
		else if (!strcmp(tag.getName(), "scripRef")) {
			if (tag.isEndTag()) {
				if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
					userData["inscriptRef"] = "false";
					buf += "</a>";
				}
				else { // end of scripRef like "<scripRef>John 3:16</scripRef>"
					buf += "<a href=\"passage=";
					buf += userData["lastTextNode"].c_str();
					buf += "\">";

					buf += userData["lastTextNode"].c_str();
					buf += "</a>";

					// let's let text resume to output again
					userData["suspendTextPassThru"] = "false";
				}
			}
			else if (tag.getAttribute("passage")) { //passage given
				userData["inscriptRef"] = "true";

				buf += "<a href=\"";
				if (const char* version = tag.getAttribute("version")) {
					buf += "version=";
					buf += version;
					buf += " ";
				}
				if (const char* passage = tag.getAttribute("passage")) {
					buf += "passage=";
					buf += passage;
				}
				buf += "\">";
			}
			else { //no passage or version given
				userData["inscriptRef"] = "false";
				// let's stop text from going to output
				userData["suspendTextPassThru"] = "true";
			}
		}
		else if (!strcmp(tag.getName(), "div")) {
			if (tag.isEndTag() && userData["SecHead"] == "true") {
				buf += "</i></b><br />";
				userData["SecHead"] = "false";
			}
			else if (tag.getAttribute("class")) {
				if (!strcasecmp(tag.getAttribute("class"), "sechead")) {
					userData["SecHead"] = "true";
					buf += "<br /><b><i>";
				}
				else if (!strcasecmp(tag.getAttribute("class"), "title")) {
					userData["SecHead"] = "true";
					buf += "<br /><b><i>";
				}
			}
		}
/*		else if (!strncmp(token, "sync type=\"Strongs\" value=\"T", 28)) {
			buf +="<a href=\"");
			for (tok = token + 5; *(tok+1); tok++)
				if(*tok != '\"')
					*(*buf)++ = *tok;
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
			for (tok = token + 29; *(tok+2); tok++)				
				if(*tok != '\"') 			
					*(*buf)++ = *tok;		
			buf +="</a>");
		}
*/
		else if (!strcmp(tag.getName(), "img")) {
			const char *src = strstr(token, "src");
			if (!src)		// assert we have a src attribute
				return false;

			buf += '<';
			for (const char *c = token; *c; c++) {
				if (c == src) {
					for (;((*c) && (*c != '"')); c++)
						buf += *c;

					if (!*c) { c--; continue; }

					buf += '"';
					if (*(c+1) == '/') {
						buf += "file:";
						buf += module->getConfigEntry("AbsoluteDataPath");
						if (buf[buf.length()-2] == '/')
							c++;		// skip '/'
					}
					continue;
				}
				buf += *c;
			}
			buf += '>';
		}
		else {
			buf += '<';
			/*for (const char *tok = token; *tok; tok++)
				buf += *tok;*/
			buf += token;
			buf += '>';
			//return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END
