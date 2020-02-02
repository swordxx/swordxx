/******************************************************************************
 *
 *  mod2osis.cpp -    Exports a module as an OSIS doc
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

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <swordxx/filters/thmlosis.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/modules/comments/zcom.h>
#include <swordxx/modules/common/lzsscomprs.h>
#include <swordxx/modules/common/zipcomprs.h>
#include <swordxx/modules/lexdict/zld.h>
#include <swordxx/modules/texts/ztext.h>
#include <swordxx/swmgr.h>


using namespace swordxx;
using std::endl;
using std::cerr;
using std::cout;


int main(int argc, char **argv)
{
    ThMLOSIS filter;

    cerr << "\n\n*** Don't use this utility *** \n\n";
    cerr << "Its purpose is to prove the engine can do\n";
    cerr << "lossless import / export, but we are not there yet. \n\n";
    cerr << "This utility is done, in fact it is already too complex.\n";
    cerr << "The ENGINE needs more work to assure export as OSIS works\n";
    cerr << "This utility only gives us occasion to improve the engine.\n";
    cerr << "Our goal is not to produce an export tool.\n\n";
    cerr << "In fact, you should never export Sword++ modules.\n";
    cerr << "Many CrossWire modules are licensed for use from publishers\n";
    cerr << "and you will need to obtain your own permissions.\n";
    cerr << "We also do not encourage propagating encoding errors\n";
    cerr << "which you will avoid by obtaining text data from the source.\n\n";
    cerr << "Please see the TextSource entry in the module's .conf file\n";
    cerr << "for information where to obtain module data from our source.\n\n";
    cerr << "If you still must export Sword++ module data, use mod2imp.\n";
    cerr << "It is more lossless; or less lossful, and easier to read.\n\n";

    if ((argc != 2)
        || (!std::strcmp(argv[1], "-h"))
        || (!std::strcmp(argv[1], "--help"))
        || (!std::strcmp(argv[1], "/?"))
        || (!std::strcmp(argv[1], "-?"))
        || (!std::strcmp(argv[1], "-help")))
    {
        cerr << argv[0u] << " - a tool to output a Sword++ module in OSIS "
                            "format\n";
        cerr << "usage: "<< argv[0u] << " <modname> \n";
        cerr << "\n\n";
        return 1;
    }

    SWMgr mgr(std::make_shared<MarkupFilterMgr>(FMT_OSIS));
    for (auto const & option : mgr.getGlobalOptions()) {
        auto const values(mgr.getGlobalOptionValues(option.c_str()));
        if (find(values.begin(), values.end(), "On") != values.end())
            mgr.setGlobalOption(option.c_str(), "On");
        if (find(values.begin(), values.end(), "All Readings") != values.end())
            mgr.setGlobalOption(option.c_str(), "All Readings");
    }

//    mgr.setGlobalOption("Strong's Numbers", "Off");
//    mgr.setGlobalOption("Morphological Tags", "Off");

    auto const it = mgr.modules().find(argv[1]);
    if (it == mgr.modules().end()) {
        fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
        std::exit(-2);
    }

    SWModule & inModule = *it->second;
//    inModule.AddRenderFilter(&filter);

    auto const key(inModule.getKey());
    auto const vkey = dynamic_cast<VerseKey *>(key.get());

    bool opentest = false;
    bool openbook = false;
    bool openchap = false;
    int lastTest = 5;
    int lastBook = 9999;
    int lastChap = 9999;
    if (!vkey) {
        cerr << "Currently mod2osis only works with verse keyed modules\n\n";
        std::exit(-1);
    }

    vkey->setIntros(false);

    cout << "<?xml version=\"1.0\" ";
        if (inModule.getConfigEntry("Encoding")) {
            if (*(inModule.getConfigEntry("Encoding")))
                cout << "encoding=\"" << inModule.getConfigEntry("Encoding")
                     << "\" ";
            else cout << "encoding=\"UTF-8\" ";
        }
        else cout << "encoding=\"UTF-8\" ";
        cout << "?>\n\n";


    cout << "<osis";
        cout << " xmlns=\"http://www.bibletechnologies.net/2003/OSIS/namespace\"";
        cout << " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"";
        cout << " xsi:schemaLocation=\"http://www.bibletechnologies.net/2003/OSIS/namespace http://www.bibletechnologies.net/osisCore.2.1.1.xsd\">\n\n";
    cout << "<osisText";
        cout << " osisIDWork=\"";
        cout << inModule.getName() << "\"";
        cout << " osisRefWork=\"defaultReferenceScheme\"";
        if (!inModule.getLanguage().empty())
            cout << " xml:lang=\"" << inModule.getLanguage() << "\"";
        cout << ">\n\n";

    cout << "\t<header>\n";
    cout << "\t\t<work osisWork=\"";
    cout << inModule.getName() << "\">\n";
    cout << "\t\t\t<title>" << inModule.getDescription() << "</title>\n";
    cout << "\t\t\t<identifier type=\"OSIS\">Bible." << inModule.getName()
         << "</identifier>\n";
    cout << "\t\t\t<refSystem>Bible.KJV</refSystem>\n";
    cout << "\t\t</work>\n";
    cout << "\t\t<work osisWork=\"defaultReferenceScheme\">\n";
    cout << "\t\t\t<refSystem>Bible.KJV</refSystem>\n";
    cout << "\t\t</work>\n";
    cout << "\t</header>\n\n";


    inModule.positionToTop();

    std::shared_ptr<VerseKey> tmpKey;
    {
        std::shared_ptr<SWKey> p(inModule.createKey());
        if (auto vk = std::dynamic_pointer_cast<VerseKey>(p)) {
            tmpKey = std::move(vk);
        } else {
            tmpKey = std::make_shared<VerseKey>();
        }
    }
    tmpKey->setText(inModule.getKeyText());

    tmpKey->setIntros(true);
    tmpKey->setAutoNormalize(false);

    for (inModule.positionToTop(); !inModule.popError(); inModule.increment()) {
        bool newTest = false;
        bool newBook = false;

        if (inModule.renderText().empty()) {
            continue;
        }

        if ((vkey->getTestament() != lastTest)) {
            if (openchap)
                cout << "\t</chapter>\n";
            if (openbook)
                cout << "\t</div>\n";
            if (opentest)
                cout << "\t</div>\n";
            cout << "\t<div type=\"x-testament\">\n";
            opentest = true;
            newTest = true;
        }
        if ((vkey->getBook() != lastBook) || newTest) {
            if (!newTest) {
                if (openchap)
                    cout << "\t</chapter>\n";
                if (openbook)
                    cout << "\t</div>\n";
            }
            *tmpKey = *vkey;
            tmpKey->setChapter(0);
            tmpKey->setVerse(0);
//            filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
            cout << "\t<div type=\"book\" osisID=\"" << tmpKey->getOSISRef()
                 << "\">\n" << endl;
            openbook = true;
            newBook = true;
        }
        if ((vkey->getChapter() != lastChap) || newBook) {
            if (!newBook) {
                if (openchap)
                    cout << "\t</chapter>\n";
            }
            *tmpKey = *vkey;
            tmpKey->setVerse(0);
//            filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
            cout << "\t<chapter osisID=\"" << tmpKey->getOSISRef() << "\">\n";
            openchap = true;
        }
        std::string verseText = inModule.getRawEntry();
        cout << "\t\t<verse osisID=\"" << vkey->getOSISRef() << "\">"
             << verseText.c_str() << "</verse>\n" << endl;
        lastChap = vkey->getChapter();
        lastBook = vkey->getBook();
        lastTest = vkey->getTestament();
    }
    if (openchap)
        cout << "\t</chapter>\n";
    if (openbook)
        cout << "\t</div>\n";
    if (opentest)
        cout << "\t</div>\n";
    cout << "\t</osisText>\n";
    cout << "</osis>\n";
    return 0;
}


