%{
#include "swconfig.h"
%}

%ignore sword::SWConfig::operator[];
%ignore sword::SWConfig::operator+=;
%ignore sword::SWConfig::sections;
%ignore sword::SWConfig::Sections;

%include "swconfig.h"


%extend sword::SWConfig {
  void set(const char* group, const char* entry, const char* value) {
	self->Sections[group][entry] = value;
  };
  const char* get(const char* group, const char* entry) {
	return self->Sections[group][entry].c_str();
  };
  PySectionMap /*std::map < sword::SWBuf, std::multimap <sword::SWBuf, sword::SWBuf> > */
  *getSections() {
    return (PySectionMap* /*std::map < sword::SWBuf, std::multimap < sword::SWBuf, sword::SWBuf > > * */) &self->Sections;
  }
}

