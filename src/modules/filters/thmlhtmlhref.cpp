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
#include <url.h>

SWORD_NAMESPACE_START
 

ThMLHTMLHREF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	if (module) {
		version = module->Name();
		BiblicalText = (!strcmp(module->Type(), "Biblical Texts"));
		SecHead = false;
	}	
}


ThMLHTMLHREF::ThMLHTMLHREF() {
	setTokenStart("<");
	setTokenEnd(">");

	setTokenCaseSensitive(true);
	addTokenSubstitute("scripture", "<i> ");
	addTokenSubstitute("/scripture", "</i> ");
}


bool ThMLHTMLHREF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
	const char *tok;
	if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
		MyUserData *u = (MyUserData *)userData;		

		XMLTag tag(token);
		if ((!tag.isEndTag()) && (!tag.isEmpty()))
			u->startTag = tag;

		if (tag.getName() && !strcmp(tag.getName(), "sync")) {
			SWBuf value = tag.getAttribute("value");
			if (tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "morph")) { //&gt;
				if(value.length())
					buf.appendFormatted("<small><em>(<a href=\"passagestudy.jsp?action=showMorph&type=Greek&value=%s\">*s</a>) </em></small>", 
						URL::encode(value.c_str()).c_str(),
						value.c_str());
			}
			else if (tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "Strongs")) {
				if(*value == 'H') {
					value<<1;
					buf.appendFormatted("<small><em>&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Hebrew&value=%s\">",
						URL::encode(value.c_str()).c_str());
				}
				else if(*value == 'G') {
					value<<1;
					buf.appendFormatted("<small><em>&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Greek&value=%s\">",
						URL::encode(value.c_str()).c_str());
				}
				buf += (value.length()) ? value.c_str() : "";
				buf += "</a>&gt; </em></small>";
			}
			else if (tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "Dict")) {
				if (!tag.isEndTag())
					buf += "<b>";
				else	buf += "</b>";
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
					SWTRY {
						vkey = SWDYNAMIC_CAST(VerseKey, u->key);
					}
					SWCATCH ( ... ) {	}
					if (vkey) {
						// leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
						char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
						buf.appendFormatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup>*%c</sup></small></a> ", 
							ch, 
							URL::encode(footnoteNumber.c_str()).c_str(), 
							URL::encode(u->version.c_str()).c_str(), 
							URL::encode(vkey->getText()).c_str(), 
							ch);
					}
					u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = false;
			}
		}
		// <scripRef> tag
		else if (!strcmp(tag.getName(), "scripRef")) {
			if (!tag.isEndTag()) {
				if (!tag.isEmpty()) {
					u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {	//	</scripRef>
				if (!u->BiblicalText) {
					SWBuf refList = u->startTag.getAttribute("passage");
					if (!refList.length())
						refList = u->lastTextNode;
					SWBuf version = tag.getAttribute("version");
					
					buf.appendFormatted("&nbsp<a href=\"passagestudy.jsp?action=showRef&type=scripRef&value=%s&module=%s\">",
						(refList.length()) ? URL::encode(refList.c_str()).c_str() : "", 
						(version.length()) ? URL::encode(version.c_str()).c_str() : "");
					buf += u->lastTextNode.c_str();
					buf += "</a>&nbsp";
				}
				else {
					SWBuf footnoteNumber = u->startTag.getAttribute("swordFootnote");
					VerseKey *vkey;
					// see if we have a VerseKey * or descendant
					SWTRY {
						vkey = SWDYNAMIC_CAST(VerseKey, u->key);
					}
					SWCATCH ( ... ) {}
					if (vkey) {
						// leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
						//buf.appendFormatted("<a href=\"noteID=%s.x.%s\"><small><sup>*x</sup></small></a> ", vkey->getText(), footnoteNumber.c_str());
						buf.appendFormatted("<a href=\"passagestudy.jsp?action=showNote&type=x&value=%s&module=%s&passage=%s\"><small><sup>*x</sup></small></a> ",  
							URL::encode(footnoteNumber.c_str()).c_str(), 
							URL::encode(u->version.c_str()).c_str(),
							URL::encode(vkey->getText()).c_str());
					
					}
				}

				// let's let text resume to output again
				u->suspendTextPassThru = false;
			}
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
				else {
					buf += tag;
				}
			}
			else {
				buf += tag;
			}
		}
		else if (tag.getName() && (!strcmp(tag.getName(), "img") || !strcmp(tag.getName(), "image"))) {
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
