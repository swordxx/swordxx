/***************************************************************************
                     teirtf.cpp  -  TEI to RTF filter
                             -------------------
    begin                : 2006-07-03
    copyright            : 2006 by CrossWire Bible Society
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
#include <ctype.h>
#include <teirtf.h>
#include <utilxml.h>
#include <swmodule.h>

SWORD_NAMESPACE_START


TEIRTF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	BiblicalText = false;
	if (module) {
		version = module->Name();
		BiblicalText = (!strcmp(module->Type(), "Biblical Texts"));
	}
}


TEIRTF::TEIRTF() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

	addEscapeStringSubstitute("amp", "&");
	addEscapeStringSubstitute("apos", "'");
	addEscapeStringSubstitute("lt", "<");
	addEscapeStringSubstitute("gt", ">");
	addEscapeStringSubstitute("quot", "\"");

	setTokenCaseSensitive(true);
}


bool TEIRTF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		//MyUserData *u = (MyUserData *)userData;
		XMLTag tag(token);

		// <p> paragraph tag
		if (!strcmp(tag.getName(), "p")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				buf += "{\\par}";
			}
			else if (tag.isEndTag()) {	// end tag
				buf += "{\\par}";
				userData->supressAdjacentWhitespace = true;
			}
			else {					// empty paragraph break marker
				buf += "{\\par\\par}";
				userData->supressAdjacentWhitespace = true;
			}
		}

		// <hi>
		else if (!strcmp(tag.getName(), "hi")) {
			SWBuf rend = tag.getAttribute("rend");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (rend == "ital")
					buf += "{\\i1 ";
				else if (rend == "bold")
					buf += "{\\b1 ";
				else if (rend == "sup")
				        buf += "{\\super ";

			}
			else if (tag.isEndTag()) {
				buf += "}";
			}
		}

		// <entryFree>
		else if (!strcmp(tag.getName(), "entryFree")) {
			SWBuf n = tag.getAttribute("n");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
			        if (n != "") {
                                	buf += "{\\b1 ";
					buf += n;
					buf += ". }";				}
			}
		}

		// <sense>
		else if (!strcmp(tag.getName(), "sense")) {
			SWBuf n = tag.getAttribute("n");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
			        if (n != "") {
                                	buf += "{\\par\\b1 ";
					buf += n;
					buf += ". }";
				}
			}
			else if (tag.isEndTag()) {
			                buf += "\\par ";
			}
		}

		// <div>
		else if (!strcmp(tag.getName(), "div")) {

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf.append("\\par\\par\\pard ");
			}
			else if (tag.isEndTag()) {
			}
		}

		// <pos>
		else if (!strcmp(tag.getName(), "pos")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
			        buf += "}";
			}
		}

		// <gen>
		else if (!strcmp(tag.getName(), "gen")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
			        buf += "}";
			}
		}

		// <case>
		else if (!strcmp(tag.getName(), "case")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
			        buf += "}";
			}
		}

		// <tr>
		else if (!strcmp(tag.getName(), "tr")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
			        buf += "}";
			}
		}

		// <number>
		else if (!strcmp(tag.getName(), "number")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
			        buf += "}";
			}
		}

		// <mood>
		else if (!strcmp(tag.getName(), "mood")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
			        buf += "}";
			}
		}

		// <etym>
		else if (!strcmp(tag.getName(), "etym")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "[";
			}
			else if (tag.isEndTag()) {
			        buf += "]";
			}
		}

		else {
			return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END

