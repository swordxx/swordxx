%{
#include <swmgr.h>
using namespace sword;
%}

typedef map < string, SWModule *, less < string > > ModMap;
typedef list < string > OptionsList;
typedef map < string, SWFilter * > FilterMap;

class SWMgr {
public:
//member data
//  static bool debug;
//  static const char *globalConfPath;
  static void findConfig (char *configType, char **prefixPath, char **configPath);

  SWConfig *config;
  SWConfig *sysconfig;
  ModMap Modules;
  char *prefixPath;
  char *configPath;

//member functions
  SWMgr (SWConfig* iconfig = 0, SWConfig* isysconfig = 0, bool autoload = true, SWFilterMgr* filterMgr = 0);
//  SWMgr (SWFilterMgr *filterMgr);
//  SWMgr (const char *iConfigPath, bool autoload = true, SWFilterMgr *filterMgr = 0);
  virtual ~SWMgr();

  virtual signed char Load ();
  virtual void setGlobalOption (const char *option, const char *value);
  virtual const char *getGlobalOption (const char *option);
  virtual const char *getGlobalOptionTip (const char *option);
  virtual OptionsList getGlobalOptions ();
  virtual OptionsList getGlobalOptionValues (const char *option);
  virtual signed char setCipherKey (const char *modName, const char *key);

%extend {
  SWModule* module(const char* modulename) {
  	return self->Modules[modulename];
  };
}
};
