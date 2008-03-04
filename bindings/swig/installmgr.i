%include "std_map.i"
%include "std_vector.i"
%ignore sword::InstallSource::userData;
%ignore sword::FTPTransport::getDirList;
//%ignore sword::InstallMgr::getModuleStatus;

%{
#include "installmgr.h"
#include "ftptrans.h"
%}


%template() std::pair<sword::SWBuf, sword::InstallSource*>;
%template(InstallSourceMap) std::map<sword::SWBuf, sword::InstallSource*>;
%template() std::pair<sword::SWModule *, int>;
%template() std::map<sword::SWModule *, int>;
//%ignore sword::InstallSource::MODSTAT_OLDER;



%include "installmgr.h"
%include "ftptrans.h"


