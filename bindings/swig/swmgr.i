%include "std_map.i"

%template() std::pair<sword::SWBuf, sword::SWModule*>;
%template(ModuleMap) std::map<sword::SWBuf, sword::SWModule*>;

%{
#include <swmgr.h>
%}

%ignore sword::SWMgr::Modules;
%ignore sword::SWMgr::findConfig;
%ignore sword::SWMgr::getGlobalOptions;
%ignore sword::SWMgr::getGlobalOptionValues;
%include "swmgr.h"

%extend sword::SWMgr {
        std::map<sword::SWBuf, sword::SWModule*> &getModules() {
          return self->Modules;
        }
	
	SWModule* getModuleAt( const int pos ) {
		if (pos < 0 || pos > self->Modules.size() )
			return 0;
	
		sword::ModMap::iterator it = self->Modules.begin(); 
		
		for (int i = 0; i < pos; ++i) {
			it++;
		}

		if ( it != self->Modules.end() ) {
			return (*it).second;
		}
		
		return 0;
	}
} 
