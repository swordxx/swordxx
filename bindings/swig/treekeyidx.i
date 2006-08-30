%{
#include "treekeyidx.h"
%}

%include "treekeyidx.h"

%extend sword::TreeKeyIdx {
	static sword::TreeKeyIdx *castTo(sword::TreeKey *o) {
		return dynamic_cast<sword::TreeKeyIdx*>(o);
	}
}