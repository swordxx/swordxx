#ifndef DIATHEKEMGR_H
#define DIATHEKEMGR_H
#include <swmarkupmgr.h>

#define FMT_CGI 127

enum PlatformIDs { WIN32S = 0, WIN9X, WINNT, WINCE };

class DiathekeMgr : public SWMarkupMgr {
	SWFilter *arshaping;
	SWFilter *bidireorder;
        SWFilter *transliterator;

#ifdef WIN32
        char platformID;
#endif

protected:
	virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
        virtual void Load ();
        virtual void AddGlobalOptions (SWModule * module, ConfigEntMap & section, ConfigEntMap::iterator start, ConfigEntMap::iterator end);
	virtual void CreateFilters(char mark);

public:
	bool shape;
	bool bidi;

	virtual char Markup(char markup = NULL);
	DiathekeMgr(SWConfig * iconf = NULL, SWConfig * isysconfig = NULL, bool autoload = false, char enc = ENC_UTF8, char mark = FMT_PLAIN, bool bidi = false, bool shape = false);
	virtual ~DiathekeMgr();
};

#endif
 
