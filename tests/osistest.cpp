/******************************************************************************
 *
 * Copyright 20122013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <iostream>
#include <memory>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace std;
using namespace swordxx;


void outputCurrentVerse(SWModule & module) {

    module.renderText();

    cout << "Key:\n";
    cout << module.getKeyText() << "\n";
    cout << "-------\n";

    cout << "Preverse Header 0:\nRaw:\n";
    std::string header = module.getEntryAttributes()["Heading"]["Preverse"]["0"];
    cout << header << endl;
    cout << "-------\n";
    cout << "Rendered Header:\n";
    cout << module.renderText(header.c_str()) << endl;
    cout << "-------\n";
    cout << "CSS:\n";
    cout << module.getRenderHeader() << endl;
    cout << "-------\n";
    cout << "RenderText:\n";
    cout << module.renderText() << endl;
    cout << "-------\n";
    cout << "-------\n\n";
}


int main(int argc, char **argv) {

    if (argc != 2) {
        cerr << "\nusage: " << *argv << " <modName>\n" << endl;
        std::exit(-1);
    }

    SWMgr library(std::make_shared<MarkupFilterMgr>(FMT_XHTML));
    library.setGlobalOption("Headings", "On");

    auto const module(library.getModule(argv[1]));

    if (!module) {
        cerr << "\nCouldn't find module: " << argv[1] << "\n" << endl;
        std::exit(-2);
    }

    module->setKey("Ps.3.1");
    outputCurrentVerse(*module);

    module->setKey("Matt.2.6");
    outputCurrentVerse(*module);

    module->setKey("Mark.1.14");
    outputCurrentVerse(*module);


    cout << "\nWhitespace tests around headings:\n";
    module->getKeyAs<VerseKey>()->setIntros(true);
    (*module).positionToTop();
    // module heading
    cout << module->renderText() << "\n";
    (*module).increment();
    // testament heading
    cout << module->renderText() << "\n";
    (*module).increment();
    // book heading
    cout << module->renderText() << "\n";
    (*module).increment();
    // chapter heading
    cout << module->renderText() << "\n";
    (*module).increment();
    // verse body
    module->renderText();
    std::string header = module->getEntryAttributes()["Heading"]["Preverse"]["0"];
    cout << module->renderText(header.c_str()) << endl;
    cout << "[ " << module->getKeyText() << " ] " << module->renderText() << "\n";
    (*module).increment();
    // verse body
    module->renderText();
    header = module->getEntryAttributes()["Heading"]["Preverse"]["0"];
    cout << module->renderText(header.c_str()) << endl;
    cout << "[ " << module->getKeyText() << " ] " << module->renderText() << "\n";

    return 0;
}
