%{
#include <localemgr.h>
%}

%ignore sword::LocaleMgr::getAvailableLocales;

%include "localemgr.h"

%extend sword::LocaleMgr {
    std::vector < sword::SWBuf > getAvailableLocalesVector() {
        std::list<sword::SWBuf> l(self->getAvailableLocales());
        return std::vector< sword::SWBuf >(l.begin(), l.end());
    }
}

