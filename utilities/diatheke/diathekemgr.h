#ifndef DIATHEKEMGR_H
#define DIATHEKEMGR_H
#include <swmarkupmgr.h>

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

public:
	bool shape;
	bool bidi;

	DiathekeMgr();
	virtual ~DiathekeMgr();
};

#endif
 
