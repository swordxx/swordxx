/******************************************************************************
 *
 *  webmgr.cpp -	
 *
 * $Id$
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

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
	char *extraConf;

public:
	WebMgr(const char *path, const char *extraConfPath = 0) : SWMgr(path, false, new MarkupFilterMgr(FMT_WEBIF)) { init(); if (extraConfPath) stdstr(&extraConf, extraConfPath); }
	WebMgr(SWConfig *sysConf) : SWMgr(0, sysConf, false, new MarkupFilterMgr(FMT_WEBIF)) { init(); }
	void init() {
		extraConf         = 0;
		defaultGreekLex   = 0;
		defaultHebLex     = 0;
		defaultGreekParse = 0;
		defaultHebParse   = 0;

		osisWordJS = new OSISWordJS();
		thmlWordJS = new ThMLWordJS();
		gbfWordJS = new GBFWordJS();
		load();
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
		delete extraConf;
	}

	void createAllModules(bool multiMod) {

		if (extraConf) {
			bool exists = FileMgr::existsFile(extraConf);
			if (exists) {
				SWConfig addConfig(extraConf);
				this->config->augment(addConfig);
			}
		}
		SWMgr::createAllModules(multiMod);
	}

	void addGlobalOptionFilters(SWModule *module, ConfigEntMap &section) {

		// ThML word stuff needs to process before strongs strip
		if (module->getMarkup() == FMT_THML) {
			module->addOptionFilter(thmlWordJS);
		}

		if (module->getMarkup() == FMT_GBF) {
			module->addOptionFilter(gbfWordJS);
		}

		// add other module filters
		SWMgr::addGlobalOptionFilters(module, section);

		// add our special filters
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
				module->addOptionFilter((*it).second);	// add filter to module and option as a valid option
			}
		}

		if (module->getMarkup() == FMT_OSIS) {
			module->addOptionFilter(osisWordJS);
		}
	}


	void setJavascript(bool val) {
		osisWordJS->setOptionValue((val)?"On":"Off");
		thmlWordJS->setOptionValue((val)?"On":"Off");
		gbfWordJS->setOptionValue((val)?"On":"Off");
	}
};

#endif
