%{
#include <localemgr.h>

using namespace std;
using namespace sword;
%}

typedef map < SWBuf, SWLocale *, less < string > >LocaleMap;

class LocaleMgr {
public:
  LocaleMgr (const char *iConfigPath = 0);
  virtual ~LocaleMgr ();
  virtual SWLocale *getLocale (const char *name);
  virtual std::list < SWBuf > getAvailableLocales ();
  //virtual const char *translate (const char *name, const char *text);
  virtual const char *getDefaultLocaleName();
  virtual void setDefaultLocaleName (const char *name);

%extend {
  static LocaleMgr* const getSystemLocaleMgr() {
  	return LocaleMgr::getSystemLocaleMgr();
  };
}
};
