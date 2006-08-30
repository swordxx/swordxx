%{
#include <markupfiltmgr.h>
using namespace sword;
%}

%include "markupfiltmgr.h"


%extend sword::MarkupFilterMgr {
	static sword::MarkupFilterMgr *castTo(sword::EncodingFilterMgr *o) {
		return dynamic_cast<sword::MarkupFilterMgr*>(o);
	}
}