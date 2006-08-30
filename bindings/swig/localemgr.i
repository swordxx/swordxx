%{
#include <localemgr.h>
%}

%ignore sword::LocaleMgr::getAvailableLocales;

%include "localemgr.h"

