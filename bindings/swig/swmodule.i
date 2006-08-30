%include "stl.i"
%include "std_pair.i"
%include "std_map.i"


%inline %{
typedef std::map< sword::SWBuf, sword::SWBuf > AttributeValueMap;
typedef std::map< sword::SWBuf, AttributeValueMap> AttributeListMap;
typedef std::map< sword::SWBuf, AttributeListMap> AttributetypeListMap; 
%}

%template() std::pair <sword::SWBuf, sword::SWBuf>;
%template(AttributeValueMap) std::map < sword::SWBuf, sword::SWBuf >;
%template() std::pair <sword::SWBuf, AttributeValueMap>;
%template(AttributeListMap) std::map < sword::SWBuf, AttributeValueMap>;
%template() std::pair < sword::SWBuf, AttributeListMap>;
%template(AttributeTypeListMap) std::map < sword::SWBuf, AttributeListMap>;

%{
#include "swmodule.h"
%}

%warnfilter(401) sword::SWModule;

%ignore sword::SWModule::operator const char *;
%ignore sword::SWModule::operator sword::SWKey &;
%ignore sword::SWModule::operator sword::SWKey *;
%ignore sword::SWModule::operator +=;
%ignore sword::SWModule::operator -=;
%ignore sword::SWModule::operator <<;
%ignore sword::SWModule::operator ++;
%ignore sword::SWModule::operator --;
%ignore sword::SWModule::operator =;

%ignore sword::SWModule::Search;

%ignore sword::SWModule::SWModule;
%ignore sword::SWModule::filterBuffer;
%ignore sword::SWModule::getEntryAttributes;
%ignore sword::SWModule::getConfig;
%ignore sword::SWModule::setConfig;

%include "swmodule.h"

%extend sword::SWModule {
	
	static sword::SWModule *castTo(sword::SWSearchable *o) {
		return dynamic_cast<sword::SWModule*>(o);
	}


    std::map < sword::SWBuf, std::map < sword::SWBuf, std::map < sword::SWBuf, sword::SWBuf > > > 
    
      &getEntryAttributesMap() {
            return self->getEntryAttributes();
    }
}

