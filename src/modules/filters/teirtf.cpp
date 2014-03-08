/***************************************************************************
 *
 *  teirtf.cpp -	TEI to RTF filter
 *
 * $Id$
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include <teirtf.h>
#include <utilxml.h>
#include <swmodule.h>
#include <versekey.h>

SWORD_NAMESPACE_START


TEIRTF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	BiblicalText = false;
	inOsisRef = false;
	if (module) {
		version = module->getName();
		BiblicalText = (!strcmp(module->getType(), "Biblical Texts"));
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
		MyUserData *u = (MyUserData *)userData;
		XMLTag tag(token);

		// <p> paragraph tag
		if (!strcmp(tag.getName(), "p")) {
			if (!tag.isEndTag()) {	// non-empty start tag
				buf += "{\\sb100\\fi200\\par}";
			}
		}

		// <hi>
		else if (!strcmp(tag.getName(), "hi") || !strcmp(tag.getName(), "emph")) {
			SWBuf rend = tag.getAttribute("rend");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (rend == "ital" || rend == "italic")
					buf += "{\\i1 ";
				else if (rend == "bold")
					buf += "{\\b1 ";
				else if (rend == "subscript")
					buf += "{\\sub ";
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
                                	buf += "{\\sb100\\par\\b1 ";
					buf += n;
					buf += ". }";
				}
			}
		}

 		// <orth>
 		else if (!strcmp(tag.getName(), "orth")) {
 			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
 				buf += "{\\b1 ";
 			}
 			else if (tag.isEndTag()) {
 			        buf += "}";
 			}
 		}

		// <div>
		else if (!strcmp(tag.getName(), "div")) {

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf.append("{\\pard\\sa300}");
			}
			else if (tag.isEndTag()) {
			}
		}

		// <pos>, <gen>, <case>, <gram>, <number>, <mood>
		else if (!strcmp(tag.getName(), "pos") || !strcmp(tag.getName(), "gen") || !strcmp(tag.getName(), "case") || !strcmp(tag.getName(), "gram") || !strcmp(tag.getName(), "number") || !strcmp(tag.getName(), "mood")) {
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

		// <etym>
		else if (!strcmp(tag.getName(), "etym")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "[";
			}
			else if (tag.isEndTag()) {
			        buf += "]";
			}
		}

       		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				if (!tag.isEmpty()) {
					SWBuf type = tag.getAttribute("type");

					SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
					VerseKey *vkey = 0;
					// see if we have a VerseKey * or descendant
					SWTRY {
						vkey = SWDYNAMIC_CAST(VerseKey, u->key);
					}
					SWCATCH ( ... ) {	}
					if (vkey) {
						buf.appendFormatted("{\\super <a href=\"\">*%s</a>} ", footnoteNumber.c_str());
					}
					u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = false;
			}
		}

		// <lb/> tag
		else if (!strcmp(tag.getName(), "lb")) {
			buf += "{\\par}";
			userData->supressAdjacentWhitespace = true;
		}

		// <ref> tag
		else if (!strcmp(tag.getName(), "ref")) {
			if (!tag.isEndTag() && tag.getAttribute("osisRef")) {
				buf += "{<a href=\"\">";
				u->inOsisRef = true;
			}
			else if (tag.isEndTag() && u->inOsisRef) {
				buf += "</a>}";
				u->inOsisRef = false;
			}
		}


		else {
			return false;  // we still didn't handle token
		}

	}
	return true;
}


SWORD_NAMESPACE_END

