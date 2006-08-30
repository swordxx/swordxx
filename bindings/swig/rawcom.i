%{
#include "rawcom.h"
%}


%include "rawcom.h"



%extend sword::RawCom {
	static sword::RawCom *castTo(sword::SWCom *o) {
		return dynamic_cast<sword::RawCom*>(o);
	}
}