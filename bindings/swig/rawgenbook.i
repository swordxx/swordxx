%{
#include "rawgenbook.h"
%}

%include "rawgenbook.h"

%extend sword::RawGenBook {
	static sword::RawGenBook *castTo(sword::SWGenBook *o) {
		return dynamic_cast<sword::RawGenBook*>(o);
	}
	
}