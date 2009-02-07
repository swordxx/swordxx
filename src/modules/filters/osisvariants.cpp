/******************************************************************************
 *
 * osisvariants -	SWFilter descendant to hide or show textual variants
 *			in an OSIS module.
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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
#include <osisvariants.h>
#include <utilstr.h>

SWORD_NAMESPACE_START

const char OSISVariants::primary[] = "Primary Reading";
const char OSISVariants::secondary[] = "Secondary Reading";
const char OSISVariants::all[] = "All Readings";

const char OSISVariants::optName[] = "Textual Variants";
const char OSISVariants::optTip[] = "Switch between Textual Variants modes";


OSISVariants::OSISVariants() {
	option = false;
	options.push_back(primary);
	options.push_back(secondary);
	options.push_back(all);
}


OSISVariants::~OSISVariants() {
}

void OSISVariants::setOptionValue(const char *ival)
{
	if (!stricmp(ival, primary)) option = 0;
        else if (!stricmp(ival, secondary)) option = 1;
        else option = 2;
}

const char *OSISVariants::getOptionValue()
{
        if (option == 0) {
	        return primary;
	}
	else if (option == 1) {
	        return secondary;
	}
	else {
	        return all;
	}
}

char OSISVariants::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
        if (option == 0 || option == 1) { //we want primary or variant only
		bool intoken = false;
		bool hide = false;
		bool invar = false;
		
		SWBuf token;
		SWBuf orig = text;
		const char *from = orig.c_str();

		//we use a fixed comparision string to make sure the loop is as fast as the original two blocks with almost the same code
		//const char* variantCompareString = (option == 0) ? "div type=\"variant\" class=\"1\"" : "div type=\"variant\" class=\"2\"";
		
		for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				token = "";
				continue;
			}
			else if (*from == '>') {	// process tokens
				intoken = false;
				
				if (!strncmp(token.c_str(), "seg ", 4)) { //only one of the variants
					invar = true;
					hide = true;
					continue;
				}
				if (!strncmp(token.c_str(), "div type=\"variant\"", 18)) {
					invar = true;
					continue;
				}
				if (!strncmp(token.c_str(), "/div", 4)) {
					hide = false;
					if (invar) {
						invar = false;
						continue;
					}
				}
				if (!hide) {
					text += '<';
					text.append(token);
					text += '>';
				}

				continue;
			}
			if (intoken) {
				token += *from;
			}
			else if (!hide) {
				text += *from;
			}
		}

	}

	return 0;
}






SWORD_NAMESPACE_END
