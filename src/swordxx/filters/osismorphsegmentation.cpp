/******************************************************************************
 *
 *  osismorphsegmentation.cpp -    SWFilter descendant to toggle splitting of
 *                morphemes (for morpheme segmented Hebrew in
 *                the WLC)
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osismorphsegmentation.h"

#include <cstdlib>
#include <string>
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Morpheme Segmentation";
    static const char oTip[]  = "Toggles Morpheme Segmentation On and Off, when present";

}

OSISMorphSegmentation::OSISMorphSegmentation()
    : OffOnOptionFilter(oName, oTip)
{}

OSISMorphSegmentation::~OSISMorphSegmentation() {}


char OSISMorphSegmentation::processText(std::string &text, const SWKey * /*key*/, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    std::string token;
    bool intoken    = false;
    bool hide       = false;

    std::string out;

    XMLTag tag;
    std::string tagText = "";
    unsigned int morphemeNum = 0;
    bool inMorpheme = false;
    std::string buf;

    for (auto const * from = text.c_str(); *from; ++from) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }

        if (*from == '>') { // process tokens
            intoken = false;

            if (startsWith(token, "seg "sv) || startsWith(token, "/seg"sv)) {
                tag = token.c_str();

                if (!tag.isEndTag()
                    && ((tag.attribute("type") == "morph")
                        || (tag.attribute("type") == "x-morph")))
                {  //<seg type="morph"> start tag
                    hide = isOptionOff(); //only hide if option is Off
                    tagText = "";
                    inMorpheme = true;
                }

                if (tag.isEndTag() && inMorpheme) {
                        buf = formatted("%.3d", morphemeNum++);
                        module->getEntryAttributes()["Morpheme"][buf]["body"] = tagText;
                        inMorpheme = false;
                }
                if (hide) { //hides start and end tags as long as hide is set

                    if (tag.isEndTag()) { //</seg>
                        hide = false;
                    }

                    continue; //leave out the current token
                }
            } //end of seg tag handling

            out.push_back('<');
            out.append(token);
            out.push_back('>');

            if (inMorpheme) {
                tagText.push_back('<');
                tagText.append(token);
                tagText.push_back('>');
            }

            hide = false;

            continue;
        } //end of intoken part

        if (intoken) { //copy token
            token.push_back(*from);
        }
        else { //copy text which is not inside of a tag
            out.push_back(*from);
            if (inMorpheme) {
                tagText.push_back(*from);
            }
        }
    }
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */
