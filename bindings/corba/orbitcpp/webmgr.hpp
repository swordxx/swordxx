#ifndef WEBMGR_HPP
#define WEBMGR_HPP

#include <swmgr.h>
#include <swmodule.h>
#include <swfilter.h>
#include <markupfiltmgr.h>
#include <osiswordjs.h>
#include <thmlwordjs.h>
#include <gbfwordjs.h>

using namespace sword;

class WebMgr : public SWMgr {
	OSISWordJS *osisWordJS;
	ThMLWordJS *thmlWordJS;
	GBFWordJS *gbfWordJS;
	SWModule *defaultGreekLex;
	SWModule *defaultHebLex;
	SWModule *defaultGreekParse;
	SWModule *defaultHebParse;

public:
	WebMgr() : SWMgr(0, 0, false, new MarkupFilterMgr(FMT_WEBIF)) {
		defaultGreekLex   = 0;
		defaultHebLex     = 0;
		defaultGreekParse = 0;
		defaultHebParse   = 0;

		osisWordJS = new OSISWordJS();
		thmlWordJS = new ThMLWordJS();
		gbfWordJS = new GBFWordJS();
		Load();
		osisWordJS->setDefaultModules(defaultGreekLex, defaultHebLex, defaultGreekParse, defaultHebParse);
		thmlWordJS->setDefaultModules(defaultGreekLex, defaultHebLex, defaultGreekParse, defaultHebParse);
		gbfWordJS->setDefaultModules(defaultGreekLex, defaultHebLex, defaultGreekParse, defaultHebParse);
		osisWordJS->setMgr(this);
		thmlWordJS->setMgr(this);
		gbfWordJS->setMgr(this);
		setGlobalOption("Textual Variants", "Primary Reading");
	}

	~WebMgr() {
		delete osisWordJS;
		delete thmlWordJS;
		delete gbfWordJS;
	}


	void AddGlobalOptions(SWModule *module, ConfigEntMap &section, ConfigEntMap::iterator start, ConfigEntMap::iterator end) {
		SWMgr::AddGlobalOptions(module, section, start, end);

		if (module->getConfig().has("Feature", "GreekDef")) {
			defaultGreekLex = module;
		}
		if (module->getConfig().has("Feature", "HebrewDef")) {
			defaultHebLex = module;
		}
		if (module->getConfig().has("Feature", "GreekParse")) {
			defaultGreekParse = module;
		}
		if (module->getConfig().has("Feature", "HebrewParse")) {
			defaultHebParse = module;
		}
		if (module->getConfig().has("GlobalOptionFilter", "ThMLVariants")) {
			OptionFilterMap::iterator it = optionFilters.find("ThMLVariants");
			if (it != optionFilters.end()) {
				module->AddOptionFilter((*it).second);	// add filter to module and option as a valid option
			}
		}

		if (module->Markup() == FMT_OSIS) {
			module->AddOptionFilter(osisWordJS);
		}
		if (module->Markup() == FMT_THML) {
			module->AddOptionFilter(thmlWordJS);
		}
		if (module->Markup() == FMT_GBF) {
			module->AddOptionFilter(gbfWordJS);
		}
	}


	void setJavascript(bool val) {
		osisWordJS->setOptionValue((val)?"On":"Off");
		thmlWordJS->setOptionValue((val)?"On":"Off");
		gbfWordJS->setOptionValue((val)?"On":"Off");
	}
};

#endif
