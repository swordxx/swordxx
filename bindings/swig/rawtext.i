%{
#include "rawtext.h"
%}

%include "rawtext.h"


%extend sword::RawText {
	static sword::RawText *castTo(sword::SWText *o) {
		return dynamic_cast<sword::RawText*>(o);
	}	
}