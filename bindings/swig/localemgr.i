%{
#include <localemgr.h>
%}

typedef map < string, SWLocale *, less < string > >LocaleMap;

class LocaleMgr {
public:
  LocaleMgr (const char *iConfigPath = 0);
  virtual ~LocaleMgr ();
  virtual SWLocale *getLocale (const char *name);
  virtual list < string > getAvailableLocales ();
  //virtual const char *translate (const char *name, const char *text);
  virtual const char *getDefaultLocaleName();
  virtual void setDefaultLocaleName (const char *name);

%extend {
  static LocaleMgr* const systemLocaleMgr() {
  	return &(LocaleMgr::systemLocaleMgr);
  };
}
};
