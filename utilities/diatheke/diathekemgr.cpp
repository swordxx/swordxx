//---------------------------------------------------------------------------
#include <thmlplain.h>
#include <gbfplain.h>
#include <thmlgbf.h>
#include <gbfthml.h>
#include <thmlhtml.h>
#include <gbfhtml.h>
#include <plainhtml.h>
#include <thmlhtmlhref.h>
#include <gbfhtmlhref.h>
#include <thmlrtf.h>
#include <gbfrtf.h>

#include <swconfig.h>
#include "diafiltmgr.h"

#ifdef _ICU_
#include <utf8arshaping.h>
#include <utf8bidireorder.h>
#include <utf8transliterator.h>
#endif

#include "diathekemgr.h"
#include "gbfcgi.h"
#include "thmlcgi.h"

//---------------------------------------------------------------------------
DiathekeMgr::DiathekeMgr (SWConfig * iconfig, SWConfig * isysconfig, bool autoload, char enc, char mark, bool ibidi, bool ishape)
        : SWMgr(iconfig, isysconfig, autoload, new DiathekeFilterMgr(mark, enc))
{
	bidi = ibidi;
	shape = ishape;

#ifdef _ICU_
        arshaping = new UTF8arShaping();
 	bidireorder = new UTF8BiDiReorder();
        transliterator = new UTF8Transliterator();
#endif
	Load();

#ifdef WIN32
	OSVERSIONINFO osvi;
	memset (&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	platformID = osvi.dwPlatformId;
#endif

}


DiathekeMgr::~DiathekeMgr()
{
#ifdef _ICU_
        if (arshaping)
                delete arshaping;
        if (bidireorder)
                delete bidireorder;
        if (transliterator)
                delete transliterator;
#endif
}


void DiathekeMgr::AddRenderFilters(SWModule *module, ConfigEntMap &section)
{
	string lang;
	ConfigEntMap::iterator entry;

	lang = ((entry = section.find("Lang")) != section.end()) ? (*entry).second : (string)"en";

        if (module->Direction() == DIRECTION_RTL) {
#ifdef _ICU_
                if (shape && !strnicmp(lang.c_str(), "ar", 2)) {
                        module->AddRenderFilter(arshaping);
                }
#ifdef WIN32
		if (bidi && (platformID == WIN9X || (strnicmp(lang.c_str(), "he", 2) && strnicmp(lang.c_str(), "ar", 2)))) {
                        module->AddRenderFilter(bidireorder);
		}
#elseif
		if (bidi) {
                        module->AddRenderFilter(bidireorder);
		}
#endif

#endif
        }
	SWMgr::AddRenderFilters(module, section);
}

void DiathekeMgr::Load () {
        SWMgr::Load();
#ifdef _ICU_
	optionFilters.insert(FilterMap::value_type("UTF8Transliterator", transliterator));
        options.push_back(transliterator->getOptionName());
#endif
};

void DiathekeMgr::AddGlobalOptions (SWModule * module, ConfigEntMap & section,
                                   ConfigEntMap::iterator start,
                                   ConfigEntMap::iterator end) {

        SWMgr::AddGlobalOptions(module, section, start, end);
#ifdef _ICU_
        module->AddOptionFilter(transliterator);
#endif
};

