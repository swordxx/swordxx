/******************************************************************************
 *
 *  mod2imp.cpp -    Utility to export a module in IMP format
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#ifdef _MSC_VER
    #pragma warning( disable: 4251 )
#endif

#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using std::map;
using std::cout;
using std::endl;

using namespace swordxx;


void usage(const char * progName, const char * error = nullptr) {
    if (error) fprintf(stderr, "\n%s: %s\n", progName, error);
    fprintf(stderr, "\n=== mod2imp (Revision $Rev$) Sword++ module exporter.\n");
    fprintf(stderr, "\nusage: %s <module_name> [options]\n"
        "\t -r [output_format]  - render content instead of outputting raw native\n"
        "\t\tdata.  output_format can be: OSIS, XHTML, LATEX, HTMLHREF, RTF.\n"
        "\t -s - strip markup instead of outputting raw native data.\n"
        "\t -f <option_name> <option_value> - when rendering (-r, above), option\n"
        "\t\tfilter values can be set with this option.\n\n"
        , progName);
    exit(-1);
}


int main(int argc, char **argv)
{
    // handle options
    if (argc < 2) usage(*argv);

    const char *progName   = argv[0];
    const char *modName    = argv[1];
    bool render            = false;
    bool strip             = false;
    std::string renderForm;
    std::string optionName;
    map<std::string, std::string> options; // optionName, optionValue;

    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "-r")) {
            if (strip) usage(progName, "-r can't be supplied when using -s");
            if (i+1 < argc) renderForm = argv[++i];
            render = true;
        }
        else if (!strcmp(argv[i], "-s")) {
            if (render) usage(progName, "-s can't be supplied when using -r");
            strip = true;
        }
        else if (!strcmp(argv[i], "-f")) {
            if (i+1 < argc) optionName          = argv[++i];
            if (i+1 < argc) options[optionName] = argv[++i];
            else usage(progName, "-f requires <option_name> <option_value>");
        }
        else usage(progName, (((std::string)"Unknown argument: ")+ argv[i]).c_str());
    }
    // -----------------------------------------------------

    std::shared_ptr<MarkupFilterMgr> markupMgr;
    if       (renderForm == "HTMLHREF") markupMgr = std::make_shared<MarkupFilterMgr>(swordxx::FMT_HTMLHREF);
    else if  (renderForm == "OSIS")     markupMgr = std::make_shared<MarkupFilterMgr>(swordxx::FMT_OSIS);
    else if  (renderForm == "RTF")      markupMgr = std::make_shared<MarkupFilterMgr>(swordxx::FMT_RTF);
    else if  (renderForm == "LATEX")    markupMgr = std::make_shared<MarkupFilterMgr>(swordxx::FMT_LATEX);
    else if  (renderForm == "XHTML")    markupMgr = std::make_shared<MarkupFilterMgr>(swordxx::FMT_XHTML);

    else if  (renderForm.length())      usage(progName, (((std::string) "Unknown output_format for -r (")+renderForm+")").c_str());

    SWMgr *mgr = (markupMgr) ? new SWMgr(std::move(markupMgr)) : new SWMgr();

    // set any options filters passed with -f
    for (map<std::string, std::string>::iterator it = options.begin(); it != options.end(); it++) {
        mgr->setGlobalOption(it->first.c_str(), it->second.c_str());
    }

    SWModule *module = mgr->getModule(modName);

    if (!module) usage(progName, (((std::string) "Couldn't find module: ") + modName).c_str());


    if (VerseKey * const vkey = dynamic_cast<VerseKey *>(module->getKey()))
        vkey->setIntros(true);

    for (module->positionToTop(); !module->popError(); module->increment()) {
        std::cout << "$$$" << module->getKeyText() << std::endl;
        std::cout << ((render) ? module->renderText().c_str() : (strip) ? module->stripText() : module->getRawEntry()) << "\n";
    }

    cout << endl;

    return 0;
}

