%{
#include "rawld.h"
%}

%include "rawld.h"

%extend sword::RawLD {
	static sword::RawLD *castTo(sword::SWLD *o) {
		return dynamic_cast<sword::RawLD*>(o);
	}

}