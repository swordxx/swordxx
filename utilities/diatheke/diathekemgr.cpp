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

#include <swencodingmgr.h>
#include <swconfig.h>

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
        : SWEncodingMgr(iconfig, isysconfig, autoload, enc)
{
  
        markup = mark;
	bidi = ibidi;
	shape = ishape;

        CreateFilters(markup);

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
        if (fromthml)
                delete (fromthml);
        if (fromgbf)
                delete (fromgbf);
        if (fromplain)
                delete (fromplain);
        if (fromosis)
                delete (fromosis);
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
        SWEncodingMgr::AddRenderFilters(module, section);
        switch (module->Markup()) {
        case FMT_THML:
                if (fromthml)
                        module->AddRenderFilter(fromthml);
                break;
        case FMT_GBF:
                if (fromgbf)
                        module->AddRenderFilter(fromgbf);
                break;
        case FMT_PLAIN:
                if (fromplain)
                        module->AddRenderFilter(fromplain);
                break;
        case FMT_OSIS:
                if (fromosis)
                        module->AddRenderFilter(fromosis);
                break;
        }

}

void DiathekeMgr::Load () {
        SWEncodingMgr::Load();
#ifdef _ICU_
	optionFilters.insert(FilterMap::value_type("UTF8Transliterator", transliterator));
        options.push_back(transliterator->getOptionName());
#endif
};

void DiathekeMgr::AddGlobalOptions (SWModule * module, ConfigEntMap & section,
                                   ConfigEntMap::iterator start,
                                   ConfigEntMap::iterator end) {

        SWEncodingMgr::AddGlobalOptions(module, section, start, end);
#ifdef _ICU_
        module->AddOptionFilter(transliterator);
#endif
};


char DiathekeMgr::Markup(char mark) {
        if (mark && mark != markup) {
                markup = mark;
                ModMap::const_iterator module;

                SWFilter * oldplain = fromplain;
                SWFilter * oldthml = fromthml;
                SWFilter * oldgbf = fromgbf;
                SWFilter * oldosis = fromosis;

                CreateFilters(markup);

                for (module = Modules.begin(); module != Modules.end(); module++)
                        switch (module->second->Markup()) {
                        case FMT_THML:
                                if (oldthml != fromthml) {
                                        if (oldthml) {
                                                if (!fromthml) {
                                                        module->second->RemoveRenderFilter(oldthml);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldthml, fromthml);
                                                }
                                        }
                                        else if (fromthml) {
                                                module->second->AddRenderFilter(fromthml);
                                        }
                                }
                                break;
                        case FMT_GBF:
                                if (oldgbf != fromgbf) {
                                        if (oldgbf) {
                                                if (!fromgbf) {
                                                        module->second->RemoveRenderFilter(oldgbf);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldgbf, fromgbf);
                                                }
                                        }
                                        else if (fromgbf) {
                                                module->second->AddRenderFilter(fromgbf);
                                        }
                                        break;
                                }
                        case FMT_PLAIN:
                                if (oldplain != fromplain) {
                                        if (oldplain) {
                                                if (!fromplain) {
                                                        module->second->RemoveRenderFilter(oldplain);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldplain, fromplain);
                                                }
                                        }
                                        else if (fromplain) {
                                                module->second->AddRenderFilter(fromplain);
                                        }
                                        break;
                                }
                        case FMT_OSIS:
                                if (oldosis != fromosis) {
                                        if (oldosis) {
                                                if (!fromosis) {
                                                        module->second->RemoveRenderFilter(oldosis);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldosis, fromosis);
                                                }
                                        }
                                        else if (fromosis) {
                                                module->second->AddRenderFilter(fromosis);
                                        }
                                        break;
                                }
                        }

                if (oldthml)
                        delete oldthml;
                if (oldgbf)
                        delete oldgbf;
                if (oldplain)
                        delete oldplain;
                if (oldosis)
                        delete oldosis;
        }
        return markup;
}

void DiathekeMgr::CreateFilters(char markup) {

                switch (markup) {
		case FMT_CGI:
			fromplain = NULL;
			fromthml = new ThMLCGI();
			fromgbf = new GBFCGI();
			fromosis = NULL;
			break;
                case FMT_PLAIN:
                        fromplain = NULL;
                        fromthml = new ThMLPlain();
                        fromgbf = new GBFPlain();
                        fromosis = NULL;
                        break;
                case FMT_THML:
                        fromplain = NULL;
                        fromthml = NULL;
                        fromgbf = new GBFThML();
                        fromosis = NULL;
                        break;
                case FMT_GBF:
                        fromplain = NULL;
                        fromthml = new ThMLGBF();
                        fromgbf = NULL;
                        fromosis = NULL;
                        break;
                case FMT_HTML:
                        fromplain = new PLAINHTML();
                        fromthml = new ThMLHTML();
                        fromgbf = new GBFHTML();
                        fromosis = NULL;
                        break;
                case FMT_HTMLHREF:
                        fromplain = NULL;
                        fromthml = new ThMLHTMLHREF();
                        fromgbf = new GBFHTMLHREF();
                        fromosis = NULL;
                        break;
                case FMT_RTF:
                        fromplain = NULL;
                        fromthml = new ThMLRTF();
                        fromgbf = new GBFRTF();
                        fromosis = NULL;
                        break;
                case FMT_OSIS:
                        fromplain = NULL;
                        fromthml = NULL;
                        fromgbf = NULL;
                        fromosis = NULL;
                        break;
                }

}

