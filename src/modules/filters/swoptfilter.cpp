/******************************************************************************
 *
 * osisstrongs -	SWFilter descendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <swoptfilter.h>

SWORD_NAMESPACE_START


SWOptionFilter::SWOptionFilter(const char *oName, const char *oTip, const StringList *oValues) {
	optName   = oName;
	optTip    = oTip;
	optValues = oValues;
}


SWOptionFilter::~SWOptionFilter() {
}


void SWOptionFilter::setOptionValue(const char *ival) {
	for (StringList::const_iterator loop = optValues->begin(); loop != optValues->end(); loop++) {
		if (!stricmp(loop->c_str(), ival)) {
			optionValue = *loop;
			option = (!stricmp(ival, "On"));	// convenience for boolean filters
			break;
		}
	}
}

const char *SWOptionFilter::getOptionValue() {
	return optionValue;
}


SWORD_NAMESPACE_END
