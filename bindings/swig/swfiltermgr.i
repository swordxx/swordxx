%{
#include <swfiltermgr.h>
using namespace sword;
%}

class SWFilterMgr {
  SWFilterMgr ();
  virtual ~SWFilterMgr ();

  virtual void setParentMgr(SWMgr *parentMgr);
  virtual SWMgr *getParentMgr();
  virtual void AddGlobalOptions (SWModule * module, ConfigEntMap & section,
				 ConfigEntMap::iterator start,
				 ConfigEntMap::iterator end);
  virtual void AddLocalOptions (SWModule * module, ConfigEntMap & section,
				ConfigEntMap::iterator start,
				ConfigEntMap::iterator end);
  virtual void AddEncodingFilters (SWModule * module, ConfigEntMap & section);
  virtual void AddRenderFilters (SWModule * module, ConfigEntMap & section);
  virtual void AddStripFilters (SWModule * module, ConfigEntMap & section);
  virtual void AddRawFilters (SWModule * module, ConfigEntMap & section);
};
