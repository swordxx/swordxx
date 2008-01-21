#ifndef DIATHEKEMGR_H
#define DIATHEKEMGR_H

#include <swmgr.h>
#include "diafiltmgr.h"

enum PlatformIDs { WIN32S = 0, WIN9X, WINNT, WINCE };

class DiathekeMgr : public SWMgr {
	SWFilter *arshaping;
	SWFilter *bidireorder;
    SWOptionFilter *transliterator;

#ifdef WIN32
        char platformID;
#endif

protected:
	virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
        virtual signed char Load ();
        virtual void AddGlobalOptions (SWModule * module, ConfigEntMap & section, ConfigEntMap::iterator start, ConfigEntMap::iterator end);

public:
	bool shape;
	bool bidi;

	unsigned char Markup(unsigned char m = FMT_UNKNOWN) {return ((DiathekeFilterMgr*)filterMgr)->Markup(m);};
	unsigned char Encoding(unsigned char e = ENC_UNKNOWN) {return ((EncodingFilterMgr*)filterMgr)->Encoding(e);};

	DiathekeMgr(SWConfig * iconf = NULL, SWConfig * isysconfig = NULL, bool autoload = false, char enc = ENC_UTF8, char mark = FMT_PLAIN, bool bidi = false, bool shape = false);
	virtual ~DiathekeMgr();
};

#endif
 
