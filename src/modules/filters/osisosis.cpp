/***************************************************************************
                     osisosis.cpp  -  internal OSIS to public OSIS filter
                             -------------------
    begin                : 2004-03-13
    copyright            : 2003 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation version 2 of the License.
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <osisosis.h>
#include <utilxml.h>
#include <versekey.h>
#include <swmodule.h>

SWORD_NAMESPACE_START


OSISOSIS::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));
}


OSISOSIS::OSISOSIS() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);
	setTokenCaseSensitive(true);
}


bool OSISOSIS::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		MyUserData *u = (MyUserData *)userData;
		XMLTag tag(token);

		if (!tag.isEmpty() && (!tag.isEndTag()))
			u->startTag = tag;

		// <w> tag
		if (!strcmp(tag.getName(), "w")) {

			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
				SWBuf attr = tag.getAttribute("lemma");
				if (attr.length()) {
					if (!strncmp(attr.c_str(), "x-Strongs:", 10)) {
						memcpy(attr.getRawData()+3, "strong", 6);
						attr << 3;
						tag.setAttribute("lemma", attr);
					}
				}
				attr = tag.getAttribute("morph");
				if (attr.length()) {
					if (!strncmp(attr.c_str(), "x-StrongsMorph:", 15)) {
						memcpy(attr.getRawData()+3, "strong", 6);
						attr << 3;
						tag.setAttribute("lemma", attr);
					}
					if (!strncmp(attr.c_str(), "x-Robinson:", 11)) {
						attr[2] = 'r';
						attr << 2;
						tag.setAttribute("lemma", attr);
					}
				}
			}

			tag.setAttribute("wn", 0);
			buf += tag;
		}

		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				if (!tag.isEmpty()) {
					SWBuf type = tag.getAttribute("type");

					if (type != "strongsMarkup") {
						buf += tag;
					}
					else u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
				if (u->suspendTextPassThru == false)
					buf+=tag;
				else u->suspendTextPassThru = false;
			}
		}

		else {
		      return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END
