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
#include <thmlhtmlhref.h>
#include <swmodule.h>
#include <utilxml.h>
#include <versekey.h>

SWORD_NAMESPACE_START


ThMLHTMLHREF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	if (module) {
		version = module->Name();
		BiblicalText = (!strcmp(module->Type(), "Biblical Texts"));
	}	
}


ThMLHTMLHREF::ThMLHTMLHREF() {
	setTokenStart("<");
	setTokenEnd(">");

	setTokenCaseSensitive(true);
	addTokenSubstitute("/scripture", "</i> ");
}


bool ThMLHTMLHREF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
	const char *tok;
	if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
		MyUserData *u = (MyUserData *)userData;		
		XMLTag tag(token);
		if (tag.getName() && !strcmp(tag.getName(), "sync")) {
			SWBuf value = tag.getAttribute("value");
			if( tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "morph")) { //&gt;
				buf += "<small><em>(<a href=\"";
				buf += "type=";
				buf += tag.getAttribute("type");
	
				//const char* value = tag.getAttribute("value");
				buf += " value=";
				buf += (value.length()) ? value : "";
				buf += "\">";
				buf += (value.length()) ? value : "";
				buf += "</a>) </em></small>";
			}
			else if( tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "Strongs")) {
				buf += "<small><em>&lt;<a href=\"";
				buf += "type=";
				buf += tag.getAttribute("type");
	
				//const char* value = tag.getAttribute("value");
				buf += " value=";
				buf += (value.length()) ? value : "";
				buf += "\">";
				value<<1;
				buf += (value.length()) ? value : "";
				buf += "</a>&gt; </em></small>";
			}
			else if( tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "Dict")) {
				buf += "<b>";
			}
			else if(tag.isEndTag()) {
				buf += "</b>";
			}
		}
		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				if (!tag.isEmpty()) {
					SWBuf type = tag.getAttribute("type");
					SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
					VerseKey *vkey;
					// see if we have a VerseKey * or descendant
					try {
						vkey = SWDYNAMIC_CAST(VerseKey, u->key);
					}
					catch ( ... ) {	}
					if (vkey) {
						// leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
						char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
						buf.appendFormatted("<a href=\"noteID=%s.%c.%s\"><small><sup>*%c</sup></small></a> ", vkey->getText(), ch, footnoteNumber.c_str(), ch);
					}
					u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = false;
			}
		}
		else if (tag.getName() && !strcmp(tag.getName(), "scripRef")) {
			if (tag.isEndTag()) {
				if (u->inscriptRef) { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
					if (u->BiblicalText)
						buf += "<small><sup>*x</sup></small>";
					buf += "</a>&nbsp";
					u->inscriptRef = false;
					u->suspendTextPassThru = false;
				}
				else { // end of scripRef like "<scripRef>John 3:16</scripRef>"
					buf += "&nbsp<a href=\"";
					if (u->BiblicalText) {
						if (u->version) {
							buf += "version=";
							buf += u->version;
							buf += " ";
						}
					}
					buf += "passage=";
					buf += u->lastTextNode.c_str();
					buf += "\">";
					if (u->BiblicalText)
						buf += "<small><sup>*x</sup></small>";
					else {
						buf += u->lastTextNode.c_str();
					}
					buf += "</a>&nbsp";
					// let's let text resume to output again
					u->suspendTextPassThru = false;
				}
			}
			else if (tag.getAttribute("passage")) { //passage given
				u->inscriptRef = true;

				buf += "&nbsp<a href=\"";
				SWBuf version = tag.getAttribute("version");
				if (version.length()) {
					buf += "version=";
					buf += version;
					buf += " ";
				}
				SWBuf passage = tag.getAttribute("passage");
				if (passage.length()) {
					buf += "passage=";
					buf += passage;
				}
				buf += "\">";
				u->suspendTextPassThru = true;
			}
			else { //no passage or version given
				u->inscriptRef = false;
				// let's stop text from going to output
				u->suspendTextPassThru = true;
			}
		}
		else if (tag.getName() && !strcmp(tag.getName(), "scripture")) {
			u->inscriptRef = true;
			buf += "<i>";
		}
		else if (tag.getName() && !strcmp(tag.getName(), "div")) {
			if (tag.isEndTag() && u->SecHead) {
				buf += "</i></b><br />";
				u->SecHead = false;
			}
			else if (tag.getAttribute("class")) {
				if (!stricmp(tag.getAttribute("class"), "sechead")) {
					u->SecHead = true;
					buf += "<br /><b><i>";
				}
				else if (!stricmp(tag.getAttribute("class"), "title")) {
					u->SecHead = true;
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
		else if (tag.getName() && !strcmp(tag.getName(), "img")) {
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
						buf += userData->module->getConfigEntry("AbsoluteDataPath");
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
