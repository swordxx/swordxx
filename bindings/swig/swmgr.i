%{
#include <swmgr.h>
using namespace sword;
%}

typedef map < SWBuf, SWModule *, less < SWBuf > > ModMap;
typedef list < SWBuf > StringList;
typedef map < SWBuf, SWFilter * > FilterMap;

class SWMgr {
public:
//member data
  static void findConfig (char *configType, char **prefixPath, char **configPath);

  SWConfig *config;
  SWConfig *sysconfig;
  ModMap Modules;
  char *prefixPath;
  char *configPath;

//member functions
  SWMgr (SWConfig* iconfig = 0, SWConfig* isysconfig = 0, bool autoload = true, SWFilterMgr* filterMgr = 0);
  SWMgr(const char *iConfigPath, bool autoload = true, SWFilterMgr *filterMgr = 0);
  virtual ~SWMgr();

  virtual signed char Load ();
  virtual void setGlobalOption (const char *option, const char *value);
  virtual const char *getGlobalOption (const char *option);
  virtual const char *getGlobalOptionTip (const char *option);
  virtual StringList getGlobalOptions ();
  virtual StringList getGlobalOptionValues (const char *option);
  virtual signed char setCipherKey (const char *modName, const char *key);
	
  SWModule *getModule(const char *modName);
  virtual void InstallScan(const char *dir);
  
%extend {
	const int moduleCount() {
		return self->Modules.size();
	}
	
	SWModule* getModuleAt( const int pos ) {
		if (pos < 0 || pos > self->Modules.size() )
			return 0;
	
		ModMap::iterator it = self->Modules.begin(); 
		
		for (int i = 0; i < pos; ++i) {
			it++;
		}

		if ( it != self->Modules.end() ) {
			return (*it).second;
		}
		
		return 0;
	}
} 
};
