/******************************************************************************
 *
 *  osisheadings.cpp -    SWFilter descendant to hide or show headings
 *            in an OSIS module
 *
 * $Id$
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#include "osisheadings.h"

#include <cstdlib>
#include <cstdio>
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Headings";
    static const char oTip[]  = "Toggles Headings On and Off if they exist";

    class MyUserData : public BasicFilterUserData {
    public:
        std::string currentHeadingName;
        XMLTag currentHeadingTag;
        std::string sID;
        std::string heading;
        int depth;
        int headerNum;
        bool canonical;

        MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
            clear();
        }
        void clear() {
            currentHeadingName = "";
            currentHeadingTag = "";
            sID = "";
            heading = "";
            depth = 0;
            headerNum = 0;
            canonical=false;
        }
    };
}

std::unique_ptr<BasicFilterUserData> OSISHeadings::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

OSISHeadings::OSISHeadings()
    : OffOnOptionFilter(oName, oTip)
{ setPassThruUnknownToken(true); }

bool OSISHeadings::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {

    MyUserData * u = static_cast<MyUserData *>(userData);
    XMLTag tag(token);
    std::string name = tag.name();

    // we only care about titles and divs or if we're already in a heading
    //
    // are we currently in a heading?
    if (u->currentHeadingName.size()) {
        u->heading.append(u->lastTextNode);
        if (std::string("true") == tag.attribute("canonical")) u->canonical = true;
        if (name == u->currentHeadingName) {
            if (tag.isEndTag(u->sID.c_str())) {
                if (!u->depth-- || !u->sID.empty()) {
                    // see comment below about preverse div changed and needing to preserve the <title> container tag for old school pre-verse titles
                    // we've just finished a heading.  It's all stored up in u->heading
                    bool preverse = (std::string("x-preverse") == u->currentHeadingTag.attribute("subType") || std::string("x-preverse") == u->currentHeadingTag.attribute("subtype"));

                    // do we want to put anything in EntryAttributes?
                    if (u->module->isProcessEntryAttributes() && (option || u->canonical || !preverse)) {
                        std::string buf2(formatted("%i", u->headerNum++));
                        // leave the actual <title...> wrapper in if we're part of an old school preverse title
                        // because now frontend have to deal with preverse as a div which may or may not include <title> elements
                        // and they can't simply wrap all preverse material in <h1>, like they probably did previously
                        std::string heading;
                        if (u->currentHeadingName == "title") {
                            XMLTag wrapper = u->currentHeadingTag;
                            if (wrapper.attribute("subType") == "x-preverse") {
                                wrapper.eraseAttribute("subType");
                            } else if (wrapper.attribute("subtype")
                                       == "x-preverse")
                            {
                                wrapper.eraseAttribute("subtype");
                            }
                            heading = wrapper.toString();
                            heading += u->heading;
                            heading += tag.toString();
                        }
                        else heading = u->heading;
                        u->module->getEntryAttributes()["Heading"][(preverse)?"Preverse":"Interverse"][buf2] = heading;

                        for (auto const & attr : u->currentHeadingTag.attributeNames())
                            u->module->getEntryAttributes()["Heading"][buf2][attr.c_str()] =
                                    u->currentHeadingTag.attribute(attr.c_str());
                    }

                    // do we want the heading in the body?
                    if (!preverse && (option || u->canonical)) {
                        buf.append(u->currentHeadingTag.toString());
                        buf.append(u->heading);
                        buf.append(tag.toString());
                    }
                    u->suspendTextPassThru = false;
                    u->clear();
                }
            }
            else u->depth++;
        }
        u->heading.append(tag.toString());
        return true;
    }

    // are we a title or a preverse div?
    else if (   name == "title"
        || (name == "div"
            && ( std::string("x-preverse") == tag.attribute("subType")
              || std::string("x-preverse") == tag.attribute("subtype")))) {

        u->currentHeadingName = name;
        u->currentHeadingTag = tag;
        u->heading = "";
        u->sID = u->currentHeadingTag.attribute("sID");
        u->depth = 0;
        u->suspendTextPassThru = true;
        u->canonical = (std::string("true") == tag.attribute("canonical"));

        return true;
    }

    return false;
}



} /* namespace swordxx */

