/******************************************************************************
 *
 * OSISReferenceLinks
 *		      modified:       2009 by John Austin
 *		      SWFilter descendant to toggle OSIS reference links with
 *		      specified type/subType
 */


#include <stdlib.h>
#include <osisreferencelinks.h>
#include <utilxml.h>
#include <swmodule.h>


SWORD_NAMESPACE_START


namespace {

	static const StringList *oValues() {
		static const SWBuf choices[3] = {"On", "Off", ""};
		static const StringList oVals(&choices[0], &choices[2]);
		return &oVals;
	}
}


OSISReferenceLinks::OSISReferenceLinks(const char *optionName, const char *optionTip, const char *type, const char *subType, const char *defaultValue)
		: SWOptionFilter(),
		  optionName(optionName),
		  optionTip(optionTip),
		  type(type),
		  subType(subType) {

	optName   = this->optionName.c_str();
	optTip    = this->optionTip.c_str();
	optValues = oValues();
	setOptionValue(defaultValue);
}


OSISReferenceLinks::~OSISReferenceLinks() {
}


char OSISReferenceLinks::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (option) return 0;
       
	SWBuf token;
	bool intoken        = false;
	bool stripThisToken = false;

	SWBuf orig = text;
	const char *from = orig.c_str();

	for (text = ""; *from; ++from) {
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		else if (*from == '>') {	// process tokens
			intoken = false;
			if (strncmp(token, "reference", 9) && strncmp(token.c_str(), "/reference", 10)) {
				text.append('<');
				text.append(token);
				text.append('>');
			}
			else {
				XMLTag tag;
				tag = token;
				if (!tag.isEndTag() && type == tag.getAttribute("type") && (!subType.size() || subType == tag.getAttribute("subType"))) {
					stripThisToken = true;
					continue;
				}
				else if (tag.isEndTag() && stripThisToken) {
					stripThisToken = false;
					continue;
				}
				text.append('<');
				text.append(token);
				text.append('>');
			}
			continue;
		}
	       
		if (intoken) { //copy token
			token.append(*from);
		}
		else { //copy text which is not inside a token
			text.append(*from);
		}
	}
	return 0;
}

SWORD_NAMESPACE_END


