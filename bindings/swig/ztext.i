%{
#include "ztext.h"
%}

%include "ztext.h"

%extend sword::zText {
	static sword::zText *castTo(sword::SWText *o) {
		return dynamic_cast<sword::zText*>(o);
	}
}