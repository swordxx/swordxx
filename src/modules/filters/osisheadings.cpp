/******************************************************************************
 *
 *  osisheadings.cpp -	SWFilter descendant to hide or show headings
 *			in an OSIS module
 *
 * $Id$
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <stdio.h>
#include <osisheadings.h>
#include <swmodule.h>
#include <utilxml.h>
#include <utilstr.h>

SWORD_NAMESPACE_START

const char oName[] = "Headings";
const char oTip[] = "Toggles Headings On and Off if they exist";

const SWBuf choices[3] = {"Off", "On", ""};
const StringList oValues(&choices[0], &choices[2]);


namespace {
	class MyUserData : public BasicFilterUserData {
	public:
		SWBuf currentHeadingName;
		XMLTag currentHeadingTag;
		const char *sID;
		SWBuf heading;
		int depth;
		int headerNum;

		MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
			clear();
		}
		void clear() {
			currentHeadingName = "";
			currentHeadingTag = "";
			sID = 0;
			heading = "";
			depth = 0;
			headerNum = 0;
		}
	};
};


BasicFilterUserData *OSISHeadings::createUserData(const SWModule *module, const SWKey *key) {
	return new MyUserData(module, key);
}


OSISHeadings::OSISHeadings() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
	setPassThruUnknownToken(true);
}


bool OSISHeadings::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {

	MyUserData *u = (MyUserData *)userData;
	XMLTag tag(token);
	SWBuf name = tag.getName();

	// we only care about titles and divs or if we're already in a heading
	//
	// are we currently in a heading?
	if (u->currentHeadingName.size()) {
		u->heading.append(u->lastTextNode);
		if (name == u->currentHeadingName) {
			if (tag.isEndTag(u->sID)) {
				if (!u->depth-- || u->sID) {
					// we've just finished a heading.  It's all stored up in u->heading
					bool canonical = (SWBuf("true") == u->currentHeadingTag.getAttribute("canonical"));
					bool preverse = (SWBuf("x-preverse") == u->currentHeadingTag.getAttribute("subType") || SWBuf("x-preverse") == u->currentHeadingTag.getAttribute("subtype"));

					// do we want to put anything in EntryAttributes?
					if (u->module->isProcessEntryAttributes() && (option || canonical || !preverse)) {
						SWBuf buf; buf.appendFormatted("%i", u->headerNum++);
						u->module->getEntryAttributes()["Heading"][(preverse)?"Preverse":"Interverse"][buf] = u->heading;

						StringList attributes = u->currentHeadingTag.getAttributeNames();
						for (StringList::const_iterator it = attributes.begin(); it != attributes.end(); it++) {
							u->module->getEntryAttributes()["Heading"][buf][it->c_str()] = u->currentHeadingTag.getAttribute(it->c_str());
						}
					}

					// do we want the heading in the body?
					if (!preverse && (option || canonical)) {
						buf.append(u->currentHeadingTag);
						buf.append(u->heading);
						buf.append(tag);
					}
					u->suspendTextPassThru = false;
					u->clear();
				}
			}
			else u->depth++;
		}
		u->heading.append(tag);
		return true;
	}

	// are we a title or a preverse div?
	else if (   name == "title"
		|| (name == "div"
			&& ( SWBuf("x-preverse") == tag.getAttribute("subType")
			  || SWBuf("x-preverse") == tag.getAttribute("subtype")))) {

		u->currentHeadingName = name;
		u->currentHeadingTag = tag;
		u->heading = "";
		u->sID = u->currentHeadingTag.getAttribute("sID");
		u->depth = 0;
		u->suspendTextPassThru = true;

		return true;
	}

	return false;
}



SWORD_NAMESPACE_END

